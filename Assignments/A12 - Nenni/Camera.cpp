#include "Camera.h"

// ---------------------------------- Constructor ----------------------------------
Camera::Camera(float aspectRatio, XMFLOAT3 initPosition, float inFov, float inMoveSpeed, float inMouseSpeed) : transform(), fov(inFov), moveSpeed(inMoveSpeed), mouseSpeed(inMouseSpeed)
{
	transform.SetPosition(initPosition);

	//Set up projection and view matrices
	UpdateViewMatrix();
	UpdateProjectionMatrix(aspectRatio);
}

// ---------------------------------- Getter Methods ----------------------------------
// Gets view matrix
XMFLOAT4X4 Camera::GetViewMatrix()
{
	return viewMatrix;
}

// Gets projection matrix
XMFLOAT4X4 Camera::GetProjectionMatrix()
{
	return projectionMatrix;
}

float Camera::GetFOV()
{
	return fov;
}

shared_ptr<Transformation> Camera::GetTransform()
{
	return make_shared<Transformation>(transform);
}

// ---------------------------------- Update Methods ----------------------------------
/// <summary>
/// Updates the projection matrix if aspect ratio is changed
/// </summary>
/// <param name="aspectRatio">Program's current aspect ratio</param>
void Camera::UpdateProjectionMatrix(float aspectRatio)
{
	XMStoreFloat4x4(&projectionMatrix, XMMatrixPerspectiveFovLH(fov, aspectRatio, 0.01f, 100.0f));
}

/// <summary>
/// Updates view matrix every frame, also based on player movement
/// </summary>
void Camera::UpdateViewMatrix()
{
	XMVECTOR position = XMLoadFloat3(&transform.GetPosition());
	XMVECTOR direction = XMLoadFloat3(&transform.GetForward());
	XMVECTOR up = XMLoadFloat3(&transform.GetUp());

	XMStoreFloat4x4(&viewMatrix, XMMatrixLookToLH(position, direction, up));
}

/// <summary>
/// Allows player input, updates view matrix
/// </summary>
/// <param name="dt">Delta time</param>
void Camera::Update(float dt)
{
	if (Input::KeyDown('W')) 
	{
		transform.MoveRelative(0, 0, moveSpeed * dt);
	}

	if (Input::KeyDown('S')) 
	{
		transform.MoveRelative(0, 0, -moveSpeed * dt);
	}

	if (Input::KeyDown('A')) 
	{
		transform.MoveRelative(-moveSpeed * dt, 0, 0);
	}

	if (Input::KeyDown('D')) 
	{
		transform.MoveRelative(moveSpeed * dt, 0, 0);
	}

	if (Input::KeyDown(VK_SPACE)) 
	{
		transform.MoveRelative(0, moveSpeed * dt, 0);
	}

	if (Input::KeyDown('X')) 
	{
		transform.MoveRelative(0, -moveSpeed * dt, 0);
	}

	if (Input::MouseLeftDown()) 
	{
		int cursorMovementX = Input::GetMouseXDelta();
		int cursorMovementY = Input::GetMouseYDelta();

		transform.Rotation(cursorMovementY * mouseSpeed, cursorMovementX * mouseSpeed, 0);
		
		if (transform.GetRotation().x >= (0.5 * XM_PI) - 0.01f) 
		{
			transform.SetRotation(0.5 * XM_PI, transform.GetRotation().y, transform.GetRotation().z);
		} 
		else if(transform.GetRotation().x <= (-0.5 * XM_PI) + 0.01f)
		{
			transform.SetRotation(-0.5 * XM_PI, transform.GetRotation().y, transform.GetRotation().z);
		}
		
	}

	UpdateViewMatrix();
}
