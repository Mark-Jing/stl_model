#include "camera.h"
#include <QKeyEvent>
#include <QOpenGLWidget>

Camera::Camera() {

}

void Camera::move(float x, float y, float z) {
    QVector3D right = QVector3D::crossProduct(m_front, m_up).normalized();
    m_pos += m_front * z;
    m_pos += right * x;
    m_pos += m_up *y;
}

void Camera::look(float yaw, float pitch, float roll) {
    m_yaw += yaw;
    while (m_yaw >= 360)
        m_yaw -= 360;
    while (m_yaw < 0)
        m_yaw += 360;
    m_pitch += pitch;
    if (m_pitch > 89)
        m_pitch = 89;
    if (m_pitch < -89)
        m_pitch = -89;
    QVector3D _front{ 0,0,-1 };
    QMatrix4x4 _mat;
    _mat.setToIdentity();
    _mat.rotate(m_pitch, 1, 0, 0);
    _mat.rotate(m_yaw, 0, 1, 0);
    m_front = _front * _mat;
}

void Camera::update() {
    auto _move = m_move.normalized() * m_moveSpeed;
    move(_move.x(), _move.y(), _move.z());

    view.setToIdentity();
    view.lookAt(m_pos, m_pos + m_front, m_up);
}

bool Camera::eventFilter(QObject *obj, QEvent *ev) {
//    auto _widget = qobject_cast<QOpenGLWidget *>(obj);
    auto widget = qobject_cast<QOpenGLWidget *>(obj);
    if (widget) {
        if (ev->type() == QEvent::KeyPress) {
            auto event = static_cast<QKeyEvent*>(ev);
            if (event->key() == Qt::Key_Escape) {
                widget->setMouseTracking(false);
                widget->setCursor(Qt::ArrowCursor);
            } else if (event->key() == Qt::Key_W) {
                m_move.setZ(1);
            } else if (event->key() == Qt::Key_S) {
                m_move.setZ(-1);
            } else if (event->key() == Qt::Key_D) {
                m_move.setX(1);
            } else if (event->key() == Qt::Key_A) {
                m_move.setX(-1);
            } else if (event->key() == Qt::Key_Up) {
                m_move.setY(1);
            } else if (event->key() == Qt::Key_Down) {
                m_move.setY(-1);
            }
        } else if (ev->type() == QEvent::KeyRelease) {
            auto event = static_cast<QKeyEvent*>(ev);
            if (event->key() == Qt::Key_W) {
                m_move.setZ(0);
            } else if (event->key() == Qt::Key_S) {
                m_move.setZ(0);
            } else if (event->key() == Qt::Key_D) {
                m_move.setX(0);
            } else if (event->key() == Qt::Key_A) {
                m_move.setX(0);
            } else if (event->key() == Qt::Key_Up) {
                m_move.setY(0);
            } else if (event->key() == Qt::Key_Down) {
                m_move.setY(0);
            }
        } else if (ev->type() == QEvent::MouseButtonPress) {
            auto lastPos = widget->mapToGlobal(widget->rect().center());
            QCursor::setPos(lastPos);
            widget->setMouseTracking(true);
            widget->setCursor(Qt::BlankCursor);
        } else if (ev->type() == QEvent::MouseMove) {
            auto event = static_cast<QMouseEvent*>(ev);
            auto lastPos = widget->mapToGlobal(widget->rect().center());
            QCursor::setPos(lastPos);
            auto move = event->globalPos() - lastPos;
            look(move.x() * m_lookSpeed, move.y() * m_lookSpeed, 0);
        } else if (ev->type() == QEvent::Leave) {
            widget->setMouseTracking(false);
            widget->setCursor(Qt::ArrowCursor);
        }
    }
    return false;
}
