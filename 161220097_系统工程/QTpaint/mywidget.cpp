#include "mywidget.h"
#include<QDebug>
#include"thepen.h"
#include"Line.h"
#include"Circle.h"
#include"Ellipse.h"
#include"Polygon.h"
#include"Bezier.h"
#include <QCursor>
#include <QStack>
#include <QFileDialog>
#include <QMessageBox>
#include <QCloseEvent>

myWidget::myWidget(QWidget *parent) : QWidget(parent)
{
     s =  new myopenGL;
     setAutoFillBackground(true);    //自动设定背景颜色
     setPalette(QPalette(Qt::gray));      //设置调色板的颜色为白色

     cur_draw_area = new QPixmap(801,541);
     cur_draw_area->fill(Qt::white);

     QPixmap *temp = new QPixmap;
     *temp = *cur_draw_area;
     draw_area.push_back(temp);

     type_of_draw = none;       //不在画图形
     is_drawing = false;        //不能画，所以不能够行动

      int style = static_cast<int>(Qt::SolidLine);//设置QPainter的属性
      int weight = 2;
      pen.setStyle((Qt::PenStyle)style);		//设置画笔的格式
      pen.setWidth(weight);					//设置画笔的线宽值
      pen.setColor(Qt::black);

      is_drawing = false;
      draw_or_not = false;
      is_editing = false; //全部为false
      is_moving = -1;       //标志位设置成-1表示无操作
      is_rotating = -1;
      is_resizing = -1;
      is_procced = nullptr;
      is_closed_by = false;
}

void myWidget::set_type_to_pen()//设置为笔
{
    if(this->is_drawing == true || this->is_editing == true) return;
    draw_or_not = true;
    type_of_draw = thepen;
}

void myWidget::set_type_to_none()//设置为无图形
{
    if(this->is_drawing == true || this->is_editing == true) return;
    draw_or_not = false;
    type_of_draw = none;
}

void myWidget::set_type_to_line()//设置为直线
{
    if(this->is_drawing == true || this->is_editing == true) return;
    draw_or_not = true;
    type_of_draw = line;
}

void myWidget::set_type_to_circle()//设置为圆
{
    if(this->is_drawing == true || this->is_editing == true) return;
    draw_or_not = true;
    type_of_draw = circle;
}

void myWidget::set_type_to_ellipse()//设置为椭圆
{
    if(this->is_drawing == true || this->is_editing == true) return;
     draw_or_not = true;
     type_of_draw = ellipse;
}

void myWidget::set_type_to_polygon()//设置为多边形
{
    if(this->is_drawing == true || this->is_editing == true) return;
    draw_or_not = true;
    type_of_draw = polygon;
}

void myWidget::set_type_to_fillcolor()
{
    if(this->is_drawing == true || this->is_editing == true) return;
    draw_or_not = true;
    type_of_draw = fillcolor;
}

void myWidget::set_type_to_Bezier()
{
    if(this->is_drawing == true || this->is_editing == true)
    {
        if(this->type_of_draw != Bezier) return;
    }
    draw_or_not = true;
    if(type_of_draw == Bezier && (is_drawing == true || is_editing == true) && cur_figure != nullptr) //
    {
        cur_figure->set_Bezier_finished(true); //设置可以画曲线了
        if(this->is_drawing == true)
        {
            this->is_drawing = false;
            this->is_editing = true; //进入编辑模式
        }
        my_paint(temp_draw_area);
    }
    type_of_draw = Bezier; //设置画曲线
}

bool  myWidget::set_new_figure(Figure *& temp)//构造新的图形
{
    switch (type_of_draw)
    {
            case none:
            {
                temp = nullptr;
                break;
            }
            case thepen:
            {
                temp = nullptr;
                 break;
            }
            case line:
            {
                temp = new myLine;
                break;
            }
            case circle:
            {
                temp = new myCircle;
                break;
            }
            case ellipse:
            {
                temp = new myEllipse;
                break;
            }
            case polygon:
            {
                temp = new myPloygon;
                break;
            }
            case Bezier:
            {
                temp = new myBezier;
                break;
            }
            default:
            {
                temp = nullptr;
                qDebug()<<"none"<<endl;
                break;
            }
    }
    if(temp == nullptr)
    {
        return false; //没有创建图形对象
    }
    else return true; //成功创建图形对象之一
}

void myWidget::mousePressEvent(QMouseEvent *e) //鼠标按压
{
    temp_draw_area = new QPixmap(801,541);
    temp_draw_area->fill(Qt::white);
    *temp_draw_area = *cur_draw_area;
    qDebug()<<"press"<<endl;
    if(e->button() & Qt::LeftButton) //点击鼠标的左键
    {
        qDebug()<<"start"<<start_Pos.rx()<<" "<<start_Pos.ry()<<endl;
        if(type_of_draw == none) //什么也不画
        {
            qDebug()<<"donothing";
        }
        else if(type_of_draw == thepen) //进行画笔类的操作
        {
            start_Pos = e->pos(); //记录下开始的点的位置
            end_Pos = e->pos();
            qDebug()<<"begin using pen";
            is_drawing = true;
        }
        else if(type_of_draw == fillcolor) //实现填充算法
        {
            start_Pos = e->pos();
            is_drawing = true;
            is_editing = true;
            this->cur_image = this->cur_draw_area->toImage(); //转化为QIMAGE实现填充
            this->fill_start_pos.rx() = start_Pos.rx();
            this->fill_start_pos.ry() = start_Pos.ry(); //设置点
            this->pcolor = this->cur_image.pixelColor(e->pos().x(),e->pos().y());   //得到Press点的信息
            qDebug()<< pcolor <<endl;
            my_paint(cur_draw_area);//在当前图片上直接进行绘图
            QPixmap *temp = getPixCopy();
            draw_area.push_back(temp);//保存下
        }
        else if(is_drawing == false && is_editing == false) //进行绘图
        {
            start_Pos = e->pos(); //记录下开始的点的位置
            qDebug()<<"begin drawing";
            //开始创建一个新的Figure对象进行操作
            bool succ =  this->set_new_figure(cur_figure);
            if(succ == false && (type_of_draw != none && type_of_draw != thepen))
            {
                qDebug()<<"error when creating Figure";
                assert(0); //中断
            }
            is_drawing = true; //开始绘图
            if(type_of_draw != polygon && type_of_draw != Bezier) //非多边形和曲线的绘制方法
            {
                cur_figure->set_begin_point(start_Pos); //设置起始位置
            }
            else if(type_of_draw == polygon) //为多边形的编辑，刚初始按下的时候
            {
                if(cur_figure->num_of_set() == 0) //为空集
                {
                    cur_figure->set_begin_point(start_Pos);
                    cur_figure->add_into_set(start_Pos);
                }
                else if(cur_figure->is_polyon_to(start_Pos) == true) //按下的是一个新的点
                {
                    cur_figure->add_into_set(start_Pos);                    //则将这个点加入
                    *temp_draw_area = *cur_draw_area;
                     my_paint(temp_draw_area);
                }
                else //按下的达到了第一个点，结束绘画，进入编辑模式
                {
                    cur_figure->add_into_set(start_Pos);  //最后一个点
                    qDebug()<<"begin to edit";
                    *temp_draw_area = *cur_draw_area;
                     my_paint(temp_draw_area);
                    is_drawing = false;
                    is_editing = true;                      //进入编辑模式
                }
            }
            else if(type_of_draw == Bezier) //为曲线的编辑,刚开始按下的时候
            {
                if(cur_figure->num_of_set() == 0) //为空集的话,则进行加入
                {
                    qDebug()<<"new pos of Bezier 1 ";
                    cur_figure->set_begin_point(start_Pos);
                    cur_figure->add_into_set(start_Pos);
                    *temp_draw_area = *cur_draw_area;
                     my_paint(temp_draw_area);
                }
                else //一直加入就可以了
                {
                    cur_figure->add_into_set(start_Pos);  //最后一个点
                    *temp_draw_area = *cur_draw_area;
                    my_paint(temp_draw_area);
                }
            }
        }
        else if(is_drawing == false && is_editing == true) //编辑模式
        {
            start_Pos = e->pos(); //记录下开始的点的位置
            end_Pos = e->pos();
            qDebug()<<"begin editing";
            //根据鼠标位置来进行判断
            is_moving = cur_figure->is_to_move(start_Pos); //判断是哪种模式
            is_resizing = cur_figure->is_to_resize(start_Pos);
            is_rotating = cur_figure->is_to_rotate(start_Pos);
            //判断结束,鼠标移动根据这些moving来进行迁徙
            qDebug()<<is_moving<<" "<<is_rotating<<" "<<is_resizing;
            if(is_moving == -1 && is_rotating == -1 && is_resizing == -1) //这三个功能都不使用，则进行保存功能
            {
                if(this->type_of_draw == Bezier)
                {
                    this->cur_figure->set_draw_line_or_not(false); //设置为完成
                }
                if(this->type_of_draw == line && is_cuting == true )//直线则需要考虑裁剪
                {
                    //按下的第一个顶点是矩形的起点
                    this->cur_figure->set_cuting(true); //可以编辑
                    this->cur_figure->add_into_cut(start_Pos); //加入点集
                    my_paint(temp_draw_area); //
                    return; //立即返回
                }
                is_editing = false; //退出编辑模式
                my_paint(cur_draw_area);
                delete temp_draw_area;
                if(type_of_draw != none)
                {
                    QPixmap *temp = getPixCopy();
                    draw_area.push_back(temp);//保存下来
                }
                delete cur_figure; //删除,改写
                cur_figure = nullptr;
                qDebug()<<"finish drawing";
            }
            else{
                 this->setCursor(Qt::ClosedHandCursor);
            }
        }
        else if(is_drawing == true && is_editing == false)
        {
            if(type_of_draw == polygon)
            {
                start_Pos = e->pos(); //记录下开始的点的位置
                assert(type_of_draw == polygon);
                this->is_polygon = true;
                if(cur_figure->num_of_set() == 0) //为空集
                {
                    cur_figure->set_begin_point(start_Pos);
                    cur_figure->add_into_set(start_Pos);
                }
                else if(cur_figure->is_polyon_to(start_Pos) == true) //按下的是一个新的点
                {
                    cur_figure->add_into_set(start_Pos);     //则将这个点加入
                    *temp_draw_area = *cur_draw_area;
                    my_paint(temp_draw_area);
                }
                else //按下的达到了第一个点，结束绘画，进入编辑模式
                {
                    cur_figure->add_into_set(start_Pos);
                    qDebug()<<"begin to edit";
                    *temp_draw_area = *cur_draw_area;
                    my_paint(temp_draw_area);
                    is_drawing = false;
                    is_editing = true;                      //进入编辑模式
                }
            }
            if(type_of_draw == Bezier)
            {
                start_Pos = e->pos(); //记录下开始的点的位置
                assert(type_of_draw == Bezier);
                this->is_polygon = true;
                if(cur_figure->num_of_set() == 0) //为空集
                {
                    cur_figure->set_begin_point(start_Pos);
                    cur_figure->add_into_set(start_Pos);
                }
                else if(cur_figure->is_polyon_to(start_Pos) == true) //按下的是一个新的点
                {
                    cur_figure->add_into_set(start_Pos);     //则将这个点加入
                    *temp_draw_area = *cur_draw_area;
                    my_paint(temp_draw_area);
                }
            }
        }
    }
}

void myWidget::mouseMoveEvent(QMouseEvent *e)  //鼠标移动
{
    e->accept();
    if(e->buttons() & Qt::LeftButton)
    {//-----鼠标为左键且正在移动
         QPoint *temp_pen_e = new QPoint;
         *temp_pen_e = start_Pos;
         QPoint *temp_pen_b = new QPoint;
         *temp_pen_b = end_Pos;
         end_Pos = e->pos(); //更新
         if(type_of_draw == none) //
         {
             qDebug()<<"do nothing"; //什么也不做
         }
         else if(type_of_draw == thepen) //进行画笔操作
         {
             qDebug()<<"use pen";
             type_of_draw = thepen;
             start_Pos = *temp_pen_e;
             my_paint(cur_draw_area);
             start_Pos = *temp_pen_b;
             delete temp_pen_b;
             delete temp_pen_e;
         }
         else if(type_of_draw == fillcolor) //为填充
         {
             qDebug()<<"do nothing";//什么也不做
         }
         else //图形
         {
            *temp_draw_area = *cur_draw_area;
            //对于不是thepen的图形来说，移动鼠标要分成三个部分来进行操作
            if(is_drawing == true && is_editing == false) //进入画图模式
            {
                //进行画图操作
                if(type_of_draw == polygon) //多边形的操作
                {
                       qDebug()<<"num:"<<this->cur_figure->num_of_set();
                       if(this->cur_figure->num_of_set() == 1) //准备插入下一个点
                       {
                                cur_figure->add_into_set(end_Pos); //设置当前位置的结尾
                                cur_figure->set_end_point(end_Pos);
                                qDebug()<<"num:"<<this->cur_figure->num_of_set();
                                my_paint(temp_draw_area);
                       }
                       else //更改最后的顶点
                       {
                           cur_figure->set_end_point(end_Pos);
                           qDebug()<<"num:"<<this->cur_figure->num_of_set();
                           my_paint(temp_draw_area);
                       }
                }
                else if(type_of_draw == Bezier) //进行曲线的绘制，曲线的绘制
                {
                    qDebug()<<"Bezier_num:"<<this->cur_figure->num_of_set();
                    qDebug()<<"end_pos"<<end_Pos.rx() <<" "<<end_Pos.ry();
                    if(this->cur_figure->num_of_set() == 1) //准备插入下一个点
                    {
                             cur_figure->add_into_set(end_Pos); //设置当前位置的结尾
                             cur_figure->set_end_point(end_Pos);
                             qDebug()<<"num:"<<this->cur_figure->num_of_set();
                             my_paint(temp_draw_area);
                    }
                    else //更改最后的顶点
                    {
                        cur_figure->set_end_point(end_Pos);
                        qDebug()<<"num:"<<this->cur_figure->num_of_set();
                        my_paint(temp_draw_area);
                    }
                }
                else
                {
                    cur_figure->set_end_point(end_Pos); //设置当前位置的结尾
                    my_paint(temp_draw_area);
                }
            }
            else if(is_drawing == false && is_editing == true) //进入编辑模式
            {
                qDebug()<<"start editing";
                if(is_moving != -1) //显示移动功能
                {
                    //todo:实现图片的平行移动
                    QPainter *painter = new QPainter;		//新建一个QPainter对象
                    painter->begin(temp_draw_area);			//在当前PIXMAP进行绘制
                    painter->setPen(pen);					//将QPen对象应用到绘制对象中
                    cur_figure->move_(painter,(end_Pos.rx()-temp_pen_b->rx()),(end_Pos.ry()-temp_pen_b->ry()));
                    cur_figure->show_edit_func(painter);
                    painter->end();
                    update();
                }
                else if(is_rotating != -1) //使用旋转功能
                {
                    QPainter *painter = new QPainter;		//新建一个QPainter对象
                    painter->begin(temp_draw_area);			//在当前PIXMAP进行绘制
                    painter->setPen(pen);					//将QPen对象应用到绘制对象中
                    cur_figure->rotate_(painter,end_Pos);
                    cur_figure->show_edit_func(painter);
                    painter->end();
                    update();
                }
                else if(is_resizing != -1) //使用重新编辑的功能
                {
                    QPainter *painter = new QPainter;		//新建一个QPainter对象
                    painter->begin(temp_draw_area);			//在当前PIXMAP进行绘制
                    painter->setPen(pen);					//将QPen对象应用到绘制对象中
                    cur_figure->resize_(painter,end_Pos,is_resizing);
                    cur_figure->show_edit_func(painter);
                    painter->end();
                    update();
                }
                else
                {
                   if(this->type_of_draw == line) //设置最后一个点
                   {
                       this->cur_figure->set_end_cut(end_Pos);
                       my_paint(temp_draw_area); //绘制
                   }
                }
            }
         }
    }

}

void myWidget::mouseReleaseEvent(QMouseEvent *e)  //鼠标释放
{
    //鼠标释放
    this->setCursor(Qt::ArrowCursor);
    end_Pos = e->pos(); //记录下结束的位置
    //qDebug()<<"end"<<end_Pos.rx()<<" "<<end_Pos.ry()<<endl;
    if(type_of_draw == none)
    {
       qDebug()<<"do nothing after release";
    }
    else if(type_of_draw == thepen)
    {
        qDebug()<<"finish using pen";
        is_drawing = false;
        is_editing = false;
        cur_figure = nullptr;
    }
    else if(type_of_draw == fillcolor) //填充
    {
        if(is_drawing == true && is_editing == true)
        {
            is_drawing = false;
            is_editing = false;
            update();
        }
    }
    else if(type_of_draw == polygon ) //多边形
    {
        //判断是否完成画图
        if(is_drawing == true || is_editing == true)
        {
            if(is_drawing == true && is_editing == false && this->cur_figure->is_polyon_finished() == true) //完成画图
            {
                *temp_draw_area = *cur_draw_area;
                my_paint(temp_draw_area);
                qDebug()<<"begin to edit";
                is_drawing = false;
                is_editing = true;
            }
            if(is_editing == true)
             {
                 qDebug()<<"end of editing";
                 QPainter *painter = new QPainter;		//新建一个QPainter对象
                 painter->begin(temp_draw_area);			//在当前PIXMAP进行绘制
                 painter->setPen(pen);        			//将QPen对象应用到绘制对象中
                 cur_figure->show_edit_func(painter); //利用这个函数来进行对与编辑窗口的展示
                 painter->end();
                 update();
             }
        }
    }
    else if(type_of_draw == Bezier)
    {
        if(is_drawing == true || is_editing == true)
        {
            if(is_drawing == true && is_editing == false) //完成画图
            {
                *temp_draw_area = *cur_draw_area;
                my_paint(temp_draw_area);
            }
        }
    }
    else if(is_drawing == true) //绘画结束
    {
        cur_figure->set_end_point(end_Pos);
        cur_figure->add_into_set(end_Pos);
        qDebug()<<"begin to edit";
        is_drawing = false;
        is_editing = true; //进入编辑模式
        QPainter *painter = new QPainter;		//新建一个QPainter对象
        painter->begin(temp_draw_area);			//在当前PIXMAP进行绘制
        painter->setPen(pen);        			//将QPen对象应用到绘制对象中
        cur_figure->show_edit_func(painter); //利用这个函数来进行对与编辑窗口的展示
        painter->end();
        update();
    }
    else if(is_editing == true) //编辑结束，仍然是编辑模式
    {
        qDebug()<<"end of editing";
        QPainter *painter = new QPainter;		//新建一个QPainter对象
        painter->begin(temp_draw_area);			//在当前PIXMAP进行绘制
        painter->setPen(pen);        			//将QPen对象应用到绘制对象中
        cur_figure->show_edit_func(painter); //利用这个函数来进行对与编辑窗口的展示
        painter->end();
        update();
    }
    else
    {
        qDebug()<<"do nothing";
    }
}

void myWidget::paintEvent(QPaintEvent *)  //画画
{
    QPainter painter(this);
    if((is_drawing == true  || is_editing == true)&& type_of_draw != thepen){
        painter.drawPixmap(QPoint(0,0),*temp_draw_area);//-------如果正在或正在编辑绘图，既鼠标点击或者正在移动，画在tempImage上
     }
    else if(is_drawing == false && is_editing == false) //进行保存函数
    {
        painter.drawPixmap(QPoint(0,0),*cur_draw_area);//-----------如果鼠标释放，将图保存在image上
    }
    else if(type_of_draw == thepen && is_drawing == true){
        painter.drawPixmap(QPoint(0,0),*cur_draw_area);
    }
}

void myWidget::resizeEvent(QResizeEvent *)//
{

}

void myWidget:: my_paint (QPixmap *the_iamge){
    QPainter *painter = new QPainter;		//新建一个QPainter对象
    painter->begin(the_iamge);			//在当前PIXMAP进行绘制
    painter->setPen(pen);					//将QPen对象应用到绘制对象中
    switch (type_of_draw)
    {
            case none:
            {
                qDebug()<<"pick"<<endl;
                break;
            }
            case thepen:
            {
                qDebug()<<"draw point"<<endl;
                draw_pen(painter,start_Pos,end_Pos);
                break;
            }
            case line:
            {
                qDebug()<<"draw line"<<endl;
                this->cur_figure->draw_(painter,cur_figure->get_start_pos(),cur_figure->get_end_pos());
                break;
            }
            case circle:
            {
                qDebug()<<"draw circle"<<endl;
                this->cur_figure->draw_(painter,cur_figure->get_start_pos(),cur_figure->get_end_pos());
                break;
            }
            case ellipse:
            {
                qDebug()<<"drwa ellipse"<<endl;
                this->cur_figure->draw_(painter,cur_figure->get_start_pos(),cur_figure->get_end_pos());
                break;
            }
            case polygon:
            {
                qDebug()<<"drwa polygon"<<endl;
                this->cur_figure->draw_(painter,cur_figure->get_start_pos(),cur_figure->get_end_pos());
                break;
            }
            case fillcolor:
            {
                 qDebug()<<"drwa fillcolor"<<endl;
                 this->fillColor(&this->cur_image,this->pcolor,painter,this->fill_start_pos);
                 break;
            }
            case Bezier:
            {
                qDebug()<<"drwa Bezier"<<endl;
                this->cur_figure->draw_(painter,cur_figure->get_start_pos(),cur_figure->get_end_pos());
                break;
            }
            default:
            {
                qDebug()<<"none"<<endl;
                break;
            }
    }
    if(this->type_of_draw == polygon || this->type_of_draw == Bezier)
    {
        if(this->is_editing == true || this->is_drawing == true)
        {
            cur_figure->show_edit_func(painter);
        }
    }
    else if(this->is_editing == true && type_of_draw != fillcolor)
    {
        cur_figure->show_edit_func(painter);
    }
    painter->end();
    update();						//重绘绘制区窗体
}

QPixmap *myWidget::getPixCopy()
{
    QPixmap *newPix = new QPixmap(801,541);	//创建一个新的QPixmap对象
    newPix->fill(Qt::white);                //填充新QPixmap对象newPix的颜色为白色背景色
    QPainter p(newPix);
    p.drawPixmap(QPoint(0,0),*cur_draw_area);         //在newPix中绘制原pix中的内容
    qDebug() <<"Pix Copy at"<<&newPix<<endl;
    return newPix;
}

void myWidget::set_draw_area_back() //撤销
{
    qDebug() <<"return"<<draw_area.size();
    if((int)draw_area.size() >=2)
    {
       draw_area.pop_back();
    }
    if((int)draw_area.size() >= 1) //当前超过
    {
        QPixmap *temp = draw_area.back();
        if(temp == nullptr)
        {
            qDebug()<<"错误没有指针";
            assert(0);
        }
        *cur_draw_area = *temp;
        update();
    }
}

void myWidget::set_draw_area_clear()//清屏
{
    QPixmap *newPix = new QPixmap(801,541);	//创建一个新的QPixmap对象
    newPix->fill(Qt::white);                //填充新QPixmap对象newPix的颜色为白色背景色
    *cur_draw_area = *newPix;
    draw_area.push_back(newPix);
    update();
}

void myWidget::set_pen_width(int i) //设置笔的宽度
{
         pen.setWidth(i);	//改变宽度
}

void myWidget::set_pen_color(QString i)//设置笔的颜色
{
      pen.setColor(i);
}

 void myWidget::save_pixmap()//保存图片到固定的路径，TOOO:设置到指定的路径
 {
     QString fileName = QFileDialog::getSaveFileName(nullptr,tr("保存文件"),
                                                        tr("C:/Users/dell/Desktop/未命名.png"),
                                                        tr("file(*.png )"));
     qDebug()<<"save";
     if(fileName != nullptr)
     {
        cur_draw_area->save(fileName);
        QMessageBox::information(nullptr,tr("提示"),tr("导出成功!"));
     }
     else
     {
        QMessageBox::warning(nullptr,tr("警告"),tr("文件名未指定!"),QMessageBox::Ok);
     }
 }

void myWidget::fillColor(QImage *img, QColor backcolor, QPainter *painter, QPoint t)//实现填充算法
{
    QPen init = painter->pen();
    QPen temp = painter->pen();
    temp.setWidth(1);
    painter->setPen(temp);
    QStack<QPoint> *stack = new QStack<QPoint>;
    stack->clear();
    int maxWidth = this->cur_draw_area->width()-1;
    int maxHeight = this->cur_draw_area->height()-1;
    bool **point_used = new bool*[maxWidth];
    for(int i=0;i<maxWidth;i++)
    {
        point_used[i] = new bool[maxHeight];
    }
    for(int i=0;i<maxWidth;i++)
    {
        for(int j=0;j<maxHeight;j++)
        {
            point_used[i][j] = false;
        }
    }
    this->is_procced = point_used;
    stack->push(t);    //要填充的press的那个点
    while(!stack->empty()) //当栈不为0的时候
    {
        QPoint p = stack->pop();
        int x = p.x();
        int y = p.y();

        if(this->is_procced[x][y] == true) continue;      //已经上色
        this->is_procced[x][y] = true;
        if(img->pixelColor(x,y) != backcolor) continue;   //边界
        if(x<=0 || x>=this->cur_draw_area->width()-2) continue;     //超限
        if(y<=0 || y>=this->cur_draw_area->height()-2) continue;    //超限

        painter->drawPoint(x,y);
        if(!this->is_procced[x][y+1]) stack->push(QPoint(x,y+1));
        if(!this->is_procced[x][y-1]) stack->push(QPoint(x,y-1));
        if(!this->is_procced[x-1][y]) stack->push(QPoint(x-1,y));
        if(!this->is_procced[x+1][y]) stack->push(QPoint(x+1,y));
    }
    stack->clear();
    painter->setPen(init);
}


void  myWidget::closeEvent(QCloseEvent *e)
{
     this->is_closed_by = true;
     QMessageBox::StandardButton rb = QMessageBox::warning(nullptr, "warning", "是否进行保存？",
                                                      QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
    if(rb == QMessageBox::Yes)
    {
        this->save_pixmap();
    }
}

void  myWidget:: set_cuting()
{
    if(this->type_of_draw == line && this->is_editing == true && this->cur_figure != nullptr)//是直线，并且是正在编辑的
    {
        if(this->is_cuting == false) //如果不是的话就执行is_cuting操作
        {
            this->is_cuting = true;
        }
        else //裁剪
        {
            this->is_cuting = false;
            this->cur_figure->set_cuting(false); //不进行
            bool t = this->cur_figure->cut_line(); //进行裁剪函数
            if(t == false) //没有了
            {
                this->is_editing = false;
            }
            qDebug()<<"完成裁剪";
            *temp_draw_area = *cur_draw_area;
            my_paint(temp_draw_area);
        }
    }
}

void myWidget::zoom_in()
{
    if(this->is_editing == true && this->cur_figure != nullptr)
    {
        this->cur_figure->zoom_in_point();
        *temp_draw_area = *cur_draw_area;
        my_paint(temp_draw_area);
    }
}

void myWidget::zoom_out()
{
    if(this->is_editing == true && this->cur_figure != nullptr)
    {
        this->cur_figure->zoom_out_point();
        *temp_draw_area = *cur_draw_area;
        my_paint(temp_draw_area);
    }
}
