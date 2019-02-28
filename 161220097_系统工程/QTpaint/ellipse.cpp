#include "Ellipse.h"
#include<cmath>
#include<QDebug>

myEllipse::myEllipse()
{
    this->type_of_figure = ellipse;
    this->is_rotating = false; //没有在旋转
    this->is_moving = false;
    this->is_resizing = false;
    this->rotate_angle = 0;//设置旋转角为0
    this->fixed_point_of_move.clear(); //平移点
    this->fixed_point_of_rotate.clear(); //旋转
    this->fixed_point_of_resize.clear(); //实现编辑大小
}

void myEllipse::add_by_x_y(int x,int y)
{
    this->point_begin.rx() += x;
    this->point_begin.ry() += y;
    this->point_end.rx() += x;
    this->point_end.ry() += y;
    this->point_center = this->point_begin; //判断
    for(int i = 0; i <this->point_of_move.size();i++)
    {
        this->point_of_move[i].rx() += x;
        this->point_of_move[i].ry() += y;
        this->fixed_point_of_move[i].rx() += x;
        this->fixed_point_of_move[i].ry() += y;
    }
    for(int i = 0; i <this->point_of_resize.size();i++)
    {
        this->point_of_resize[i].rx() += x;
        this->point_of_resize[i].ry() += y;
        this->fixed_point_of_resize[i].rx() += x;
        this->fixed_point_of_resize[i].ry() += y;
    }
    for(int i = 0; i <this->point_of_rotate.size();i++)
    {
        this->point_of_rotate[i].rx() += x;
        this->point_of_rotate[i].ry() += y;
        this->fixed_point_of_rotate[i].rx() += x;
        this->fixed_point_of_rotate[i].ry() += y;
    }
}

void myEllipse:: rotate_by_angle()
{
    for(int i = 0; i <this->point_of_move.size();i++)
    {
        this->change_point_by_angle(this->point_of_move[i],sin(this->rotate_angle),cos(this->rotate_angle));
    }
    for(int i = 0; i <this->point_of_resize.size();i++)
    {
        this->change_point_by_angle(this->point_of_resize[i],sin(this->rotate_angle),cos(this->rotate_angle));
    }
    for(int i = 0; i <this->point_of_rotate.size();i++)
    {
        this->change_point_by_angle(this->point_of_rotate[i],sin(this->rotate_angle),cos(this->rotate_angle));
    }
}

void myEllipse::show_edit_func(QPainter * painter)  //显示编辑点
{
    qDebug()<<"show how to edit";
    QPen pen;
    pen.setWidth(10);					//设置画笔的线宽值
    pen.setColor(Qt::blue);
    pen.setCapStyle(Qt::RoundCap);
    painter->setPen(pen);//改变画笔的颜色
    for(int i = 0; i <this->point_of_move.size();i++)
    {
        qDebug()<<"show how to edit1";
        painter->drawPoint(point_of_move[i]);
    }
    for(int i = 0; i <this->point_of_resize.size();i++)
    {
        qDebug()<<"show how to edit2";
        pen.setColor(Qt::red);
        painter->setPen(pen);//改变画笔的颜色
        painter->drawPoint(point_of_resize[i]);
    }
    for(int i = 0; i <this->point_of_rotate.size();i++)
    {
        qDebug()<<"show how to edit3";
        pen.setColor(Qt::green);
        painter->setPen(pen);//改变画笔的颜色
        painter->drawPoint(point_of_rotate[i]);
    }
}

void myEllipse::draw_point_of_ellipse(QPainter * painter, int x_centre,int y_centre,int x,int y)
{
    QPoint temp1(x_centre+x,y_centre+y);
    QPoint temp2(x_centre+x,y_centre-y);
    QPoint temp3(x_centre-x,y_centre+y);
    QPoint temp4(x_centre-x,y_centre-y);
    this->change_point_by_angle(temp1,sin(this->rotate_angle),cos(this->rotate_angle));
    this->change_point_by_angle(temp2,sin(this->rotate_angle),cos(this->rotate_angle));
    this->change_point_by_angle(temp3,sin(this->rotate_angle),cos(this->rotate_angle));
    this->change_point_by_angle(temp4,sin(this->rotate_angle),cos(this->rotate_angle));
    //绘制这些点
    painter->drawPoint(temp1);
    painter->drawPoint(temp2);
    painter->drawPoint(temp3);
    painter->drawPoint(temp4);
}

void myEllipse::draw_(QPainter * painter,QPoint centre,QPoint end)//每个图形进行绘制
{
    //初始化坐标
    int x_centre = centre.rx();
    int y_centre = centre.ry();
    int x_end = end.rx();
    int y_end = end.ry();
    //椭圆是以centre为中心开始创建的
    int r_x = abs(x_end-x_centre);
    int r_y = abs(y_end-y_centre);
    //得到两个直径
    double sqx = r_x * r_x;
    double sqy = r_y * r_y;

    qDebug()<<"r_x"<<r_x<<endl;
    qDebug()<<"r_y"<<r_y<<endl;
    int x = 0;
    int y = r_y;
    double p_1 = (sqy - sqx * r_y + 0.25 * sqx) ;
    draw_point_of_ellipse(painter,x_centre,y_centre,x,y);
    //开始进行区域一的绘制
    while(sqy * x <= sqx * y)
    {
            //计算P_0
            if(p_1 < 0)
            {
                p_1 = p_1 + 2 * sqy * x + 3 * sqy;
            }
            else
            {
                p_1 = p_1 +  2 * sqy * x  - 2 * sqx * y + 3 * sqy+ 2 * sqx;
                y--;
            }
            //计算这些点
            x++;
            draw_point_of_ellipse(painter,x_centre,y_centre,x,y);
     }
     //进行区域二的绘制
     double p_2 = (sqy * (x + 0.5) * (x + 0.5) + sqx* (y - 1) * (y - 1) - sqx * sqy);
     while(y>0)
     {
         if(p_2 >= 0)
         {
              p_2 = p_2 - 2 * sqx * y + 3 * sqx;
         }
         else
         {
              p_2 = p_2 - 2 * sqx * y + 2 * sqy * x  +  3 * sqx + 2 * sqy;
              x++;
          }
          y--;
          draw_point_of_ellipse(painter,x_centre,y_centre,x,y);
      }
     //画完之后，开始进行特使编辑点的存放
     if(is_rotating == false && is_moving == false && is_resizing == false) //不是编辑的话
     {
        point_of_move.clear(); //平移点
        point_of_rotate.clear(); //旋转
        point_of_resize.clear(); //实现编辑大小
        this->fixed_point_of_move.clear();
        this->fixed_point_of_resize.clear();
        this->fixed_point_of_rotate.clear(); //存入
        //移动点move
        this->point_of_move.push_back(this->point_begin); //设置椭圆心为中点
        this->point_center = this->point_begin; //椭圆中心和起始点一样
        //编辑点resize
        QPoint resize_0(x_centre - r_x ,y_centre - r_y);
        this->point_of_resize.push_back(resize_0);
        QPoint rotate_0(x_centre,y_centre - (r_y)/2); //改变椭圆的旋转点
        this->point_of_rotate.push_back(rotate_0);

        this->fixed_point_of_move = this->point_of_move;
        this->fixed_point_of_resize = this->point_of_resize;
        this->fixed_point_of_rotate = this->point_of_rotate; //存入
     }
}

bool myEllipse:: move_(QPainter *painter,int change_rx,int change_ry)
{
    this->is_moving = true;
    this->add_by_x_y(change_rx,change_ry); //改变一些点
    this->draw_(painter,this->point_begin,this->point_end);
    this->is_moving = false;
    return true;
}

void myEllipse:: change_point_by_angle(QPoint &t, double sina,double cosa) //随着角度来改变点
{
    QPoint temp_center = this->point_center;
    QPoint t_end = t;
    t.rx() = (t_end.rx()-temp_center.rx() ) * cosa - (t_end.ry() -temp_center.ry()) *sina + temp_center.rx()+ 0.5;
    t.ry() =  (t_end.rx()-temp_center.rx() ) * sina + (t_end.ry() -temp_center.ry()) *cosa + temp_center.ry()+ 0.5;
}

bool myEllipse::rotate_(QPainter *painter,QPoint end_pos)
{
    this->is_rotating = true; //是正在旋转
    //获取旋转角
    QPoint temp_center = this->point_center;
    QPoint temp_begin = this->fixed_point_of_rotate[0]; //开始旋转的点
    this->change_point_by_angle(temp_center,sin(this->rotate_angle),cos(this->rotate_angle));
    this->change_point_by_angle(temp_begin,sin(this->rotate_angle),cos(this->rotate_angle));
    //根据三角型公式来求a的值
    double AB2 = get_2_distance(temp_center,temp_begin);
    double AC2 = get_2_distance(temp_center,end_pos);
    double BC2 = get_2_distance(temp_begin,end_pos); //
    double sina,cosa; //a的值为0到pi
    cosa = (AB2 + AC2 - BC2)/(2 * sqrt(AB2*AC2)); //得到cosa的值
    sina = sqrt(1 - cosa * cosa); //得到sina的值
    qDebug()<<sina<<" "<<cosa;
    //判断是否是正旋转还是负旋转
    if(temp_begin.rx() == temp_center.rx()) //可以知道两个点是一条直线上的，则我们可以知道斜率不存在
    {
        if(temp_begin.ry() > temp_center.ry()) //负旋转
        {
            if(end_pos.rx() < temp_begin.rx())
            {
                sina = -sina; //为负
            }
        }
        else
        {
            if(end_pos.rx() > temp_begin.rx())
            {
                sina = -sina; //为负
            }
        }
    }
    else
    {
        double k = double(temp_begin.ry()-temp_center.ry())/(temp_begin.rx()-temp_center.rx());//算出斜率
        if(end_pos.ry()- k * (end_pos.rx() - temp_center.rx()) - temp_center.ry() > 0) //在直线的下方
        {
            if(temp_begin.rx() < temp_center.rx())
            {
                sina = - sina;
            }
        }
        else if(end_pos.ry()- k * (end_pos.rx() - temp_center.rx()) + temp_center.ry()  > 0) //上方
        {
            if(temp_begin.rx() > temp_center.rx())
            {
                sina = - sina;
            }
        }
    }
    //根据旋转的弧度得出最后偏移的弧度，得出旋转后的旋转点
    double dealt_angle = acos(cosa); //求出cosa
    if(sina >0) //正向旋转
    {
        this->rotate_angle += dealt_angle;
        if(sin(rotate_angle) < 0) //大于180
        {
            this->rotate_angle = this->rotate_angle -  2 * M_PI;
        }
    }
    else //逆向旋转
    {
        this->rotate_angle -= dealt_angle;
        if(sin(rotate_angle) > 0) //负超过180
        {
            this->rotate_angle = this->rotate_angle +  2 * M_PI;
        }
    }
    //得出旋转角，结束绘制图像
    double sinb = sin(this->rotate_angle);
    double cosb = cos(this->rotate_angle);
    for(int i = 0; i < this->point_of_resize.size(); i++)
    {
        QPoint temp_center = this->point_center;
        QPoint t_end = fixed_point_of_resize[i];
        point_of_resize[i].rx() = (t_end.rx()-temp_center.rx() ) * cosb - (t_end.ry() -temp_center.ry()) *sinb + temp_center.rx()+ 0.5;
        point_of_resize[i].ry() = (t_end.rx()-temp_center.rx() ) * sinb + (t_end.ry() -temp_center.ry()) *cosb + temp_center.ry()+ 0.5;
    }
    for(int i = 0; i < this->point_of_rotate.size(); i++)
    {
        QPoint temp_center = this->point_center;
        QPoint t_end = fixed_point_of_rotate[i];
        point_of_rotate[i].rx() = (t_end.rx()-temp_center.rx() ) * cosb - (t_end.ry() -temp_center.ry()) *sinb + temp_center.rx()+ 0.5;
        point_of_rotate[i].ry() = (t_end.rx()-temp_center.rx() ) * sinb + (t_end.ry() -temp_center.ry()) *cosb + temp_center.ry()+ 0.5;
    }
    this->draw_(painter,this->point_begin,this->point_end);
    is_rotating = false; //结束旋转
    return true;
}

bool myEllipse::resize_(QPainter *painter,QPoint end_pos,int num) //重写resize函数，after
{
    is_resizing = true;
    int t = num;
    assert(t >= 0); //t一定是大于0的
    //根据最终的角返回相应的点
    this->change_point_by_angle(end_pos,sin(-this->rotate_angle),cos(-this->rotate_angle));
    this->point_end = end_pos;
    this->draw_(painter,this->point_begin,this->point_end);//画画
    //点的重写
    point_of_move.clear(); //平移点
    point_of_rotate.clear(); //旋转
    point_of_resize.clear(); //实现编辑大小
    this->fixed_point_of_move.clear();
    this->fixed_point_of_resize.clear();
    this->fixed_point_of_rotate.clear(); //存入
    this->fixed_point_of_move.push_back(this->point_begin); //设置椭圆心为中点
    this->point_center = this->point_begin; //椭圆中心和起始点一样
    //编辑点resize
    int r_x = abs(this->point_center.rx()-this->point_end.rx());
    int r_y = abs(this->point_center.ry()-this->point_end.ry());
    this->fixed_point_of_resize.push_back(end_pos);
    QPoint rotate_0(this->point_center.rx(),this->point_center.ry() - (r_y)/2); //改变椭圆的旋转点
    this->fixed_point_of_rotate.push_back(rotate_0);
    this->point_of_move  = this->fixed_point_of_move;
    this->point_of_resize = this->fixed_point_of_resize;
    this->point_of_rotate = this->fixed_point_of_rotate ; //存入
    this->rotate_by_angle(); //改变角度
    is_resizing = false;
    return true;
}
