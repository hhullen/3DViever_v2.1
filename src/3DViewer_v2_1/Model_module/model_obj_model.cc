#include "model_obj_model.h"

namespace S21 {

OBJModel::OBJModel() { SetDefaultValues(); }

OBJModel::~OBJModel() { RemoveModel(); }

void OBJModel::RemoveModel() { SetDefaultValues(); }

unsigned int OBJModel::get_vertexes_amount() { return v_.size() / 3; }

unsigned int OBJModel::get_facets_amount() { return f_amount_; }

unsigned int OBJModel::get_indices_amount() { return indices_.size(); }

const vector<float> *OBJModel::get_vertexes_vector() { return &v_; }

const vector<unsigned int> *OBJModel::get_indices_vector() { return &indices_; }

bool OBJModel::UploadModel(const string &file_path) {
  bool returnable = false;
  thread *v_thread = nullptr;
  thread *vt_thread = nullptr;
  thread *vn_thread = nullptr;
  thread *f_thread = nullptr;

  RemoveModel();
  file_path_ = file_path;
  f_thread = new thread(&OBJModel::UploadFacets, this, facets_);
  v_thread = new thread(&OBJModel::UploadCoords, this, v_, "v %f %f %f", 3);
  vt_thread = new thread(&OBJModel::UploadCoords, this, vt_, "vt %f %f", 2);
  vn_thread = new thread(&OBJModel::UploadCoords, this, vn_, "vn %f %f %f", 3);
  v_thread->join();
  vt_thread->join();
  vn_thread->join();
  f_thread->join();
  delete v_thread;
  delete vt_thread;
  delete vn_thread;
  delete f_thread;

  if (IsCorrectModel()) {
    returnable = true;
    v_.shrink_to_fit();
    vt_.shrink_to_fit();
    vn_.shrink_to_fit();
    indices_.shrink_to_fit();
  } else {
    RemoveModel();
  }

  return returnable;
}

void OBJModel::UploadCoords(vector<float> &data, char *format,
                            unsigned int dimension) {
  float coord[kMaxDimension];
  ifstream file;
  string line;

  file.open(file_path_);
  if (file.is_open() && format) {
    while (!getline(file, line, '\n').eof()) {
      if (line.size() > 2 && line[0] == format[0] && line[1] == format[1]) {
        sscanf(line.data(), format, &coord[0], &coord[1], &coord[2]);
        for (unsigned int i = 0; i < dimension; ++i) {
          data.push_back(coord[i]);
        }
      }
    }
    file.close();
  }
}

void OBJModel::UploadFacets(Facets &data) {
  unsigned int first_index = 0;
  ifstream file;
  string line;

  file.open(file_path_);
  if (file.is_open()) {
    while (!getline(file, line, '\n').eof()) {
      if (line.size() > 2 && line[0] == 'f' && line[1] == ' ') {
        ReadFacet(data, line);
        ++data.f_amount;
      }
    }
    file.close();
  }
}

void OBJModel::ReadFacet(vector<unsigned int> &data, string &line) {
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
  v_.clear();
  v_.shrink_to_fit();
  vt_.clear();
  vt_.shrink_to_fit();
  vn_.clear();
  vn_.shrink_to_fit();
  f_amount_ = 0;
  indices_.clear();
  indices_.shrink_to_fit();
}

bool OBJModel::IsCorrectModel() {
  return v.size() > 2 && facets_.f_n > 0 && indices_.size() > 1 &&
         v.size() % 3 == 0;
}

}  // namespace S21
