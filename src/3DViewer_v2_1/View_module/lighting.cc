#include "lighting.h"
#include "ui_lighting.h"

namespace s21 {

Lighting::Lighting(QFont &font, QWidget *parent) :
    QWidget(parent),
    ui_(new Ui::Lighting) {
    this->setFont(font);
    ui_->setupUi(this);
    ui_->label_panel_name->setFont(font);

    set_turned_on(false);
    settings_ = new QSettings("LightSetup", "LightSettings", this);

    connect(ui_->palette_light, &QPushButton::clicked, this, &Lighting::GetNewColor);
    connect(ui_->cb_switcher, &QComboBox::currentIndexChanged, this, &Lighting::SwitchState);
    connect(ui_->d_spin_box_power, &QDoubleSpinBox::valueChanged, this, &Lighting::DataUpdatedSignal);
    connect(ui_->d_spin_box_x_pos, &QDoubleSpinBox::valueChanged, this, &Lighting::DataUpdatedSignal);
    connect(ui_->d_spin_box_y_pos, &QDoubleSpinBox::valueChanged, this, &Lighting::DataUpdatedSignal);
    connect(ui_->d_spin_box_z_pos, &QDoubleSpinBox::valueChanged, this, &Lighting::DataUpdatedSignal);
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

QVector3D Lighting::get_position() {
    QVector3D returnable;

    returnable.setX( ui_->d_spin_box_x_pos->value());
    returnable.setY( ui_->d_spin_box_y_pos->value());
    returnable.setZ( ui_->d_spin_box_z_pos->value());

    return returnable;
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
        QColorDialog::getColor(color_, this, "Select light color",
                               QColorDialog::ColorDialogOption::ShowAlphaChannel);

    if (temp.isValid()) {
      color_ = temp;
      PanelFuncs::SetColor(color_, nullptr, ui_->palette_light);
      PanelFuncs::ShowChosenColorInfo(ui_->label_light_rgb, color_);
      emit DataUpdatedSignal();
    }
}

void Lighting::SwitchState(int index) {
    if (index == 0) {
        set_turned_on(false);
    } else if (index == 1) {
        set_turned_on(true);
    }
    emit DataUpdatedSignal();
}

void Lighting::SaveSettings() {
  settings_->setValue("light_color", color_);
  settings_->setValue("light_power", ui_->d_spin_box_power->value());
}

void Lighting::UploadSettings() {
  QVariant temp;

  temp = settings_->value("light_color");
  PanelFuncs::SetColor(temp.value<QColor>(), &color_, ui_->palette_light);
  PanelFuncs::ShowChosenColorInfo(ui_->label_light_rgb, color_);

  ui_->d_spin_box_power->setValue(settings_->value("light_power").toFloat());
}

}
