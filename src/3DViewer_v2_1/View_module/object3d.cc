#include "object3d.h"

namespace s21 {

Object3D::Object3D() : index_buffer_(QOpenGLBuffer::IndexBuffer), texture_(nullptr)
{

}

Object3D::Object3D(const std::vector<float> &vertex, const std::vector<unsigned int> &indices, const QImage &texture)
 : index_buffer_(QOpenGLBuffer::IndexBuffer), texture_(nullptr) {
    init(vertex, indices, texture);
}

Object3D::~Object3D() {
    if (vertex_buffer_.isCreated()) {
        vertex_buffer_.destroy();
    }
    if (index_buffer_.isCreated()) {
        index_buffer_.destroy();
    }
    if (texture_ && texture_->isCreated()) {
        texture_->destroy();
        delete texture_;
    }
}

void Object3D::init(const std::vector<float> &vertex, const std::vector<unsigned int> &indices, const QImage &texture) {
    vertex_buffer_.create();
    vertex_buffer_.bind();
    vertex_buffer_.allocate(vertex.data(), vertex.size() * sizeof(float));
    vertex_buffer_.release();

    index_buffer_.create();
    index_buffer_.bind();
    index_buffer_.allocate(indices.data(), indices.size() * sizeof(unsigned int));

    texture_ = new QOpenGLTexture(texture.mirrored());
    texture_->setMinificationFilter(QOpenGLTexture::Nearest);
    texture_->setMagnificationFilter(QOpenGLTexture::Linear);
    texture_->setWrapMode(QOpenGLTexture::Repeat);

    vertexes_ = vertex.size() / 8;
    indices_ = indices.size();
}

void Object3D::draw(QOpenGLShaderProgram *program, QOpenGLFunctions *gl_functions) {
    texture_->bind(0);
    program->setUniformValue("u_texture", 0);
    program->setUniformValue("u_model_matrix", m_model_matrix_);

    vertex_buffer_.bind();

    int stride = sizeof(float) * 8;
    int offset = 0;
    int location = program->attributeLocation("a_position");
    program->enableAttributeArray(location);
    program->setAttributeBuffer(location, GL_FLOAT, offset, 3, stride);

    offset += sizeof(float) * 3;
    location = program->attributeLocation("a_texcoord");
    program->enableAttributeArray(location);
    program->setAttributeBuffer(location, GL_FLOAT, offset, 2, stride);

    offset += sizeof(float) * 2;
    location = program->attributeLocation("a_normal");
    program->enableAttributeArray(location);
    program->setAttributeBuffer(location, GL_FLOAT, offset, 3, stride);

    index_buffer_.bind();

    gl_functions->glDrawElements(GL_TRIANGLES, index_buffer_.size(), GL_UNSIGNED_INT, 0);
//    DrawVertexes(gl_functions);
//    DrawEdges(gl_functions);

    vertex_buffer_.release();
    index_buffer_.release();
    texture_->release();
}

void Object3D::setup_vertexes(int size, QColor color, VertexStyle style) {
    vertexes_size_ = size;
    vertexes_color_ = color;
    vertexes_style_ = style;
}

void Object3D::setup_edges(int size, QColor color, EdgeStyle style) {
    edges_size_ = size;
    edges_color_ = color;
    edges_style_ = style;
}

void Object3D::DrawVertexes(QOpenGLFunctions *gl_function) {
      if (vertexes_style_ == VertexStyle::ROUND) {
        glEnable(GL_POINT_SMOOTH);
      } else if (vertexes_style_ == VertexStyle::SQUARE) {
        glDisable(GL_POINT_SMOOTH);
      }
      glPointSize(vertexes_size_);
      glColor3f(vertexes_color_.redF(), vertexes_color_.greenF(),
                vertexes_color_.blueF());
      if (vertexes_style_ != VertexStyle::NONE) {
        gl_function->glDrawArrays(GL_POINTS, 0, vertexes_);
      }
}

void Object3D::DrawEdges(QOpenGLFunctions *gl_function) {
      if (edges_style_ == EdgeStyle::STIPPLE) {
        glLineStipple(2, 0x00F0);
        glEnable(GL_LINE_STIPPLE);
      } else if (edges_style_ == EdgeStyle::SOLID) {
        glDisable(GL_LINE_STIPPLE);
      }
      glLineWidth(edges_size_);
      glColor3f(edges_color_.redF(), edges_color_.greenF(), edges_color_.blueF());
      gl_function->glDrawElements(GL_LINES, indices_, GL_UNSIGNED_INT, 0);
}

}
