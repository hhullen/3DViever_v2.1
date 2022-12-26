#ifndef TEXTURING_H
#define TEXTURING_H

#include <QWidget>

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

private:
    Ui::Texturing *ui_;
};

}

#endif // TEXTURING_H
