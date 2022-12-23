#ifndef OBJECT3D_H
#define OBJECT3D_H

#include <vector>
#include <QOpenGLBuffer>
#include <QMatrix4x4>
#include <QOpenGLTexture>
#include <QOpenGLShaderProgram>
#include <QOpenGLFunctions>
#include "View_module/enum_parameters.h"

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
    void setup_vertexes(int size, QColor color, VertexStyle style);
    void setup_edges(int size, QColor color, EdgeStyle style);

private:
    QOpenGLBuffer vertex_buffer_;
    QOpenGLBuffer index_buffer_;
    QOpenGLTexture *texture_;
    QOpenGLTexture *edge_texture_;
    QOpenGLTexture *vertex_texture_;
    size_t vertexes_;
    size_t indices_;

    QColor edges_color_;
    QColor vertexes_color_;
    VertexStyle vertexes_style_;
    EdgeStyle edges_style_;
    int edges_size_;
    int vertexes_size_;

    QMatrix4x4 m_model_matrix_;

    void DrawVertexes(QOpenGLFunctions *gl_function);
    void DrawEdges(QOpenGLFunctions *gl_function);
};

}

#endif // OBJECT3D_H
