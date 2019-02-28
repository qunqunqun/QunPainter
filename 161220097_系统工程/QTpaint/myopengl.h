#ifndef MYOPENGL_H
#define MYOPENGL_H

#include <QWidget>
#include <GL/gl.h>
#include <GL/glu.h>
#include<QOpenGLWidget>
#include"QOpenGLFunctions"
#include<QtDebug>

class myopenGL : public QOpenGLWidget,protected QOpenGLFunctions
{
    Q_OBJECT
public:
    explicit myopenGL(QWidget *parent = nullptr);
public:
    void initializeGL() Q_DECL_OVERRIDE;
    void paintGL() Q_DECL_OVERRIDE;
    void resizeGL(int width, int height) Q_DECL_OVERRIDE;
signals:

public slots:
};

#endif // MYOPENGL_H
