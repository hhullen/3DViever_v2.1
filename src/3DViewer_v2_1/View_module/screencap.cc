#include "screencap.h"

#include "ui_screencap.h"

namespace S21 {

ScreenCap::ScreenCap(QWidget *parent)
    : QWidget(parent), ui_(new Ui::ScreenCap) {
  ui_->setupUi(this);
  name_pattern_.setPattern("[^\\/]*$");
  settings_ = new QSettings("ScreenCapture", "FilePath", this);
  UploadSettings();

  ConnectSignalSlot();
}

ScreenCap::~ScreenCap() {
  SaveSettings();
  delete ui_;
}

QString ScreenCap::get_media_path() { return files_path_; }

ScreenshotFile ScreenCap::get_screenshot_type() {
  return (ScreenshotFile)ui_->cb_screenshot_type->currentIndex();
}

void ScreenCap::SetPathToButtonName() {
  QRegularExpressionMatch match = name_pattern_.match(files_path_);
  ui_->btn_folder_path->setText(match.captured());
}

void ScreenCap::ChooseDirectory() {
  QString temp = QFileDialog::getExistingDirectory(
      this, "Choose folder to save screen", "/Users",
      QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
  if (!temp.isEmpty() && temp != "0") {
    files_path_ = temp;
    SetPathToButtonName();
  }
}

void ScreenCap::OpenMediaFolder() {
  CheckMediaPath();
  if (!files_path_.isEmpty() && files_path_ != "0") {
    QDesktopServices::openUrl(QUrl::fromLocalFile(files_path_));
  }
}

void ScreenCap::TakeScreenshotSlot() {
  CheckMediaPath();
  if (!files_path_.isEmpty() && files_path_ != "0") {
    emit TakeScreenshotSignal();
  }
}

void ScreenCap::RecordGifSlot() {
  CheckMediaPath();
  if (!files_path_.isEmpty() && files_path_ != "0") {
    emit RecordGifSignal();
  }
}

void ScreenCap::CheckMediaPath() {
  if (files_path_.isEmpty() || files_path_ == "0") {
    ChooseDirectory();
  }
}

void ScreenCap::SaveSettings() {
  settings_->setValue("media_path", files_path_);
}

void ScreenCap::UploadSettings() {
  files_path_ = settings_->value("media_path").toString();
  SetPathToButtonName();
}

void ScreenCap::ConnectSignalSlot() {
  connect(ui_->btn_folder_path, &QPushButton::clicked, this,
          &ScreenCap::ChooseDirectory);
  connect(ui_->btn_open_folder, &QPushButton::clicked, this,
          &ScreenCap::OpenMediaFolder);
  connect(ui_->btn_screenshot, &QPushButton::clicked, this,
          &ScreenCap::TakeScreenshotSlot);
  connect(ui_->btn_record, &QPushButton::clicked, this,
          &ScreenCap::RecordGifSlot);
}

}  // namespace S21
