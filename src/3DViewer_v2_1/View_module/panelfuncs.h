#ifndef PANELFUNCS_H
#define PANELFUNCS_H

#include <QColor>
#include <QLabel>
#include <QPushButton>

namespace s21 {

class PanelFuncs {
 public:
  static void SetColor(QColor color, QColor *var, QPushButton *btn) {
    if (var) {
      *var = color;
    }
    btn->setStyleSheet(
        "QPushButton {"
        "border: 2px solid rgba(90, 90, 90, 0.6);"
        "border-radius: 13px;"
        "background-color: " +
        color.name(QColor::NameFormat::HexArgb) +
        ";"
        "color: rgba(0, 0, 0, 0);"
        "}\n"
        "QPushButton:pressed {"
        "background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,"
        "stop: 0 #404040 stop: 1 #686868);"
        "}");
  }

  static void ShowChosenColorInfo(QLabel *txt, QColor col) {
    txt->setText("R: " + QString::number(col.red()) +
                 " G: " + QString::number(col.green()) +
                 " B: " + QString::number(col.blue()));
  }
};

}  // namespace s21

#endif  // PANELFUNCS_H
