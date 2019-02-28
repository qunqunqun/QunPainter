#include "3DWidget.h"


my_3DWidget::my_3DWidget(QWidget *parent) : QOpenGLWidget(parent) //初始化
{
    numbers_of_Vertices = 0;
    numbers_of_Faces = 0;
    numbers_of_Edges = 0;
    angle = 0;
    offsetX = 0;
    offsetY = 0;
    offsetZ = 0;
    this->setFocusPolicy(Qt::StrongFocus); //摄者样式
}

void my_3DWidget::initializeGL()
{
    initializeOpenGLFunctions();    //初始化OpenGLFunctions
    glClearColor(0.0, 0.0, 0.0, 0.0); //底色
    glShadeModel(GL_SMOOTH);
    glClearDepth(1.0);
    glEnable(GL_DEPTH_TEST); //开启测试值
}

void my_3DWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW); //模型视图
    glLoadIdentity();   //恢复初始坐标系
    glTranslatef(offsetX, offsetY, offsetZ); //更改坐标系
    glRotatef(angle, 0,0.5,0); //按角度在旋转
    gluLookAt(1.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
    this->draw3D(); //绘制3D图形
}

void my_3DWidget::resizeGL(int width, int height)
{
    glViewport(0, 0, (GLint)width, (GLint)height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (GLfloat)width/(GLfloat)height, 0.1, 100.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void my_3DWidget::init()
{
    numbers_of_Vertices = 0;
    numbers_of_Faces = 0;
    numbers_of_Edges = 0;
    angle = 0;
    offsetX = 0;
    offsetY = 0;
    offsetZ = 0;
    this->readOffFiles(); //读取文件
    update();
}


void my_3DWidget::readOffFiles()
{
    QString fileName;
      fileName = QFileDialog::getOpenFileName(this,tr("Open File"),tr(""),tr("OFF File (*.off)"));
      if(fileName == "")                  //如果用户直接关闭了文件浏览对话框，那么文件名就为空值，直接返回
      {
          QMessageBox::warning(this,tr("错误"),tr("指定的目录为空"));
          return;
      }
      else
      {
         QFile file(fileName);
         if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
         {
             QMessageBox::warning(this,tr("错误"),tr("打开文件失败"));
             return;
         }
         else
         {
             if(!file.isReadable())
             {
                 QMessageBox::warning(this,tr("错误"),tr("该文件不可读"));
             }
             else
             {
                 //读取文件
                 vecs.clear();
                 faces.clear();
                 int i = 0;
                 QTextStream txtInput(&file);
                 QString lineStr;
                 while(!txtInput.atEnd())
                 {
                     lineStr = txtInput.readLine();
                     //qDebug()<<lineStr;
                     i++;
                     if(i == 1) continue; //为OFF
                     if(i == 2){ //对点，面，线
                         QVector<QString> t = this->divide_string(lineStr); //进行读取
                         this->numbers_of_Vertices = t[0].toInt(); //转换字符串
                         this->numbers_of_Faces = t[1].toInt(); //面
                         this->numbers_of_Edges = t[2].toInt(); //线
                     }
                     else //读取点坐标和面构成，区别是个数
                     {
                         QVector<QString> t = this->divide_string(lineStr);
                         if(t.length() == 3) //读取点
                         {
                             QVector3D point(t[0].toFloat(),t[1].toFloat(),t[2].toFloat());
                             this->vecs.push_back(point);//存放
                         }
                         else //读面
                         {
                             QVector<int> temp;
                             for(int j = 0; j < t.length() ; j++)
                             {
                                 temp.push_back(t[j].toInt()); //存放点，第一个为个数
                             }
                             this->faces.push_back(temp); //存放
                         }
                     }
                 }
                 //处理一下坐标
                 this->normalize_point();
             }
         }
         file.close();
      }
}
void my_3DWidget::normalize_point(){
    GLfloat max = 0;
    for(int i = 0; i < this->vecs.length(); i++)
    {
        if(max < vecs[i].x())
        {
            max = vecs[i].x();
        }
        if(max < vecs[i].y())
        {
            max = vecs[i].y();
        }
        if(max < vecs[i].z())
        {
            max = vecs[i].z();
        }
    }
    if(max != 0)
    {
        for(int i = 0; i < this->vecs.length(); i++)
       {
           vecs[i].setX(vecs[i].x() / max);
           vecs[i].setY(vecs[i].y() / max);
           vecs[i].setZ(vecs[i].z() / max);
       }
    }
}
void my_3DWidget::keyPressEvent(QKeyEvent *event)
{
    switch(event->key()){
        case Qt::Key_Left:  angle+=5;break;
        case Qt::Key_Right: angle-=5;break;
        case Qt::Key_Up:    offsetZ+=0.5;break;
        case Qt::Key_Down:  offsetZ-=0.5;break;
        case Qt::Key_A:     offsetX+=0.5;break;
        case Qt::Key_D:     offsetX-=0.5;break;
        case Qt::Key_W:     offsetY+=0.5;break;
        case Qt::Key_S:     offsetY-=0.5;break;
    default:break;
    }
    update();
}

void my_3DWidget::draw3D()
{
    drawFaces(); //绘制面
    drawLines(); //绘制直线
    //drawPoints();//绘制点
}

void my_3DWidget::drawPoints()
{
    glColor3f(0.0, 1.0, 0.0);
    glPointSize(2);
    glBegin(GL_POINTS);
    for(int i = 0 ;i < numbers_of_Vertices; i++)
    {
        glVertex3f(vecs[i].x(),vecs[i].y(),vecs[i].z());
    }
    glEnd();
}

void my_3DWidget::drawLines()
{
    for(int i=0;i<faces.size();i++)
    {
        QVector<int> face = faces[i];
        glColor3f(0.0, 0, 1.0); //青色
        glBegin(GL_LINES); //开始划线
        for(int j = 2 ; j < face.length();j++)
        {
            glVertex3f(vecs[face[j-1]].x(), vecs[face[j-1]].y(), vecs[face[j-1]].z());
            glVertex3f(vecs[face[j]].x(), vecs[face[j]].y(), vecs[face[j]].z());
        }
        glVertex3f(vecs[face[face.length()-1]].x(), vecs[face[face.length()-1]].y(),vecs[face[face.length()-1]].z());
        glVertex3f(vecs[face[1]].x(), vecs[face[1]].y(), vecs[face[1]].z());
        glEnd();
    }
}

void my_3DWidget::drawFaces()
{
    for(int i = 0 ;i < faces.size(); i++)
    {
        QVector<int> face = faces[i];
        glColor3f(1.0, 1.0, 1.0); //白色
        if(face[0] == 3)
        {
            glBegin(GL_TRIANGLES);
        }
        else if(face[0] == 4)
        {
           glBegin(GL_TRIANGLES);
        }
        else
        {
               glBegin(GL_POLYGON);
        }
        for(int j = 1 ; j< face.length();j++)
        {
            glVertex3f(vecs[face[j]].x(), vecs[face[j]].y(), vecs[face[j]].z());
        }
        glEnd();
    }
}

QVector<QString> my_3DWidget:: divide_string(QString a)
{
    QVector<QString> res;
    res.clear(); //清空函数
    QString temp;
    temp.clear();
    for(int i = 0; i < a.length() ; i++) //进行划分
    {
        if(a[i] == ' ' || i + 1 >= a.length()) //如果是进行空格或遇到最后一个
        {
            if(i + 1 >= a.length() && a[i] != ' ') //如果是最后一个，需要加上去
            {
                temp += a[i];
            }
            if(!temp.isEmpty()) //不为空
            {
                res.push_back(temp);
                temp.clear();
            }
        }
        else
        {
            temp += a[i];
        }
    }
    //最后返回
    return res;
}
