#include "texturing.h"
#include "ui_texturing.h"

namespace s21 {

Texturing::Texturing(QWidget *parent) :
    QWidget(parent),
    ui_(new Ui::Texturing) {
    ui_->setupUi(this);
}

Texturing::~Texturing() {
    delete ui_;
}

}
