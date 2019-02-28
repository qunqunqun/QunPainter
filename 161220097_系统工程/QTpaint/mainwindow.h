#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QScrollArea>
#include<myopengl.h>
#include<mywidget.h> //画板的使用
#include <QComboBox> //组合键的使用
#include<QVector>
#include <QPainter> //绘制图片
//#include<QtDebug> //debug输出
#include<QtDebug>
#include<QMdiSubWindow>
#include<3DWidget.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void refresh_widget();
    void refresh_3Dwifget();
    void closeEvent(QCloseEvent *e);
private slots:

    void on_choose_clicked();

    void on_line_clicked();

    void on_actions_T_triggered();

    void on_circle_clicked();

    void on_ellipse_clicked();

    void on_pen_clicked();

    void on_to_back_clicked();

    void on_clear_clicked();

    void on_comboBox_currentIndexChanged(int index);

    void on_comboBox_2_currentTextChanged(const QString &arg1);

    void on_action_S_triggered();

    void on_polygon_clicked();

    void on_fillcolor_clicked();

    void on_Bezier_clicked();

    void on_caijian_clicked();

    void on_zoomin_clicked();

    void on_zoomout_clicked();

    void on_actionnew3D_triggered();

    void on_actionclose_triggered();

    void on_read3D_clicked();

    void on_actions_H_triggered();

private:
    Ui::MainWindow *ui;
    myWidget* cur_widget;
    my_3DWidget * cur_3D_widget;
    QMdiSubWindow * cur_subwindow;
};



#endif // MAINWINDOW_H
