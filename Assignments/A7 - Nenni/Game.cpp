#include "Game.h"
#include "Graphics.h"
#include "Vertex.h"
#include "Input.h"
#include "PathHelpers.h"
#include "Window.h"


// This code assumes files are in "ImGui" subfolder!
// Adjust as necessary for your own folder structure and project setup
#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_dx11.h"
#include "ImGui/imgui_impl_win32.h"

// Needed for a helper function to load pre-compiled shader files
#pragma comment(lib, "d3dcompiler.lib")
#include <d3dcompiler.h>

// For the DirectX Math library
using namespace DirectX;
using namespace std;



// --------------------------------------------------------
// The constructor is called after the window and graphics API
// are initialized but before the game loop begins
// --------------------------------------------------------
Game::Game()
{
	// Initializes Variables
	background[0] = 0.4f;
	background[1] = 0.6f;
	background[2] = 0.75f;
	background[3] = 0.0f;

	constBuffData.colorTint = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

	cameras[0] = make_shared<Camera>(Camera(Window::AspectRatio(), XMFLOAT3(0, 0, -1.0f), XM_PI / 2, 1.0, 0.01));
	cameras[1] = make_shared<Camera>(Camera(Window::AspectRatio(), XMFLOAT3(0, 0, -2.0f), XM_PI / 3, 1.0, 0.01));
	cameras[2] = make_shared<Camera>(Camera(Window::AspectRatio(), XMFLOAT3(0, 0, -3.0f), XM_PI / 4, 1.0, 0.01));

	camera = cameras[0];

	currentCam = 1;

	// Describe the constant buffer
	D3D11_BUFFER_DESC cbDesc = {}; // Sets struct to all zeros
	cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbDesc.ByteWidth = (sizeof(BufferStructs) + 15) / 16 * 16; // Must be a multiple of 16
	cbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cbDesc.Usage = D3D11_USAGE_DYNAMIC;
	Graphics::Device->CreateBuffer(&cbDesc, 0, constBuffer.GetAddressOf());

	Graphics::Context->VSSetConstantBuffers(
		0, // Which slot (register) to bind the buffer to?
		1, // How many are we setting right now?
		constBuffer.GetAddressOf()); // Array of buffers (or address of just one)

	// Helper methods for loading shaders, creating some basic
	// geometry to draw and some simple camera matrices.
	//  - You'll be expanding and/or replacing these later
	LoadShaders();
	CreateMaterials();
	CreateGeometry();
	CreateEntities();

	// Set initial graphics API state
	//  - These settings persist until we change them
	//  - Some of these, like the primitive topology & input layout, probably won't change
	//  - Others, like setting shaders, will need to be moved elsewhere later
	{
		// Tell the input assembler (IA) stage of the pipeline what kind of
		// geometric primitives (points, lines or triangles) we want to draw.  
		// Essentially: "What kind of shape should the GPU draw with our vertices?"
		Graphics::Context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		// Ensure the pipeline knows how to interpret all the numbers stored in
		// the vertex buffer. For this course, all of your vertices will probably
		// have the same layout, so we can just set this once at startup.
		Graphics::Context->IASetInputLayout(inputLayout.Get());

		// Initialize ImGui itself & platform/renderer backends
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGui_ImplWin32_Init(Window::Handle());
		ImGui_ImplDX11_Init(Graphics::Device.Get(), Graphics::Context.Get());

		// Pick a style (uncomment one of these 3)
		ImGui::StyleColorsDark();
		//ImGui::StyleColorsLight();
		//ImGui::StyleColorsClassic();

	}
}


// --------------------------------------------------------
// Clean up memory or objects created by this class
// 
// Note: Using smart pointers means there probably won't
//       be much to manually clean up here!
// --------------------------------------------------------
Game::~Game()
{
	// ImGui clean up
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}


// --------------------------------------------------------
// Loads shaders from compiled shader object (.cso) files
// and also created the Input Layout that describes our 
// vertex data to the rendering pipeline. 
// - Input Layout creation is done here because it must 
//    be verified against vertex shader byte code
// - We'll have that byte code already loaded below
// --------------------------------------------------------
void Game::LoadShaders()
{
	// BLOBs (or Binary Large OBjects) for reading raw data from external files
	// - This is a simplified way of handling big chunks of external data
	// - Literally just a big array of bytes read from a file
	ID3DBlob* pixelShaderBlob;
	ID3DBlob* vertexShaderBlob;

	// Loading shaders
	//  - Visual Studio will compile our shaders at build time
	//  - They are saved as .cso (Compiled Shader Object) files
	//  - We need to load them when the application starts
	{
		// Read our compiled shader code files into blobs
		// - Essentially just "open the file and plop its contents here"
		// - Uses the custom FixPath() helper from Helpers.h to ensure relative paths
		// - Note the "L" before the string - this tells the compiler the string uses wide characters
		D3DReadFileToBlob(FixPath(L"PixelShader.cso").c_str(), &pixelShaderBlob);
		D3DReadFileToBlob(FixPath(L"VertexShader.cso").c_str(), &vertexShaderBlob);

		// Create the actual Direct3D shaders on the GPU
		Graphics::Device->CreatePixelShader(
			pixelShaderBlob->GetBufferPointer(),	// Pointer to blob's contents
			pixelShaderBlob->GetBufferSize(),		// How big is that data?
			0,										// No classes in this shader
			pixelShader.GetAddressOf());			// Address of the ID3D11PixelShader pointer

		Graphics::Device->CreateVertexShader(
			vertexShaderBlob->GetBufferPointer(),	// Get a pointer to the blob's contents
			vertexShaderBlob->GetBufferSize(),		// How big is that data?
			0,										// No classes in this shader
			vertexShader.GetAddressOf());			// The address of the ID3D11VertexShader pointer
	}

	// Create an input layout 
	//  - This describes the layout of data sent to a vertex shader
	//  - In other words, it describes how to interpret data (numbers) in a vertex buffer
	//  - Doing this NOW because it requires a vertex shader's byte code to verify against!
	//  - Luckily, we already have that loaded (the vertex shader blob above)
	{
		D3D11_INPUT_ELEMENT_DESC inputElements[3] = {};

		// Set up the first element - a position, which is 3 float values
		inputElements[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;				// Most formats are described as color channels; really it just means "Three 32-bit floats"
		inputElements[0].SemanticName = "POSITION";							// This is "POSITION" - needs to match the semantics in our vertex shader input!
		inputElements[0].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;	// How far into the vertex is this?  Assume it's after the previous element

		// Set up the second element - UV coords, which is 2 more float values
		inputElements[1].Format = DXGI_FORMAT_R32G32_FLOAT;					// 2x 32-bit floats
		inputElements[1].SemanticName = "UV";								// Match our vertex shader input!
		inputElements[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;	// After the previous element

		// Set up the second element - UV coords, which is 3 more float values
		inputElements[2].Format = DXGI_FORMAT_R32G32B32_FLOAT;				// 3x 32-bit floats
		inputElements[2].SemanticName = "NORMAL";							// Match our vertex shader input!
		inputElements[2].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;	// After the previous element


		// Create the input layout, verifying our description against actual shader code
		Graphics::Device->CreateInputLayout(
			inputElements,							// An array of descriptions
			3,										// How many elements in that array?
			vertexShaderBlob->GetBufferPointer(),	// Pointer to the code of a shader that uses this layout
			vertexShaderBlob->GetBufferSize(),		// Size of the shader code that uses this layout
			inputLayout.GetAddressOf());			// Address of the resulting ID3D11InputLayout pointer
	}
}

// --------------------------------------------------------
// Creates the geometry we're going to draw
// --------------------------------------------------------
void Game::CreateMaterials()
{

}

// --------------------------------------------------------
// Creates the geometry we're going to draw
// --------------------------------------------------------
void Game::CreateGeometry()
{
	shapes[0] = make_shared<Mesh>("assets/cube.obj");
	shapes[1] = make_shared<Mesh>("assets/cylinder.obj");
	shapes[2] = make_shared<Mesh>("assets/helix.obj");
	shapes[3] = make_shared<Mesh>("assets/quad.obj");
	shapes[4] = make_shared<Mesh>("assets/quad_double_sided.obj");
	shapes[5] = make_shared<Mesh>("assets/sphere.obj");
	shapes[6] = make_shared<Mesh>("assets/torus.obj");

}

// --------------------------------------------------------
// Creates entities to put in window
// --------------------------------------------------------
void Game::CreateEntities() 
{
	entities[0] = make_shared<Entity>(shapes[0]);
	entities[1] = make_shared<Entity>(shapes[1]);
}


// --------------------------------------------------------
// Handle resizing to match the new window size
//  - Eventually, we'll want to update our 3D camera
// --------------------------------------------------------
void Game::OnResize()
{
	for (shared_ptr cam : cameras) 
	{
		cam->UpdateProjectionMatrix(Window::AspectRatio());
	}
}


// --------------------------------------------------------
// Update your game here - user input, move objects, AI, etc.
// --------------------------------------------------------
void Game::Update(float deltaTime, float totalTime)
{
	Game::ResetUI(deltaTime);

	camera->Update(deltaTime);

	// Example input checking: Quit if the escape key is pressed
	if (Input::KeyDown(VK_ESCAPE))
		Window::Quit();
}


// --------------------------------------------------------
// Clear the screen, redraw everything, present to the user
// --------------------------------------------------------
void Game::Draw(float deltaTime, float totalTime)
{
	// Frame START
	// - These things should happen ONCE PER FRAME
	// - At the beginning of Game::Draw() before drawing *anything*
	{

		// Clear the back buffer (erase what's on screen) and depth buffer
		Graphics::Context->ClearRenderTargetView(Graphics::BackBufferRTV.Get(),	background);
		Graphics::Context->ClearDepthStencilView(Graphics::DepthBufferDSV.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);
	}

	constBuffData.projection = camera->GetProjectionMatrix();
	constBuffData.view = camera->GetViewMatrix();

	// Draws each entity
	// - Binds constant buffer
	// - Collects world data for entity (pos, rot, scale)
	// - Maps/copies/unmaps data
	// - sets Vertex and Index buffers
	for (shared_ptr ent : entities) {

		constBuffData.world = ent->GetTransform()->GetWorldMatrix();
		
		D3D11_MAPPED_SUBRESOURCE mappedBuffer = {};
		Graphics::Context->Map(constBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedBuffer);
		memcpy(mappedBuffer.pData, &constBuffData, sizeof(constBuffData));
		Graphics::Context->Unmap(constBuffer.Get(), 0);

		ent->Draw();
	}

	ImGui::Render(); // Turns this frame’s UI into renderable triangles
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData()); // Draws it to the screen

	// Frame END
	// - These should happen exactly ONCE PER FRAME
	// - At the very end of the frame (after drawing *everything*)
	{
		// Present at the end of the frame
		bool vsync = Graphics::VsyncState();
		Graphics::SwapChain->Present(
			vsync ? 1 : 0,
			vsync ? 0 : DXGI_PRESENT_ALLOW_TEARING);

		// Re-bind back buffer and depth buffer after presenting
		Graphics::Context->OMSetRenderTargets(
			1,
			Graphics::BackBufferRTV.GetAddressOf(),
			Graphics::DepthBufferDSV.Get());
	}
}

// --------------------------------------------------------
// Refreshed UI every frame
// --------------------------------------------------------
void Game::ResetUI(float deltaTime) 
{
	// Feed fresh data to ImGui
	ImGuiIO & io = ImGui::GetIO();
	io.DeltaTime = deltaTime;
	io.DisplaySize.x = (float)Window::Width();
	io.DisplaySize.y = (float)Window::Height();

	// Reset the frame
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	// Determine new input capture
	Input::SetKeyboardCapture(io.WantCaptureKeyboard);
	Input::SetMouseCapture(io.WantCaptureMouse);

	//Show custom window
	Game::ShowUIWindow();
}

// --------------------------------------------------------
// Shows and builds custom UI
// --------------------------------------------------------
void Game::ShowUIWindow() {

	ImGui::Begin("Welcome!");

	// Outputs framerate to window
	ImGui::Text("Current Framerate: %f", ImGui::GetIO().Framerate);

	// Outputs resolution to window
	ImGui::Text("Window Resolution: %dx%d", Window::Width(), Window::Height());

	// Edit background color
	ImGui::ColorEdit4("Background Color Editor", background);

	GraphicChangeUI();

	// inverts colors **Broken as of now
	/*
	if (ImGui::Button("Invert Colors")) 
	{
		InvertColor();
	}
	*/

	ShowStats();

	TransformStats();

	CameraStats();

	ImGui::End(); // Ends the current window
}

// --------------------------------------------------------
// Creates Graphic Updating UI
// --------------------------------------------------------
void Game::GraphicChangeUI() {
	ImGui::ColorEdit4("Vertex Tint Editor", &constBuffData.colorTint.x);
	//ImGui::SliderFloat3("Vertex Position Editor", &constBuffData.offset.x, -1.0f, 1.0f);
}

// --------------------------------------------------------
// Inverts color of background, triangle and text
// --------------------------------------------------------
void Game::InvertColor() {
	
	background[0] = 1 - background[0];
	background[1] = 1 - background[1];
	background[2] = 1 - background[2];

	for (int i = 0; i < 3; i++) {
		color1[i] = 1 - color1[i];
		color2[i] = 1 - color2[i];
		color3[i] = 1 - color3[i];
		color4[i] = 1 - color4[i];
	}

	Game::CreateGeometry();
}

// --------------------------------------------------------
// Creates subsection for shape statistics
// --------------------------------------------------------
void Game::ShowStats() 
{
	if (ImGui::TreeNode("Meshes")) 
	{
		if (ImGui::TreeNode("Triangle"))
		{
			ImGui::Text("Triangles: %d", shapes[0]->GetIndexCount() / 3);
			ImGui::Text("Vertices: %d", shapes[0]->GetVertexCount());
			ImGui::Text("Indices: %d", shapes[0]->GetIndexCount());

			ImGui::TreePop();
		}

		if (ImGui::TreeNode("Rectangle"))
		{
			ImGui::Text("Triangles: %d", shapes[1]->GetIndexCount() / 3);
			ImGui::Text("Vertices: %d", shapes[1]->GetVertexCount());
			ImGui::Text("Indices: %d", shapes[1]->GetIndexCount());

			ImGui::TreePop();
		}

		if (ImGui::TreeNode("Star"))
		{
			ImGui::Text("Triangles: %d", shapes[2]->GetIndexCount() / 3);
			ImGui::Text("Vertices: %d", shapes[2]->GetVertexCount());
			ImGui::Text("Indices: %d", shapes[2]->GetIndexCount());

			ImGui::TreePop();
		}

		ImGui::TreePop();
	}
}

// --------------------------------------------------------
// Creates ability to transform entity using ImGui
// --------------------------------------------------------
void Game::TransformStats() 
{	
	int i = 0;
	if (ImGui::TreeNode("Entities")) 
	{
		for (auto& ent : entities) 
		{
			ImGui::PushID(i);

			XMFLOAT3& pos = ent->GetTransform()->GetPosition();
			XMFLOAT3& rot = ent->GetTransform()->GetRotation();
			XMFLOAT3& scale = ent->GetTransform()->GetScale();

			if (ImGui::TreeNode(("Entity " + std::to_string(i)).c_str()))
			{
				ImGui::DragFloat3("Position", &pos.x, 0.0f, -1.0f, 1.0f);
				ImGui::DragFloat3("Rotation", &rot.x, 0.0f, -1.0f, 1.0f);
				ImGui::DragFloat3("Scale", &scale.x, 0.0f, 0.0f, 2.0f);
				ImGui::Text("Mesh Index Count: %d", ent->GetMesh()->GetIndexCount());

				ImGui::TreePop();
			}

			ImGui::PopID();
			i++;
		}

		ImGui::TreePop();
	}
}

void Game::CameraStats() 
{
	if (ImGui::TreeNode("Cameras")) 
	{
		if (ImGui::TreeNode("Current Camera")) 
		{
			float fov = camera->GetFOV();
			float x = camera->GetTransform()->GetPosition().x;
			float y = camera->GetTransform()->GetPosition().y;
			float z = camera->GetTransform()->GetPosition().z;

			ImGui::Text("Camera number: %d", currentCam);
			ImGui::Text("Position: x: %.2f, y: %.2f, z: %.2f", x, y, z);
			ImGui::Text("Fov: %.2f", fov);
			ImGui::TreePop();
		}
		if (ImGui::Button("Camera 1")) 
		{
			camera = cameras[0];
			currentCam = 1;
		}
		if (ImGui::Button("Camera 2")) 
		{
			camera = cameras[1];
			currentCam = 2;
		}
		if (ImGui::Button("Camera 3")) 
		{
			camera = cameras[2];
			currentCam = 3;
		}
		ImGui::TreePop();
	}
}
