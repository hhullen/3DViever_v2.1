#ifndef LIGHTING_H
#define LIGHTING_H

#include <QWidget>

namespace Ui {
class Lighting;
}

class Lighting : public QWidget
{
    Q_OBJECT

public:
    explicit Lighting(QWidget *parent = nullptr);
    ~Lighting();

private:
    Ui::Lighting *ui;
};

#endif // LIGHTING_H
