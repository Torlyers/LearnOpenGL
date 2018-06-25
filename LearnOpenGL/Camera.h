#pragma once

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

using namespace glm;

const vec3  WORLD_UP      = vec3(0.0f, 1.0f, 0.0f);
const vec3  DEFAULT_EULAR = vec3(0.0f, 0.0f, 0.0f);
const float DEFAULT_ZOOM  = 45.0f;
const float DEFAULT_SPEED = 0.1f;
const float DEFAULT_SENTIVITY = 0.1f;

enum KeyCode
{
	W,
	S,
	A,
	D
};

class Camera
{
public:

	Camera();
	Camera(const vec3 & i_Position, const vec3 & i_EularAngles);
	~Camera();

	mat4   GetViewMatrix();
	
	float  GetZoom() { return m_Zoom; }

	inline void SetPosition(const vec3 & i_Position)       { m_Position = i_Position; }
	inline void SetEularAngles(const vec3 & i_EularAngles) { m_EularAngles = i_EularAngles; }
	inline void SetZoom(float i_Zoom)                      { m_Zoom = i_Zoom; }

	void ProcessMouseInput(float offsetX, float offsetY);
	void ProcessScrollInput(float offsetY);
	void ProcessKeyInput(KeyCode code, double dt);

private:

	vec3 m_Position;         //x, y, z
	vec3 m_EularAngles;      //yaw, pitch, roll

	vec3 m_Up;
	vec3 m_Right;
	vec3 m_Forward;

	float m_Zoom;
	float m_Speed;
	float m_Sensitivity;

	void UpdateCameraVectors();
};

