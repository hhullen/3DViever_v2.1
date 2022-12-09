#ifndef SRC_3DVIEVER_V2_CONTROLLER_MODULE_VIEWER_CONTROLLER_H
#define SRC_3DVIEVER_V2_CONTROLLER_MODULE_VIEWER_CONTROLLER_H

#include "../Model_module/model_obj_model.h"
using s21::OBJModel;

namespace S21 {

class ViewerController {
 public:
  ViewerController(OBJModel *model);
  bool UploadNewModel(string file_path);

  unsigned int get_vertexes_amount();
  unsigned int get_facets_amount();
  unsigned int get_indices_amount();
  float get_max_value(Axis axis);
  float get_min_value(Axis axis);
  const vector<float> *get_vertexes_vector();
  const vector<float> *get_ordered_data_vector();
  const vector<unsigned int> *get_indices_vector();

 private:
  OBJModel *model_;
};

}  // namespace S21

#endif  // SRC_3DVIEVER_V2_CONTROLLER_MODULE_VIEWER_CONTROLLER_H
