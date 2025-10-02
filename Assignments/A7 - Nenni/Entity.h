#pragma once

#include <memory>
#include "Mesh.h"
#include "Transformation.h"
#include "Material.h"

using namespace std;

class Entity
{
public:
	Entity(shared_ptr<Mesh> inMesh);

	shared_ptr<Mesh> GetMesh();
	shared_ptr<Transformation> GetTransform();

	void Draw();

private:
	shared_ptr<Mesh> mesh;
	shared_ptr<Transformation> transform;
	shared_ptr<Material> material
};

