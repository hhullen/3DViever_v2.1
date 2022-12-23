#include "oglview.h"

#include "ui_oglview.h"

namespace s21 {

OGLview::OGLview(QWidget *parent)
    : QOpenGLWidget(parent), ui_(new Ui::OGLview), texture_(nullptr), object_(nullptr) {
  ui_->setupUi(this);
  new_cursor_.setShape(Qt::OpenHandCursor);
  setCursor(new_cursor_);
  SetDefaulValues();

  timer_ = new QTimer(this);
  connect(timer_, &QTimer::timeout, this, &OGLview::ClearMessageSlot);
  update();
  texture_ = new QImage(10, 10, QImage::Format_ARGB32);
  texture_->fill(QColor(200, 50, 100));
}

OGLview::~OGLview() { delete ui_; }

void OGLview::set_key_spcace_state(bool state) { key_space_ = state; }

void OGLview::set_projection_state(bool state) {
  projection_type_changed_ = state;
}

void OGLview::set_edges_color(QColor color) { edges_color_ = color; }

void OGLview::set_vertexes_color(QColor color) { vertexes_color_ = color; }

void OGLview::set_background_color(QColor color) { background_color_ = color; }

void OGLview::set_projection_type(ProjectionType type) {
  projection_type_ = type;
}

void OGLview::set_edges_style(EdgeStyle style) { edges_style_ = style; }

void OGLview::set_vertexes_style(VertexStyle style) { vertexes_style_ = style; }

void OGLview::set_edges_size(int size) { edges_size_ = size; }

void OGLview::set_vertexes_size(int size) { vertexes_size_ = size; }

void OGLview::set_position(QVector3D positions) {
  position_ = positions;
}

void OGLview::set_angle(QVector3D angles) {
  angle_ = angles;
}

void OGLview::set_scale(float scale) { scale_ = scale; }

QVector3D *OGLview::get_position() {
    return &position_;
}

QVector3D *OGLview::get_angle() {
  return &angle_;
}

float OGLview::get_scale() { return scale_; }

void OGLview::set_model_vertexes_vector(const std::vector<float> *vector) {
  vertexes_ = vector;
}

void OGLview::set_model_indices_vector(
    const std::vector<unsigned int> *vector) {
  indices_ = vector;
}

void OGLview::set_model_facets_amount(unsigned int facets) {
  facets_n_ = facets;
}

void OGLview::DrawModel() {
  if (vertexes_ && indices_) {
    if (object_) {
        delete object_;
    }
    object_ = new Object3D(*vertexes_, *indices_, *texture_);

    new_model_loaded_ = true;
    projection_type_changed_ = true;
    QOpenGLWidget::update();
  }
}

void OGLview::initializeGL() {
    gl_func_ = QOpenGLContext::currentContext()->functions();
    gl_func_->glEnable(GL_DEPTH_TEST);
    gl_func_->glEnable(GL_CULL_FACE);
  m_view_.setToIdentity();

  program_.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/View_module/fshader.frag");
  program_.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/View_module/vshader.vert");
  program_.link();
}

void OGLview::resizeGL(int w, int h) {
  window_w_ = w;
  window_h_ = h;
  screenRatio_ = w / float(h);

//  m_projection_.setToIdentity();
//  m_projection_.perspective(45, screenRatio_, 0.01f, 2000.0f);
  gl_func_->glViewport(0, 0, w, h);
  SetProjectionType();
}

void OGLview::paintGL() {
    gl_func_->glClearColor(background_color_.redF(), background_color_.greenF(),
                           background_color_.blueF(), 1.0f);
    gl_func_->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  if (vertexes_ && indices_) {
      m_view_.setToIdentity();
      m_view_.translate(position_);
      m_view_.rotate(rotation_);
      projection_type_changed_ = true;
      SetProjectionType();

      program_.bind();
      program_.setUniformValue("u_projection_matrix", m_projection_);
      program_.setUniformValue("u_view_matrix", m_view_);
      program_.setUniformValue("u_light_position", QVector4D(0.0, 0.0, 0.0, 1.0));
      program_.setUniformValue("u_light_power", 5.0f);

    object_->draw(&program_, gl_func_);
  }
}

void OGLview::SetDefaulValues() {
  vertexes_ = nullptr;
  indices_ = nullptr;
  far_dist_ = 30000;
  fov_ = 30;
  axis_scale_ = 2;
  start_z_position_ = 2;
  key_space_ = false;
  left_mouse_button_ = false;
  new_model_loaded_ = false;
  projection_type_changed_ = false;
}

void OGLview::GetVertexesBuffer() {
//  glVertexPointer(3, GL_DOUBLE, 0, vertexes_->data());
//  new_model_loaded_ = false;
}

void OGLview::SetModelPosition() {
//  glMatrixMode(GL_MODELVIEW);
//  glLoadIdentity();
//  glTranslated(0, 0, -start_z_position_);
//  glTranslated(position_x_, position_y_, position_z_ - 5);
//  glRotatef(angle_x_, -1, 0, 0);
//  glRotatef(angle_y_, 0, 1, 0);
//  glRotatef(angle_z_, 0, 0, 1);
//  glScalef(scale_, scale_, scale_);
}

void OGLview::DrawPoints() {
//  if (vertexes_style_ == VertexStyle::ROUND) {
//    glEnable(GL_POINT_SMOOTH);
//  } else if (vertexes_style_ == VertexStyle::SQUARE) {
//    glDisable(GL_POINT_SMOOTH);
//  }
//  glPointSize(vertexes_size_);
//  glColor3d(vertexes_color_.redF(), vertexes_color_.greenF(),
//            vertexes_color_.blueF());
//  if (vertexes_style_ != VertexStyle::NONE) {
//    glDrawArrays(GL_POINTS, 0, vertexes_->size() / 3);
//  }
}

void OGLview::DrawLines() {
//  if (edges_style_ == EdgeStyle::STIPPLE) {
//    glLineStipple(2, 0x00F0);
//    glEnable(GL_LINE_STIPPLE);
//  } else if (edges_style_ == EdgeStyle::SOLID) {
//    glDisable(GL_LINE_STIPPLE);
//  }
//  glLineWidth(edges_size_);
//  glColor3d(edges_color_.redF(), edges_color_.greenF(), edges_color_.blueF());
//  glDrawElements(GL_LINES, indices_->size(), GL_UNSIGNED_INT, indices_->data());
}

void OGLview::SetProjectionType() {
    m_projection_.setToIdentity();
  if (projection_type_ == ProjectionType::PERSPECTIVE) {
      m_projection_.perspective(fov_, screenRatio_, 0.01, far_dist_ + 2000);
  } else if (projection_type_ == ProjectionType::ORTHOGONAL) {
      m_projection_.ortho(-axis_scale_ * screenRatio_, axis_scale_ * screenRatio_,
                             -axis_scale_, axis_scale_, 0.01, far_dist_ + 2000);
  }
  projection_type_changed_ = false;
}

void OGLview::ShowEventMessage(QString message, int delay) {
  ui_->label_message->setText(message);
  timer_->start(delay);
}

void OGLview::ClearMessageSlot() { ui_->label_message->clear(); }

void OGLview::mousePressEvent(QMouseEvent *event) {
  mouse_now_ = QVector2D(event->pos());
  new_cursor_.setShape(Qt::ClosedHandCursor);
  setCursor(new_cursor_);
  left_mouse_button_ = event->button() == Qt::LeftButton;
  event->accept();
}

void OGLview::mouseReleaseEvent(QMouseEvent *event) {
  new_cursor_.setShape(Qt::OpenHandCursor);
  setCursor(new_cursor_);
  left_mouse_button_ = false;
  event->accept();
}

void OGLview::mouseMoveEvent(QMouseEvent *event) {
  QVector2D diff = QVector2D(event->pos()) - mouse_now_;
  mouse_now_ = QVector2D(event->pos());

  float length = diff.length() / 2;

  QVector3D axis(-diff.y(), -diff.x(), 0.0);

    rotation_ = QQuaternion::fromAxisAndAngle(axis, length) * rotation_;
    QOpenGLWidget::update();

//  if ((mouse_now_.x() != 0 || mouse_now_.y() != 0) && left_mouse_button_)
//    MoveModelByMouse(mouse_now_);
//  mouse_now_ = event->pos();
}

void OGLview::wheelEvent(QWheelEvent *event) {
  if (event->pixelDelta().y() != 0 && key_space_)
    MoveModelByWheel(event->pixelDelta().y());
  else if (event->pixelDelta().y() != 0 && !key_space_)
    ScaleModelByWheel(event->pixelDelta().y());
  if (vertexes_ && indices_) {
    emit PositionUpdatedSignal();
  }
}

void OGLview::MoveModelByWheel(int dz) {
//  if (dz > 0) {
//    position_. += 1;
//  } else if (dz < 0) {
//    position_z_ -= 1;
//  }
//  update();
}

void OGLview::ScaleModelByWheel(int ds) {
  if (ds < 0) {
    scale_ *= 0.8;
  } else if (ds > 0) {
    scale_ *= 1.25;
  }
  update();
}

void OGLview::MoveModelByMouse(QPoint pos) {
//  if (key_space_) {
//    float dx_move_ = axis_scale_ * 2 * pos.x() / window_w_;
//    float dy_move_ = axis_scale_ * 2 * -pos.y() / window_h_;

//    position_x_ += dx_move_;
//    position_y_ += dy_move_;
//  } else {
//    float dx_rotate_ = axis_scale_ * 100 * pos.x() / window_w_;
//    float dy_rotate_ = axis_scale_ * 100 * -pos.y() / window_h_;

//    IncreaseAngle(&angle_x_, dy_rotate_);
//    IncreaseAngle(&angle_y_, dx_rotate_);
//  }
//  if (vertexes_ && indices_) {
//    emit PositionUpdatedSignal();
//  }
//  update();
}

void OGLview::IncreaseAngle(float *angle, float dr) {
  *angle += dr;
  if (*angle >= 360) {
    *angle -= 360;
  } else if (*angle <= -360) {
    *angle += 360;
  }
}

}  // namespace S21
