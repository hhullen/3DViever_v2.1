#ifndef SCREENCAP_H
#define SCREENCAP_H

#include <QDesktopServices>
#include <QFileDialog>
#include <QRegularExpression>
#include <QSettings>
#include <QWidget>

#include "enum_parameters.h"

namespace Ui {
class ScreenCap;
}

namespace S21 {

class ScreenCap : public QWidget {
  Q_OBJECT

 public:
  explicit ScreenCap(QWidget *parent = nullptr);
  ~ScreenCap();
  QString get_media_path();
  ScreenshotFile get_screenshot_type();

 signals:
  void TakeScreenshotSignal();
  void RecordGifSignal();

 private slots:
  void ChooseDirectory();
  void OpenMediaFolder();
  void TakeScreenshotSlot();
  void RecordGifSlot();

 private:
  Ui::ScreenCap *ui_;
  QRegularExpression name_pattern_;
  QString files_path_;
  QSettings *settings_;

  void CheckMediaPath();
  void SetPathToButtonName();
  void SaveSettings();
  void UploadSettings();
  void ConnectSignalSlot();
};

}  // namespace S21

#endif  // SCREENCAP_H
