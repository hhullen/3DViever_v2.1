#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QFileDialog>
#include <QMainWindow>
#include <QRegularExpression>
#include <QScrollArea>
#include <QSettings>
#include <QThread>

#include "Controller_module/viewer_controller.h"
#include "Model_module/model_frame.h"
#include "View_module/oglview.h"
#include "gifimage/qgifimage.h"
#include "ptransform.h"
#include "screencap.h"
#include "viewsetup.h"
using S21::ModelFrame;
using S21::ViewerController;

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

namespace S21 {

class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  MainWindow(ViewerController *controller, QWidget *parent = nullptr);
  ~MainWindow();
  void SaveSettings();
  void UploadSettings();

 private slots:
  void keyPressEvent(QKeyEvent *event);
  void keyReleaseEvent(QKeyEvent *event);
  void SteerPanelClosedSlot(bool state);
  void ManageSteerPanelSlot(bool state);
  void CloseAppSlot(bool state);
  void OpenNewFileSlot();
  void ManageTransformPanelSlot(bool state);
  void ManageViewSetupPanelSlot(bool state);
  void ManageScreenCapturePanelSlot(bool state);
  void GetScreenShotSlot();
  void GetGifSlot();
  void AddGifFrame();
  void UpdateTransformationSlot();
  void UpdateViewSlot();
  void UpdateTransformationPanelSlot();

 private:
  Ui::MainWindow *ui_;
  ViewerController *controller_;
  OGLview *ogl_view_;
  PTransform *transform_panel_;
  ViewSetup *view_panel_;
  ScreenCap *screen_cap_;

  QString file_path_;
  QRegularExpression name_pattern_;
  QString file_name_;
  QTimer *time_;
  QImage *frame_;
  QGifImage *gif_;
  unsigned int miliseconds_;
  bool recording_;

  void SetSteerPanelComponentsAvailability(bool state);
  void SetModelInfo();

  void AddSteeringWidgetsToDockPanel();
  void ConnectSignalSlot();
  void GetMediaName(QString *name, QString path);
};

}  // namespace S21

#endif  // MAINWINDOW_H
