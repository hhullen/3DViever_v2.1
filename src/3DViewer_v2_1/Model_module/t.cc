#include <iostream>

#include "model_obj_model.h"

using s21::OBJModel;

int main() {
  OBJModel m;

  m.UploadModel("/opt/goinfre/hhullen/models/portal-cube/source/model.obj");
}