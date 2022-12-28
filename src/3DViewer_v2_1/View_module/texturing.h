#ifndef TEXTURING_H
#define TEXTURING_H

#include <QFileDialog>
#include <QLabel>
#include <QRegularExpression>
#include <QColorDialog>
#include <QRegularExpressionMatch>
#include <QDesktopServices>
#include <QWidget>
#include <QDateTime>
#include <vector>
#include "panelfuncs.h"
#include <QVBoxLayout>

#include <QGraphicsScene>
#include <QGraphicsView>

using s21::PanelFuncs;
using std::vector;

namespace Ui {
class Texturing;
}

namespace s21 {

class Texturing : public QWidget
{
    Q_OBJECT

public:
    explicit Texturing(QFont &font, QWidget *parent = nullptr);
    ~Texturing();

    QImage get_texture();
    void set_untextured();

signals:
    void DataUpdatedSignal(bool textured);
    void SentMessageSignal(QString message);
    const vector<float> *RequireUVMapData();

private:
    Ui::Texturing *ui_;
    QString texture_path_;
    QString map_path_;
    QImage texture_;
    QRegularExpression name_pattern_;
    QColor color_;
    const vector<float> *data_;

    void AddRemove();
    void ChooseDirectory();
    void SetPathToButtonName();
    void GetNewColor();
    void SaveUVMap();
    QLineF MakeLine(size_t i, size_t shif1, size_t shift2);
    void OpenMediaFolder();
};

}

#endif // TEXTURING_H
