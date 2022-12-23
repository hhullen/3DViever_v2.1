#ifndef OBJECT3D_H
#define OBJECT3D_H

#include <vector>
#include <QOpenGLBuffer>
#include <QMatrix4x4>
#include <QOpenGLTexture>
#include <QOpenGLShaderProgram>
#include <QOpenGLFunctions>

using std::vector;

namespace s21 {

class Object3D
{
public:
    Object3D();
    Object3D(const vector<float> &vertex, const vector<unsigned int> &indices, const QImage &texture);
    ~Object3D();

    void init(const vector<float> &vertex, const vector<unsigned int> &indices, const QImage &texture);
    void draw(QOpenGLShaderProgram *program, QOpenGLFunctions *gl_functions);

private:
    QOpenGLBuffer vertex_buffer_;
    QOpenGLBuffer index_buffer_;
    QOpenGLTexture *texture_;
    size_t vertexes_;
    size_t indices_;

    QMatrix4x4 m_model_matrix_;
};

}

#endif // OBJECT3D_H
