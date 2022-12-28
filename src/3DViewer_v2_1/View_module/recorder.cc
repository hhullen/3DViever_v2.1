#include "recorder.h"

namespace s21 {

Recorder::Recorder(QImage *screen, ScreenCap *rec_panel)
    : sec_(5),
      fps_(10),
      width_(640),
      height_(480),
      screen_(screen),
      panel_(rec_panel) {}

void Recorder::TakeScreenshot(QImage screen) {
  if (!panel_->get_media_path().isEmpty()) {
    QString file_name;
    GetMediaName(&file_name, panel_->get_media_path());
    if (panel_->get_screenshot_type() == ScreenshotFile::JPEG) {
      file_name.append(".jpeg");
      screen.save(file_name, "JPEG");
    } else if (panel_->get_screenshot_type() == ScreenshotFile::BMP) {
      file_name.append(".bmp");
      screen.save(file_name, "BMP");
    }
    emit SentMessage("Screenshot saved to: " + panel_->get_media_path(), 3000);
  }
}

void Recorder::RecordGif() {
  QTimer timer;
  QGifImage gif_;
  int ms = sec_ * 1000;
  int ms_per_frame = 1000 / fps_;

  emit SentMessage("Recording in progress...", 1000);
  timer.start(ms_per_frame);
  while (ms >= 0) {
    if (timer.remainingTime() <= 0) {
      QImage frame =
          screen_->scaled(width_, height_, Qt::AspectRatioMode::KeepAspectRatio,
                          Qt::TransformationMode::SmoothTransformation);
      gif_.addFrame(frame, ms_per_frame);
      ms -= ms_per_frame;
      timer.start(ms_per_frame);
    }
  }

  QString file_name = "";
  GetMediaName(&file_name, panel_->get_media_path());
  gif_.save(file_name + ".gif");
  emit SentMessage("Gif saved to: " + panel_->get_media_path(), 3000);
  emit FinishedSignal();
}

void Recorder::SetRecordParameters(int sec, int fps, int w, int h) {
  if (sec > 0) {
    sec_ = sec;
  }
  if (fps > 0 && fps <= 240) {
    fps_ = fps;
  }
  if (w > 0) {
    width_ = w;
  }
  if (h > 0) {
    height_ = h;
  }
}

void Recorder::GetMediaName(QString *name, QString path) {
  QDateTime date_time;
  name->clear();
  name->append(path);
  name->append("/screen" +
               date_time.currentDateTime().toString("yyyy_MM_dd_hh_mm_ss"));
}

}  // namespace s21
