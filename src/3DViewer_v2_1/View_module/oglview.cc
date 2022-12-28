#include "oglview.h"

#include "ui_oglview.h"

namespace s21 {

OGLview::OGLview(QFont &font, QWidget *parent)
    : QOpenGLWidget(parent), ui_(new Ui::OGLview), object_(nullptr) {
    this->setFont(font);
  ui_->setupUi(this);
  new_cursor_.setShape(Qt::OpenHandCursor);
  setCursor(new_cursor_);
  SetDefaulValues();

  timer_ = new QTimer(this);
  connect(timer_, &QTimer::timeout, this, &OGLview::ClearMessageSlot);
  texture_ = QImage(10, 10, QImage::Format_ARGB32);
  texture_.fill(QColor(63, 63, 63));
  update();
}

OGLview::~OGLview() { delete ui_; }

QImage *OGLview::get_screen_pointer() {
    return &screen_;
}

void OGLview::set_recording(bool state) {
    is_recording_ = state;
}

void OGLview::set_key_spcace_state(bool state) { key_space_ = state; }

void OGLview::set_projection_state(bool state) {
  projection_type_changed_ = state;
}

void OGLview::set_edges_color(QColor color) { edges_color_ = color; }

void OGLview::set_vertexes_color(QColor color) { vertexes_color_ = color; }

void OGLview::set_background_color(QColor color) { background_color_ = color; }

void OGLview::set_polygon_color(QColor color) {
    polygon_color_ = color;
}

void OGLview::set_projection_type(ProjectionType type) {
    projection_type_ = type;
}

void OGLview::set_shading_type(ShadeMode type) {
    shading_type_ = type;
}

void OGLview::set_drawing_type(ViewMode type) {
    drawing_type_ = type;
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
    rotation_ = rotation_.fromEulerAngles(angle_);
}

void OGLview::set_scale(float scale) { scale_ = scale; }

QVector3D *OGLview::get_position() {
    return &position_;
}

QVector3D *OGLview::get_angle() {
  return &angle_;
}

float OGLview::get_scale() { return scale_; }

void OGLview::set_light_position(QVector3D position) {
    light_position_ = position;
}

void OGLview::set_light_power(float value) {
    light_power_ = value;
}

void OGLview::set_light_color(QColor color) {
    light_color_ = color;
}

void OGLview::set_textured(QImage texture) {
    texture_ = texture;
    is_textured_ = true;
    if (object_) {
        object_->set_texture(texture_);
    }
    QOpenGLWidget::update();
}

void OGLview::set_untextured() {
    texture_ = QImage(10, 10, QImage::Format_ARGB32);
    texture_.fill(QColor(63, 63, 63));
    if (object_) {
        object_->set_texture(texture_);
    }
    is_textured_ = false;
    QOpenGLWidget::update();
}

void OGLview::set_model_ordered_vertexes_vector(const std::vector<float> *vector) {
  ordered_data_ = vector;
}

void OGLview::set_model_ordered_indices_vector(
    const std::vector<unsigned int> *vector) {
    ordered_indices_ = vector;
}

void OGLview::set_model_vertexes_vector(const std::vector<float> *vector) {
    vertexes_ = vector;
}

void OGLview::set_model_indices_vector(const std::vector<unsigned int> *vector) {
    indices_ = vector;
}

void OGLview::DrawModel() {
  if (isModelLoaded()) {
    if (object_) {
        delete object_;
    }
    object_ = new Object3D(*ordered_data_, *ordered_indices_, *vertexes_, *indices_, texture_);

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

  gl_func_->glViewport(0, 0, w, h);
  SetProjectionType();
}

void OGLview::paintGL() {
    gl_func_->glClearColor(background_color_.redF(), background_color_.greenF(),
                           background_color_.blueF(), 1.0f);
    gl_func_->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  if (isModelLoaded()) {
      SetModelPosition();
      SetProjectionType();
      SetUniforms();

      object_->setup_edges(edges_size_, edges_color_, edges_style_);
      object_->setup_vertexes(vertexes_size_, vertexes_color_,vertexes_style_);
      object_->set_view_mode(drawing_type_);
      object_->draw(&program_, gl_func_);
      if (is_recording_) {
        screen_ = grabFramebuffer();
      }
  }
}

void OGLview::SetDefaulValues() {
  vertexes_ = nullptr;
  indices_ = nullptr;
  ordered_data_ = nullptr;
  ordered_indices_ = nullptr;
  far_dist_ = 30000;
  fov_ = 30;
  axis_scale_ = 2;
  key_space_ = false;
  left_mouse_button_ = false;
  projection_type_changed_ = false;
  is_textured_ = false;
  is_recording_ = false;
}

void OGLview::SetModelPosition() {
    m_view_.setToIdentity();
    m_view_.translate(position_);
    m_view_.translate(0,0, -10);
    m_view_.rotate(rotation_);
    m_view_.scale(scale_);
}

void OGLview::SetUniforms() {
    program_.bind();
    program_.setUniformValue("u_projection_matrix", m_projection_);
    program_.setUniformValue("u_view_matrix", m_view_);
    program_.setUniformValue("u_light_position", QVector4D(light_position_, 1.0));
    program_.setUniformValue("u_light_power", light_power_);
    program_.setUniformValue("u_light_color", light_color_);
    program_.setUniformValue("u_shadow_color", QVector4D(0.25, 0.25, 0.25, 1.0));
    program_.setUniformValue("u_shade_mode", shading_type_);
    program_.setUniformValue("u_polygon_color", polygon_color_);
    program_.setUniformValue("u_is_textured", is_textured_);
}

void OGLview::SetProjectionType() {
    m_projection_.setToIdentity();
  if (projection_type_ == ProjectionType::PERSPECTIVE) {
      m_projection_.perspective(fov_, screenRatio_, 0.01, far_dist_);
  } else if (projection_type_ == ProjectionType::ORTHOGONAL) {
      m_projection_.ortho(-axis_scale_ * screenRatio_, axis_scale_ * screenRatio_,
                             -axis_scale_, axis_scale_, 0.01, far_dist_);
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

  if ((mouse_now_.x() != 0 || mouse_now_.y() != 0) && left_mouse_button_) {
    MoveModelByMouse(diff);
  }
}

void OGLview::wheelEvent(QWheelEvent *event) {
  if (event->pixelDelta().y() != 0 && key_space_)
    MoveModelByWheel(event->pixelDelta().y());
  else if (event->pixelDelta().y() != 0 && !key_space_)
    ScaleModelByWheel(event->pixelDelta().y());
  if (isModelLoaded()) {
    emit PositionUpdatedSignal();
  }
}

void OGLview::MoveModelByWheel(int dz) {
  if (dz > 0) {
    position_.setZ(position_.z() + 1);
  } else if (dz < 0) {
    position_.setZ(position_.z() - 1);
  }
   QOpenGLWidget::update();
}

void OGLview::ScaleModelByWheel(int ds) {
  if (ds < 0) {
    scale_ *= 0.8;
  } else if (ds > 0) {
    scale_ *= 1.25;
  }
   QOpenGLWidget::update();
}

void OGLview::MoveModelByMouse(QVector2D diff) {
    float length = diff.length() / 2;

  if (key_space_) {
      position_.setX(position_.x() + diff.x() / (window_w_ / 6));
      position_.setY(position_.y() - diff.y() / (window_w_ / 6));
  } else {
      QVector3D axis(diff.y(), diff.x(), 0.0);
      rotation_ = QQuaternion::fromAxisAndAngle(axis, length) * rotation_;
      angle_ = rotation_.toEulerAngles();
  }
  if (isModelLoaded()) {
    emit PositionUpdatedSignal();
  }
    QOpenGLWidget::update();
}

bool OGLview::isModelLoaded() {
    return vertexes_ && ordered_data_ && indices_ && ordered_indices_;
}

}  // namespace S21
