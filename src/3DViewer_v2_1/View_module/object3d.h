#ifndef OBJECT3D_H
#define OBJECT3D_H

#define GL_SILENCE_DEPRECATION

#include <QMatrix4x4>
#include <QOpenGLBuffer>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <vector>

#include "View_module/enum_parameters.h"

using std::vector;

namespace s21 {

class Object3D {
 public:
  Object3D();
  Object3D(const vector<float> &ordered_vertex,
           const vector<unsigned int> &ordered_indices,
           const vector<float> &vertex, const vector<unsigned int> &indices,
           const QImage &texture);
  ~Object3D();

  void init(const vector<float> &ordered_vertex,
            const vector<unsigned int> &ordered_indices,
            const vector<float> &vertex, const vector<unsigned int> &indices,
            const QImage &texture);
  void draw(QOpenGLShaderProgram *program, QOpenGLFunctions *gl_functions);
  void setup_vertexes(int size, QColor color, VertexStyle style);
  void setup_edges(int size, QColor color, EdgeStyle style);
  void set_view_mode(ViewMode mode);
  void set_texture(const QImage &texture);

 private:
  QOpenGLBuffer ordered_vertex_buffer_;
  QOpenGLBuffer ordered_index_buffer_;
  QOpenGLBuffer vertex_buffer_;
  QOpenGLBuffer index_buffer_;
  GLsizei vertexes_;
  GLsizei indices_;
  GLsizei ordered_vertexes_;

  QOpenGLTexture *texture_;

  QColor edges_color_;
  QColor vertexes_color_;
  VertexStyle vertexes_style_;
  EdgeStyle edges_style_;
  int edges_size_;
  int vertexes_size_;
  ViewMode view_mode_;

  QMatrix4x4 m_model_matrix_;

  void DrawPolygons(QOpenGLShaderProgram *program,
                    QOpenGLFunctions *gl_function);
  void DrawVertexes(QOpenGLShaderProgram *program,
                    QOpenGLFunctions *gl_function);
  void DrawEdges(QOpenGLShaderProgram *program, QOpenGLFunctions *gl_function);
};

}  // namespace s21

#endif  // OBJECT3D_H
