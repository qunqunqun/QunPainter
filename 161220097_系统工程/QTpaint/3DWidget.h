#ifndef CAVANS_3DGL_H
#define CAVANS_3DGL_H
#include <QWidget>
#include <GL/gl.h>
#include <GL/glu.h>
#include <QOpenGLWidget>
#include "QOpenGLFunctions"
#include <QtOpenGL/QtOpenGL>
#include <QKeyEvent>

class my_3DWidget : public QOpenGLWidget,protected QOpenGLFunctions
{
public:
    explicit my_3DWidget(QWidget *parent = nullptr);
    void initializeGL() Q_DECL_OVERRIDE;
    void paintGL() Q_DECL_OVERRIDE;
    void resizeGL(int width, int height) Q_DECL_OVERRIDE;
    void readOffFiles();
    void keyPressEvent(QKeyEvent *event) Q_DECL_OVERRIDE;
    QVector<QString> divide_string(QString a); //将字符串按照空格进行划分
    void normalize_point();
    void drawPoints();
    void drawLines();
    void drawFaces();
    void draw3D();
    void init();
private:

    int numbers_of_Vertices;
    int numbers_of_Faces;
    int numbers_of_Edges;
    QVector<QVector3D> vecs;
    QVector<QVector<int>> faces;
    float angle; //旋转角度
    float offsetX;  //z轴观察距离
    float offsetY;  //z轴观察距离
    float offsetZ;  //z轴观察距离
};

#endif // CAVANS_3DGL_H
