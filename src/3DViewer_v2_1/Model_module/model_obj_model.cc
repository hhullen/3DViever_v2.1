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

unsigned int OBJModel::get_ordered_indices_amount() {
  return facets_.vt_indices.size();
}

const vector<float> *OBJModel::get_vertexes() { return &v_; }

const vector<float> *OBJModel::get_ordered_data() { return &subsequence_; }

const vector<unsigned int> *OBJModel::get_indices() {
  return &facets_.v_indices;
}

const vector<unsigned int> *OBJModel::get_ordered_indices() {
  return &facets_.vt_indices;
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
  MakeDataSubsequences();

  if (IsCorrectModel()) {
    FreeUnnecessary();
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
  int indexes[3] = {0}, counter = 0;

  for (size_t i = 2; i < line_zise; ++i) {
    if (line[i - 1] == ' ' && IsAsciiDigit(line[i])) {
      sscanf(&line.data()[i], facets_.format, &indexes[0], &indexes[1],
             &indexes[2]);
      ++counter;
      if (counter <= 3) {
        PushIndexes(indexes, FacetType::Triangle);
      } else {
        PushIndexes(indexes, FacetType::Polygon);
      }
      if (max_v_per_f_ < counter) {
        max_v_per_f_ = counter;
      }
    }
  }
}

void OBJModel::PushIndexes(int *indexes, FacetType type) {
  if (state_ == ModelState::Vert) {
    if (type == FacetType::Polygon) {
      PushPrevious(facets_.v_indices);
    }
    facets_.v_indices.push_back(indexes[0] - 1);
  } else if (state_ == ModelState::VertTex) {
    if (type == FacetType::Polygon) {
      PushPrevious(facets_.v_indices);
      PushPrevious(facets_.vt_indices);
    }
    facets_.v_indices.push_back(indexes[0] - 1);
    facets_.vt_indices.push_back(indexes[1] - 1);
  } else if (state_ == ModelState::VertNorm) {
    if (type == FacetType::Polygon) {
      PushPrevious(facets_.v_indices);
      PushPrevious(facets_.vn_indices);
    }
    facets_.v_indices.push_back(indexes[0] - 1);
    facets_.vn_indices.push_back(indexes[1] - 1);
  } else if (state_ == ModelState::VertTexNorm) {
    if (type == FacetType::Polygon) {
      PushPrevious(facets_.v_indices);
      PushPrevious(facets_.vt_indices);
      PushPrevious(facets_.vn_indices);
    }
    facets_.v_indices.push_back(indexes[0] - 1);
    facets_.vt_indices.push_back(indexes[1] - 1);
    facets_.vn_indices.push_back(indexes[2] - 1);
  }
}

void OBJModel::PushPrevious(vector<unsigned int> &indices) {
  unsigned int indx_1 = indices[indices.size() - 1];
  unsigned int indx_2 = indices[indices.size() - 3];
  indices.push_back(indx_2);
  indices.push_back(indx_1);
}

bool OBJModel::IsAsciiDigit(const char &sym) {
  return sym >= '0' && sym <= '9';
}

void OBJModel::SetDefaultValues() {
  CleanContainer(facets_.vn_indices);
  CleanContainer(facets_.vt_indices);
  CleanContainer(facets_.v_indices);
  facets_.f_amount = 0;
  CleanContainer(vn_);
  CleanContainer(vt_);
  CleanContainer(v_);
  state_ = ModelState::Empty;
  max_v_per_f_ = 0;
}

void OBJModel::CleanContainer(vector<float> &container) {
  container.clear();
  container.shrink_to_fit();
}

void OBJModel::CleanContainer(vector<unsigned int> &container) {
  container.clear();
  container.shrink_to_fit();
}

void OBJModel::MakeDataSubsequences() {
  vector<unsigned int>::iterator v_iter = facets_.v_indices.begin();
  vector<unsigned int>::iterator vt_iter = facets_.vt_indices.begin();
  vector<unsigned int>::iterator vn_iter = facets_.vn_indices.begin();

  subsequence_.clear();
  while (v_iter != facets_.v_indices.end()) {
    PushAttribute(v_, *v_iter, 3);
    ++v_iter;

    if (state_ == ModelState::VertTex || state_ == ModelState::VertTexNorm) {
      PushAttribute(vt_, *vt_iter, 2);
      ++vt_iter;
    } else {
      subsequence_.push_back(0);
      subsequence_.push_back(0);
    }

    if (state_ == ModelState::VertNorm || state_ == ModelState::VertTexNorm) {
      PushAttribute(vn_, *vn_iter, 3);
      ++vn_iter;
    } else {
      subsequence_.push_back(0);
      subsequence_.push_back(0);
      subsequence_.push_back(0);
    }
  }
  MakeArrayIndices();
  // for (auto f : v_) {
  //   std::cout << f << "\n";
  // }
}

void OBJModel::PushAttribute(vector<float> &data, unsigned int iter,
                             unsigned int amount) {
  iter *= amount;
  for (unsigned int i = 0; i < amount; ++i) {
    subsequence_.push_back(data.at(iter + i));
  }
}

bool OBJModel::IsCorrectModel() {
  return v_.size() > 3 && facets_.f_amount > 1;
}

void OBJModel::FreeUnnecessary() {
  CleanContainer(vt_);
  CleanContainer(vn_);
  CleanContainer(facets_.vn_indices);
}

void OBJModel::MakeArrayIndices() {
  vector<unsigned int> temp;
  temp.clear();

  std::cout << max_v_per_f_ << " max per F\n";

  if (max_v_per_f_ > 2) {
    for (size_t i = 0; i < facets_.v_indices.size() - 2; i += 3) {
      temp.push_back(facets_.v_indices[i]);
      temp.push_back(facets_.v_indices[i + 1]);
      temp.push_back(facets_.v_indices[i + 1]);
      temp.push_back(facets_.v_indices[i + 2]);
      temp.push_back(facets_.v_indices[i + 2]);
      temp.push_back(facets_.v_indices[i]);
    }
    facets_.v_indices.clear();
    facets_.v_indices = temp;
  }
  // for (auto f : facets_.v_indices) {
  //   std::cout << f << "\n";
  // }

  size_t size = facets_.v_indices.size();
  facets_.vt_indices.clear();
  for (size_t i = 0; i < size; i++) {
    facets_.vt_indices.push_back(i);
  }
}

}  // namespace s21
