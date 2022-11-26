#include "viewer_controller.h"

namespace S21 {

ViewerController::ViewerController(ModelFrame *model) : model_(model) {}

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

double ViewerController::get_max_value(Axis axis) {
  return model_->get_max_value(axis);
}

double ViewerController::get_min_value(Axis axis) {
  return get_min_value(axis);
}

const vector<double> *ViewerController::get_vertexes_vector() {
  return model_->get_vertexes_vector();
}

const vector<unsigned int> *ViewerController::get_indices_vector() {
  return model_->get_indices_vector();
}

}  // namespace S21