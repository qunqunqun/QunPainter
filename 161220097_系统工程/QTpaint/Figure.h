#ifndef FIGURE_H
#define FIGURE_H
#include<QVector> //实现点的存储
#include<QPainter>

enum type_draw{
    none,fillcolor,thepen,line,circle,ellipse,polygon,Bezier //一共五种图形，增加一个填充功能，
};

class Figure
{
public:
    Figure();
    void set_begin_point(QPoint begin); //设置开始点坐标
    void set_end_point(QPoint end); //设置结束点的坐标
    int is_to_move(QPoint temp);   //移动
    int is_to_rotate(QPoint temp); //旋转
    int is_to_resize(QPoint temp); //重新编辑大小
    QPoint get_start_pos();
    QPoint get_end_pos();
    bool is_polyon_to(QPoint &t); //判断多边形是否为空
    void add_into_set(QPoint t); //将点加入点集
    int num_of_set();        //判断多边形点集是否为空
    int is_ready_to();       //判断多边形是否
    bool is_polyon_finished();
    double get_2_distance(QPoint a,QPoint b); //获取两个点的坐标的距离平方
    void set_Bezier_finished(bool t);
    void set_draw_line_or_not(bool t); //设置是否结束了
    void set_cuting(bool t);
    void add_into_cut(QPoint t); //将点加入编辑
    void set_end_cut(QPoint t); //最后一个点
    bool cut_line(); //对于直线的函数要实现cut_line
    void zoom_in_point();
    void zoom_out_point();
    //虚拟函数，展示继承类相应的属性
    virtual void show_edit_func(QPainter * painter) = 0;     //显示编辑点
    virtual void draw_(QPainter * painter,QPoint centre,QPoint end) = 0; //每个图形进行绘制
    virtual bool move_(QPainter *painter,int change_rx,int change_ry) = 0;
    virtual bool rotate_(QPainter *painter,QPoint end_pos) = 0; //对于图形进行旋转，参数为结束点
    virtual bool resize_(QPainter *painter,QPoint end_pos,int num) = 0; //对于图形进行重新编辑，根据不同参数点执行不同的操作
protected:
    QPoint point_begin; //开始点
    QPoint point_end; //结束点
    QPoint point_center; //中心点
    QPoint *point_fixed; //平移的时候确定的不改变的点
    QVector<QPoint> point_of_move; //平移点
    QVector<QPoint> point_of_rotate; //旋转
    QVector<QPoint> point_of_resize; //实现编辑大小
    QVector<QPoint> point_of_cut; //实现裁剪
    type_draw type_of_figure;
    int num_of_resizing; //编辑功能，保证不改变点
    QVector<QPoint> set_of_point; //只能够在编辑多边形的时候进行输入
    bool finished; //用来保证多边形的裁剪算法
    bool Bezier_finished;
    bool draw_line_or_not;
    bool is_cuting;
};

#endif // FIGURE_H
