#include "texturing.h"
#include "ui_texturing.h"

namespace s21 {

Texturing::Texturing(QWidget *parent) :
    QWidget(parent),
    ui_(new Ui::Texturing), texture_path_(""), map_path_("") {
    ui_->setupUi(this);
    name_pattern_.setPattern("[^\\/]*$");
    ui_->btn_save->setDisabled(true);

    connect(ui_->btn_add_remove, &QPushButton::clicked, this, &Texturing::AddRemove);
    connect(ui_->btn_folder_path, &QPushButton::clicked, this, &Texturing::ChooseDirectory);
    connect(ui_->palette_map, &QPushButton::clicked, this, &Texturing::GetNewColor);
}

Texturing::~Texturing() {
    delete ui_;
}

QImage Texturing::get_texture() {
    return texture_;
}

void Texturing::set_untextured() {
    texture_path_.clear();
    ui_->btn_add_remove->setText("Add");
    emit DataUpdatedSignal(false);
}

void Texturing::AddRemove() {
    if (texture_path_.isEmpty()) {
        texture_path_ = QFileDialog::getOpenFileName(this, "Open .bmp file", "/Users",
                                              "bmp (*.bmp);;");
        if (!texture_path_.isEmpty()) {
            texture_.load(texture_path_);
            if (texture_.width() == texture_.height()) {
                ui_->btn_add_remove->setText("Remove");
                emit DataUpdatedSignal(true);
            } else {
                emit SentMessageSignal("Can not load this texture");
            }
        }
    } else {
        set_untextured();
    }
}

void Texturing::ChooseDirectory() {
  QString temp = QFileDialog::getExistingDirectory(
      this, "Choose folder to save screen", "/Users",
      QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
  if (!temp.isEmpty()) {
    map_path_ = temp;
    SetPathToButtonName();
  }
}

void Texturing::SetPathToButtonName() {
  QRegularExpressionMatch match = name_pattern_.match(map_path_);
  ui_->btn_folder_path->setText(match.captured());
  ui_->btn_save->setDisabled(false);
}

void Texturing::GetNewColor() {
    QColor temp =
        QColorDialog::getColor(color_, this, "Select lines color",
                               QColorDialog::ColorDialogOption::ShowAlphaChannel);

    if (temp.isValid()) {
      color_ = temp;
      SetColor(color_, nullptr, ui_->palette_map);
      ShowChosenColorInfo(ui_->label_map_rgb, color_);
    }
}

void Texturing::SetColor(QColor color, QColor *var, QPushButton *btn) {
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

void Texturing::ShowChosenColorInfo(QLabel *txt, QColor col) {
  txt->setText("R: " + QString::number(col.red()) +
               " G: " + QString::number(col.green()) +
               " B: " + QString::number(col.blue()));
}

void Texturing::SaveUVMap() {
    data_ =  emit RequireUVMapData();

}

}
