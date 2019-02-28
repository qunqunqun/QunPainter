#ifndef ELLIPSE_H
#define ELLIPSE_H
#include<QPainter>
#include"Figure.h"

class myEllipse : public Figure //从Figure进行继承
{
public:
    myEllipse();
    void show_edit_func(QPainter * painter);     //显示编辑点
    void draw_(QPainter * painter,QPoint centre,QPoint end); //每个图形进行绘制
    bool move_(QPainter *painter,int change_rx,int change_ry);
    bool rotate_(QPainter *painter,QPoint end_pos);
    bool resize_(QPainter *painter,QPoint end_pos,int num);
    void change_point_by_angle(QPoint &t, double sina,double cosa);
    void draw_point_of_ellipse(QPainter * painter, int x_centre,int y_centre,int x,int y);
    void add_by_x_y(int x,int y);
    void rotate_by_angle();
private:
    double rotate_angle; //旋转角的大小，为弧度制
    bool is_rotating; //是否在旋转
    bool is_moving;
    bool is_resizing;
    //关于旋转的
    QVector<QPoint> fixed_point_of_move; //平移点
    QVector<QPoint> fixed_point_of_rotate; //旋转
    QVector<QPoint> fixed_point_of_resize; //实现编辑大小
};



void draw_ellipse_one(QPainter * painter,QPoint &centre,QPoint &end);
void draw_ellipse_two(QPainter * painter,QPoint &centre,QPoint &end);

#endif // ELLIPSE_H
