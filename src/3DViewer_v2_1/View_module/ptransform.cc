#include "ptransform.h"

#include "ui_ptransform.h"

namespace S21 {

PTransform::PTransform(QWidget *parent)
    : QWidget(parent), ui_(new Ui::PTransform) {
  ui_->setupUi(this);

  ConnectSignalSlot();
}

PTransform::~PTransform() { delete ui_; }

void PTransform::get_position(double *x, double *y, double *z) {
  if (x) {
    *x = ui_->d_spin_box_x_pos->value();
  }
  if (y) {
    *y = ui_->d_spin_box_y_pos->value();
  }
  if (z) {
    *z = ui_->d_spin_box_z_pos->value();
  }
}

void PTransform::get_angle(double *x, double *y, double *z) {
  if (x) {
    *x = ui_->d_spin_box_x_ang->value();
  }
  if (y) {
    *y = ui_->d_spin_box_y_ang->value();
  }
  if (z) {
    *z = ui_->d_spin_box_z_ang->value();
  }
}

double PTransform::get_scale() { return ui_->d_spin_box_scale->value(); }

void PTransform::set_position(double *x, double *y, double *z) {
  if (x) {
    ui_->d_spin_box_x_pos->setValue(*x);
  }
  if (y) {
    ui_->d_spin_box_y_pos->setValue(*y);
  }
  if (z) {
    ui_->d_spin_box_z_pos->setValue(*z);
  }
}

void PTransform::set_angle(double *x, double *y, double *z) {
  if (x) {
    ui_->d_spin_box_x_ang->setValue(*x);
  }
  if (y) {
    ui_->d_spin_box_y_ang->setValue(*y);
  }
  if (z) {
    ui_->d_spin_box_z_ang->setValue(*z);
  }
}

void PTransform::set_scale(double scale) {
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
  connect(ui_->btn_reset_position, SIGNAL(clicked()), this, SLOT(ResetSlot()));
}

}  // namespace S21
