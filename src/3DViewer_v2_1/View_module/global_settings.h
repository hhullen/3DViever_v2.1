#ifndef GLOBAL_SETTINGS_H
#define GLOBAL_SETTINGS_H

#include <QFont>
#include <QFontDatabase>

namespace s21 {

class GlobalSettings {
 public:
  QFont font;
  GlobalSettings() {
    QFontDatabase::addApplicationFont(
        ":/font/Resources/microsoftsansserif_hard.ttf");
    font.setFamily("microsoftsansserif_hard");
    font.setPointSize(16);
    font.setBold(true);
  }
};

}  // namespace s21

#endif  // GLOBAL_SETTINGS_H
