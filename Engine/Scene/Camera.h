#pragma once


enum class CameraMovement {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT,
	UP,
	DOWN
};
// Default camera values
const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 10.0f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 45.0f;
const int WIDTH = 800;
const int HEIGHT = 600;


// An abstract camera class that processes input and calculates the corresponding Euler Angles, Vectors and Matrices for use in OpenGL
class Camera
{
public:
	struct CameraViewport
	{
		float Left;
		float Top;
		float Width;
		float Height;
	};
	
	// camera Attributes
	glm::vec3 Position;
	glm::vec3 Front;
	glm::vec3 Up;
	glm::vec3 Right;
	glm::vec3 WorldUp;
	// euler Angles
	double Yaw;
	double Pitch;
	// camera options
	float MovementSpeed;
	float MouseSensitivity;
	float Zoom;
	CameraViewport Viewport;
	
	float m_NearPlane = 0.1f;
	float m_FarPlane = 50.0f;
	

	// constructor with vectors
	Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH);
	// constructor with scalar values
	Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch);

	~Camera() = default;

	// returns the view matrix calculated using Euler Angles and the LookAt Matrix
	glm::mat4 GetViewMatrix() const;
	glm::mat4 GetProjectionMatrix() const;
	// processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
	void ProcessKeyboard(CameraMovement direction, const double& deltaTime);

	// processes input received from a mouse input system. Expects the offset value in both the x and y direction.
	void ProcessMouseMovement(const double& xoffset, const double& yoffset, const bool& constrainPitch = true);

	// processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
	void ProcessMouseScroll(const double& offset);

	void UpdateCameraWindow(int& width, int& height);
  

	inline float GetFarPlane() { return m_FarPlane; }
	inline void SetFarPlane(const float& farPlane) { m_FarPlane = farPlane; }
	
	inline float GetNearPlane() { return m_NearPlane; }
	inline void SetNearPlane(const float& nearPlane) { m_NearPlane = nearPlane; }
	// update camera view + projection matrix
	void UpdateMatrix(Shader* shader);
private:
	// calculates the front vector from the Camera's (updated) Euler Angles
	void UpdateCameraVectors();
};