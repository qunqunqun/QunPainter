#ifndef MYWIDGET_H
#define MYWIDGET_H

#include <QWidget>
#include<myopengl.h>
#include<QPainter>
#include<qpainter.h>
#include<QMouseEvent>
#include<QVector>
#include<QPen>
#include<QPixmap>
#include<QtDebug>
#include"Figure.h"


class myWidget : public QWidget
{
    Q_OBJECT
public:
    explicit myWidget(QWidget *parent = nullptr);
    void mousePressEvent(QMouseEvent *e) Q_DECL_OVERRIDE; //鼠标按压
    void mouseMoveEvent(QMouseEvent *e) Q_DECL_OVERRIDE; //鼠标移动
    void mouseReleaseEvent(QMouseEvent *e) Q_DECL_OVERRIDE; //鼠标释放
    void paintEvent(QPaintEvent *) Q_DECL_OVERRIDE; //画画
    void resizeEvent(QResizeEvent *e) Q_DECL_OVERRIDE; //
    void set_type_to_none();
    void set_type_to_pen();
    void set_type_to_line();
    void set_type_to_rectangle();
    void set_type_to_circle();
    void set_type_to_ellipse();
    void set_type_to_polygon();
    void set_type_to_fillcolor();
    void set_type_to_Bezier(); //实现曲线
    void my_paint (QPixmap *the_iamge); //画图
    void set_draw_area_back(); //撤销
    void set_draw_area_clear();//清屏
    void set_pen_width(int i);//设置笔长
    void set_pen_color(QString i);//设置笔的颜色
    void save_pixmap();
    void fillColor(QImage *img, QColor backcolor, QPainter *painter, QPoint t);
    void set_cuting();
    QPixmap *getPixCopy();
    void closeEvent(QCloseEvent *e);
    void zoom_in();
    void zoom_out();
    //编辑
    bool set_new_figure(Figure *& temp); //设置
    bool is_closed_by;
private:
    myopenGL*s;
    QVector<QPixmap*> draw_area; //用于撤销和清空
    QPixmap *cur_draw_area; //用于显示当前的Pixmap
    QPixmap *temp_draw_area;//双缓冲绘图的
    type_draw type_of_draw; //种类
    bool is_drawing; //是否在画画,用来双缓冲
    bool draw_or_not;
    QPen pen; //画笔
    QPoint start_Pos; //开始点
    QPoint end_Pos; //结束点

    //编辑操作，动态绑定
    Figure * cur_figure;
    bool is_editing;
    int is_moving;
    int is_rotating;
    int is_resizing;
    bool is_polygon; //给多边形额外的参数
    bool is_cuting; //是否是正在裁剪的
    //填充需要的变量
    QPoint fill_start_pos;
    QImage cur_image;//填充的image
    QColor pcolor; //填充的颜色
    bool **is_procced; //是否处理过

signals:

public slots:

};

#endif // MYWIDGET_H
