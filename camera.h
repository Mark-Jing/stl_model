#ifndef CAMERA_H
#define CAMERA_H

#include <QVector3D>
#include <QMatrix4x4>
#include <QObject>

class Camera : public QObject {
public:
    Camera();
    void look(float yaw, float pitch, float roll);
    void move(float x, float y, float z);
    QVector3D getPos() {
        return this->m_pos;
    }
    QVector3D getUp() {
        return this->m_up;
    }
    QVector3D getFront() {
        return this->m_front;
    }
    void update();
    QMatrix4x4 getView() {
        return this->view;
    }
protected:
    bool eventFilter(QObject*, QEvent*) override;
private:
    QMatrix4x4 view;
    QVector3D m_pos{0,0,0};
    QVector3D m_front{0,0,-1};
    QVector3D m_up{0,1,0};
    QVector3D m_move;
    float m_yaw = 0;
    float m_pitch = 0;
    float m_roll = 0;
    float m_lookSpeed = 0.5;
    float m_moveSpeed = 0.5;
};

#endif // CAMERA_H
