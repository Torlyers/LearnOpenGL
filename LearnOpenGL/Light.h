#pragma once
#include"glm/glm.hpp"

using namespace glm;


enum LightType
{
	None,
	Parallel,
	Point,
	Spot
};

class Light
{

public:
	Light();
	Light(const vec3 & i_Color, const vec3 & i_Position);
	virtual	~Light();

	inline vec3  GetColor() { return m_Color; }
	inline void  SetColor(const vec3 & i_Color) { m_Color = i_Color; }

	inline vec3  GetPosition() { return m_Position; }
	inline void  SetPosition(const vec3 & i_Position) { m_Position = i_Position; }

	inline float GetStrength() { return m_Strength; }
	inline void  SetStrength(float i_Strength) { m_Strength = i_Strength; }

protected:
	LightType m_Type;

	vec3      m_Color;
	vec3      m_Position;

	float	  m_Strength;
};

class ParallelLight : public Light
{
public:
	ParallelLight(const vec3 & i_Color, const vec3 & i_Position, const vec3 & i_Direction);
	~ParallelLight();

	inline vec3 GetDirection() { return m_Direction; }
	inline void SetDirection(const vec3 & i_Direction) { m_Direction = i_Direction; }
protected:
	vec3 m_Direction;
};

class PointLight : public Light
{
public:
	PointLight(const vec3 & i_Color, const vec3 & i_Position, float i_Range);
	~PointLight();

	inline float GetRange() { return m_Range; }
	inline void  SetRange(float i_Range) { m_Range = i_Range; }

	void CalAttParameters(float & o_Constant, float & o_Linear, float & o_Quadratic);

protected:
	float m_Range;
};

class SpotLight : public PointLight
{
public:
	SpotLight(const vec3 & i_Color, const vec3 & i_Position, const vec3 & i_Direction, float i_Range, float i_BiasAngle);
	~SpotLight();

	inline float GetBiasAngle() { return m_BiasAngle; }
	inline void  SetBiasAngle(float i_BiasAngle) { m_BiasAngle = i_BiasAngle; }

	inline vec3 GetDirection() { return m_Direction; }
	inline void SetDirection(const vec3 & i_Direction) { m_Direction = i_Direction; }

private:
	float m_BiasAngle;
	vec3  m_Direction;
};