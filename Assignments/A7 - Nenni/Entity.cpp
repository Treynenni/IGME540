#include "Entity.h"

Entity::Entity(shared_ptr<Mesh> inMesh, shared_ptr<Material> inMaterial) {
	mesh = inMesh;
	transform = make_shared<Transformation>();
	material = inMaterial;
}

shared_ptr<Mesh> Entity::GetMesh()
{
	return mesh;
}

shared_ptr<Transformation> Entity::GetTransform()
{
	return transform;
}

shared_ptr<Material> Entity::GetMaterial()
{
	return material;
}

void Entity::SetMaterial(shared_ptr<Material> newMaterial)
{
	material = newMaterial;
}

void Entity::Draw()
{
	Graphics::Context->VSSetShader(material->GetVertexShader().Get(), 0, 0);
	Graphics::Context->PSSetShader(material->GetPixelShader().Get(), 0, 0);

	mesh->Draw();
}
