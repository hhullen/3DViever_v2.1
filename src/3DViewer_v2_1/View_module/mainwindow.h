#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QFileDialog>
#include <QMainWindow>
#include <QRegularExpression>
#include <QScrollArea>
#include <QSettings>
#include <QThread>

#include "Controller_module/viewer_controller.h"
#include "Model_module/model_obj_model.h"
#include "View_module/oglview.h"
#include "gifimage/qgifimage.h"
#include "ptransform.h"
#include "screencap.h"
#include "viewsetup.h"
#include "lighting.h"
#include "texturing.h"
#include "recorder.h"

using s21::OBJModel;
using s21::ViewerController;

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

namespace s21 {

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
  void ManageLightingPanelSlot(bool state);
  void ManageTexturingPanelSlot(bool state);
  void GetScreenShotSlot();
  void GetGifSlot();
  void GetScreenCastSlot();
  void UpdateTransformationSlot();
  void UpdateViewSlot();
  void UpdateTransformationPanelSlot();
  void UpdateLightingSlot();
  void UpdateTexturingSlot(bool textured);
  const vector<float> *GetUVMapDataSlot();

 private:
  Ui::MainWindow *ui_;
  ViewerController *controller_;
  OGLview *ogl_view_;
  PTransform *transform_panel_;
  ViewSetup *view_panel_;
  ScreenCap *screen_cap_;
  Lighting *lighting_panel_;
  Texturing *texturing_panel_;
  ModelState state_;

  QString file_path_;
  QRegularExpression name_pattern_;
  Recorder *screenshot_;
  Recorder *gif_recorder_;
  Recorder *screencast_recorder_;
  QThread gif_thread_;
  QThread screencast_thread_;
  QTimer timer_;

  void SetSteerPanelComponentsAvailability(bool state);
  void SetModelInfo(ModelState state);

  void AddSteeringWidgetsToDockPanel();
  void ConnectSignalSlot();
  void SentMessage(QString message);
  void ModelRotationTick();
  void StopRecording();
};

}  // namespace S21

#endif  // MAINWINDOW_H
