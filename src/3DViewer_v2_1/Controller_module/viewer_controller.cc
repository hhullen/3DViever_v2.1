#include "viewer_controller.h"

namespace S21 {

ViewerController::ViewerController(OBJModel *model) : model_(model) {}

bool ViewerController::UploadNewModel(string file_path) {
  return model_->UploadModel(file_path);
}

unsigned int ViewerController::get_vertexes_amount() {
  return model_->get_vertexes_amount();
}

unsigned int ViewerController::get_facets_amount() {
  return model_->get_facets_amount();
}

unsigned int ViewerController::get_indices_amount() {
  return model_->get_indices_amount();
}

float ViewerController::get_max_value(Axis axis) {
  return model_->get_max_value(axis);
}

float ViewerController::get_min_value(Axis axis) { return get_min_value(axis); }

const vector<float> *ViewerController::get_vertexes_vector() {
  return model_->get_vertexes_vector();
}

const vector<float> *ViewerController::get_ordered_data_vector() {
  return model_->get_ordered_data();
}

const vector<unsigned int> *ViewerController::get_indices_vector() {
  return model_->get_indices_vector();
}

}  // namespace S21