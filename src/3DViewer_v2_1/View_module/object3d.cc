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

    vertex_buffer_.release();
    index_buffer_.release();
    texture_->release();
}

}
