#ifndef BEZIER_H
#define BEZIER_H
#include<QPainter>
#include"Figure.h"

struct PointD
{
   double x;
   double y;
   PointD()
   {
       x = 0;
       y = 0;
   }
   PointD(double a, double b)
   {
       this->x = a;
       this->y = b;
   }
};

class myBezier : public Figure //从Figure进行继承
{
public:
    myBezier();
    void show_edit_func(QPainter * painter);     //显示编辑点
    void draw_(QPainter * painter,QPoint begin,QPoint end); //每个图形进行绘制
    bool move_(QPainter *painter,int change_rx,int change_ry);
    bool rotate_(QPainter *painter,QPoint end_pos);
    bool resize_(QPainter *painter,QPoint end_pos,int num);
    void push_into_Bezier(QPoint t);
    void set_x_y_min_max();
    void change_point_by_angle(QPoint &t, double sina,double cosa);
    void set_is_finished(bool t); //设置是否结束了.
    void drawNodes(QPainter * painter,QVector<PointD> nodes,double st);
    void drawBezier(QPainter * painter,QVector<PointD> nodes);
    PointD  bezier_next_Point(PointD a,PointD b,double st);
private:
    void draw_line(QPainter * painter,QPoint begin,QPoint end,int t); //画直线
    int x_min;
    int y_min;
    int x_max;
    int y_max; //根据这四个点来确定一下外接矩形的大小
    double rotate_angle; //旋转角的大小，为弧度制
    bool is_rotating; //是否在旋转
    QVector<QPoint> BezierNodes;
    double t;
};


#endif // BEZIER_H
