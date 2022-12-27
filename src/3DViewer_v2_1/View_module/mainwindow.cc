#include "mainwindow.h"

#include "./ui_mainwindow.h"

namespace s21 {

MainWindow::MainWindow(ViewerController *controller, QWidget *parent)
    : QMainWindow(parent), ui_(new Ui::MainWindow), controller_(controller) {
  ui_->setupUi(this);
  ogl_view_ = new OGLview();
  ui_->central_widget->layout()->addWidget(ogl_view_);
  transform_panel_ = new PTransform();
  view_panel_ = new ViewSetup();
  screen_cap_ = new ScreenCap();
  lighting_panel_ = new Lighting();
  texturing_panel_ = new Texturing();
  gif_recorder_ = new Recorder(ogl_view_->get_screen_pointer(), screen_cap_);
  screencast_recorder_ =  new Recorder(ogl_view_->get_screen_pointer(), screen_cap_);

  view_panel_->setVisible(false);
  screen_cap_->setVisible(false);
  lighting_panel_->setVisible(false);
  texturing_panel_->setVisible(false);

  name_pattern_.setPattern("[^\\/]*$");

  ui_->dock_widget->hide();
  ui_->statusbar->setToolTip("HELLO MF!");

  SetSteerPanelComponentsAvailability(false);

  AddSteeringWidgetsToDockPanel();
  ConnectSignalSlot();
  UpdateViewSlot();
  UpdateTransformationSlot();
}

MainWindow::~MainWindow() {
    while (gif_thread_.isRunning() || screencast_thread_.isRunning()) {
        gif_thread_.terminate();
        screencast_thread_.terminate();
    }
    delete ui_;
}

void MainWindow::GetScreenShotSlot() {
    ogl_view_->set_recording(true);
    ogl_view_->update();
    gif_recorder_->TakeScreenshot(ogl_view_->grabFramebuffer());
    ogl_view_->set_recording(false);
}

void MainWindow::GetGifSlot() {
    if (!gif_thread_.isRunning()) {
        ogl_view_->set_recording(true);
        ogl_view_->update();
        gif_recorder_->moveToThread(&gif_thread_);
        gif_thread_.start();
    }
}

void MainWindow::GetScreenCastSlot() {
    if (!screencast_thread_.isRunning()) {
        ogl_view_->set_recording(true);
        ogl_view_->update();
        screencast_recorder_->moveToThread(&screencast_thread_);
        SetSteerPanelComponentsAvailability(false);
        screencast_thread_.start();
        timer_.start(30);
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
  ogl_view_->set_drawing_type(view_panel_->get_drawing_type());
  ogl_view_->set_polygon_color(view_panel_->get_polygon_color());

  if (state_ == ModelState::VertTexNorm || state_ == ModelState::VertNorm) {
    ogl_view_->set_shading_type(view_panel_->get_shading_type());
  } else {
      ogl_view_->set_shading_type(ShadeMode::NOMAP);
  }
  ogl_view_->update();
}

void MainWindow::UpdateTransformationPanelSlot() {
  QVector3D *transform;

  transform = ogl_view_->get_position();
  transform_panel_->set_position(*transform);

  transform = ogl_view_->get_angle();
  transform_panel_->set_angle(*transform);

  transform_panel_->set_scale(ogl_view_->get_scale());
}

void MainWindow::UpdateLightingSlot() {
    ogl_view_->set_light_color(lighting_panel_->get_color());
    ogl_view_->set_light_position(lighting_panel_->get_position());
    ogl_view_->set_light_power(lighting_panel_->get_power());
    ogl_view_->update();
}

void MainWindow::UpdateTexturingSlot(bool textured) {
    if (textured) {
        ogl_view_->set_textured(texturing_panel_->get_texture());
    } else {
        ogl_view_->set_untextured();
    }
}

void MainWindow::SentMessage(QString message) {
    ogl_view_->ShowEventMessage(message, 3000);
}

void MainWindow::ModelRotationTick() {
    if (screencast_thread_.isRunning()) {
        QVector3D transform;

        transform = transform_panel_->get_angle();
        transform.setY(transform.y() + 2);
        transform_panel_->set_angle(transform);
        ogl_view_->set_angle(transform);
    } else {
        timer_.stop();
        SetSteerPanelComponentsAvailability(true);
        screencast_thread_.terminate();
        ogl_view_->set_recording(false);
    }
}

void MainWindow::StopRecording() {
    gif_thread_.terminate();
    ogl_view_->set_recording(false);
}

const std::vector<float> *MainWindow::GetUVMapDataSlot() {
    return controller_->get_ordered_data_vector();
}

void MainWindow::UpdateTransformationSlot() {
  QVector3D transform;

  transform = transform_panel_->get_position();
  ogl_view_->set_position(transform);

  transform = transform_panel_->get_angle();
  ogl_view_->set_angle(transform);

  ogl_view_->set_scale(transform_panel_->get_scale());
  ogl_view_->update();
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

void MainWindow::ManageLightingPanelSlot(bool state) {
    lighting_panel_->setVisible(state);
}

void MainWindow::ManageTexturingPanelSlot(bool state) {
    texturing_panel_->setVisible(state);
}

void MainWindow::SteerPanelClosedSlot(bool state) {
  ui_->actionSteer_panel->setChecked(state);
}

void MainWindow::ManageSteerPanelSlot(bool state) {
  ui_->dock_widget->setVisible(state);
  ui_->dock_widget->setFloating(false);
}

void MainWindow::CloseAppSlot(bool state) {
    if (!state) {
        QApplication::quit();
    }
}

void MainWindow::OpenNewFileSlot() {
  file_path_ = QFileDialog::getOpenFileName(this, "Open .obj file", "/Users",
                                            "obj (*.obj);;");
  if (!file_path_.isEmpty()) {
    bool is_loaded = controller_->UploadNewModel(file_path_.toStdString());
    if (is_loaded) {
      texturing_panel_->set_untextured();
      SetSteerPanelComponentsAvailability(true);
      state_ = controller_->get_model_state();
      SetModelInfo(state_);
      UpdateViewSlot();
      UpdateTransformationSlot();
      ogl_view_->set_model_ordered_vertexes_vector(controller_->get_ordered_data_vector());
      ogl_view_->set_model_ordered_indices_vector(controller_->get_ordered_indices_vector());
      ogl_view_->set_model_vertexes_vector(controller_->get_vertexes_vector());
      ogl_view_->set_model_indices_vector(controller_->get_indices_vector());
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
  lighting_panel_->setDisabled(!state);
  texturing_panel_->setDisabled(!state);
}

void MainWindow::SetModelInfo(ModelState state) {
  QString status_bar_info;
  QRegularExpressionMatch match_ = name_pattern_.match(file_path_);

  status_bar_info.append("  File name: ");
  status_bar_info.append(match_.captured());
  status_bar_info.append("  Vertexes: ");
  status_bar_info.append(QString::number(controller_->get_vertexes_amount()));
  status_bar_info.append("  Edges: ");
  status_bar_info.append(QString::number(controller_->get_facets_amount()));
  if (state == ModelState::Vert) {
       status_bar_info.append("  [vertex only]");
       texturing_panel_->setDisabled(true);
  } else if (state == ModelState::VertNorm) {
      status_bar_info.append("  [vertex/normal]");
      texturing_panel_->setDisabled(true);
  } else if (state == ModelState::VertTex) {
      status_bar_info.append("  [vertex/texture]");
      texturing_panel_->setDisabled(false);
  } else if (state == ModelState::VertTexNorm) {
      status_bar_info.append("  [vertex/texture/normal]");
      texturing_panel_->setDisabled(false);
  }
  ui_->statusbar->showMessage(status_bar_info);
}

void MainWindow::AddSteeringWidgetsToDockPanel() {
  ui_->scroll_area_contents->layout()->setAlignment(Qt::AlignTop |
                                                    Qt::AlignLeft);
  ui_->scroll_area_contents->layout()->setSpacing(0);
  ui_->scroll_area_contents->layout()->addWidget(transform_panel_);
  ui_->scroll_area_contents->layout()->addWidget(view_panel_);
  ui_->scroll_area_contents->layout()->addWidget(lighting_panel_);
  ui_->scroll_area_contents->layout()->addWidget(texturing_panel_);
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
  connect(ui_->actionLighting, &QAction::triggered, this,
          &MainWindow::ManageLightingPanelSlot);
  connect(ui_->actionTexturing, &QAction::triggered, this,
          &MainWindow::ManageTexturingPanelSlot);
  connect(ui_->action_screen_capture, &QAction::triggered, this,
          &MainWindow::ManageScreenCapturePanelSlot);
  connect(ui_->action_open_new, &QAction::triggered, this,
          &MainWindow::OpenNewFileSlot);
  connect(transform_panel_, &PTransform::DataUpdatedSignal, this,
          &MainWindow::UpdateTransformationSlot);
  connect(view_panel_, &ViewSetup::DataUpdatedSignal, this,
          &MainWindow::UpdateViewSlot);
  connect(screen_cap_, &ScreenCap::TakeScreenshotSignal,
          this, &MainWindow::GetScreenShotSlot);
  connect(screen_cap_, &ScreenCap::RecordGifSignal, this,
          &MainWindow::GetGifSlot);
  connect(screen_cap_, &ScreenCap::RecordScreencastSignal, this,
          &MainWindow::GetScreenCastSlot);
  connect(ogl_view_, &OGLview::PositionUpdatedSignal, this,
          &MainWindow::UpdateTransformationPanelSlot);
  connect(lighting_panel_, &Lighting::DataUpdatedSignal, this,
          &MainWindow::UpdateLightingSlot);
  connect(texturing_panel_, &Texturing::DataUpdatedSignal, this,
          &MainWindow::UpdateTexturingSlot);
  connect(texturing_panel_, &Texturing::SentMessageSignal, this,
          &MainWindow::SentMessage);
  connect(texturing_panel_, &Texturing::RequireUVMapData, this,
          &MainWindow::GetUVMapDataSlot);

  connect(&gif_thread_, &QThread::started, gif_recorder_, &Recorder::RecordGif);
  connect(&screencast_thread_, &QThread::started, screencast_recorder_, &Recorder::RecordGif);
  connect(gif_recorder_, &Recorder::FinishedSignal, this, &MainWindow::StopRecording);
  connect(screencast_recorder_, &Recorder::FinishedSignal, &screencast_thread_, &QThread::terminate);
  connect(gif_recorder_, &Recorder::SentMessage, ogl_view_, &OGLview::ShowEventMessage);
  connect(screencast_recorder_, &Recorder::SentMessage, ogl_view_, &OGLview::ShowEventMessage);
  connect(&timer_, &QTimer::timeout, this, &MainWindow::ModelRotationTick);
}

}  // namespace S21
