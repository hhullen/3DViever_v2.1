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

const int kAXIS_AMOUNT = 3;

enum Axis { X, Y, Z };

class ModelFrame {
 public:
  ModelFrame();
  ~ModelFrame();
  bool UploadModel(const string &file_path);
  void RemoveModel();

  unsigned int get_vertexes_amount();
  unsigned int get_facets_amount();
  unsigned int get_indices_amount();
  double get_max_value(Axis axis);
  double get_min_value(Axis axis);
  const vector<double> *get_vertexes_vector();
  const vector<unsigned int> *get_indices_vector();

  static void Move(ModelFrame &model, Axis axis, double offset) {
    unsigned int vertexes_values = model.get_vertexes_amount() * 3;

    for (unsigned int i = axis; i < vertexes_values; i += 3) {
      model.vertexes_.vertexes[i] += offset;
    }
  }

  static void Rotate(ModelFrame &model, Axis axis, double angle) {
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
      double temp_1 = model.vertexes_.vertexes[i + changeable_1];
      double temp_2 = model.vertexes_.vertexes[i + changeable_2];

      model.vertexes_.vertexes[i + changeable_1] =
          temp_1 * cos(angle) + temp_2 * sin(angle);
      model.vertexes_.vertexes[i + changeable_2] =
          temp_1 * -sin(angle) + temp_2 * cos(angle);
    }
  }

  static void Scale(ModelFrame &model, double coeff) {
    unsigned int vertexes_values = model.get_vertexes_amount() * 3;

    for (unsigned int i = 0; i < vertexes_values; i += 1) {
      model.vertexes_.vertexes[i] *= coeff;
    }
  }

 private:
  struct Vertexes {
    double *max_values;
    double *min_values;
    vector<double> vertexes;
  } vertexes_;

  struct Facets {
    unsigned int facets_n;
    vector<unsigned int> indices;
  } facets_;

  string file_path_;

  void UploadVertexes(Vertexes *data);
  void ReadVertex(Vertexes &data, string &line);
  void SearcMaxMin(Vertexes &data);
  void UploadFacets(Facets *data);
  void ReadFacet(Facets &data, string &line);
  bool IsAsciiDigit(const char &sym);
  void SetDefaultValues();
  bool IsCorrectModel();
  void IsMax(const double &value, double &max_value);
  void IsMin(const double &value, double &min_value);
};

}  // namespace S21

#endif  // SRC_3DVIEVER_V2_MODEL_MODULE_MODEL_FRAME_LOADER_H
