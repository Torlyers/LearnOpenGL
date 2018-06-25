#include "Camera.h"



Camera::Camera():
	m_EularAngles(DEFAULT_EULAR),
	m_Zoom(DEFAULT_ZOOM),
	m_Speed(DEFAULT_SPEED),
	m_Sensitivity(DEFAULT_SENTIVITY)
{
}

Camera::Camera(const glm::vec3 & i_Position, const glm::vec3 & i_EularAngles):
    m_Position(i_Position),
    m_EularAngles(i_EularAngles),
	m_Zoom(DEFAULT_ZOOM),
	m_Speed(DEFAULT_SPEED),
	m_Sensitivity(DEFAULT_SENTIVITY)
{
}


Camera::~Camera()
{
}

mat4 Camera::GetViewMatrix()
{
	return lookAt(m_Position, m_Position + m_Forward, m_Up);
}

void Camera::ProcessMouseInput(float offsetX, float offsetY)
{
	m_EularAngles.x -= offsetX * m_Sensitivity;
	m_EularAngles.y -= offsetY * m_Sensitivity;

	if (m_EularAngles.y >= 90.0f)
		m_EularAngles.y = 89.9f;
	else if (m_EularAngles.y <= -90.0f)
		m_EularAngles.y = -89.9f;

	UpdateCameraVectors();
}

void Camera::ProcessScrollInput(float offsetY)
{
	m_Zoom -= offsetY;
}

void Camera::ProcessKeyInput(KeyCode code, double dt)
{
	switch (code)
	{
	case W:
		m_Position += m_Forward * m_Speed;
		break;
	case S:
		m_Position -= m_Forward * m_Speed;
		break;
	case A:
		m_Position += m_Right * m_Speed;
		break;
	case D:
		m_Position -= m_Right * m_Speed;
		break;
	default:
		break;
	}

	UpdateCameraVectors();
}

void Camera::UpdateCameraVectors()
{	
	m_Forward.x = cos(radians(m_EularAngles.y)) * cos(radians(m_EularAngles.x));
	m_Forward.y = sin(radians(m_EularAngles.y));
	m_Forward.z = cos(radians(m_EularAngles.y)) * sin(radians(m_EularAngles.x));

	m_Forward = normalize(m_Forward);
	m_Right   = normalize(cross(m_Forward, WORLD_UP));
	m_Up      = normalize(cross(m_Forward, m_Right));	
}
