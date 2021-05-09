#include "world.h"


Entity::Entity() {
	this->entity_type = NONE;
}



Entity::addChild(Entity* ent) {
	this->children.push_back(ent);
}

void EntityMesh::render(){
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

    for (int i = 0; i < children.size(); i++)
        children[i]->render();  //repeat for every child
}


void EntityMesh::update(float dt) {}

World* World::instance = NULL;

World::World() {
    instance = this;

	root = EntityMesh();
	root.model=Matrix44();
	root.texture = Texture::Get("data/texture.tga");
	root.mesh= Mesh::Get("data/box.ASE");
	root.shader= Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");
	
}

void World::render() {

	
	//set the clear color (the background color)
	//glClearColor(0.0, 0.0, 0.0, 1.0);

	// Clear the window and the depth buffer
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	//set flags
	//glDisable(GL_BLEND);
	//glEnable(GL_DEPTH_TEST);
	//glDisable(GL_CULL_FACE);

	//create model matrix for cube
	//Matrix44 m;
	//m.rotate(angle * DEG2RAD, Vector3(0, 1, 0));

	//if (shader)
	//{
	//	//enable shader
	//	shader->enable();

	//	//upload uniforms
	//	shader->setUniform("u_color", Vector4(1, 1, 1, 1));
	//	shader->setUniform("u_viewprojection", camera->viewprojection_matrix);
	//	shader->setUniform("u_texture", texture, 0);
	//	shader->setUniform("u_model", m);
	//	shader->setUniform("u_time", time);

	//	//do the draw call
	//	mesh->render(GL_TRIANGLES);

	//	//disable shader
	//	shader->disable();
	//}
	
	root.render();
	//Draw the floor grid
	//drawGrid();

	//render the FPS, Draw Calls, etc
	//drawText(2, 2, getGPUStats(), Vector3(1, 1, 1), 2);

}
