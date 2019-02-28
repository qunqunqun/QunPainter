#include"thepen.h"
#include<QDebug>
void draw_line(QPainter * painter,QPoint begin,QPoint end)//每个图形进行绘制
{
    //开始的坐标
    int x_begin = begin.rx();
    int y_begin = begin.ry();
    int x_end = end.rx();
    int y_end = end.ry();
    //DDA计算距离，判断直线的斜率
    double d_x = x_end - x_begin;
    double d_y = y_end - y_begin;
    //计算斜率
    double e = qAbs(d_x) > qAbs(d_y) ? qAbs(d_x) : qAbs(d_y); //判断划分的依据
    double de_x = d_x /e; //X增量
    double de_y = d_y /e; //Y增量
    double temp_x = x_begin;
    double temp_y = y_begin;
    QPen pen = painter->pen();
    for(int i = 0; i <= e; i++)
    {
        QPoint tempPoint((int)(temp_x + 0.5),(int)(temp_y)+0.5); //取最近的整数，而不是向下取整
        painter->drawPoint(tempPoint);
        temp_x += de_x;
        temp_y += de_y;
    }
}

void draw_pen(QPainter *painter,QPoint start_Pos,QPoint end_Pos)
{
    draw_line(painter,start_Pos,end_Pos);
}
