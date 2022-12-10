#include "viewer_controller.h"

namespace s21 {

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

const vector<float> *ViewerController::get_vertexes_vector() {
  return model_->get_vertexes();
}

const vector<float> *ViewerController::get_ordered_data_vector() {
  return model_->get_ordered_data();
}

const vector<unsigned int> *ViewerController::get_indices_vector() {
  return model_->get_indices();
}

}  // namespace s21