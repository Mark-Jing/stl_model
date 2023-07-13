#ifndef MODEL_H
#define MODEL_H

#include <QVector>
#include <QVector2D>
#include <QVector3D>
#include <QMap>
#include <QOpenGLTexture>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLShaderProgram>
#include <QMatrix4x4>
#include <QOpenGLExtraFunctions>

struct stl_vertex {
    float x, y, z;
};

struct Vertex {
    QVector3D pos;
    QVector3D texture;
    Vertex(const QVector3D &pos, const QVector2D &texture) {
        this->pos = pos;
        this->texture = texture;
    }
    Vertex(const stl_vertex& pos) {
        this->pos = QVector3D(pos.x, pos.y, pos.z);
        this->texture = QVector2D(0,0);
    }
};

class Model : public QOpenGLExtraFunctions
{
public:
    Model();
    void setScale(float scale) {
        this->m_scale = scale;
    }
    void setRotate(const QVector3D &rotate) {
        this->m_rotate = rotate;
    }
    void setPos(const QVector3D &pos) {
        this->m_pos = pos;
    }
    void setVertices(const QVector<Vertex> &vertices) { m_vertices = vertices; }
    void setTexture(QOpenGLTexture*, int index = -1);
    void setShaderProgram(QOpenGLShaderProgram *program) {
        m_program = program;
    }
    QMatrix4x4 model();
    QVector3D rotate() {
        return this->m_rotate;
    }
    virtual void init() = 0;
    virtual void update() = 0;
    virtual void paint(const QMatrix4x4& projection, const QMatrix4x4& view) = 0;
protected:
    QVector3D m_pos{0, 0, 0};
    QVector3D m_rotate{0, 0, 0};
    float m_scale = 1;
    QVector<Vertex> m_vertices;
    QMap<int, QOpenGLTexture*> m_texture;
    QOpenGLVertexArrayObject m_vao;
    QOpenGLBuffer m_vbo;
    QOpenGLShaderProgram *m_program = nullptr;
    int vertexFloatCount = 5;
};

#endif // MODEL_H
