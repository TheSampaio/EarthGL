#ifndef _EARTHGL_CAMERA_
#define _EARTHGL_CAMERA_

class Camera
{
public:
	Camera(glm::vec3 position, float fieldOfView = 60.0f, float minClipDistance = 0.05f, float maxClipDistance = 100.0f);

	// Process all camera's event
	void Update(float deltatime);
	void Draw();

	// Returns the camera's view matrix
	glm::mat4 GetViewMatrix() const { return mView; }

	// Returns the camera's projection matrix
	glm::mat4 GetProjectionMatrix() const { return mProjection; }

	glm::vec3 GetPosition() const { return mPosition; }
	glm::vec3 GetRotation() const { return mRotation; }
	glm::vec3 GetVectorUp() const { return mUp; }

	array<float, 2> GetClipDistance() const { return mClipDistance; }

	float GetFieldOfView() const { return mFieldOfView; }

private:
	// Attributes
	glm::mat4 mView;
	glm::mat4 mProjection;
	glm::vec3 mPosition;
	glm::vec3 mRotation;
    
	const glm::vec3 mUp = glm::vec3( 0.0f, 1.0f, 0.0f );
	array<float, 2> mClipDistance;

	bool mbFirstClick;
	float mSensitivity;
	float mSpeed;
	float mFieldOfView;
};

#endif
