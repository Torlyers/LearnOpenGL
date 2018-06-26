#include "Light.h"

Light::Light()
{
}

Light::Light(const vec3 & i_Color, const vec3 & i_Position):
	m_Position(i_Position),
	m_Color(i_Color)
{
}


Light::~Light()
{
}

ParallelLight::ParallelLight(const vec3 & i_Color, const vec3 & i_Position, const vec3 & i_Direction):
	Light(i_Color, i_Position),
	m_Direction(i_Direction)
{
	m_Type = Parallel;
}

ParallelLight::~ParallelLight()
{
}

PointLight::PointLight(const vec3 & i_Color, const vec3 & i_Position, float i_Range):
	Light(i_Color, i_Position),
	m_Range(i_Range)
{
	m_Type = Point;
}

PointLight::~PointLight()
{
}

SpotLight::SpotLight(const vec3 & i_Color, const vec3 & i_Position, const vec3 & i_Direction, float i_BiasAngle):
	ParallelLight(i_Color, i_Position, i_Direction),
	m_BiasAngle(i_BiasAngle)
{
	m_Type = Spot;
}

SpotLight::~SpotLight()
{
}
