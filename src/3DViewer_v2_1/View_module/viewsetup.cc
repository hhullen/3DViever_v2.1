#include "viewsetup.h"

#include "ui_viewsetup.h"

namespace S21 {

ViewSetup::ViewSetup(QWidget *parent)
    : QWidget(parent), ui_(new Ui::ViewSetup) {
  ui_->setupUi(this);
  settings_ = new QSettings("ViewSetup", "ViewSettings", this);
  ConnectSignalSlot();
  UploadSettings();
}

ViewSetup::~ViewSetup() {
  SaveSettings();
  delete ui_;
}

ProjectionType ViewSetup::get_projection_type() {
  return (ProjectionType)ui_->cb_projection_style->currentIndex();
}

EdgeStyle ViewSetup::get_edges_style() {
  return (EdgeStyle)ui_->cb_edge_style->currentIndex();
}

QColor ViewSetup::get_edges_color() { return edge_color_; }

int ViewSetup::get_edges_size() { return ui_->hs_edge_size->value(); }

VertexStyle ViewSetup::get_vertex_style() {
  return (VertexStyle)ui_->cb_vertex_style->currentIndex();
}

QColor ViewSetup::get_vertex_color() { return vertex_color_; }

int ViewSetup::get_vertex_size() { return ui_->hs_vertex_size->value(); }

QColor ViewSetup::get_background_color() { return background_color_; }

void ViewSetup::ManageVertexStyleDependenciesSlot(int index) {
  if (index == VertexStyle::ROUND) {
    ui_->palette_vertex->setDisabled(false);
    ui_->hs_vertex_size->setDisabled(false);
  } else if (index == VertexStyle::SQUARE) {
    ui_->palette_vertex->setDisabled(false);
    ui_->hs_vertex_size->setDisabled(false);
  } else if (index == VertexStyle::NONE) {
    ui_->palette_vertex->setDisabled(true);
    ui_->hs_vertex_size->setDisabled(true);
  }
  emit DataUpdatedSignal();
}

void ViewSetup::ChooseEdgeColorSlot() {
  edge_color_ =
      GetNewColor(ui_->palette_edge, ui_->label_edge_rgb, edge_color_);
  emit DataUpdatedSignal();
}

void ViewSetup::ChooseVertexColorSlot() {
  vertex_color_ =
      GetNewColor(ui_->palette_vertex, ui_->label_vertex_rgb, vertex_color_);
  emit DataUpdatedSignal();
}

void ViewSetup::ChooseBackgroundColorSlot() {
  background_color_ = GetNewColor(ui_->palette_background,
                                  ui_->label_background_rgb, background_color_);
  emit DataUpdatedSignal();
}

void ViewSetup::ResetSlot() {
  SetColor(QColor(0, 0, 0, 255), &vertex_color_, ui_->palette_vertex);
  SetColor(QColor(255, 255, 255, 255), &background_color_,
           ui_->palette_background);
  SetColor(QColor(255, 23, 190, 255), &edge_color_, ui_->palette_edge);
  ui_->hs_edge_size->setValue(2);
  ui_->hs_vertex_size->setValue(5);
  ui_->cb_projection_style->setCurrentIndex(ProjectionType::ORTHOGONAL);
  ui_->cb_vertex_style->setCurrentIndex(VertexStyle::ROUND);
  ui_->cb_edge_style->setCurrentIndex(EdgeStyle::SOLID);
  emit DataUpdatedSignal();
}

void ViewSetup::SetColor(QColor color, QColor *var, QPushButton *btn) {
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

QColor ViewSetup::GetNewColor(QPushButton *btn, QLabel *txt, QColor col) {
  QColor temp =
      QColorDialog::getColor(col, this, "Select edges color",
                             QColorDialog::ColorDialogOption::ShowAlphaChannel);

  if (temp.isValid()) {
    col = temp;
    SetColor(col, nullptr, btn);
    ShowChosenColorInfo(btn, txt, col);
  }

  return col;
}

void ViewSetup::ShowChosenColorInfo(QPushButton *btn, QLabel *txt, QColor col) {
  txt->setText("R: " + QString::number(col.red()) +
               " G: " + QString::number(col.green()) +
               " B: " + QString::number(col.blue()));
}

void ViewSetup::SaveSettings() {
  settings_->setValue("projection_type",
                      ui_->cb_projection_style->currentIndex());
  settings_->setValue("edge_style", ui_->cb_edge_style->currentIndex());
  settings_->setValue("vertex_style", ui_->cb_vertex_style->currentIndex());
  settings_->setValue("background_color", background_color_);
  settings_->setValue("vertexes_color", vertex_color_);
  settings_->setValue("edges_color", edge_color_);
  settings_->setValue("edge_size", ui_->hs_edge_size->value());
  settings_->setValue("vertex_size", ui_->hs_vertex_size->value());
}

void ViewSetup::UploadSettings() {
  QVariant temp;
  ui_->cb_projection_style->setCurrentIndex(
      settings_->value("projection_type").toInt());
  ui_->cb_edge_style->setCurrentIndex(settings_->value("edge_style").toInt());
  ui_->cb_vertex_style->setCurrentIndex(
      settings_->value("vertex_style").toInt());

  temp = settings_->value("background_color");
  SetColor(temp.value<QColor>(), &background_color_, ui_->palette_background);
  ShowChosenColorInfo(ui_->palette_background, ui_->label_background_rgb,
                      background_color_);
  temp = settings_->value("vertexes_color");
  SetColor(temp.value<QColor>(), &vertex_color_, ui_->palette_vertex);
  ShowChosenColorInfo(ui_->palette_vertex, ui_->label_vertex_rgb,
                      vertex_color_);
  temp = settings_->value("edges_color");
  SetColor(temp.value<QColor>(), &edge_color_, ui_->palette_edge);
  ShowChosenColorInfo(ui_->palette_edge, ui_->label_edge_rgb, edge_color_);

  ui_->hs_edge_size->setValue(settings_->value("edge_size").toInt());
  ui_->hs_vertex_size->setValue(settings_->value("vertex_size").toInt());
  if (!temp.value<QColor>().isValid()) {
    ResetSlot();
  }
}

void ViewSetup::ConnectSignalSlot() {
  connect(ui_->cb_projection_style, &QComboBox::activated, this,
          &ViewSetup::DataUpdatedSignal);
  connect(ui_->cb_edge_style, &QComboBox::activated, this,
          &ViewSetup::DataUpdatedSignal);
  connect(ui_->cb_vertex_style, &QComboBox::activated, this,
          &ViewSetup::ManageVertexStyleDependenciesSlot);
  connect(ui_->palette_edge, &QPushButton::clicked, this,
          &ViewSetup::ChooseEdgeColorSlot);
  connect(ui_->palette_vertex, &QPushButton::clicked, this,
          &ViewSetup::ChooseVertexColorSlot);
  connect(ui_->palette_background, &QPushButton::clicked, this,
          &ViewSetup::ChooseBackgroundColorSlot);
  connect(ui_->hs_edge_size, &QSlider::sliderMoved, this,
          &ViewSetup::DataUpdatedSignal);
  connect(ui_->hs_vertex_size, &QSlider::sliderMoved, this,
          &ViewSetup::DataUpdatedSignal);
  connect(ui_->btn_reset, &QPushButton::clicked, this, &ViewSetup::ResetSlot);
}

}  // namespace S21
