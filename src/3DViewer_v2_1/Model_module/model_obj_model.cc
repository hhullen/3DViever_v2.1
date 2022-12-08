#include "model_obj_model.h"

namespace s21 {

OBJModel::OBJModel() {
  SetDefaultValues();
  state_ = ModelState::Empty;
}

OBJModel::~OBJModel() { RemoveModel(); }

void OBJModel::RemoveModel() { SetDefaultValues(); }

unsigned int OBJModel::get_vertexes_amount() { return v_.size() / 3; }

unsigned int OBJModel::get_facets_amount() { return facets_.f_amount; }

unsigned int OBJModel::get_indices_amount() { return facets_.v_indices.size(); }

const vector<float> *OBJModel::get_vertexes_vector() { return &v_; }

const vector<unsigned int> *OBJModel::get_indices_vector() {
  return &facets_.v_indices;
}

bool OBJModel::UploadModel(const string &file_path) {
  thread *v_thread = nullptr, *vt_thread = nullptr, *vn_thread = nullptr,
         *f_thread = nullptr;
  bool returnable = false;

  RemoveModel();
  file_path_ = file_path;
  f_thread = new thread(&OBJModel::UploadFacets, this);
  v_thread =
      new thread(&OBJModel::UploadCoords, this, ref(v_), "v %f %f %f", 3);
  vt_thread =
      new thread(&OBJModel::UploadCoords, this, ref(vt_), "vt %f %f", 2);
  vn_thread =
      new thread(&OBJModel::UploadCoords, this, ref(vn_), "vn %f %f %f", 3);
  CatchThreads(v_thread, vt_thread, vn_thread, f_thread);
  MakeIndicesSubsequences();

  if (IsCorrectModel()) {
    facets_.facet_indices.shrink_to_fit();
    facets_.edge_indices.shrink_to_fit();
    facets_.vt_indices.shrink_to_fit();
    facets_.vn_indices.shrink_to_fit();
    facets_.v_indices.shrink_to_fit();
    vn_.shrink_to_fit();
    vt_.shrink_to_fit();
    v_.shrink_to_fit();
    returnable = true;
  } else {
    RemoveModel();
  }

  return returnable;
}

void OBJModel::CatchThreads(thread *v, thread *vt, thread *vn, thread *f) {
  v->join();
  vt->join();
  vn->join();
  f->join();
  delete v;
  delete vt;
  delete vn;
  delete f;
}

void OBJModel::UploadCoords(vector<float> &data, const char *format,
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

void OBJModel::UploadFacets() {
  ifstream file;
  string line;

  file.open(file_path_);
  if (file.is_open()) {
    while (!getline(file, line, '\n').eof()) {
      if (line.size() > 2 && line[0] == 'f' && line[1] == ' ') {
        CheckState(line);
        ReadFacet(line);
        ++facets_.f_amount;
      }
    }
    file.close();
  }
}

void OBJModel::CheckState(string &line) {
  if (state_ == ModelState::Empty) {
    int values[3] = {0};
    sscanf(line.data(), "f %d ", &values[0]);
    sscanf(line.data(), "f %d/%d ", &values[0], &values[1]);
    sscanf(line.data(), "f %d//%d ", &values[0], &values[2]);
    sscanf(line.data(), "f %d/%d/%d ", &values[0], &values[1], &values[2]);
    if (values[0] > 0 && values[1] == 0 && values[2] == 0) {
      state_ = ModelState::Vert;
      facets_.format = "%d";
    } else if (values[0] > 0 && values[1] > 0 && values[2] == 0) {
      state_ = ModelState::VertTex;
      facets_.format = "%d/%d";
    } else if (values[0] > 0 && values[1] == 0 && values[2] > 0) {
      state_ = ModelState::VertNorm;
      facets_.format = "%d//%d";
    } else if (values[0] > 0 && values[1] > 0 && values[2] > 0) {
      state_ = ModelState::VertTexNorm;
      facets_.format = "%d/%d/%d";
    }
  }
}

void OBJModel::ReadFacet(string &line) {
  size_t line_zise = line.size();
  int indexes[3] = {0};

  for (size_t i = 2; i < line_zise; ++i) {
    if (line[i - 1] == ' ' && IsAsciiDigit(line[i])) {
      sscanf(&line.data()[i], facets_.format, &indexes[0], &indexes[1],
             &indexes[2]);
      PushIndexes(indexes);
    }
  }
}

void OBJModel::PushIndexes(int *indexes) {
  if (state_ == ModelState::Vert) {
    facets_.v_indices.push_back(indexes[0] - 1);
  } else if (state_ == ModelState::VertTex) {
    facets_.v_indices.push_back(indexes[0] - 1);
    facets_.vt_indices.push_back(indexes[1] - 1);
  } else if (state_ == ModelState::VertNorm) {
    facets_.v_indices.push_back(indexes[0] - 1);
    facets_.vn_indices.push_back(indexes[1] - 1);
  } else if (state_ == ModelState::VertTexNorm) {
    facets_.v_indices.push_back(indexes[0] - 1);
    facets_.vt_indices.push_back(indexes[1] - 1);
    facets_.vn_indices.push_back(indexes[2] - 1);
  }
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
  facets_.f_amount = 0;
  facets_.v_indices.clear();
  facets_.v_indices.shrink_to_fit();
  facets_.vt_indices.clear();
  facets_.vt_indices.shrink_to_fit();
  facets_.vn_indices.clear();
  facets_.vn_indices.shrink_to_fit();
  facets_.edge_indices.clear();
  facets_.edge_indices.shrink_to_fit();
  facets_.facet_indices.clear();
  facets_.facet_indices.shrink_to_fit();
}

bool OBJModel::IsCorrectModel() {
  return v_.size() / 3 == vt_.size() / 2 && v_.size() / 3 == vn_.size() / 3 &&
         facets_.f_amount > 1;
}

void OBJModel::MakeIndicesSubsequences() {}

}  // namespace s21
