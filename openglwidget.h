
#ifndef OPENGLWIDGET_H
#define OPENGLWIDGET_H

#include <QWidget>
#include <QOpenGLWidget>
#include <QOpenGLExtraFunctions>
#include <QMatrix4x4>
#include "camera.h"
#include "model.h"

class OpenGLWidget : public QOpenGLWidget, public QOpenGLExtraFunctions {
    Q_OBJECT
public:
    OpenGLWidget(QWidget *parent = nullptr);
protected:
    virtual void initializeGL() override;
    virtual void resizeGL(int,int) override;
    void extracted();
    virtual void paintGL() override;

    virtual void timerEvent(QTimerEvent *event) override;
private:
    QMatrix4x4 m_projection;
    Camera m_camera;
    QVector<Model*> m_models;
};

#endif // OPENGLWIDGET_H
