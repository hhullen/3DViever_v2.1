#include "lighting.h"
#include "ui_lighting.h"

namespace s21 {

Lighting::Lighting(QWidget *parent) :
    QWidget(parent),
    ui_(new Ui::Lighting) {
    ui_->setupUi(this);
    set_turned_on(false);
    settings_ = new QSettings("LightSetup", "LightSettings", this);

    connect(ui_->palette_light, &QPushButton::clicked, this, &Lighting::GetNewColor);
    connect(ui_->cb_switcher, &QComboBox::currentIndexChanged, this, &Lighting::SwitchState);
    UploadSettings();
}

Lighting::~Lighting() {
    SaveSettings();
    delete ui_;
}

void Lighting::set_turned_on(bool state) {
    ui_->cb_switcher->setCurrentIndex(state);
    SetDisabled(!state);
}

float Lighting::get_power() {
    float returnable = 0;

    if (IsTurnedOn()) {
        returnable = ui_->d_spin_box_power->value();
    }

    return returnable;
}

void Lighting::get_position(float *x, float *y, float *z) {
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

QColor Lighting::get_color() {
    return color_;
}

bool Lighting::IsTurnedOn() {
    return ui_->d_spin_box_power->isEnabled();
}

void Lighting::SetDisabled(bool state) {
    ui_->d_spin_box_power->setDisabled(state);
    ui_->d_spin_box_x_pos->setDisabled(state);
    ui_->d_spin_box_y_pos->setDisabled(state);
    ui_->d_spin_box_z_pos->setDisabled(state);
    ui_->palette_light->setDisabled(state);
}

void Lighting::GetNewColor() {
    QColor temp =
        QColorDialog::getColor(color_, this, "Select edges color",
                               QColorDialog::ColorDialogOption::ShowAlphaChannel);

    if (temp.isValid()) {
      color_ = temp;
      SetColor(color_, nullptr, ui_->palette_light);
      ShowChosenColorInfo(ui_->label_light_rgb, color_);
    }
}

void Lighting::SetColor(QColor color, QColor *var, QPushButton *btn) {
  if (var) {
    *var = color;
  }
  btn->setStyleSheet(
      "QPushButton {"
      "border: 2px solid rgba(90, 90, 90, 0.6);"
      "border-radius: 13px;"
      "background-color: " +
      color.name(QColor::NameFormat::HexArgb) +
      ";"
      "color: rgba(0, 0, 0, 0);"
      "}\n"
      "QPushButton:pressed {"
      "background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,"
      "stop: 0 #404040 stop: 1 #686868);"
      "}");
}

void Lighting::ShowChosenColorInfo(QLabel *txt, QColor col) {
  txt->setText("R: " + QString::number(col.red()) +
               " G: " + QString::number(col.green()) +
               " B: " + QString::number(col.blue()));
}

void Lighting::SwitchState(int index) {
    if (index == 0) {
        set_turned_on(false);
    } else if (index == 1) {
        set_turned_on(true);
    }
}

void Lighting::SaveSettings() {
  settings_->setValue("light_color", color_);
  settings_->setValue("light_power", ui_->d_spin_box_power->value());
}

void Lighting::UploadSettings() {
  QVariant temp;

  temp = settings_->value("light_color");
  SetColor(temp.value<QColor>(), &color_, ui_->palette_light);
  ShowChosenColorInfo(ui_->label_light_rgb, color_);

  ui_->d_spin_box_power->setValue(settings_->value("light_power").toFloat());
}

}
