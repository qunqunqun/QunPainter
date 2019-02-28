#ifndef PAINTAREA_H
#define PAINTAREA_H
#include<QWidget>
#include <QPainter>

class PaintArea : public QWidget
{
    public:
        PaintArea();
    protected:
        void paintEvent(QPaintEvent *); //重绘事件
    private:
        QImage image; //QImage 类对象，用于在其上绘图
        QRgb backColor; //QRgb 颜色对象，存储 image 的背景色
};

#endif // PAINTAREA_H
