#pragma once

#include <d3d11.h>
#include <wrl/client.h>
#include <DirectXMath.h>
#include <memory>
#include "Mesh.h"
#include "BufferStructs.h"
#include "Entity.h"
#include "Camera.h"
#include "Material.h"

using namespace std;

class Game
{
public:
	// Basic OOP setup
	Game();
	~Game();
	Game(const Game&) = delete; // Remove copy constructor
	Game& operator=(const Game&) = delete; // Remove copy-assignment operator

	// Primary functions
	void Update(float deltaTime, float totalTime);
	void Draw(float deltaTime, float totalTime);
	void OnResize();

private:

	// List of Materials
	shared_ptr<Material> materials[3];

	// List of Meshes
	shared_ptr<Mesh> shapes[8];

	// List of Entities
	shared_ptr<Entity> entities[8];

	// List of Cameras
	shared_ptr<Camera> cameras[3];

	// Variables for UI manipulation
	float background[4];

	// Colors for triangle
	float color1[4] = { 1.0f, 0.0f, 0.0f, 1.0f };
	float color2[4] = { 0.0f, 1.0f, 0.0f, 1.0f };
	float color3[4] = { 0.0f, 0.0f, 1.0f, 1.0f };
	float color4[4] = { 1.0f, 1.0f, 1.0f, 1.0f };

	BufferStructs constBuffData;

	shared_ptr<Camera> camera;

	int currentCam;

	// Initialization helper methods - feel free to customize, combine, remove, etc.
	void LoadShaders();

	void CreateMaterials();
	void CreateGeometry();
	void CreateEntities();

	// Refreshes ImGui 
	void ResetUI(float deltaTime);

	// Builds UI
	void ShowUIWindow();

	// Inverts all colors
	void InvertColor();

	// Adds table to UI
	void ShowStats();

	void TransformStats();

	void CameraStats();

	// Adds Graphic changing UI
	void GraphicChangeUI();

	// Note the usage of ComPtr below
	//  - This is a smart pointer for objects that abide by the
	//     Component Object Model, which DirectX objects do
	//  - More info here: https://github.com/Microsoft/DirectXTK/wiki/ComPtr

	// Constant Buffer
	Microsoft::WRL::ComPtr<ID3D11Buffer> constBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> pixelConstBuffer;


	// Shader-related Construct
	Microsoft::WRL::ComPtr<ID3D11InputLayout> inputLayout;
};

