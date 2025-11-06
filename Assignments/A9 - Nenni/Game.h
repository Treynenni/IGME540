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
#include "Lights.h"

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
	shared_ptr<Mesh> shapes[7];

	// List of Entities
	shared_ptr<Entity> entities[7];

	// List of Cameras
	shared_ptr<Camera> cameras[3];

	// Variables for UI manipulation
	float background[4];

	// Colors for triangle
	float color1[4] = { 1.0f, 0.0f, 0.0f, 1.0f };
	float color2[4] = { 0.0f, 1.0f, 0.0f, 1.0f };
	float color3[4] = { 0.0f, 0.0f, 1.0f, 1.0f };
	float color4[4] = { 1.0f, 1.0f, 1.0f, 1.0f };

	XMFLOAT3 ambientColor;

	Light directionalLight1;

	VertexBufferData constVertBuffData;

	PixelBufferData constPixBuffData;

	shared_ptr<Camera> camera;

	int currentCam;

	Microsoft::WRL::ComPtr<ID3D11VertexShader> LoadVertexShader(const wchar_t* shaderPath);
	Microsoft::WRL::ComPtr<ID3D11PixelShader> LoadPixelShader(const wchar_t* shaderPath);

	// Helper Methods

	void LoadAssets();
	void CreateEntities();

	// Refreshes ImGui 
	void ResetUI(float deltaTime);

	// Builds UI
	void ShowUIWindow();

	// Adds table to UI
	void ShowStats();

	void TransformStats();

	void CameraStats();

	void MaterialUI();

	// Adds Graphic changing UI
	void GraphicChangeUI();

	// Note the usage of ComPtr below
	//  - This is a smart pointer for objects that abide by the
	//     Component Object Model, which DirectX objects do
	//  - More info here: https://github.com/Microsoft/DirectXTK/wiki/ComPtr

	// Constant Buffers
	Microsoft::WRL::ComPtr<ID3D11Buffer> vertexcBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> pixelcBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> constBuffer;

	// Shader-related Construct
	Microsoft::WRL::ComPtr<ID3D11InputLayout> inputLayout;
};

