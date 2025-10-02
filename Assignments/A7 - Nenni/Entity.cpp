#include "Entity.h"

Entity::Entity(shared_ptr<Mesh> inMesh) {
	mesh = inMesh;
	transform = make_shared<Transformation>();
}

shared_ptr<Mesh> Entity::GetMesh()
{
	return mesh;
}

shared_ptr<Transformation> Entity::GetTransform()
{
	return transform;
}

void Entity::Draw()
{
	Graphics::Context->VSSetShader(material->GetVertexShader().get(), 0, 0);
	Graphics::Context->PSSetShader(material->GetPixelShader().get(), 0, 0);

	mesh->Draw();
}
