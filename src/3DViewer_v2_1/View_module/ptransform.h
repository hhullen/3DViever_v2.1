#ifndef PTRANSFORM_H
#define PTRANSFORM_H

#include <QDoubleSpinBox>
#include <QWidget>
#include <QVector3D>

namespace Ui {
class PTransform;
}

namespace s21 {

class PTransform : public QWidget {
  Q_OBJECT

 public:
  explicit PTransform(QWidget *parent = nullptr);
  ~PTransform();
  QVector3D get_position();
  QVector3D get_angle();
  float get_scale();
  void set_position(QVector3D position);
  void set_angle(QVector3D angle);
  void set_scale(float scale);

 signals:
  void DataUpdatedSignal();

 private slots:
  void ResetSlot();

 private:
  Ui::PTransform *ui_;
  void ConnectSignalSlot();
};

}  // namespace S21

#endif  // PTRANSFORM_H
