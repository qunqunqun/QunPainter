#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
QMainWindow(parent),
ui(new Ui::MainWindow)
{

    ui->setupUi(this);
    setWindowTitle(tr("QunPaint"));

    ui->mdiArea->setViewMode(QMdiArea::TabbedView);
    ui->mdiArea->setTabShape(QTabWidget::Triangular);   //设置窗口的形状
    ui->mdiArea->setTabPosition(QTabWidget::North); //设置窗口的位置
    ui->mdiArea->setTabsClosable(false);         //可关闭的
    ui->mdiArea->setTabsMovable(true);        //可移动的
    this->cur_3D_widget = nullptr;
    this->cur_widget = nullptr;
    //初始化样式
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_choose_clicked() //选择
{
    if(this->cur_widget == nullptr) return;
    qDebug()<<"pick mouse"<<endl;
    cur_widget->set_type_to_none();
}

void MainWindow::on_pen_clicked()
{
    if(this->cur_widget == nullptr) return;
    qDebug()<<"pick pen"<<endl;
    cur_widget->set_type_to_pen();
}

void MainWindow::on_line_clicked() //画直线
{
    if(this->cur_widget == nullptr) return;
    qDebug()<<"pick line"<<endl;
    cur_widget->set_type_to_line();
}

void MainWindow::on_circle_clicked() //画圆
{
    if(this->cur_widget == nullptr) return;
    qDebug()<<"pick circle"<<endl;
    cur_widget->set_type_to_circle();
}

void MainWindow::on_ellipse_clicked()//画椭圆
{
    if(this->cur_widget == nullptr) return;
    qDebug()<<"pick ellipse"<<endl;
    cur_widget->set_type_to_ellipse();
}

void MainWindow::on_polygon_clicked() //多边形
{
    if(this->cur_widget == nullptr) return;
    qDebug()<<"pick polygon"<<endl;
    cur_widget->set_type_to_polygon();
}


void MainWindow::on_Bezier_clicked()
{
    if(this->cur_widget == nullptr) return;
    qDebug()<<"pick Bezier"<<endl;
    cur_widget->set_type_to_Bezier();
}

void MainWindow::on_actions_T_triggered()
{
      this->ui->mdiArea->closeAllSubWindows();
      this->cur_widget = nullptr;
      this->cur_3D_widget = nullptr;
      myWidget *draw_ = new myWidget;
      QMdiSubWindow * t = ui->mdiArea->addSubWindow(draw_); //加入视图
      draw_->showMaximized(); //使得最大化
      cur_widget = draw_;
      ui->mdiArea->setActiveSubWindow(t);
      t->setWindowTitle(tr("draw_widget").arg(1));
      t->show();
}


void MainWindow::on_fillcolor_clicked()
{
    if(this->cur_widget == nullptr) return;
    cur_widget->set_type_to_fillcolor();
}

void MainWindow::on_to_back_clicked() //撤销
{
    if(this->cur_widget == nullptr) return;
    cur_widget->set_draw_area_back();
}

void MainWindow::on_clear_clicked() //清屏
{
    if(this->cur_widget == nullptr) return;
    cur_widget->set_draw_area_clear();
}

void MainWindow::on_comboBox_currentIndexChanged(int index)
{
    if(this->cur_widget == nullptr) return;
    cur_widget->set_pen_width(index + 2);
    this->cur_widget->update();
}

void MainWindow::on_comboBox_2_currentTextChanged(const QString &arg1)
{
    if(this->cur_widget == nullptr) return;
    cur_widget->set_pen_color(arg1);
    this->cur_widget->update();
}


void MainWindow::on_action_S_triggered()
{
    if(this->cur_widget == nullptr) return;
    cur_widget->save_pixmap();
}


void MainWindow::on_caijian_clicked()
{
    if(this->cur_widget == nullptr) return;
    cur_widget->set_cuting(); //设置裁剪
}

void MainWindow::on_zoomin_clicked()
{
    if(this->cur_widget == nullptr) return;
    cur_widget->zoom_in();
}

void MainWindow::on_zoomout_clicked()
{
    cur_widget->zoom_out();
}

void MainWindow::on_actionnew3D_triggered()
{
    this->ui->mdiArea->closeAllSubWindows();
    this->cur_widget = nullptr;
    this->cur_3D_widget = nullptr;
    my_3DWidget *draw_ = new my_3DWidget;
    QMdiSubWindow * t = ui->mdiArea->addSubWindow(draw_); //加入视图
    draw_->showMaximized(); //使得最大化
    cur_3D_widget = draw_;
    ui->mdiArea->setActiveSubWindow(t);
    t->setWindowTitle(tr("draw_widget_3D").arg(1));
    t->show();
}

void MainWindow::on_actionclose_triggered()
{
    this->ui->mdiArea->closeAllSubWindows();
    this->cur_widget = nullptr;
    this->cur_3D_widget = nullptr;
}

void MainWindow::on_read3D_clicked()
{
    if(cur_3D_widget == nullptr) return;
    this->cur_3D_widget->init();
}

void MainWindow::closeEvent(QCloseEvent *e) //关闭
{
    if(this->cur_widget != nullptr)
    {
        QMessageBox::StandardButton rb = QMessageBox::warning(nullptr, "warning", "是否进行退出？",
                                                         QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
        if(rb == QMessageBox::No)
        {
            e->ignore(); //忽略
        }
    }

}

void MainWindow::on_actions_H_triggered()
{
    QMessageBox::about(this, tr("关于 QunPaint"),
               tr("<p>"
                  "<b>QunPaint</b>是是一个画图程序，该程序使用C++和Qt框架开发"
                  "是南京大学计算机科学与技术系《图形学》课程的大作业"
                  "目前的版本实现了画笔、直线、圆、椭圆、多边形、曲线的输入和编辑、平移、放缩等功能，"
                  "可以选择颜色，也可以填充一块区域的颜色。"
                  "除此之外，还基于梁友栋算法实现了直线的裁剪。"
                  "欢迎使用。<br/>"
                  "有任何疑问可以发送邮件<a href=\"mailto:986300572@qq.com\">986300572@qq.com</a><br/>"
                  "更多详情，可以到我的github pages上查看<a href=\"https://github.com/qunqunqun\" target=\"_blank\"><b>qunqunqun的个人主页</b></a>"
                  "<hr/>"
                  "<p style=\"text-align: right\">戚赟 2018.12</p></p>"
                 ));
}
