#include "paintarea.h"


PaintArea::PaintArea()
{
    image = QImage(400,300,QImage::Format_RGB32); // 画 布 的 初 始 化 大 小 设 为400*300，使用 32 位颜色
    backColor = qRgb(255,255,255); //画布初始化背景色使用白色
    image.fill(backColor);
}

void PaintArea::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.drawImage(0,0,image);
}
