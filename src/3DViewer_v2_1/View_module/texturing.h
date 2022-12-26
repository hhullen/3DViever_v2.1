#ifndef TEXTURING_H
#define TEXTURING_H

#include <QFileDialog>
#include <QLabel>
#include <QRegularExpression>
#include <QColorDialog>
#include <QRegularExpressionMatch>
#include <QWidget>
#include <vector>

using std::vector;

namespace Ui {
class Texturing;
}

namespace s21 {

class Texturing : public QWidget
{
    Q_OBJECT

public:
    explicit Texturing(QWidget *parent = nullptr);
    ~Texturing();

    QImage get_texture();
    void set_untextured();

signals:
    void DataUpdatedSignal(bool textured);
    void SentMessageSignal(QString message);
    vector<float> *RequireUVMapData();

private:
    Ui::Texturing *ui_;
    QString texture_path_;
    QString map_path_;
    QImage texture_;
    QRegularExpression name_pattern_;
    QColor color_;
    vector<float> *data_;

    void AddRemove();
    void ChooseDirectory();
    void SetPathToButtonName();
    void GetNewColor();
    void SetColor(QColor color, QColor *var, QPushButton *btn);
    void ShowChosenColorInfo(QLabel *txt, QColor col);
    void SaveUVMap();
};

}

#endif // TEXTURING_H
