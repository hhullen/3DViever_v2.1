#include "object3d.h"

namespace s21 {

Object3D::Object3D() : ordered_index_buffer_(QOpenGLBuffer::IndexBuffer), texture_(nullptr)
{

}

Object3D::Object3D(const vector<float> &ordered_vertex, const vector<unsigned int> &ordered_indices, const vector<float> &vertex, const vector<unsigned int> &indices, const QImage &texture)
 : ordered_index_buffer_(QOpenGLBuffer::IndexBuffer), index_buffer_(QOpenGLBuffer::IndexBuffer), texture_(nullptr) {
    init(ordered_vertex, ordered_indices, vertex, indices, texture);
}

Object3D::~Object3D() {
    if (ordered_vertex_buffer_.isCreated()) {
        ordered_vertex_buffer_.destroy();
    }
    if (ordered_vertex_buffer_.isCreated()) {
        ordered_vertex_buffer_.destroy();
    }
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

void Object3D::init(const vector<float> &ordered_vertex, const vector<unsigned int> &ordered_indices, const vector<float> &vertex, const vector<unsigned int> &indices, const QImage &texture) {
    ordered_vertex_buffer_.create();
    ordered_vertex_buffer_.bind();
    ordered_vertex_buffer_.allocate(ordered_vertex.data(), ordered_vertex.size() * sizeof(float));
    ordered_vertex_buffer_.release();

    ordered_index_buffer_.create();
    ordered_index_buffer_.bind();
    ordered_index_buffer_.allocate(ordered_indices.data(), ordered_indices.size() * sizeof(unsigned int));

    vertex_buffer_.create();
    vertex_buffer_.bind();
    vertex_buffer_.allocate(vertex.data(), vertex.size() * sizeof(float));
    vertex_buffer_.release();

    index_buffer_.create();
    index_buffer_.bind();
    index_buffer_.allocate(indices.data(), indices.size() * sizeof(unsigned int));
    index_buffer_.release();

    vertexes_ = vertex.size() / 3;
    indices_ = indices.size();
    ordered_vertexes_ = ordered_vertex.size() / 8;

    texture_ = new QOpenGLTexture(texture.mirrored());
    texture_->setMinificationFilter(QOpenGLTexture::Nearest);
    texture_->setMagnificationFilter(QOpenGLTexture::Linear);
    texture_->setWrapMode(QOpenGLTexture::Repeat);
}

void Object3D::draw(QOpenGLShaderProgram *program, QOpenGLFunctions *gl_functions) {
    texture_->bind(0);
    program->setUniformValue("u_texture", 0);
    program->setUniformValue("u_model_matrix", m_model_matrix_);

    DrawPolygons(program, gl_functions);
    if (view_mode_ == ViewMode::WIREFRAME || view_mode_ == ViewMode::SHADEFRAME) {
        DrawVertexes(program, gl_functions);
        DrawEdges(program, gl_functions);
    }
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

void Object3D::set_view_mode(ViewMode mode) {
    view_mode_ = mode;
}

void Object3D::set_texture(const QImage &texture) {
    if (texture_ && texture_->isCreated()) {
        texture_->destroy();
        delete texture_;
    }
    texture_ = new QOpenGLTexture(texture.mirrored());
    texture_->setMinificationFilter(QOpenGLTexture::Nearest);
    texture_->setMagnificationFilter(QOpenGLTexture::Linear);
    texture_->setWrapMode(QOpenGLTexture::Repeat);
}

void Object3D::DrawPolygons(QOpenGLShaderProgram *program, QOpenGLFunctions *gl_function) {
    ordered_vertex_buffer_.bind();

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

    ordered_index_buffer_.bind();

    if (view_mode_ == ViewMode::SHADE || view_mode_ == ViewMode::SHADEFRAME) {
        gl_function->glDrawElements(GL_TRIANGLES, ordered_vertexes_, GL_UNSIGNED_INT, nullptr);
    }

    ordered_vertex_buffer_.release();
    ordered_index_buffer_.release();
}

void Object3D::DrawVertexes(QOpenGLShaderProgram *program, QOpenGLFunctions *gl_function) {
    program->setUniformValue("u_light_power", 0.0f);
    program->setUniformValue("u_shadow_color", QVector4D(vertexes_color_.redF(), vertexes_color_.greenF(), vertexes_color_.blueF(), 1.0));
    program->setUniformValue("u_polygon_color", vertexes_color_);

    vertex_buffer_.bind();

    int location = program->attributeLocation("a_position");
    program->enableAttributeArray(location);
    program->setAttributeBuffer(location, GL_FLOAT, 0, 3, 0);

      if (vertexes_style_ == VertexStyle::ROUND) {
        gl_function->glEnable(GL_POINT_SMOOTH);
      } else if (vertexes_style_ == VertexStyle::SQUARE) {
        gl_function->glDisable(GL_POINT_SMOOTH);
      }
      glPointSize(vertexes_size_);
      if (vertexes_style_ != VertexStyle::NONE) {
        gl_function->glDrawArrays(GL_POINTS, 0, vertexes_);
      }
      vertex_buffer_.release();
}

void Object3D::DrawEdges(QOpenGLShaderProgram *program, QOpenGLFunctions *gl_function) {
    program->setUniformValue("u_light_power", 0.0f);
    program->setUniformValue("u_shadow_color", QVector4D(edges_color_.redF(), edges_color_.greenF(), edges_color_.blueF(), 1.0));
    program->setUniformValue("u_polygon_color", edges_color_);

    vertex_buffer_.bind();

    int location = program->attributeLocation("a_position");
    program->enableAttributeArray(location);
    program->setAttributeBuffer(location, GL_FLOAT, 0, 3, 0);

    index_buffer_.bind();

      if (edges_style_ == EdgeStyle::STIPPLE) {
        glLineStipple(2, 0x00F0);
        gl_function->glEnable(GL_LINE_STIPPLE);
      } else if (edges_style_ == EdgeStyle::SOLID) {
        gl_function->glDisable(GL_LINE_STIPPLE);
      }
      gl_function->glLineWidth(edges_size_);
      gl_function->glDrawElements(GL_LINES, indices_, GL_UNSIGNED_INT, nullptr);

      vertex_buffer_.release();
      index_buffer_.release();
}

}
