#include "world.h"


Entity::Entity() {
    entity_type = NONE;
}

void EntityMesh::render() {
    Camera* camera = Camera::current;
    Matrix44 model = this->model;

    //enable shader and pass uniforms
    shader->enable();
    shader->setUniform("u_model", model);
    shader->setUniform("u_viewprojection", camera->viewprojection_matrix);
    shader->setTexture("u_texture", texture, 0);

    //render the mesh using the shader
    mesh->render(GL_TRIANGLES);

    //disable the shader after finishing rendering
    shader->disable();
}

World* World::instance = NULL;
World::World() {
    instance = this;
}

