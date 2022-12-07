#ifndef SRC_3DVIEVER_V2_MODEL_MODULE_MODEL_FRAME_LOADER_H
#define SRC_3DVIEVER_V2_MODEL_MODULE_MODEL_FRAME_LOADER_H

#include <cmath>
#include <fstream>
#include <string>
#include <thread>
#include <vector>

using std::getline;
using std::ifstream;
using std::sscanf;
using std::stod;
using std::string;
using std::thread;
using std::vector;

namespace S21 {

const unsigned int kMaxDimension = 3;
enum Axis { X, Y, Z };
enum ModelState { Vert, VertTex, VertNorm, VertTexNorm };

class OBJModel {
 public:
  OBJModel();
  ~OBJModel();
  bool UploadModel(const string &file_path);
  void RemoveModel();

  unsigned int get_vertexes_amount();
  unsigned int get_facets_amount();
  unsigned int get_indices_amount();
  const vector<float> *get_vertexes_vector();
  const vector<unsigned int> *get_indices_vector();

  static void Move(OBJModel &model, Axis axis, float offset) {
    unsigned int vertexes_values = model.get_vertexes_amount() * 3;

    for (unsigned int i = axis; i < vertexes_values; i += 3) {
      model.v.at(i) += offset;
    }
  }

  static void Rotate(OBJModel &model, Axis axis, float angle) {
    unsigned int changeable_1 = 0, changeable_2 = 0;
    unsigned int vertexes_values = model.get_vertexes_amount() * 3;

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
      float temp_1 = model.v.at(i + changeable_1);
      float temp_2 = model.v.at(i + changeable_2);

      model.v.at(i + changeable_1) = temp_1 * cos(angle) + temp_2 * sin(angle);
      model.v.at(i + changeable_2) = temp_1 * -sin(angle) + temp_2 * cos(angle);
    }
  }

  static void Scale(OBJModel &model, float coeff) {
    unsigned int vertexes_values = model.get_vertexes_amount() * 3;

    for (unsigned int i = 0; i < vertexes_values; i += 1) {
      model.v.at(i) *= coeff;
    }
  }

 private:
  vector<float> v_;
  vector<float> vt_;
  vector<float> vn_;

  struct Facets {
    unsigned int f_amount;
    vector<unsigned int> v_indices;
    vector<unsigned int> vt_indices;
    vector<unsigned int> vn_indices;
    vector<unsigned int> edge_indices;
  } facets_;

  string file_path_;

  void UploadCoords(vector<float> &data, char *format, unsigned int dimension);
  void CatchThreads(thread *v, thread *vt, thread *vn, thread *f);
  void UploadFacets(Facets &data);
  void ReadFacet(Facets &data, string &line);
  void MakeEdgeIndices(Facets &data, string &line);
  bool IsAsciiDigit(const char &sym);
  void SetDefaultValues();
  bool IsCorrectModel();
};

}  // namespace S21

#endif  // SRC_3DVIEVER_V2_MODEL_MODULE_MODEL_FRAME_LOADER_H
