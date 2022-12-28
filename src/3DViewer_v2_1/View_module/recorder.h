#ifndef RECORDER_H
#define RECORDER_H

#include <QDateTime>
#include <QObject>
#include <QTimer>

#include "gifimage/qgifimage.h"
#include "oglview.h"
#include "screencap.h"

namespace s21 {

class Recorder : public QObject {
  Q_OBJECT

 public:
  Recorder(QImage *screen, ScreenCap *rec_panel);
  void TakeScreenshot(QImage screen);
  void RecordGif();
  void SetRecordParameters(int sec, int fps, int w, int h);

 signals:
  void FinishedSignal();
  void SentMessage(QString message, int delay);

 private:
  int sec_;
  int fps_;
  int width_;
  int height_;
  QImage *screen_;
  ScreenCap *panel_;

  void GetMediaName(QString *name, QString path);
};

}  // namespace s21

#endif  // RECORDER_H
