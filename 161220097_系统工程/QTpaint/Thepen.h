#ifndef THEPEN_H
#define THEPEN_H
#include<QPainter>

void draw_pen(QPainter *painter,QPoint start_Pos,QPoint end_Pos);
void draw_line(QPainter * painter,QPoint begin,QPoint end);//每个图形进行绘制
#endif // THEPEN_H
