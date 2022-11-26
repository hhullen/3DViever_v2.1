#include "model_obj_model.h"

namespace S21 {

OBJModel::OBJModel() { SetDefaultValues(); }

OBJModel::~OBJModel() { RemoveModel(); }

void OBJModel::RemoveModel() { SetDefaultValues(); }

unsigned int OBJModel::get_vertexes_amount() { return v.size() / 3; }

unsigned int OBJModel::get_facets_amount() { return facets_.f_n; }

unsigned int OBJModel::get_indices_amount() { return facets_.indices.size(); }

const vector<double> *OBJModel::get_vertexes_vector() { return &v; }

const vector<unsigned int> *OBJModel::get_indices_vector() {
  return &facets_.indices;
}

bool OBJModel::UploadModel(const string &file_path) {
  bool returnable = false;
  thread *vertexes_thread;
  thread *facets_thread;

  RemoveModel();
  file_path_ = file_path;
  vertexes_thread = new thread(&OBJModel::UploadVertexes, this, &vertexes_);
  facets_thread = new thread(&OBJModel::UploadFacets, this, &facets_);
  vertexes_thread->join();
  facets_thread->join();
  delete vertexes_thread;
  delete facets_thread;

  if (IsCorrectModel()) {
    returnable = true;
    v.shrink_to_fit();
    facets_.indices.shrink_to_fit();
  } else {
    RemoveModel();
  }

  return returnable;
}

void OBJModel::UploadVertexes(vector<float> *data) {
  ifstream file;
  string line;

  file.open(file_path_);
  if (file.is_open()) {
    while (!getline(file, line, '\n').eof()) {
      ReadVertex(*data, line);
    }
    file.close();
  }
  if (data->v.size() > 0) {
    SearcMaxMin(*data);
  }
}

void OBJModel::ReadVertex(vector<float> &data, string &line) {
  double x, y, z;

  if (line[0] == 'v' && line[1] == ' ') {
    sscanf(line.data(), "v %lf %lf %lf", &x, &y, &z);
    data.v.push_back(x);
    data.v.push_back(y);
    data.v.push_back(z);
  }
}

void OBJModel::UploadFacets(Facets *data) {
  ifstream file;
  string line;
  unsigned int first_index = 0;

  file.open(file_path_);
  if (file.is_open()) {
    while (!getline(file, line, '\n').eof()) {
      if (line[0] == 'f' && line[1] == ' ') {
        ReadFacet(*data, line);
        ++(*data).f_n;
      }
    }
    file.close();
  }
}

void OBJModel::ReadFacet(Facets &data, string &line) {
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

bool OBJModel::IsAsciiDigit(const char &sym) {
  return sym >= '0' && sym <= '9';
}

void OBJModel::SetDefaultValues() {
  v.clear();
  v.shrink_to_fit();
  facets_.f_n = 0;
  facets_.indices.clear();
  facets_.indices.shrink_to_fit();
}

bool OBJModel::IsCorrectModel() {
  return v.size() > 2 && facets_.f_n > 0 && facets_.indices.size() > 1 &&
         v.size() % 3 == 0;
}

}  // namespace S21
