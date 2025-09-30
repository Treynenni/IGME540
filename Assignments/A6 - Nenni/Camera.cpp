#include "Camera.h"

#define _USE_MATH_DEFINES

#include <math.h>

// ---------------------------------- Constructor ----------------------------------
Camera::Camera(float aspectRatio, XMFLOAT3 initPosition, float inFov, float inMoveSpeed, float inMouseSpeed)
{
	transform = Transformation();
	transform.SetPosition(initPosition);

	fov = inFov;
	moveSpeed = inMoveSpeed;
	mouseSpeed = inMouseSpeed;

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
	XMVECTOR position = XMVECTOR{ transform.GetPosition().x, transform.GetPosition().y, transform.GetPosition().z };
	XMVECTOR direction = XMVECTOR{ transform.GetForward().x, transform.GetForward().y, transform.GetForward().z };
	XMVECTOR up = XMVECTOR{ transform.GetUp().x, transform.GetUp().y, transform.GetUp().z };

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
		transform.MoveRelative(XMFLOAT3((transform.GetForward().x * moveSpeed), (transform.GetForward().y * moveSpeed), (transform.GetForward().z * moveSpeed)));
	}

	if (Input::KeyDown('S')) 
	{
		transform.MoveRelative(XMFLOAT3((transform.GetForward().x * -moveSpeed), (transform.GetForward().y * -moveSpeed), (transform.GetForward().z * -moveSpeed)));
	}

	if (Input::KeyDown('A')) 
	{
		transform.MoveRelative(XMFLOAT3((transform.GetRight().x * -moveSpeed), (transform.GetRight().y * -moveSpeed), (transform.GetRight().z * -moveSpeed)));
	}

	if (Input::KeyDown('D')) 
	{
		transform.MoveRelative(XMFLOAT3((transform.GetRight().x * moveSpeed), (transform.GetRight().y * moveSpeed), (transform.GetRight().z * moveSpeed)));
	}

	if (Input::KeyDown(VK_SPACE)) 
	{
		transform.MoveAbsolute(XMFLOAT3((transform.GetUp().x * moveSpeed), (transform.GetUp().y * moveSpeed), (transform.GetUp().z * moveSpeed)));
	}

	if (Input::KeyDown('X')) 
	{
		transform.MoveAbsolute(XMFLOAT3((transform.GetUp().x * -moveSpeed), (transform.GetUp().y * -moveSpeed), (transform.GetUp().z * -moveSpeed)));
	}

	if (Input::MouseLeftDown()) 
	{
		int cursorMovementX = Input::GetMouseXDelta() * mouseSpeed;
		int cursorMovementY = Input::GetMouseYDelta() * mouseSpeed;

		/*
		transform.Rotation(cursorMovementY, cursorMovementX, 0);

		if (transform.GetRotation().x >= (0.5 * M_PI)) 
		{
			transform.SetRotation(0.5 * M_PI, transform.GetRotation().y, transform.GetRotation().z);
		} 
		else if(transform.GetRotation().x <= (-0.5 * M_PI))
		{
			transform.SetRotation(-0.5 * M_PI, transform.GetRotation().y, transform.GetRotation().z);
		}
		*/
	}

	UpdateViewMatrix();
}
