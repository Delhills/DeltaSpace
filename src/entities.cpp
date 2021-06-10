#include "entities.h"
#include "game.h"
Entity::Entity() {
	this->entity_type = NONE;
}


void Entity::addChild(Entity* ent) {
	this->children.push_back(ent);
}

EntityMesh::EntityMesh()
{
	mesh = NULL;
	texture = Texture::getWhiteTexture();
	color = Vector4();

}

EntityMesh::EntityMesh(Mesh* mesh, Texture* texture, Shader* shader, Vector4 color)
{
	this->model = Matrix44();
	this->mesh = mesh;
	this->texture = texture;
	this->shader = shader;
	this->color = color;
}

void EntityMesh::render() {
	Camera* camera = Camera::current;
	Matrix44 model = this->model;

	//enable shader and pass uniforms
	shader->enable();
	shader->setUniform("u_model", model);
	shader->setUniform("u_viewprojection", camera->viewprojection_matrix);
	shader->setTexture("u_texture", this->texture, 0);
	shader->setUniform("u_color", this->color);
	float time = Game::instance->time;
	//std::cout << time << "\n";
	shader->setUniform("u_time", time);
	//shader->setUniform("u_Res", Vector2();
	
	//render the mesh using the shader
	mesh->render(GL_TRIANGLES);

	//disable the shader after finishing rendering
	shader->disable();

	for (int i = 0; i < children.size(); i++)
		children[i]->render();  //repeat for every child
}


void EntityMesh::render_anim(Animation* anim) {
	Camera* camera = Camera::current;
	Matrix44 model = this->model;

	//enable shader and pass uniforms
	shader->enable();
	shader->setUniform("u_model", model);
	shader->setUniform("u_viewprojection", camera->viewprojection_matrix);
	shader->setTexture("u_texture", this->texture, 0);
	shader->setUniform("u_color", this->color);
	float time = Game::instance->time;
	//std::cout << time << "\n";
	shader->setUniform("u_time", time);
	//shader->setUniform("u_Res", Vector2();

	//render the mesh using the shader
	mesh->renderAnimated(GL_TRIANGLES, &anim->skeleton);

	//disable the shader after finishing rendering
	shader->disable();

	for (int i = 0; i < children.size(); i++)
		children[i]->render();  //repeat for every child
}

void EntityMesh::render(Matrix44 model)
{
	Camera* camera = Camera::current;
	
	//enable shader and pass uniforms
	shader->enable();
	shader->setUniform("u_model", model);
	shader->setUniform("u_viewprojection", camera->viewprojection_matrix);
	shader->setTexture("u_texture", this->texture, 0);
	shader->setUniform("u_color", this->color);
	//render the mesh using the shader
	mesh->render(GL_TRIANGLES);

	//disable the shader after finishing rendering
	shader->disable();

	for (int i = 0; i < children.size(); i++)
		children[i]->render();  //repeat for every child
	
}
Obstacle::Obstacle(Mesh* mesh, Texture* texture, Shader* shader, Vector4 color,eObstacleType type)
{
	this->model = Matrix44();
	this->mesh = mesh;
	this->texture = texture;
	this->shader = shader;
	this->color = color;
	this->type = type;
}

void EntityMesh::update(float dt) {}