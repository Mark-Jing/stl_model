#include <QImage>
#include "dice.h"
#include <QDebug>
#include <iostream>
#include <fstream>
#include <cstdio>
#include <vector>

Dice::Dice() : Model() {
    fill_in_vertices();
    setTexture(new QOpenGLTexture(QImage("C:/dice.png")));
    QOpenGLShaderProgram *program = new QOpenGLShaderProgram();
    program->addShaderFromSourceCode(QOpenGLShader::Vertex, u8R"(
        #version 330 core
        in vec3 vPos;
        in vec2 vTexture;
        out vec2 oTexture;
        uniform mat4 projection;
        uniform mat4 view;
        uniform mat4 model;
        void main() {
            gl_Position = projection * view * model * vec4(vPos, 1.0);
            oTexture = vTexture;
        }
    )");
    program->addShaderFromSourceCode(QOpenGLShader::Fragment, u8R"(
        #version 330 core
        in vec2 oTexture;
        uniform sampler2D uTexture;
        void main() {
            gl_FragColor = texture(uTexture, oTexture);
        }
    )");
    setShaderProgram(program);
}

void Dice::fill_in_vertices() {
    std::string filename = "C:\\intern\\note\\OpenGL\\use_stl\\use_model\\i100.STL";
    std::ifstream file(filename, std::ios::binary);
    if (!file) {
        qDebug() << "failure" << endl;
//        return 1;
    }
    char header[80];
    file.read(header, 80);
    unsigned int numTriangles;
    file.read(reinterpret_cast<char*>(&numTriangles), 4);
    std::vector<stl_vertex> stl_vertices;
    qDebug() << numTriangles;
    for (unsigned int i = 0; i < numTriangles; i++) {
        file.seekg(12, std::ios::cur);
        for (unsigned int j = 0; j < 3; j++) {
            stl_vertex vertex;
            file.read(reinterpret_cast<char*>(&vertex), sizeof(stl_vertex));
            vertex.x /=100.f; vertex.y/=100.f;
            vertex.z/=100.f;
            stl_vertices.push_back(vertex);
        }

        file.seekg(2, std::ios::cur);
    }
    file.close();
    for (const auto& vertex : stl_vertices) {
        m_vertices.push_back(Vertex(vertex));
    }
}

void Dice::init() {
    initializeOpenGLFunctions();
    if(!m_vao.isCreated()) {
        m_vao.create();
    }
    if(!m_vbo.isCreated()) {
        m_vbo.create();
    }
    if(!m_program->isLinked()) {
        m_program->link();
    }
    if(m_vertexCount<m_vertices.count()) {
        if(m_vertexBuffer) {
            delete[] m_vertexBuffer;
        }
        m_vertexBuffer = new float[m_vertices.count()*vertexFloatCount];
        m_vertexCount = m_vertices.count();
        int pnt = 0;
        for(auto &vertex : m_vertices) {
            m_vertexBuffer[pnt++] = vertex.pos.x();
            m_vertexBuffer[pnt++] = vertex.pos.y();
            m_vertexBuffer[pnt++] = vertex.pos.z();
            m_vertexBuffer[pnt++] = vertex.texture.x();
            m_vertexBuffer[pnt++] = vertex.texture.y();
        }
    }

    m_vao.bind();
    m_vbo.bind();
    m_vbo.allocate(m_vertexBuffer, m_vertexCount*sizeof(float)*vertexFloatCount);
    // vertexFloatCount stands for the number of float in the vertex structure

    m_program->bind();
    m_program->setAttributeBuffer("vPos", GL_FLOAT, 0*sizeof(float), 3, 5*sizeof(float));
    m_program->enableAttributeArray("vPos");
    m_program->setAttributeBuffer("vTexture", GL_FLOAT, 3*sizeof(float), 2, 5*sizeof(float));
    m_program->enableAttributeArray("vTexture");

    m_program->release();
    m_vao.release();
    m_vbo.release();
}

void Dice::update() {

}

void Dice::paint(const QMatrix4x4 &projection, const QMatrix4x4 &view) {
    for(int index : m_texture.keys()) {
        m_texture[index]->bind(index);
    }
    m_vao.bind();
    m_program->bind();
    m_program->setUniformValue("projection",projection);
    m_program->setUniformValue("view",view);
    m_program->setUniformValue("model",model());
    for(int i=0; i<5952; i++) {
        glDrawArrays(GL_TRIANGLES, i*3, 3);
    }

    m_vao.release();
    m_program->release();
    for(auto texture : qAsConst(m_texture)) {
        texture->release();
    }
}
