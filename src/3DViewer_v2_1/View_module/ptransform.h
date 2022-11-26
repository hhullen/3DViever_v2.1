#ifndef PTRANSFORM_H
#define PTRANSFORM_H

#include <QDoubleSpinBox>
#include <QWidget>

namespace Ui {
class PTransform;
}

namespace S21 {

class PTransform : public QWidget {
  Q_OBJECT

 public:
  explicit PTransform(QWidget *parent = nullptr);
  ~PTransform();
  void get_position(double *x, double *y, double *z);
  void get_angle(double *x, double *y, double *z);
  double get_scale();
  void set_position(double *x, double *y, double *z);
  void set_angle(double *x, double *y, double *z);
  void set_scale(double scale);

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
