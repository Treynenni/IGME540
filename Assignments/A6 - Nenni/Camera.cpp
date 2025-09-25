#include "Camera.h"

Camera::Camera(float aspectRatio, XMFLOAT3 initPosition, float inFov, float inMoveSpeed, float inMouseSpeed)
{
	transform = Transformation();
	transform.SetPosition(initPosition);

	fov = inFov;
	moveSpeed = inMoveSpeed;
	mouseSpeed = inMouseSpeed;

	UpdateViewMatrix();
	UpdateProjectionMatrix(aspectRatio);
}

XMFLOAT4X4 Camera::GetViewMatrix()
{
	return viewMatrix;
}

XMFLOAT4X4 Camera::GetProjectionMatrix()
{
	return projectionMatrix;
}

void Camera::UpdateProjectionMatrix(float aspectRatio)
{
	XMStoreFloat4x4(&projectionMatrix, XMMatrixPerspectiveFovLH(fov, aspectRatio, 0.01f, 100.0f));
}

void Camera::UpdateViewMatrix()
{
	XMVECTOR position = XMVECTOR(transform.GetPosition().x, transform.GetPosition().y, transform.GetPosition().z);
	XMVECTOR direction = XMVECTOR(transform.GetForward().x, transform.GetForward().y, transform.GetForward().z);
	XMVECTOR up = XMVECTOR(transform.GetUp().x, transform.GetUp().y, transform.GetUp().z);

	XMStoreFloat4x4(&viewMatrix, XMMatrixLookToLH(position, direction, up));
}

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
		int cursorMovementX = Input::GetMouseXDelta();
		int cursorMovementY = Input::GetMouseYDelta();

	}

	UpdateViewMatrix();
}
