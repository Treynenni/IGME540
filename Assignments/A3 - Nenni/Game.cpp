#include "Game.h"
#include "Graphics.h"
#include "Vertex.h"
#include "Input.h"
#include "PathHelpers.h"
#include "Window.h"

#include <DirectXMath.h>
#include <string>

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

	showDemo = false;

	// Helper methods for loading shaders, creating some basic
	// geometry to draw and some simple camera matrices.
	//  - You'll be expanding and/or replacing these later
	LoadShaders();
	CreateGeometry();

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

		// Set the active vertex and pixel shaders
		//  - Once you start applying different shaders to different objects,
		//    these calls will need to happen multiple times per frame
		Graphics::Context->VSSetShader(vertexShader.Get(), 0, 0);
		Graphics::Context->PSSetShader(pixelShader.Get(), 0, 0);

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
		D3D11_INPUT_ELEMENT_DESC inputElements[2] = {};

		// Set up the first element - a position, which is 3 float values
		inputElements[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;				// Most formats are described as color channels; really it just means "Three 32-bit floats"
		inputElements[0].SemanticName = "POSITION";							// This is "POSITION" - needs to match the semantics in our vertex shader input!
		inputElements[0].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;	// How far into the vertex is this?  Assume it's after the previous element

		// Set up the second element - a color, which is 4 more float values
		inputElements[1].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;			// 4x 32-bit floats
		inputElements[1].SemanticName = "COLOR";							// Match our vertex shader input!
		inputElements[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;	// After the previous element

		// Create the input layout, verifying our description against actual shader code
		Graphics::Device->CreateInputLayout(
			inputElements,							// An array of descriptions
			2,										// How many elements in that array?
			vertexShaderBlob->GetBufferPointer(),	// Pointer to the code of a shader that uses this layout
			vertexShaderBlob->GetBufferSize(),		// Size of the shader code that uses this layout
			inputLayout.GetAddressOf());			// Address of the resulting ID3D11InputLayout pointer
	}
}


// --------------------------------------------------------
// Creates the geometry we're going to draw
// --------------------------------------------------------
void Game::CreateGeometry()
{
	Vertex triangleVertices[] =
	{
		{ XMFLOAT3(+0.0f, +0.5f, +0.0f), XMFLOAT4(color1)},
		{ XMFLOAT3(+0.5f, -0.5f, +0.0f), XMFLOAT4(color2)},
		{ XMFLOAT3(-0.5f, -0.5f, +0.0f), XMFLOAT4(color3)},
	};

	Vertex squareVertices[] = 
	{
		{ XMFLOAT3(+0.8f, +0.8f, +0.0f), XMFLOAT4(color1)},
		{ XMFLOAT3(+0.6f, +0.8f, +0.0f), XMFLOAT4(color2)},
		{ XMFLOAT3(+0.6f, +0.6f, +0.0f), XMFLOAT4(color3)},
		{ XMFLOAT3(+0.8f, +0.6f, +0.0f), XMFLOAT4(color4)},
	};

	Vertex polygonVertices[] = {
		{ XMFLOAT3(+0.0f, +0.5f, +0.0f), XMFLOAT4(color1)},
		{ XMFLOAT3(+0.5f, -0.5f, +0.0f), XMFLOAT4(color2)},
		{ XMFLOAT3(-0.5f, -0.5f, +0.0f), XMFLOAT4(color3)},
		{ XMFLOAT3(-0.5f, -0.5f, +0.0f), XMFLOAT4(color4)},
	};

	unsigned int triangleIndices[] = { 0, 1, 2 };

	unsigned int squareIndices[] = { 0, 1, 2, 3 };

	unsigned int polygonIndices[] = { 0, 1, 2, 3 };

	shapes[0] = std::make_shared<Mesh>(triangleIndices, triangleVertices);
	shapes[1] = std::make_shared<Mesh>(squareIndices, squareVertices);
	shapes[2] = std::make_shared<Mesh>(polygonIndices, polygonVertices);
}


// --------------------------------------------------------
// Handle resizing to match the new window size
//  - Eventually, we'll want to update our 3D camera
// --------------------------------------------------------
void Game::OnResize()
{
	
}


// --------------------------------------------------------
// Update your game here - user input, move objects, AI, etc.
// --------------------------------------------------------
void Game::Update(float deltaTime, float totalTime)
{
	Game::ResetUI(deltaTime);
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

	//PUT CODE TO DRAW MESHES
	///
	///
	/// 
	/// 
	/// 

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

	if (showDemo) 
	{
		// Show the demo window
		ImGui::ShowDemoWindow();
	}	
}

// --------------------------------------------------------
// Shows and builds custom UI
// --------------------------------------------------------
void Game::ShowUIWindow() {

	std::string intro = "Welcome! Type in the box and I will change!##";
	
	//Decides which text to show
	if (!str[0])
	{
		ImGui::Begin(&intro[0]);
	}
	else 
	{
		std::string name = "Wow! Your name is '" + str + "'. Nice name!##";
		ImGui::Begin(&name[0]); 
	}
	
	// Outputs framerate to window
	ImGui::Text("Current Framerate: %f", ImGui::GetIO().Framerate);

	// Outputs resolution to window
	ImGui::Text("Window Resolution: %dx%d", Window::Width(), Window::Height());

	// Edit background color
	ImGui::ColorEdit4("Background Color Editor", background);

	// Changes button based on if demo is hidden or not
	if (showDemo) {
		if (ImGui::Button("Hide ImGui Demo Window")) 
		{
			showDemo = !showDemo;
		}
	}
	else
	{
		if (ImGui::Button("Show ImGui Demo Window")) 
		{
			showDemo = !showDemo;
		}
	}

	// inverts colors
	if (ImGui::Button("Invert Colors")) 
	{
		InvertColor();
	}

	// input text, will add it to window name
	ImGui::Text("What's your name?");

	ImGui::InputText(" ", &input[0], 100);
	if (ImGui::Button("Submit Name")) 
	{
		str = "";
		str.insert(0, &input[0]);
		input = "";
	}

	ImGui::Text("Nested Table:");

	AddTable();

	ImGui::End(); // Ends the current window
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
	}

	//indexBuffer.ReleaseAndGetAddressOf();
	//vertexBuffer.ReleaseAndGetAddressOf();

	Game::CreateGeometry();
}

void Game::AddTable() {
	if (ImGui::TreeNode("Your List"))
	{
		if (ImGui::BeginTable("table_nested1", 2, ImGuiTableFlags_Borders | ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable))
		{
			ImGui::TableSetupColumn("Food");
			ImGui::TableSetupColumn("Non-Food");
			ImGui::TableHeadersRow();

			ImGui::TableNextColumn();
			ImGui::PushID(0);
			ImGui::InputText("", &a00[0], 100);
			ImGui::PopID();
			{
				float rows_height = 10 * 2;
				if (ImGui::BeginTable("table_nested2", 2, ImGuiTableFlags_Borders | ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable))
				{
					ImGui::TableSetupColumn("Refridgerated");
					ImGui::TableSetupColumn("Non-Refridgerated");
					ImGui::TableHeadersRow();

					ImGui::TableNextRow(ImGuiTableRowFlags_None, rows_height);
					
					ImGui::TableNextColumn();
					ImGui::PushID(1);
					ImGui::InputText("", &b00[0], 100);
					ImGui::PopID();

					ImGui::TableNextColumn();
					ImGui::PushID(2);
					ImGui::InputText("", &b10[0], 100);
					ImGui::PopID();

					ImGui::TableNextRow(ImGuiTableRowFlags_None, rows_height);
					
					ImGui::TableNextColumn();
					ImGui::PushID(3);
					ImGui::InputText("", &b01[0], 100);
					ImGui::PopID();

					ImGui::TableNextColumn();
					ImGui::PushID(4);
					ImGui::InputText("", &b11[0], 100);
					ImGui::PopID();

					ImGui::EndTable();
				}
			}
			ImGui::TableNextColumn();

			ImGui::PushID(5);
			ImGui::InputText("", &a10[0], 100);
			ImGui::PopID();

			ImGui::TableNextColumn();
			ImGui::PushID(6);
			ImGui::InputText("", &a01[0], 100);
			ImGui::PopID();

			ImGui::TableNextColumn(); 
			ImGui::PushID(7);
			ImGui::InputText("", &a11[0], 100);
			ImGui::PopID();

			ImGui::EndTable();
		}
		ImGui::TreePop();
	}
}
