#ifndef LIGHTING_H
#define LIGHTING_H

#include <QColorDialog>
#include <QLabel>
#include <QSettings>
#include <QStyle>
#include <QWidget>

namespace Ui {
class Lighting;
}

namespace s21 {

class Lighting : public QWidget
{
    Q_OBJECT

public:
    explicit Lighting(QWidget *parent = nullptr);
    ~Lighting();
    void set_turned_on(bool state);

    float get_power();
    void get_position(float *x, float *y, float *z);
    QColor get_color();
    bool IsTurnedOn();

private:
    Ui::Lighting *ui_;
    QColor color_;
    QSettings *settings_;

    void SetDisabled(bool state);
    void GetNewColor();
    void SetColor(QColor color, QColor *var, QPushButton *btn);
    void ShowChosenColorInfo(QLabel *txt, QColor col);
    void SwitchState(int index);
    void SaveSettings();
    void UploadSettings();

};

}

#endif // LIGHTING_H
