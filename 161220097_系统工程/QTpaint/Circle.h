#ifndef CIRCLE_H
#define CIRCLE_H
#include<QPainter>
#include "Figure.h"

class myCircle : public Figure //从Figure进行继承
{
public:
    myCircle();
    void show_edit_func(QPainter * painter);     //显示编辑点
    void draw_(QPainter * painter,QPoint centre,QPoint end); //每个图形进行绘制
    bool move_(QPainter *painter,int change_rx,int change_ry);
    bool rotate_(QPainter *painter,QPoint end_pos);
    bool resize_(QPainter *painter,QPoint end_pos,int num);
private:
    double R; //半径
};

void draw_circle_Bresenham(QPainter * painter,QPoint &centre,QPoint &end);

#endif // CIRCLE_H
