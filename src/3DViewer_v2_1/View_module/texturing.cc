#include "texturing.h"

#include "ui_texturing.h"

namespace s21 {

Texturing::Texturing(QFont &font, QWidget *parent)
    : QWidget(parent),
      ui_(new Ui::Texturing),
      texture_path_(""),
      map_path_(""),
      data_(nullptr) {
  this->setFont(font);
  ui_->setupUi(this);
  ui_->label_panel_name->setFont(font);
  ui_->btn_add_remove->setFont(font);
  ui_->btn_folder_path->setFont(font);
  ui_->btn_open->setFont(font);
  ui_->btn_save->setFont(font);

  name_pattern_.setPattern("[^\\/]*$");
  ui_->btn_save->setDisabled(true);
  ui_->btn_open->setDisabled(true);

  color_ = QColor(0, 255, 0);
  PanelFuncs::SetColor(QColor(0, 255, 0), nullptr, ui_->palette_map);
  PanelFuncs::ShowChosenColorInfo(ui_->label_map_rgb, QColor(0, 255, 0));

  connect(ui_->btn_add_remove, &QPushButton::clicked, this,
          &Texturing::AddRemove);
  connect(ui_->btn_folder_path, &QPushButton::clicked, this,
          &Texturing::ChooseDirectory);
  connect(ui_->palette_map, &QPushButton::clicked, this,
          &Texturing::GetNewColor);
  connect(ui_->btn_save, &QPushButton::clicked, this, &Texturing::SaveUVMap);
  connect(ui_->btn_open, &QPushButton::clicked, this,
          &Texturing::OpenMediaFolder);
}

Texturing::~Texturing() { delete ui_; }

QImage Texturing::get_texture() { return texture_; }

void Texturing::set_untextured() {
  texture_path_.clear();
  ui_->btn_add_remove->setText("Add");
  ui_->btn_save->setDisabled(true);
  emit DataUpdatedSignal(false);
}

void Texturing::AddRemove() {
  if (texture_path_.isEmpty()) {
    texture_path_ = QFileDialog::getOpenFileName(this, "Open .bmp file",
                                                 "/Users", "bmp (*.bmp);;");
    if (!texture_path_.isEmpty()) {
      texture_.load(texture_path_);
      if (texture_.width() == texture_.height()) {
        ui_->btn_add_remove->setText("Remove");
        if (!map_path_.isEmpty()) {
          ui_->btn_save->setDisabled(false);
        }
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
  if (ui_->btn_add_remove->text() == "Remove") {
    ui_->btn_save->setDisabled(false);
  }
  ui_->btn_open->setDisabled(false);
}

void Texturing::GetNewColor() {
  QColor temp =
      QColorDialog::getColor(color_, this, "Select lines color",
                             QColorDialog::ColorDialogOption::ShowAlphaChannel);

  if (temp.isValid()) {
    color_ = temp;
    PanelFuncs::SetColor(color_, nullptr, ui_->palette_map);
    PanelFuncs::ShowChosenColorInfo(ui_->label_map_rgb, color_);
  }
}

void Texturing::SaveUVMap() {
  data_ = emit RequireUVMapData();

  QGraphicsScene scene;
  scene.addPixmap(QPixmap::fromImage(texture_.mirrored(false, true)));

  QGraphicsView view;
  view.setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  view.setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

  QWidget *viewport;
  viewport = view.viewport();
  viewport->setFixedSize(texture_.width(), texture_.height());
  view.setViewport(viewport);
  view.setScene(&scene);
  view.setSceneRect(texture_.rect());

  QPen pen(color_, 1);
  size_t shift = 8;
  for (size_t i = shift * 2 + 3; data_ && i < data_->size(); i += shift * 3) {
    scene.addLine(MakeLine(i, shift * 2, shift), pen);
    scene.addLine(MakeLine(i, shift, 0), pen);
    scene.addLine(MakeLine(i, 0, shift * 2), pen);
  }

  QDateTime date_time;
  QString filename =
      map_path_ + "/" + "map_" +
      date_time.currentDateTime().toString("yyyy_MM_dd_hh_mm_ss") + ".bmp";
  view.grab(texture_.rect()).toImage().mirrored(false, true).save(filename);
  emit SentMessageSignal("UV map saved to " + filename);
}

QLineF Texturing::MakeLine(size_t i, size_t shif1, size_t shift2) {
  QPointF point1((*data_)[i - shif1] * texture_.width(),
                 (*data_)[i - shif1 + 1] * texture_.width());
  QPointF point2((*data_)[i - shift2] * texture_.width(),
                 (*data_)[i - shift2 + 1] * texture_.width());
  return QLineF(point1, point2);
}

void Texturing::OpenMediaFolder() {
  if (!map_path_.isEmpty()) {
    QDesktopServices::openUrl(QUrl::fromLocalFile(map_path_));
  }
}

}  // namespace s21
