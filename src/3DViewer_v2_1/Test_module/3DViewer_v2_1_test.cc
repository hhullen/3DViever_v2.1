#include <gtest/gtest.h>

#include <iostream>
#include <string>

#include "../Model_module/model_obj_model.h"
using std::cout;
using std::string;

const float kACCURACUY = 0.00001;

const string kCUBE_PATH = "models/cube.obj";
const string kEMPTY_PATH = "models/Empty.obj";
const string kMILLION_PATH = "models/one_million.obj";

TEST(loader_test, uploading_cube) {
  s21::OBJModel m;
  bool is_load = m.UploadModel(kCUBE_PATH);
  EXPECT_TRUE(is_load);
  EXPECT_EQ(m.get_facets_amount(), 12);
  EXPECT_EQ(m.get_indices_amount(), 72);
  EXPECT_EQ(m.get_vertexes_amount(), 8);
}

TEST(loader_test, uploading_million) {
  s21::OBJModel m;
  bool is_load = m.UploadModel(kMILLION_PATH);
  EXPECT_TRUE(is_load);
  EXPECT_EQ(m.get_facets_amount(), 1669317);
  EXPECT_EQ(m.get_vertexes_amount(), 1000000);
}

TEST(loader_test, uploading_empty) {
  s21::OBJModel m;
  bool is_load = m.UploadModel(kEMPTY_PATH);
  EXPECT_FALSE(is_load);
  EXPECT_EQ(m.get_facets_amount(), 0);
  EXPECT_EQ(m.get_indices_amount(), 0);
  EXPECT_EQ(m.get_vertexes_amount(), 0);
}

TEST(loader_test, uploading_wrong_path) {
  s21::OBJModel m;
  bool is_load = m.UploadModel("hello_world.kek");
  EXPECT_FALSE(is_load);
  EXPECT_EQ(m.get_facets_amount(), 0);
  EXPECT_EQ(m.get_indices_amount(), 0);
  EXPECT_EQ(m.get_vertexes_amount(), 0);
}

TEST(transfotmation_test, move_x) {
  s21::OBJModel m;
  bool is_load = m.UploadModel(kCUBE_PATH);
  EXPECT_TRUE(is_load);
  EXPECT_EQ(m.get_facets_amount(), 12);
  EXPECT_EQ(m.get_indices_amount(), 72);
  EXPECT_EQ(m.get_vertexes_amount(), 8);

  s21::OBJModel::Move(m, s21::Axis::X, 5.0);

  const vector<float> *vert = m.get_vertexes();

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
  s21::OBJModel m;
  bool is_load = m.UploadModel(kCUBE_PATH);
  EXPECT_TRUE(is_load);
  EXPECT_EQ(m.get_facets_amount(), 12);
  EXPECT_EQ(m.get_indices_amount(), 72);
  EXPECT_EQ(m.get_vertexes_amount(), 8);

  s21::OBJModel::Move(m, s21::Axis::Y, 5.0);

  const vector<float> *vert = m.get_vertexes();

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
  s21::OBJModel m;
  bool is_load = m.UploadModel(kCUBE_PATH);
  EXPECT_TRUE(is_load);
  EXPECT_EQ(m.get_facets_amount(), 12);
  EXPECT_EQ(m.get_indices_amount(), 72);
  EXPECT_EQ(m.get_vertexes_amount(), 8);

  s21::OBJModel::Move(m, s21::Axis::Z, 5.0);

  const vector<float> *vert = m.get_vertexes();

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
  s21::OBJModel m;
  bool is_load = m.UploadModel(kCUBE_PATH);
  EXPECT_TRUE(is_load);
  EXPECT_EQ(m.get_facets_amount(), 12);
  EXPECT_EQ(m.get_indices_amount(), 72);
  EXPECT_EQ(m.get_vertexes_amount(), 8);

  s21::OBJModel::Rotate(m, s21::Axis::X, 180.0);

  const vector<float> *vert = m.get_vertexes();

  s21::OBJModel ref;
  ref.UploadModel(kCUBE_PATH);
  const vector<float> *vert_ref = ref.get_vertexes();

  for (int i = 2; i < m.get_vertexes_amount() * 3; i += 3) {
    EXPECT_TRUE(abs((*vert)[i] * -1 - (*vert_ref)[i]) < kACCURACUY);  // Z axis
    EXPECT_TRUE(abs((*vert)[i - 1] * -1 - (*vert_ref)[i - 1]) <
                kACCURACUY);  // Y axis
    EXPECT_TRUE(abs((*vert)[i - 2] - (*vert_ref)[i - 2]) <
                kACCURACUY);  // X axis
  }
}

TEST(transfotmation_test, rotate_y) {
  s21::OBJModel m;
  bool is_load = m.UploadModel(kCUBE_PATH);
  EXPECT_TRUE(is_load);
  EXPECT_EQ(m.get_facets_amount(), 12);
  EXPECT_EQ(m.get_indices_amount(), 72);
  EXPECT_EQ(m.get_vertexes_amount(), 8);

  s21::OBJModel::Rotate(m, s21::Axis::Y, 180.0);

  const vector<float> *vert = m.get_vertexes();

  s21::OBJModel ref;
  ref.UploadModel(kCUBE_PATH);
  const vector<float> *vert_ref = ref.get_vertexes();

  for (int i = 2; i < m.get_vertexes_amount() * 3; i += 3) {
    EXPECT_TRUE(abs((*vert)[i] * -1 - (*vert_ref)[i]) < kACCURACUY);  // Z axis
    EXPECT_TRUE(abs((*vert)[i - 1] - (*vert_ref)[i - 1]) <
                kACCURACUY);  // Y axis
    EXPECT_TRUE(abs((*vert)[i - 2] * -1 - (*vert_ref)[i - 2]) <
                kACCURACUY);  // X axis
  }
}

TEST(transfotmation_test, rotate_z) {
  s21::OBJModel m;
  bool is_load = m.UploadModel(kCUBE_PATH);
  EXPECT_TRUE(is_load);
  EXPECT_EQ(m.get_facets_amount(), 12);
  EXPECT_EQ(m.get_indices_amount(), 72);
  EXPECT_EQ(m.get_vertexes_amount(), 8);

  s21::OBJModel::Rotate(m, s21::Axis::Z, 180.0);

  const vector<float> *vert = m.get_vertexes();

  s21::OBJModel ref;
  ref.UploadModel(kCUBE_PATH);
  const vector<float> *vert_ref = ref.get_vertexes();

  for (int i = 2; i < m.get_vertexes_amount() * 3; i += 3) {
    EXPECT_TRUE(abs((*vert)[i] - (*vert_ref)[i]) < kACCURACUY);  // Z axis
    EXPECT_TRUE(abs((*vert)[i - 1] * -1 - (*vert_ref)[i - 1]) <
                kACCURACUY);  // Y axis
    EXPECT_TRUE(abs((*vert)[i - 2] * -1 - (*vert_ref)[i - 2]) <
                kACCURACUY);  // X axis
  }
}

TEST(transfotmation_test, scale) {
  s21::OBJModel m;
  bool is_load = m.UploadModel(kCUBE_PATH);
  EXPECT_TRUE(is_load);
  EXPECT_EQ(m.get_facets_amount(), 12);
  EXPECT_EQ(m.get_indices_amount(), 72);
  EXPECT_EQ(m.get_vertexes_amount(), 8);

  s21::OBJModel::Scale(m, 0.1);

  const vector<float> *vert = m.get_vertexes();

  for (int i = 0; i < m.get_vertexes_amount() * 3; i += 1) {
    EXPECT_TRUE(abs((*vert)[i]) - 0.1 < kACCURACUY);
  }
}

int main(int argc, char *argv[]) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}