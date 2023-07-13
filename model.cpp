#include "model.h"

Model::Model() {

}

void Model::setTexture(QOpenGLTexture* texture, int index) {
    if(index == -1) {
        if(m_texture.isEmpty()) {
            index = 0;
        } else {
            index = m_texture.keys().last() + 1;
        }
    }
    m_texture.insert(index, texture);
}

QMatrix4x4 Model::model() {
    QMatrix4x4 mat;
    mat.setToIdentity();
    mat.translate(m_pos);
    mat.rotate(m_rotate.x() ,1, 0, 0);
    mat.rotate(m_rotate.y(), 0, 1, 0);
    mat.rotate(m_rotate.z(), 0, 0, 1);
    mat.scale(m_scale);
    return mat;
}
