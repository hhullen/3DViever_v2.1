#include <gtest/gtest.h>

#include <iostream>
#include <string>

#include "../Model_module/model_frame.h"
using std::cout;
using std::string;

const double kACCURACUY = 0.000001;

const string kCUBE_PATH = "models/cube.obj";
const string kEMPTY_PATH = "models/Empty.obj";
const string kMILLION_PATH = "models/one_million.obj";

TEST(loader_test, uploading_cube) {
  S21::ModelFrame m;
  bool is_load = m.UploadModel(kCUBE_PATH);
  EXPECT_TRUE(is_load);
  EXPECT_EQ(m.get_facets_amount(), 12);
  EXPECT_EQ(m.get_indices_amount(), 72);
  EXPECT_EQ(m.get_vertexes_amount(), 8);
  EXPECT_TRUE(abs(m.get_max_value(S21::Axis::X) - 1) <= kACCURACUY);
  EXPECT_TRUE(abs(m.get_max_value(S21::Axis::Y) - 1) <= kACCURACUY);
  EXPECT_TRUE(abs(m.get_max_value(S21::Axis::Z) - 1) <= kACCURACUY);
  EXPECT_TRUE(abs(m.get_min_value(S21::Axis::X) - (-1)) <= kACCURACUY);
  EXPECT_TRUE(abs(m.get_min_value(S21::Axis::Y) - (-1)) <= kACCURACUY);
  EXPECT_TRUE(abs(m.get_min_value(S21::Axis::Z) - (-1)) <= kACCURACUY);

  const vector<double> *vert = m.get_vertexes_vector();
  const vector<unsigned int> *indi = m.get_indices_vector();
  const double *data = vert->data();
}

TEST(loader_test, uploading_million) {
  S21::ModelFrame m;
  bool is_load = m.UploadModel(kMILLION_PATH);
  EXPECT_TRUE(is_load);
  EXPECT_EQ(m.get_facets_amount(), 1669317);
  EXPECT_EQ(m.get_indices_amount(), 6677268);
  EXPECT_EQ(m.get_vertexes_amount(), 1000000);
}

TEST(loader_test, uploading_empty) {
  S21::ModelFrame m;
  bool is_load = m.UploadModel(kEMPTY_PATH);
  EXPECT_FALSE(is_load);
  EXPECT_EQ(m.get_facets_amount(), 0);
  EXPECT_EQ(m.get_indices_amount(), 0);
  EXPECT_EQ(m.get_vertexes_amount(), 0);
  EXPECT_TRUE(m.get_max_value(S21::Axis::X) <= kACCURACUY);
  EXPECT_TRUE(m.get_max_value(S21::Axis::Y) <= kACCURACUY);
  EXPECT_TRUE(m.get_max_value(S21::Axis::Z) <= kACCURACUY);
  EXPECT_TRUE(m.get_min_value(S21::Axis::X) <= kACCURACUY);
  EXPECT_TRUE(m.get_min_value(S21::Axis::Y) <= kACCURACUY);
  EXPECT_TRUE(m.get_min_value(S21::Axis::Z) <= kACCURACUY);
}

TEST(loader_test, uploading_wrong_path) {
  S21::ModelFrame m;
  bool is_load = m.UploadModel("hello_world.kek");
  EXPECT_FALSE(is_load);
  EXPECT_EQ(m.get_facets_amount(), 0);
  EXPECT_EQ(m.get_indices_amount(), 0);
  EXPECT_EQ(m.get_vertexes_amount(), 0);
  EXPECT_TRUE(m.get_max_value(S21::Axis::X) <= kACCURACUY);
  EXPECT_TRUE(m.get_max_value(S21::Axis::Y) <= kACCURACUY);
  EXPECT_TRUE(m.get_max_value(S21::Axis::Z) <= kACCURACUY);
  EXPECT_TRUE(m.get_min_value(S21::Axis::X) <= kACCURACUY);
  EXPECT_TRUE(m.get_min_value(S21::Axis::Y) <= kACCURACUY);
  EXPECT_TRUE(m.get_min_value(S21::Axis::Z) <= kACCURACUY);
}

TEST(transfotmation_test, move_x) {
  S21::ModelFrame m;
  bool is_load = m.UploadModel(kCUBE_PATH);
  EXPECT_TRUE(is_load);
  EXPECT_EQ(m.get_facets_amount(), 12);
  EXPECT_EQ(m.get_indices_amount(), 72);
  EXPECT_EQ(m.get_vertexes_amount(), 8);
  EXPECT_TRUE(abs(m.get_max_value(S21::Axis::X) - 1) <= kACCURACUY);
  EXPECT_TRUE(abs(m.get_max_value(S21::Axis::Y) - 1) <= kACCURACUY);
  EXPECT_TRUE(abs(m.get_max_value(S21::Axis::Z) - 1) <= kACCURACUY);
  EXPECT_TRUE(abs(m.get_min_value(S21::Axis::X) - (-1)) <= kACCURACUY);
  EXPECT_TRUE(abs(m.get_min_value(S21::Axis::Y) - (-1)) <= kACCURACUY);
  EXPECT_TRUE(abs(m.get_min_value(S21::Axis::Z) - (-1)) <= kACCURACUY);

  S21::ModelFrame::Move(m, S21::Axis::X, 5.0);

  const vector<double> *vert = m.get_vertexes_vector();
  const vector<unsigned int> *indi = m.get_indices_vector();

  EXPECT_TRUE(abs((*vert)[0] - 6) <= kACCURACUY);
  EXPECT_TRUE(abs((*vert)[3] - 6) <= kACCURACUY);
  EXPECT_TRUE(abs((*vert)[12] - 6) <= kACCURACUY);
  EXPECT_TRUE(abs((*vert)[15] - 6) <= kACCURACUY);

  EXPECT_TRUE(abs((*vert)[6] - 4) <= kACCURACUY);
  EXPECT_TRUE(abs((*vert)[9] - 4) <= kACCURACUY);
  EXPECT_TRUE(abs((*vert)[18] - 4) <= kACCURACUY);
  EXPECT_TRUE(abs((*vert)[21] - 4) <= kACCURACUY);
}

TEST(transfotmation_test, move_y) {
  S21::ModelFrame m;
  bool is_load = m.UploadModel(kCUBE_PATH);
  EXPECT_TRUE(is_load);
  EXPECT_EQ(m.get_facets_amount(), 12);
  EXPECT_EQ(m.get_indices_amount(), 72);
  EXPECT_EQ(m.get_vertexes_amount(), 8);
  EXPECT_TRUE(abs(m.get_max_value(S21::Axis::X) - 1) <= kACCURACUY);
  EXPECT_TRUE(abs(m.get_max_value(S21::Axis::Y) - 1) <= kACCURACUY);
  EXPECT_TRUE(abs(m.get_max_value(S21::Axis::Z) - 1) <= kACCURACUY);
  EXPECT_TRUE(abs(m.get_min_value(S21::Axis::X) - (-1)) <= kACCURACUY);
  EXPECT_TRUE(abs(m.get_min_value(S21::Axis::Y) - (-1)) <= kACCURACUY);
  EXPECT_TRUE(abs(m.get_min_value(S21::Axis::Z) - (-1)) <= kACCURACUY);

  S21::ModelFrame::Move(m, S21::Axis::Y, 5.0);

  const vector<double> *vert = m.get_vertexes_vector();
  const vector<unsigned int> *indi = m.get_indices_vector();

  EXPECT_TRUE(abs((*vert)[1] - 4) <= kACCURACUY);
  EXPECT_TRUE(abs((*vert)[4] - 4) <= kACCURACUY);
  EXPECT_TRUE(abs((*vert)[13] - 6) <= kACCURACUY);
  EXPECT_TRUE(abs((*vert)[16] - 6) <= kACCURACUY);

  EXPECT_TRUE(abs((*vert)[7] - 4) <= kACCURACUY);
  EXPECT_TRUE(abs((*vert)[10] - 4) <= kACCURACUY);
  EXPECT_TRUE(abs((*vert)[19] - 6) <= kACCURACUY);
  EXPECT_TRUE(abs((*vert)[22] - 6) <= kACCURACUY);
}

TEST(transfotmation_test, move_z) {
  S21::ModelFrame m;
  bool is_load = m.UploadModel(kCUBE_PATH);
  EXPECT_TRUE(is_load);
  EXPECT_EQ(m.get_facets_amount(), 12);
  EXPECT_EQ(m.get_indices_amount(), 72);
  EXPECT_EQ(m.get_vertexes_amount(), 8);
  EXPECT_TRUE(abs(m.get_max_value(S21::Axis::X) - 1) <= kACCURACUY);
  EXPECT_TRUE(abs(m.get_max_value(S21::Axis::Y) - 1) <= kACCURACUY);
  EXPECT_TRUE(abs(m.get_max_value(S21::Axis::Z) - 1) <= kACCURACUY);
  EXPECT_TRUE(abs(m.get_min_value(S21::Axis::X) - (-1)) <= kACCURACUY);
  EXPECT_TRUE(abs(m.get_min_value(S21::Axis::Y) - (-1)) <= kACCURACUY);
  EXPECT_TRUE(abs(m.get_min_value(S21::Axis::Z) - (-1)) <= kACCURACUY);

  S21::ModelFrame::Move(m, S21::Axis::Z, 5.0);

  const vector<double> *vert = m.get_vertexes_vector();
  const vector<unsigned int> *indi = m.get_indices_vector();

  EXPECT_TRUE(abs((*vert)[2] - 4) <= kACCURACUY);
  EXPECT_TRUE(abs((*vert)[5] - 6) <= kACCURACUY);
  EXPECT_TRUE(abs((*vert)[14] - 4) <= kACCURACUY);
  EXPECT_TRUE(abs((*vert)[17] - 6) <= kACCURACUY);

  EXPECT_TRUE(abs((*vert)[8] - 6) <= kACCURACUY);
  EXPECT_TRUE(abs((*vert)[11] - 4) <= kACCURACUY);
  EXPECT_TRUE(abs((*vert)[20] - 6) <= kACCURACUY);
  EXPECT_TRUE(abs((*vert)[23] - 4) <= kACCURACUY);
}

TEST(transfotmation_test, rotate_x) {
  S21::ModelFrame m;
  bool is_load = m.UploadModel(kCUBE_PATH);
  EXPECT_TRUE(is_load);
  EXPECT_EQ(m.get_facets_amount(), 12);
  EXPECT_EQ(m.get_indices_amount(), 72);
  EXPECT_EQ(m.get_vertexes_amount(), 8);
  EXPECT_TRUE(abs(m.get_max_value(S21::Axis::X) - 1) <= kACCURACUY);
  EXPECT_TRUE(abs(m.get_max_value(S21::Axis::Y) - 1) <= kACCURACUY);
  EXPECT_TRUE(abs(m.get_max_value(S21::Axis::Z) - 1) <= kACCURACUY);
  EXPECT_TRUE(abs(m.get_min_value(S21::Axis::X) - (-1)) <= kACCURACUY);
  EXPECT_TRUE(abs(m.get_min_value(S21::Axis::Y) - (-1)) <= kACCURACUY);
  EXPECT_TRUE(abs(m.get_min_value(S21::Axis::Z) - (-1)) <= kACCURACUY);

  S21::ModelFrame::Rotate(m, S21::Axis::X, 180.0);

  const vector<double> *vert = m.get_vertexes_vector();
  const vector<unsigned int> *indi = m.get_indices_vector();

  S21::ModelFrame ref;
  ref.UploadModel(kCUBE_PATH);
  const vector<double> *vert_ref = ref.get_vertexes_vector();

  for (int i = 2; i < m.get_vertexes_amount() * 3; i += 3) {
    EXPECT_TRUE(abs((*vert)[i] * -1 - (*vert_ref)[i]) < kACCURACUY);  // Z axis
    EXPECT_TRUE(abs((*vert)[i - 1] * -1 - (*vert_ref)[i - 1]) <
                kACCURACUY);  // Y axis
    EXPECT_TRUE(abs((*vert)[i - 2] - (*vert_ref)[i - 2]) <
                kACCURACUY);  // X axis
  }
}

TEST(transfotmation_test, rotate_y) {
  S21::ModelFrame m;
  bool is_load = m.UploadModel(kCUBE_PATH);
  EXPECT_TRUE(is_load);
  EXPECT_EQ(m.get_facets_amount(), 12);
  EXPECT_EQ(m.get_indices_amount(), 72);
  EXPECT_EQ(m.get_vertexes_amount(), 8);
  EXPECT_TRUE(abs(m.get_max_value(S21::Axis::X) - 1) <= kACCURACUY);
  EXPECT_TRUE(abs(m.get_max_value(S21::Axis::Y) - 1) <= kACCURACUY);
  EXPECT_TRUE(abs(m.get_max_value(S21::Axis::Z) - 1) <= kACCURACUY);
  EXPECT_TRUE(abs(m.get_min_value(S21::Axis::X) - (-1)) <= kACCURACUY);
  EXPECT_TRUE(abs(m.get_min_value(S21::Axis::Y) - (-1)) <= kACCURACUY);
  EXPECT_TRUE(abs(m.get_min_value(S21::Axis::Z) - (-1)) <= kACCURACUY);

  S21::ModelFrame::Rotate(m, S21::Axis::Y, 180.0);

  const vector<double> *vert = m.get_vertexes_vector();
  const vector<unsigned int> *indi = m.get_indices_vector();

  S21::ModelFrame ref;
  ref.UploadModel(kCUBE_PATH);
  const vector<double> *vert_ref = ref.get_vertexes_vector();

  for (int i = 2; i < m.get_vertexes_amount() * 3; i += 3) {
    EXPECT_TRUE(abs((*vert)[i] * -1 - (*vert_ref)[i]) < kACCURACUY);  // Z axis
    EXPECT_TRUE(abs((*vert)[i - 1] - (*vert_ref)[i - 1]) <
                kACCURACUY);  // Y axis
    EXPECT_TRUE(abs((*vert)[i - 2] * -1 - (*vert_ref)[i - 2]) <
                kACCURACUY);  // X axis
  }
}

TEST(transfotmation_test, rotate_z) {
  S21::ModelFrame m;
  bool is_load = m.UploadModel(kCUBE_PATH);
  EXPECT_TRUE(is_load);
  EXPECT_EQ(m.get_facets_amount(), 12);
  EXPECT_EQ(m.get_indices_amount(), 72);
  EXPECT_EQ(m.get_vertexes_amount(), 8);
  EXPECT_TRUE(abs(m.get_max_value(S21::Axis::X) - 1) <= kACCURACUY);
  EXPECT_TRUE(abs(m.get_max_value(S21::Axis::Y) - 1) <= kACCURACUY);
  EXPECT_TRUE(abs(m.get_max_value(S21::Axis::Z) - 1) <= kACCURACUY);
  EXPECT_TRUE(abs(m.get_min_value(S21::Axis::X) - (-1)) <= kACCURACUY);
  EXPECT_TRUE(abs(m.get_min_value(S21::Axis::Y) - (-1)) <= kACCURACUY);
  EXPECT_TRUE(abs(m.get_min_value(S21::Axis::Z) - (-1)) <= kACCURACUY);

  S21::ModelFrame::Rotate(m, S21::Axis::Z, 180.0);

  const vector<double> *vert = m.get_vertexes_vector();
  const vector<unsigned int> *indi = m.get_indices_vector();

  S21::ModelFrame ref;
  ref.UploadModel(kCUBE_PATH);
  const vector<double> *vert_ref = ref.get_vertexes_vector();

  for (int i = 2; i < m.get_vertexes_amount() * 3; i += 3) {
    EXPECT_TRUE(abs((*vert)[i] - (*vert_ref)[i]) < kACCURACUY);  // Z axis
    EXPECT_TRUE(abs((*vert)[i - 1] * -1 - (*vert_ref)[i - 1]) <
                kACCURACUY);  // Y axis
    EXPECT_TRUE(abs((*vert)[i - 2] * -1 - (*vert_ref)[i - 2]) <
                kACCURACUY);  // X axis
  }

  // for (int i = 0; i < m.get_vertexes_amount() * 3; ++i) {
  //   std::cout << (*vert)[i] << "\n";
  // }
}

TEST(transfotmation_test, scale) {
  S21::ModelFrame m;
  bool is_load = m.UploadModel(kCUBE_PATH);
  EXPECT_TRUE(is_load);
  EXPECT_EQ(m.get_facets_amount(), 12);
  EXPECT_EQ(m.get_indices_amount(), 72);
  EXPECT_EQ(m.get_vertexes_amount(), 8);
  EXPECT_TRUE(abs(m.get_max_value(S21::Axis::X) - 1) <= kACCURACUY);
  EXPECT_TRUE(abs(m.get_max_value(S21::Axis::Y) - 1) <= kACCURACUY);
  EXPECT_TRUE(abs(m.get_max_value(S21::Axis::Z) - 1) <= kACCURACUY);
  EXPECT_TRUE(abs(m.get_min_value(S21::Axis::X) - (-1)) <= kACCURACUY);
  EXPECT_TRUE(abs(m.get_min_value(S21::Axis::Y) - (-1)) <= kACCURACUY);
  EXPECT_TRUE(abs(m.get_min_value(S21::Axis::Z) - (-1)) <= kACCURACUY);

  S21::ModelFrame::Scale(m, 0.1);

  const vector<double> *vert = m.get_vertexes_vector();
  const vector<unsigned int> *indi = m.get_indices_vector();

  for (int i = 0; i < m.get_vertexes_amount() * 3; i += 1) {
    EXPECT_TRUE(abs((*vert)[i]) - 0.1 < kACCURACUY);
  }
}

int main(int argc, char *argv[]) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}