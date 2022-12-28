#ifndef ENUM_PARAMETERS_H
#define ENUM_PARAMETERS_H

namespace s21 {

enum ProjectionType { ORTHOGONAL, PERSPECTIVE };
enum EdgeStyle { SOLID, STIPPLE };
enum VertexStyle { ROUND, SQUARE, NONE };
enum ShadeMode { FLAT, SMOOTH, NOMAP };
enum ScreenshotFile { JPEG, BMP };
enum ViewMode { WIREFRAME, SHADE, SHADEFRAME };

}  // namespace s21

#endif  // ENUM_PARAMETERS_H
