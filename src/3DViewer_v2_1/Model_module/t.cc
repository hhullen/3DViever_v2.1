#include <iostream>

#include "model_obj_model.h"

using s21::OBJModel;
using std::cout;

int main() {
  OBJModel m;

  // cout << m.UploadModel(
  //     "/opt/goinfre/hhullen/models/portal-cube/source/model.obj");
  // cout << m.UploadModel("/opt/goinfre/hhullen/models/cube.obj");
  cout << m.UploadModel("/opt/goinfre/hhullen/models/cube_no_vtvn.obj");
  // m.UploadModel("/opt/goinfre/hhullen/models/Kalashnikov.obj");
}