#include "mainwindow.h"

#include "./ui_mainwindow.h"

namespace S21 {

MainWindow::MainWindow(ViewerController *controller, QWidget *parent)
    : controller_(controller), QMainWindow(parent), ui_(new Ui::MainWindow) {
  ui_->setupUi(this);
  ogl_view_ = new OGLview();
  ui_->central_widget->layout()->addWidget(ogl_view_);
  transform_panel_ = new PTransform();
  view_panel_ = new ViewSetup();
  screen_cap_ = new ScreenCap();
  recording_ = false;

  view_panel_->setVisible(false);
  screen_cap_->setVisible(false);

  name_pattern_.setPattern("[^\\/]*$");

  ui_->dock_widget->hide();
  ui_->statusbar->setToolTip("HELLO MF!");

  SetSteerPanelComponentsAvailability(false);

  AddSteeringWidgetsToDockPanel();
  ConnectSignalSlot();
  UpdateViewSlot();
  UpdateTransformationSlot();
}

MainWindow::~MainWindow() { delete ui_; }

void MainWindow::GetScreenShotSlot() {
  if (!screen_cap_->get_media_path().isEmpty()) {
    QString file_name;
    QImage img = ogl_view_->grabFramebuffer();
    GetMediaName(&file_name, screen_cap_->get_media_path());
    if (screen_cap_->get_screenshot_type() == ScreenshotFile::JPEG) {
      file_name.append(".jpeg");
      img.save(file_name, "JPEG");
    } else if (screen_cap_->get_screenshot_type() == ScreenshotFile::BMP) {
      file_name.append(".bmp");
      img.save(file_name, "BMP");
    }
    ogl_view_->ShowEventMessage(
        "Screenshot saved to: " + screen_cap_->get_media_path(), 3000);
  }
}

void MainWindow::GetGifSlot() {
  if (!screen_cap_->get_media_path().isEmpty() && !recording_) {
    file_name_ = screen_cap_->get_media_path();
    recording_ = true;
    miliseconds_ = 0;
    gif_ = new QGifImage;
    frame_ = new QImage();
    time_ = new QTimer();
    GetMediaName(&file_name_, screen_cap_->get_media_path());
    file_name_.append(".gif");
    connect(time_, &QTimer::timeout, this, &MainWindow::AddGifFrame);
    time_->start(100);
    ogl_view_->ShowEventMessage("Recording in progress...", 1000);
  }
}

void MainWindow::AddGifFrame() {
  *frame_ = ogl_view_->grabFramebuffer();
  *frame_ = frame_->scaled(640, 480);
  gif_->addFrame(*frame_, 100);
  miliseconds_ += 100;
  if (miliseconds_ >= 5000) {
    time_->stop();
    gif_->save(file_name_);
    ogl_view_->ShowEventMessage(
        "Gif saved to: " + screen_cap_->get_media_path(), 3000);
    recording_ = false;
    delete time_;
    delete frame_;
    delete gif_;
  }
}

void MainWindow::UpdateViewSlot() {
  ogl_view_->set_background_color(view_panel_->get_background_color());
  ogl_view_->set_edges_color(view_panel_->get_edges_color());
  ogl_view_->set_edges_size(view_panel_->get_edges_size());
  ogl_view_->set_edges_style(view_panel_->get_edges_style());
  ogl_view_->set_vertexes_color(view_panel_->get_vertex_color());
  ogl_view_->set_vertexes_size(view_panel_->get_vertex_size());
  ogl_view_->set_vertexes_style(view_panel_->get_vertex_style());
  ogl_view_->set_projection_type(view_panel_->get_projection_type());
  ogl_view_->set_projection_state(true);
  ogl_view_->update();
}

void MainWindow::UpdateTransformationPanelSlot() {
  double x, y, z;

  ogl_view_->get_position(&x, &y, &z);
  transform_panel_->set_position(&x, &y, &z);

  ogl_view_->get_angle(&x, &y, &z);
  transform_panel_->set_angle(&x, &y, &z);

  transform_panel_->set_scale(ogl_view_->get_scale());
}

void MainWindow::UpdateTransformationSlot() {
  double x, y, z;

  transform_panel_->get_position(&x, &y, &z);
  ogl_view_->set_position(x, y, z);

  transform_panel_->get_angle(&x, &y, &z);
  ogl_view_->set_angle(x, y, z);

  ogl_view_->set_scale(transform_panel_->get_scale());
  ogl_view_->update();
}

void MainWindow::GetMediaName(QString *name, QString path) {
  QDateTime date_time;
  name->clear();
  name->append(path);
  name->append("/screen" +
               date_time.currentDateTime().toString("yyyy_MM_dd_hh_mm_ss"));
}

void MainWindow::keyPressEvent(QKeyEvent *event) {
  if (event->key() == Qt::Key_Space) {
    ogl_view_->set_key_spcace_state(true);
  }
}

void MainWindow::keyReleaseEvent(QKeyEvent *event) {
  if (event->key() == Qt::Key_Space) {
    ogl_view_->set_key_spcace_state(false);
  }
}

void MainWindow::ManageTransformPanelSlot(bool state) {
  transform_panel_->setVisible(state);
}

void MainWindow::ManageViewSetupPanelSlot(bool state) {
  view_panel_->setVisible(state);
}

void MainWindow::ManageScreenCapturePanelSlot(bool state) {
  screen_cap_->setVisible(state);
}

void MainWindow::SteerPanelClosedSlot(bool state) {
  ui_->actionSteer_panel->setChecked(state);
}

void MainWindow::ManageSteerPanelSlot(bool state) {
  ui_->dock_widget->setVisible(state);
  ui_->dock_widget->setFloating(false);
}

void MainWindow::CloseAppSlot(bool state) { QApplication::quit(); }

void MainWindow::OpenNewFileSlot() {
  file_path_ = QFileDialog::getOpenFileName(this, "Open .obj file", "/Users",
                                            "obj (*.obj);;");
  if (!file_path_.isEmpty()) {
    bool is_loaded = controller_->UploadNewModel(file_path_.toStdString());
    if (is_loaded) {
      SetModelInfo();
      UpdateViewSlot();
      UpdateTransformationSlot();
      SetSteerPanelComponentsAvailability(true);
      ogl_view_->set_model_vertexes_vector(controller_->get_vertexes_vector());
      ogl_view_->set_model_indices_vector(controller_->get_indices_vector());
      ogl_view_->set_model_facets_amount(controller_->get_facets_amount());
      ogl_view_->ShowEventMessage("Successfully loaded", 2000);
      ogl_view_->DrawModel();
    } else {
      ogl_view_->ShowEventMessage("Can not upload file", 3000);
      SetSteerPanelComponentsAvailability(false);
    }
  }
}

void MainWindow::SetSteerPanelComponentsAvailability(bool state) {
  transform_panel_->setDisabled(!state);
  view_panel_->setDisabled(!state);
  screen_cap_->setDisabled(!state);
}

void MainWindow::SetModelInfo() {
  QString status_bar_info;
  QRegularExpressionMatch match_ = name_pattern_.match(file_path_);

  status_bar_info.append("  File name: ");
  status_bar_info.append(match_.captured());
  status_bar_info.append("  Vertexes: ");
  status_bar_info.append(QString::number(controller_->get_vertexes_amount()));
  status_bar_info.append("  Edges: ");
  status_bar_info.append(QString::number(controller_->get_facets_amount()));
  ui_->statusbar->showMessage(status_bar_info);
}

void MainWindow::AddSteeringWidgetsToDockPanel() {
  ui_->scroll_area_contents->layout()->setAlignment(Qt::AlignTop |
                                                    Qt::AlignLeft);
  ui_->scroll_area_contents->layout()->setSpacing(0);
  ui_->scroll_area_contents->layout()->addWidget(transform_panel_);
  ui_->scroll_area_contents->layout()->addWidget(view_panel_);
  ui_->scroll_area_contents->layout()->addWidget(screen_cap_);
}

void MainWindow::ConnectSignalSlot() {
  connect(ui_->dock_widget, &QDockWidget::visibilityChanged, this,
          &MainWindow::SteerPanelClosedSlot);
  connect(ui_->actionSteer_panel, &QAction::triggered, this,
          &MainWindow::ManageSteerPanelSlot);
  connect(ui_->action_close, &QAction::triggered, this,
          &MainWindow::CloseAppSlot);
  connect(ui_->action_transform, &QAction::triggered, this,
          &MainWindow::ManageTransformPanelSlot);
  connect(ui_->action_setup_view, &QAction::triggered, this,
          &MainWindow::ManageViewSetupPanelSlot);
  connect(ui_->action_screen_capture, &QAction::triggered, this,
          &MainWindow::ManageScreenCapturePanelSlot);
  connect(ui_->action_open_new, &QAction::triggered, this,
          &MainWindow::OpenNewFileSlot);
  connect(transform_panel_, &PTransform::DataUpdatedSignal, this,
          &MainWindow::UpdateTransformationSlot);
  connect(view_panel_, &ViewSetup::DataUpdatedSignal, this,
          &MainWindow::UpdateViewSlot);
  connect(screen_cap_, &ScreenCap::TakeScreenshotSignal, this,
          &MainWindow::GetScreenShotSlot);
  connect(screen_cap_, &ScreenCap::RecordGifSignal, this,
          &MainWindow::GetGifSlot);
  connect(ogl_view_, &OGLview::PositionUpdatedSignal, this,
          &MainWindow::UpdateTransformationPanelSlot);
}

}  // namespace S21
