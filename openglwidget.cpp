#include "dice.h"
#include "openglwidget.h"

OpenGLWidget::OpenGLWidget(QWidget *parent)
    : QOpenGLWidget(parent) {
    startTimer(1000/60);

    m_camera.move(0,4,7);
    m_camera.look(0,30,0);
    m_camera.update();

    installEventFilter(&m_camera);
}

void OpenGLWidget::initializeGL() {
    initializeOpenGLFunctions();
    glClearColor(0, 0.5, 0.7, 1);
    for (int i = 0; i < 1; i++) {
        auto dice = new Dice();
        dice->init();
        dice->setPos({ 0, i * 3.f, 0 });
        m_models << dice;
    }


}

void OpenGLWidget::resizeGL(int w, int h) {
    m_projection.setToIdentity();
    m_projection.perspective(60, (float)w / h, 0.001, 1000);
}

void OpenGLWidget::paintGL() {
    glEnable(GL_DEPTH_TEST);
    for (auto model : qAsConst(m_models)) {
        model->paint(m_projection, m_camera.getView());
    }
}


void OpenGLWidget::timerEvent(QTimerEvent *event) {
    m_camera.update();
    float speed = 1;
//    for (auto model : m_models) {
//        float zed = model->rotate().z() + speed;
//        if (zed >= 360) {
//            zed -= 360;
//        }
//        model->setRotate({0,0,zed});
//        speed++;
//    }
    repaint();
}
