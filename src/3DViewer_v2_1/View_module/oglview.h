#ifndef OGLVIEW_H
#define OGLVIEW_H

#define GL_SILENCE_DEPRECATION

#include <QLabel>
#include <QMouseEvent>
#include <QOpenGLFunctions>
#include <QOpenGLContext>
#include <QOpenGLWidget>
#include <QTimer>
#include <QOpenGLShaderProgram>
#include <vector>
#include <QMatrix4x4>
#include <QOpenGLTexture>
#include <QQuaternion>

#include "View_module/enum_parameters.h"
#include "object3d.h"
using std::vector;

namespace Ui {
class OGLview;
}

namespace s21 {

class OGLview : public QOpenGLWidget,
                public QOpenGLFunctions {
  Q_OBJECT

 public:
  explicit OGLview(QWidget *parent = nullptr);
  ~OGLview();
  void DrawModel();
  void ShowEventMessage(QString message, int delay);
  void set_key_spcace_state(bool state);
  void set_projection_state(bool state);

  void set_edges_color(QColor color);
  void set_vertexes_color(QColor color);
  void set_background_color(QColor color);
  void set_projection_type(ProjectionType type);
  void set_edges_style(EdgeStyle style);
  void set_vertexes_style(VertexStyle style);
  void set_edges_size(int size);
  void set_vertexes_size(int size);

  void set_position(QVector3D positions);
  void set_angle(QVector3D angles);
  void set_scale(float scale);
  QVector3D *get_position();
  QVector3D *get_angle();
  float get_scale();

  void set_model_ordered_vertexes_vector(const vector<float> *vector);
  void set_model_ordered_indices_vector(const vector<unsigned int> *vector);
  void set_model_vertexes_vector(const vector<float> *vector);
  void set_model_indices_vector(const vector<unsigned int> *vector);
//  void set_model_facets_amount(unsigned int facets);

 signals:
  void PositionUpdatedSignal();

 private slots:
  void mousePressEvent(QMouseEvent *event) override;
  void mouseReleaseEvent(QMouseEvent *event) override;
  void mouseMoveEvent(QMouseEvent *event) override;
  void wheelEvent(QWheelEvent *event) override;
  void ClearMessageSlot();

 private:
  Ui::OGLview *ui_;
  QOpenGLFunctions *gl_func_;
  QTimer *timer_;
  QCursor new_cursor_;
  QVector2D mouse_now_;
  bool key_space_;
  bool left_mouse_button_;
  bool new_model_loaded_;
  bool projection_type_changed_;

  QColor edges_color_;
  QColor vertexes_color_;
  QColor background_color_;
  ProjectionType projection_type_;
  EdgeStyle edges_style_;
  VertexStyle vertexes_style_;
  int edges_size_;
  int vertexes_size_;
  QVector3D position_;
  QVector3D angle_;
  float scale_;
  float start_z_position_;
  float axis_scale_;
  float screenRatio_;
  int window_w_;
  int window_h_;
  float fov_;
  float far_dist_;

  const vector<float> *vertexes_;
  const vector<float> *ordered_data_;
  const vector<unsigned int> *indices_;
  const vector<unsigned int> *ordered_indices_;
//  unsigned int facets_n_;
//  unsigned int ordered_facets_n_;

    QMatrix4x4 m_projection_;
    QMatrix4x4 m_view_;
    QQuaternion rotation_;
    QOpenGLShaderProgram program_;
    QImage *texture_;
    Object3D *object_;

  void initializeGL() override;
  void resizeGL(int w, int h) override;
  void paintGL() override;

  void SetDefaulValues();
  void SetProjectionType();
  void GetVertexesBuffer();
  void SetModelPosition();
  void DrawPoints();
  void DrawLines();
  void MoveModelByMouse(QPoint pos);
  void MoveModelByWheel(int dz);
  void ScaleModelByWheel(int ds);
  void IncreaseAngle(float *angle, float dr);
};

}  // namespace S21

#endif  // OGLVIEW_H
