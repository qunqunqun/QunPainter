#ifndef LINE_H
#define LINE_H
#include<QPainter>
#include"Figure.h"

class myLine : public Figure //从Figure进行继承
{
public:
    myLine();
    void show_edit_func(QPainter * painter);     //显示编辑点
    void draw_(QPainter * painter,QPoint begin,QPoint end); //每个图形进行绘制
    bool move_(QPainter *painter,int change_rx,int change_ry);
    bool rotate_(QPainter *painter,QPoint end_pos);
    bool resize_(QPainter *painter,QPoint end_pos,int num);
};

#endif // LINE_H
