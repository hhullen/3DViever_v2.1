#include "ptransform.h"

#include "ui_ptransform.h"

namespace s21 {

PTransform::PTransform(QFont &font, QWidget *parent)
    : QWidget(parent), ui_(new Ui::PTransform) {
  this->setFont(font);
  ui_->setupUi(this);
  ui_->label_panel_name->setFont(font);
  ui_->btn_reset_position->setFont(font);

  ConnectSignalSlot();
}

PTransform::~PTransform() { delete ui_; }

QVector3D PTransform::get_position() {
  QVector3D position;
  position.setX(ui_->d_spin_box_x_pos->value());
  position.setY(ui_->d_spin_box_y_pos->value());
  position.setZ(ui_->d_spin_box_z_pos->value());

  return position;
}

QVector3D PTransform::get_angle() {
  QVector3D angle;
  angle.setX(ui_->d_spin_box_x_ang->value());
  angle.setY(ui_->d_spin_box_y_ang->value());
  angle.setZ(ui_->d_spin_box_z_ang->value());

  return angle;
}

float PTransform::get_scale() { return ui_->d_spin_box_scale->value(); }

void PTransform::set_position(QVector3D position) {
  ui_->d_spin_box_x_pos->setValue(position.x());
  ui_->d_spin_box_y_pos->setValue(position.y());
  ui_->d_spin_box_z_pos->setValue(position.z());
}

void PTransform::set_angle(QVector3D angle) {
  ui_->d_spin_box_x_ang->setValue(angle.x());
  ui_->d_spin_box_y_ang->setValue(angle.y());
  ui_->d_spin_box_z_ang->setValue(angle.z());
}

void PTransform::set_scale(float scale) {
  ui_->d_spin_box_scale->setValue(scale);
}

void PTransform::ResetSlot() {
  ui_->d_spin_box_scale->setValue(1);
  ui_->d_spin_box_x_ang->setValue(0);
  ui_->d_spin_box_y_ang->setValue(0);
  ui_->d_spin_box_z_ang->setValue(0);
  ui_->d_spin_box_x_pos->setValue(0);
  ui_->d_spin_box_y_pos->setValue(0);
  ui_->d_spin_box_z_pos->setValue(0);
}

void PTransform::ConnectSignalSlot() {
  connect(ui_->d_spin_box_x_pos, &QDoubleSpinBox::valueChanged, this,
          &PTransform::DataUpdatedSignal);
  connect(ui_->d_spin_box_y_pos, &QDoubleSpinBox::valueChanged, this,
          &PTransform::DataUpdatedSignal);
  connect(ui_->d_spin_box_z_pos, &QDoubleSpinBox::valueChanged, this,
          &PTransform::DataUpdatedSignal);
  connect(ui_->d_spin_box_x_ang, &QDoubleSpinBox::valueChanged, this,
          &PTransform::DataUpdatedSignal);
  connect(ui_->d_spin_box_y_ang, &QDoubleSpinBox::valueChanged, this,
          &PTransform::DataUpdatedSignal);
  connect(ui_->d_spin_box_z_ang, &QDoubleSpinBox::valueChanged, this,
          &PTransform::DataUpdatedSignal);
  connect(ui_->d_spin_box_scale, &QDoubleSpinBox::valueChanged, this,
          &PTransform::DataUpdatedSignal);
  connect(ui_->btn_reset_position, &QPushButton::clicked, this,
          &PTransform::ResetSlot);
}

}  // namespace s21
