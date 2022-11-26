#ifndef VIEWSETUP_H
#define VIEWSETUP_H

#include <QColorDialog>
#include <QLabel>
#include <QSettings>
#include <QStyle>
#include <QWidget>

#include "enum_parameters.h"

namespace Ui {
class ViewSetup;
}

namespace S21 {

class ViewSetup : public QWidget {
  Q_OBJECT

 public:
  explicit ViewSetup(QWidget *parent = nullptr);
  ~ViewSetup();
  ProjectionType get_projection_type();
  EdgeStyle get_edges_style();
  QColor get_edges_color();
  int get_edges_size();
  VertexStyle get_vertex_style();
  QColor get_vertex_color();
  int get_vertex_size();
  QColor get_background_color();

 signals:
  void DataUpdatedSignal();

 private slots:
  void ManageVertexStyleDependenciesSlot(int index);
  void ChooseEdgeColorSlot();
  void ChooseVertexColorSlot();
  void ChooseBackgroundColorSlot();
  void ResetSlot();

 private:
  Ui::ViewSetup *ui_;
  QSettings *settings_;

  QColor vertex_color_;
  QColor background_color_;
  QColor edge_color_;
  bool first_launch_;

  void SetColor(QColor color, QColor *var, QPushButton *btn);
  QColor GetNewColor(QPushButton *btn, QLabel *txt, QColor col);
  void ShowChosenColorInfo(QPushButton *btn, QLabel *txt, QColor col);
  void SaveSettings();
  void UploadSettings();
  void ConnectSignalSlot();
};

}  // namespace S21

#endif  // VIEWSETUP_H
