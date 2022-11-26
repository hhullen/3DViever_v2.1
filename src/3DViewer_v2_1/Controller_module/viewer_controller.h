#ifndef SRC_3DVIEVER_V2_CONTROLLER_MODULE_VIEWER_CONTROLLER_H
#define SRC_3DVIEVER_V2_CONTROLLER_MODULE_VIEWER_CONTROLLER_H

#include "../Model_module/model_frame.h"

namespace S21 {

class ViewerController {
 public:
  ViewerController(ModelFrame *model);
  bool UploadNewModel(string file_path);

  unsigned int get_vertexes_amount();
  unsigned int get_facets_amount();
  unsigned int get_indices_amount();
  double get_max_value(Axis axis);
  double get_min_value(Axis axis);
  const vector<double> *get_vertexes_vector();
  const vector<unsigned int> *get_indices_vector();

 private:
  ModelFrame *model_;
};

}  // namespace S21

#endif  // SRC_3DVIEVER_V2_CONTROLLER_MODULE_VIEWER_CONTROLLER_H
