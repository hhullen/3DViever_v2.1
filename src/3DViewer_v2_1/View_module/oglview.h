#ifndef OGLVIEW_H
#define OGLVIEW_H

#define GL_SILENCE_DEPRECATION

#include <QLabel>
#include <QMouseEvent>
#include <QOpenGLFunctions>
#include <QOpenGLWidget>
#include <QTimer>
#include <vector>

#include "View_module/enum_parameters.h"
using std::vector;

namespace Ui {
class OGLview;
}

namespace S21 {

class OGLview : public QOpenGLWidget, public QOpenGLFunctions {
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

  void set_position(double x, double y, double z);
  void set_angle(double x, double y, double z);
  void set_scale(double scale);
  void get_position(double *x, double *y, double *z);
  void get_angle(double *x, double *y, double *z);
  double get_scale();

  void set_model_vertexes_vector(const vector<double> *vector);
  void set_model_indices_vector(const vector<unsigned int> *vector);
  void set_model_facets_amount(unsigned int facets);

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
  QTimer *timer_;
  QCursor new_cursor_;
  QPoint mouse_now_;
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
  double position_x_;
  double position_y_;
  double position_z_;
  double angle_x_;
  double angle_y_;
  double angle_z_;
  double scale_;
  double start_z_position_;
  double axis_scale_;
  float screenRatio_;
  int window_w_;
  int window_h_;
  double fov_;
  double dx_move_;
  double dy_move_;
  double dx_rotate_;
  double dy_rotate_;
  double far_dist_;

  const vector<double> *vertexes_;
  const vector<unsigned int> *indices_;
  unsigned int facets_n_;

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
  void IncreaseAngle(double *angle, double dr);
};

}  // namespace S21

#endif  // OGLVIEW_H
