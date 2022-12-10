#ifndef PTRANSFORM_H
#define PTRANSFORM_H

#include <QDoubleSpinBox>
#include <QWidget>

namespace Ui {
class PTransform;
}

namespace s21 {

class PTransform : public QWidget {
  Q_OBJECT

 public:
  explicit PTransform(QWidget *parent = nullptr);
  ~PTransform();
  void get_position(float *x, float *y, float *z);
  void get_angle(float *x, float *y, float *z);
  float get_scale();
  void set_position(float *x, float *y, float *z);
  void set_angle(float *x, float *y, float *z);
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
