#include <QApplication>
#include <QLocale>
#include <QTranslator>

#include "View_module/mainwindow.h"
using s21::MainWindow;

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);
  a.setApplicationName("3D viewer v2.1");

  QTranslator translator;
  const QStringList uiLanguages = QLocale::system().uiLanguages();
  for (const QString &locale : uiLanguages) {
    const QString baseName = "3DViewer_v2_1" + QLocale(locale).name();
    if (translator.load(":/i18n/" + baseName)) {
      a.installTranslator(&translator);
      break;
    }
  }

  OBJModel model;
  ViewerController controller(&model);
  MainWindow view(&controller);
  view.setWindowTitle("3D viewer v2.1");
  view.show();

  return a.exec();
}
