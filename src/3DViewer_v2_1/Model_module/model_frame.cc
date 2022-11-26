#include "model_frame.h"

namespace S21 {

ModelFrame::ModelFrame() {
  vertexes_.max_values = new double[kAXIS_AMOUNT];
  vertexes_.min_values = new double[kAXIS_AMOUNT];
  SetDefaultValues();
}

ModelFrame::~ModelFrame() {
  RemoveModel();
  delete vertexes_.max_values;
  delete vertexes_.min_values;
}

void ModelFrame::RemoveModel() { SetDefaultValues(); }

unsigned int ModelFrame::get_vertexes_amount() {
  return vertexes_.vertexes.size() / 3;
}

unsigned int ModelFrame::get_facets_amount() { return facets_.facets_n; }

unsigned int ModelFrame::get_indices_amount() { return facets_.indices.size(); }

double ModelFrame::get_max_value(Axis axis) {
  return vertexes_.max_values[axis];
}

double ModelFrame::get_min_value(Axis axis) {
  return vertexes_.min_values[axis];
}

const vector<double> *ModelFrame::get_vertexes_vector() {
  return &vertexes_.vertexes;
}

const vector<unsigned int> *ModelFrame::get_indices_vector() {
  return &facets_.indices;
}

bool ModelFrame::UploadModel(const string &file_path) {
  bool returnable = false;
  thread *vertexes_thread;
  thread *facets_thread;

  RemoveModel();
  file_path_ = file_path;
  vertexes_thread = new thread(&ModelFrame::UploadVertexes, this, &vertexes_);
  facets_thread = new thread(&ModelFrame::UploadFacets, this, &facets_);
  vertexes_thread->join();
  facets_thread->join();
  delete vertexes_thread;
  delete facets_thread;

  if (IsCorrectModel()) {
    returnable = true;
    vertexes_.vertexes.shrink_to_fit();
    facets_.indices.shrink_to_fit();
  } else {
    RemoveModel();
  }

  return returnable;
}

void ModelFrame::UploadVertexes(Vertexes *data) {
  ifstream file;
  string line;

  file.open(file_path_);
  if (file.is_open()) {
    while (!getline(file, line, '\n').eof()) {
      ReadVertex(*data, line);
    }
    file.close();
  }
  if (data->vertexes.size() > 0) {
    SearcMaxMin(*data);
  }
}

void ModelFrame::ReadVertex(Vertexes &data, string &line) {
  double x, y, z;

  if (line[0] == 'v' && line[1] == ' ') {
    sscanf(line.data(), "v %lf %lf %lf", &x, &y, &z);
    data.vertexes.push_back(x);
    data.vertexes.push_back(y);
    data.vertexes.push_back(z);
  }
}

void ModelFrame::SearcMaxMin(Vertexes &data) {
  size_t vector_size = data.vertexes.size();

  for (int i = 0; i < kAXIS_AMOUNT; ++i) {
    data.max_values[i] = data.vertexes[i];
    data.min_values[i] = data.vertexes[i];
  }

  for (size_t i = 3; i < vector_size; i += 3) {
    IsMax(data.vertexes[i + Axis::X], data.max_values[Axis::X]);
    IsMin(data.vertexes[i + Axis::X], data.min_values[Axis::X]);
    IsMax(data.vertexes[i + Axis::Y], data.max_values[Axis::Y]);
    IsMin(data.vertexes[i + Axis::Y], data.min_values[Axis::Y]);
    IsMax(data.vertexes[i + Axis::Z], data.max_values[Axis::Z]);
    IsMin(data.vertexes[i + Axis::Z], data.min_values[Axis::Z]);
  }
}

void ModelFrame::UploadFacets(Facets *data) {
  ifstream file;
  string line;
  unsigned int first_index = 0;

  file.open(file_path_);
  if (file.is_open()) {
    while (!getline(file, line, '\n').eof()) {
      if (line[0] == 'f' && line[1] == ' ') {
        ReadFacet(*data, line);
        ++(*data).facets_n;
      }
    }
    file.close();
  }
}

void ModelFrame::ReadFacet(Facets &data, string &line) {
  size_t line_zise = line.size();
  unsigned int first_index = 0;
  bool is_first_index = true;

  for (size_t i = 2; i < line_zise; ++i) {
    if (!is_first_index && line[i - 1] == ' ' && IsAsciiDigit(line[i])) {
      data.indices.push_back(stod(&line.data()[i]) - 1);
      data.indices.push_back(data.indices.back());
    } else if (is_first_index && line[i - 1] == ' ' && IsAsciiDigit(line[i])) {
      first_index = stod(&line.data()[i]) - 1;
      data.indices.push_back(first_index);
      is_first_index = false;
    }
  }
  data.indices.push_back(first_index);
}

bool ModelFrame::IsAsciiDigit(const char &sym) {
  return sym >= '0' && sym <= '9';
}

void ModelFrame::SetDefaultValues() {
  for (int i = 0; i < kAXIS_AMOUNT; ++i) {
    vertexes_.max_values[i] = 0;
    vertexes_.min_values[i] = 0;
  }
  vertexes_.vertexes.clear();
  vertexes_.vertexes.shrink_to_fit();
  facets_.facets_n = 0;
  facets_.indices.clear();
  facets_.indices.shrink_to_fit();
}

bool ModelFrame::IsCorrectModel() {
  return vertexes_.vertexes.size() > 2 && facets_.facets_n > 0 &&
         facets_.indices.size() > 1 && vertexes_.vertexes.size() % 3 == 0;
}

void ModelFrame::IsMax(const double &value, double &max_value) {
  if (value > max_value) {
    max_value = value;
  }
}

void ModelFrame::IsMin(const double &value, double &min_value) {
  if (value < min_value) {
    min_value = value;
  }
}

}  // namespace S21
