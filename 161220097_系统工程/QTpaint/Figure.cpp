#include "Figure.h"
#include"QDebug.h"
Figure::Figure()
{
    point_of_move.clear(); //平移点
    point_of_rotate.clear(); //旋转
    point_of_resize.clear(); //实现编辑大小
    point_of_cut.clear(); //实现裁剪，主要是直线
    this->set_of_point.clear(); //多边形进行清空
    type_of_figure = none; //设置成无图形属性
    num_of_resizing = -1;
    this->finished = false; //为空
}

bool is_near(QPoint t,QPoint dest)
{
    double dx =  t.rx() - dest.rx() ;
    double dy =  t.ry() - dest.ry() ;
    double d = dx*dx + dy * dy;
    if(d <= 25)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void Figure::zoom_in_point()
{
    this->point_begin.rx() = this->point_begin.rx() * 2;
    this->point_begin.ry() = this->point_begin.ry() * 2;
    this->point_end.rx() = this->point_end.rx() * 2;
    this->point_end.ry() = this->point_end.ry() * 2;
    for(int i = 0; i < this->set_of_point.length();i++){
        this->set_of_point[i].rx() = this->set_of_point[i].rx() * 2;
        this->set_of_point[i].ry() = this->set_of_point[i].ry() * 2;
    }
}

void Figure::zoom_out_point()
{
    this->point_begin.rx() = this->point_begin.rx() / 2;
    this->point_begin.ry() = this->point_begin.ry() / 2;
    this->point_end.rx() = this->point_end.rx() / 2;
    this->point_end.ry() = this->point_end.ry() / 2;
    for(int i = 0; i < this->set_of_point.length();i++){
        this->set_of_point[i].rx() = this->set_of_point[i].rx() / 2;
        this->set_of_point[i].ry() = this->set_of_point[i].ry() / 2;
    }
}

void Figure::set_Bezier_finished(bool t)
{
      this->Bezier_finished = t; //是true还是false
}

void Figure:: set_draw_line_or_not(bool t)
{
       this->draw_line_or_not = t;
}

void Figure::set_cuting(bool t)
{
      this->is_cuting = t;
}

void Figure:: add_into_cut(QPoint t) //将点加入编辑
{
    this->point_of_cut.clear();
    this->point_of_cut.push_back(t); //加入
}

void Figure::set_end_cut(QPoint t)
{
    assert(this->point_of_cut.length()!= 0);
    if(this->point_of_cut.length() == 1)
    {
        this->point_of_cut.push_back(t);
    }
    else
    {
        this->point_of_cut[1] = t; //改变最后一点
    }
}

void Figure::set_begin_point(QPoint begin) //设置开始点坐标
{
    this->point_begin = begin;
}

void Figure::set_end_point(QPoint end)//设置结束点的坐标
{
    this->point_end = end;
    if(this->type_of_figure == polygon || this->type_of_figure == Bezier) //多边形
    {
        int t = (int)this->set_of_point.size() - 1;
        if(t > 0)
        {
            this->set_of_point[t] = end; //改变最后一个点
            qDebug()<<"change last point";
        }
    }
}

bool catch_the_point(QPoint &a,QPoint &b)
{
    int dis = (a.rx()-b.rx())*(a.rx()-b.rx()) + (a.ry()-b.ry())*(a.ry()-b.ry());
    if(dis <= 9)
    {
        return true;
    }
    else return false;
}


int Figure::is_to_move(QPoint temp)  //移动
{
    for(int i = 0; i < this->point_of_move.size();i++)
    {
        if(catch_the_point(point_of_move[i],temp) == true)
        {
            return i;
        }
    }
    return -1;
}

int Figure::is_to_rotate(QPoint temp) //旋转
{
    for(int i = 0; i < this->point_of_rotate.size();i++)
    {
        if(catch_the_point(point_of_rotate[i],temp) == true)
        {
            return i;
        }
    }
    return -1;
}

int Figure::is_to_resize(QPoint temp) //重新编辑大小
{
    for(int i = 0; i < this->point_of_resize.size();i++)
    {
        if(catch_the_point(point_of_resize[i],temp) == true)
        {
            return i;
        }
    }
    return -1;
}


QPoint  Figure:: get_start_pos()
{
    return this->point_begin;
}

QPoint  Figure::get_end_pos()
{
    return  this->point_end;
}


bool Figure::is_polyon_to(QPoint &t) //判断多边形是否为空
{
    if(this->set_of_point.empty() == true) //没有点
    {
        return true;
    }
    else
    {
         if(is_near(t,set_of_point[0]) == true)
         {
             t = set_of_point[0]; //改变最后一个定点
             return false;
         }
         else
             return  true;
    }
}

void Figure::add_into_set(QPoint t)
{
    this->set_of_point.push_back(t);
    if(this->type_of_figure == Bezier || this->type_of_figure == polygon)
        this->point_of_resize.push_back(t);
}

int Figure::num_of_set()
{
    return this->set_of_point.size(); //返回为真
}

bool  Figure:: is_polyon_finished()
{
    if(this->set_of_point.size()<2)
    {
        return false;
    }
    QPoint t = this->set_of_point.back(); //最后一个元素
    if(is_near(t,this->set_of_point[0]) == true)        //如果相同
    {
        t  = this->set_of_point[0];
        this->set_of_point.pop_back();
        this->set_of_point.push_back(t);
        return true;
    }
    return false;
}

double  Figure::get_2_distance(QPoint a,QPoint b) //获取两个点的坐标的距离平方
{
    int dx = a.rx()-b.rx();
    int dy = a.ry()- b.ry();
    double dis2 =  dx * dx + dy * dy;
    return dis2;
}

bool Figure::cut_line()
{
    assert(this->type_of_figure == line); //一定是直线
    //梁友栋直线裁剪算法
    int x_min = this->point_of_cut[0].rx();
    int x_max = this->point_of_cut[1].rx();
    int y_min = this->point_of_cut[0].ry();
    int y_max = this->point_of_cut[1].ry();
    if(x_min > x_max)
    {
        int t = x_min; x_min = x_max; x_max =t;
    }
    if(y_min >y_max)
    {
         int t = y_min; y_min = y_max; y_max =t;
    }
    //设置完矩形的最小X和最大X
    int x_start = this->point_begin.rx();
    int y_start = this->point_begin.ry();
    int x_end = this->point_end.rx();
    int y_end = this->point_end.ry(); //四个点值
    double u_1 = 0, u_2 = 1;
    double p[4],q[4];
    p[0] = x_start - x_end;
    p[1] = -p[0];
    p[2] = y_start - y_end;
    p[3] = -p[2]; //取反
    q[0] = x_start - x_min;
    q[1] = x_max - x_start;
    q[2] = y_start - y_min;
    q[3] = y_max - y_start;
    bool flag = false;
    for(int i = 0; i < 4; i ++)
    {
        double r = q[i] / p[i];
        if(p[i] < 0)
        {
            u_1 = u_1 > r ? u_1 : r;
            if(u_1 > u_2)
            {
                flag = true;
            }
        }
        else if(p[i] > 0)
        {
            u_2 = u_2 < r ? u_2 : r;
            if(u_1 > u_2)
            {
                flag = true;
            }
        }
        else if(p[i] == 0 && q[i] < 0)
        {
            flag = true; //
        }
    }
    if(flag == true)
    {
        this->set_of_point.clear(); //清空
        this->point_begin.rx() = -1;
        this->point_begin.ry() = -1;
        this->point_end.rx() = -1;
        this->point_end.ry() = -1;
        return false;
    }
    else
    {
        this->point_begin.rx() = x_start - (x_start - x_end) * u_1;
        this->point_end.rx() = x_start - (x_start - x_end) * u_2;
        this->point_begin.ry() = y_start - (y_start -y_end) * u_1;
        this->point_end.ry() =  y_start - (y_start -y_end) * u_2;
        this->set_of_point.clear();
        this->set_of_point.push_back(this->point_begin);
        this->set_of_point.push_back(this->point_end);
        return true;
    }
}
