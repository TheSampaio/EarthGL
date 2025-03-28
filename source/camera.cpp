#include "pch.hpp"
#include "camera.hpp"

#include "input.hpp"
#include "renderer.hpp"
#include "window.hpp"

Camera::Camera(glm::vec3 position, float fieldOfView, float minClipDistance, float maxClipDistance)
	: mbFirstClick(true)
{
	mClipDistance = { minClipDistance, maxClipDistance };
	mSensitivity = 100.0f;
	mSpeed = 0.0f;
	mFieldOfView = fieldOfView;

	mPosition = position;
	mRotation = glm::vec3{ 0.0f, 0.0f, -1.0f };

	mView = glm::mat4{ 1.0f };
	mProjection = glm::mat4{ 1.0f };
}

void Camera::Update(float deltatime)
{
	// Forward and backward
	if (Input::GetKeyPress(W))
		mPosition += mSpeed * mRotation * deltatime;

	else if (Input::GetKeyPress(A))
		mPosition += mSpeed * -glm::normalize(glm::cross(mRotation, mUp)) * deltatime;

	// Right and left
	if (Input::GetKeyPress(D))
		mPosition += mSpeed * glm::normalize(glm::cross(mRotation, mUp)) * deltatime;

	else if (Input::GetKeyPress(S))
		mPosition += mSpeed * -mRotation * deltatime;

	// Up and down
	if (Input::GetKeyPress(E))
		mPosition += mSpeed * mUp * deltatime;

	else if (Input::GetKeyPress(Q))
		mPosition += mSpeed * -mUp * deltatime;

	// Max speed
	if (Input::GetKeyPress(ShiftLeft))
		mSpeed = 300.0f;

	// Normal speed
	else if (Input::GetKeyRelease(ShiftLeft))
		mSpeed = 150.0f;

	// Min speed
	if (Input::GetKeyPress(AltLeft))
		mSpeed = 75.0f;

	if (glfwGetMouseButton(Window::GetId(), GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
	{
		// Hides the mouse's cursor
		glfwSetInputMode(Window::GetId(), GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

		// Prevents camera from jumping on the first click
		if (mbFirstClick)
		{
			glfwSetCursorPos(Window::GetId(), static_cast<double>(Window::GetSize()[0] / 2), static_cast<double>(Window::GetSize()[1] / 2));
			mbFirstClick = false;
		}

		// Stores the coordinates of the cursor
		double mouseX = 0.0;
		double mouseY = 0.0;

		// Fetches the coordinates of the cursor
		glfwGetCursorPos(Window::GetId(), &mouseX, &mouseY);

		// Calculates camera's pitch and yaw
		float pitch = mSensitivity * static_cast<float>(mouseY - static_cast<double>(Window::GetSize()[1] / 2)) / Window::GetSize()[1];
		float yaw =   mSensitivity * static_cast<float>(mouseX - static_cast<double>(Window::GetSize()[0] / 2)) / Window::GetSize()[0];

		// Calculates the new camera's orientaion (Bug fix)
		glm::vec3 newOrientation = glm::rotate(mRotation, glm::radians(-pitch), glm::normalize(glm::cross(mRotation, mUp)));

		if (abs(glm::angle(newOrientation, mUp) - glm::radians(90.0f)) <= glm::radians(85.0f))
			mRotation = newOrientation;

		// Rotates the camera left and right
		mRotation = glm::rotate(mRotation, glm::radians(-yaw), mUp);

		// Sets mouse cursor to the middle of the screen
		glfwSetCursorPos(Window::GetId(), static_cast<double>(Window::GetSize()[0] / 2), static_cast<double>(Window::GetSize()[1] / 2));
	}

	// Shows the mouse's cursor
	else if (glfwGetMouseButton(Window::GetId(), GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE)
	{
		glfwSetInputMode(Window::GetId(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		mbFirstClick = true;
	}
}

void Camera::Draw()
{
	// Calculates the view projection matrix
	mView = glm::lookAt(mPosition, mPosition + mRotation, mUp);
	mProjection = glm::perspective(glm::radians(mFieldOfView), static_cast<float>(Window::GetSize()[0]) / static_cast<float>(Window::GetSize()[1]), mClipDistance[0], mClipDistance[1]);

	Renderer::UseShaderProgram();

	// Sends the VP matrix to the GPU
	Renderer::SetUniformMatrix4fv("uView", mView);
	Renderer::SetUniformMatrix4fv("uProjection", mProjection);
	Renderer::SetUniform3f("uViewPosition", mPosition.x, mPosition.y, mPosition.z);
}