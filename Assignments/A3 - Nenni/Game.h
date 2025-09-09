#pragma once

#include <d3d11.h>
#include <wrl/client.h>
#include <DirectXMath.h>
#include <string>

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

	// Initialization helper methods - feel free to customize, combine, remove, etc.
	void LoadShaders();
	void CreateGeometry();

	//Refreshes ImGui 
	void ResetUI(float deltaTime);

	//Builds UI
	void ShowUIWindow();

	//Variables for UI manipulation
	float background[4];
	bool showDemo;
	bool table;

	//Inverts all colors
	void InvertColor();

	//Colors for triangle
	float color1[4] = { 1.0f, 0.0f, 0.0f, 1.0f };
	float color2[4] = { 0.0f, 1.0f, 0.0f, 1.0f };
	float color3[4] = { 0.0f, 0.0f, 1.0f, 1.0f };

	//Strings for user input
	std::string str;
	std::string input;

	//Strings for table

	std::string a00 = "Cake";

	std::string b00 = "Eggs";
	std::string b10 = "Sugar";
	std::string b01 = "Milk";
	std::string b11 = "Flour";

	std::string a01 = "Chips";

	std::string a10 = "Paper Towels";
	std::string a11 = "Shampoo";

	//Adds table to UI
	void AddTable();

	// Note the usage of ComPtr below
	//  - This is a smart pointer for objects that abide by the
	//     Component Object Model, which DirectX objects do
	//  - More info here: https://github.com/Microsoft/DirectXTK/wiki/ComPtr

	// Buffers to hold actual geometry data
	Microsoft::WRL::ComPtr<ID3D11Buffer> vertexBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> indexBuffer;

	// Shaders and shader-related constructs
	Microsoft::WRL::ComPtr<ID3D11PixelShader> pixelShader;
	Microsoft::WRL::ComPtr<ID3D11VertexShader> vertexShader;
	Microsoft::WRL::ComPtr<ID3D11InputLayout> inputLayout;
};

