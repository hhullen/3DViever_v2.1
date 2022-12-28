#ifndef SRC_3DVIEVER_V2_1_MODEL_MODULE_MODEL_OBJ_MODEL_H
#define SRC_3DVIEVER_V2_1_MODEL_MODULE_MODEL_OBJ_MODEL_H

#include <cmath>
#include <fstream>
#include <string>
#include <thread>
#include <vector>

using std::getline;
using std::ifstream;
using std::ref;
using std::sscanf;
using std::stod;
using std::string;
using std::thread;
using std::vector;

namespace s21 {

const unsigned int kMaxDimension = 3;
enum Axis { X, Y, Z };
enum class ModelState { Vert, VertTex, VertNorm, VertTexNorm, Empty };
enum class FacetType { Triangle, Polygon, Line };

class OBJModel {
 public:
  OBJModel();
  ~OBJModel();
  bool UploadModel(const string &file_path);
  void RemoveModel();

  unsigned int get_vertexes_amount();
  unsigned int get_facets_amount();
  unsigned int get_indices_amount();
  unsigned int get_ordered_indices_amount();
  const vector<float> *get_vertexes();
  const vector<float> *get_ordered_data();
  const vector<unsigned int> *get_indices();
  const vector<unsigned int> *get_ordered_indices();
  ModelState get_model_state();

  static void Move(OBJModel &model, Axis axis, float offset) {
    unsigned int vertexes_values = model.v_.size();

    for (unsigned int i = axis; i < vertexes_values; i += 3) {
      model.v_.at(i) += offset;
    }
  }

  static void Rotate(OBJModel &model, Axis axis, float angle) {
    unsigned int changeable_1 = 0, changeable_2 = 0;
    unsigned int vertexes_values = model.v_.size();
    unsigned int normals_values = model.subsequence_.size() / 8 * 3;

    angle = angle / 180 * M_PI;
    if (axis == Axis::X) {
      changeable_1 = Axis::Y;
      changeable_2 = Axis::Z;
    } else if (axis == Axis::Y) {
      changeable_1 = Axis::X;
      changeable_2 = Axis::Z;
    } else if (axis == Axis::Z) {
      changeable_1 = Axis::X;
      changeable_2 = Axis::Y;
    }

    for (unsigned int i = 0; i < vertexes_values; i += 3) {
      Rotate_vector(model.v_, i, changeable_1, changeable_2, angle);
    }
    for (unsigned int i = 5; i < normals_values; i += 8) {
      Rotate_vector(model.subsequence_, i, changeable_1, changeable_2, angle);
    }
  }

  static void Scale(OBJModel &model, float coeff) {
    unsigned int vertexes_values = model.v_.size();

    for (unsigned int i = 0; i < vertexes_values; i += 1) {
      model.v_.at(i) *= coeff;
    }
  }

 private:
  vector<float> v_;
  vector<float> vt_;
  vector<float> vn_;
  vector<float> subsequence_;

  struct Facets {
    unsigned int f_amount;
    vector<unsigned int> v_indices;
    vector<unsigned int> vt_indices;
    vector<unsigned int> vn_indices;
    const char *format;
  } facets_;

  string file_path_;
  ModelState state_;
  int max_v_per_f_;

  void UploadCoords(vector<float> &data, const char *format,
                    unsigned int dimension);
  void CatchThreads(thread *v, thread *vt, thread *vn, thread *f);
  void UploadFacets();
  void CheckState(string &line);
  void ReadFacet(string &line);
  void PushIndexes(int *indexes, FacetType type);
  void PushPrevious(vector<unsigned int> &indices);
  bool IsAsciiDigit(const char &sym);
  void SetDefaultValues();
  void CleanContainer(vector<float> &container);
  void CleanContainer(vector<unsigned int> &container);
  void MakeDataSubsequences();
  void PushAttribute(vector<float> &data, unsigned int iter,
                     unsigned int amount);
  bool IsCorrectModel();
  void FreeUnnecessary();
  void MakeArrayIndices();

  static void Rotate_vector(vector<float> &vector, unsigned int i,
                            unsigned int changeable_1,
                            unsigned int changeable_2, unsigned int angle) {
    float tmp_1 = vector.at(i + changeable_1);
    float tmp_2 = vector.at(i + changeable_2);

    vector.at(i + changeable_1) = tmp_1 * cos(angle) + tmp_2 * sin(angle);
    vector.at(i + changeable_2) = tmp_1 * -sin(angle) + tmp_2 * cos(angle);
  }
};

}  // namespace s21

#endif  // SRC_3DVIEVER_V2_1_MODEL_MODULE_MODEL_OBJ_MODEL_H
