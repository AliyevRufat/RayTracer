#include "Ray.h"
#include <iostream>
#include "Camera.h"

Ray::Ray()
	:Ray{ 0,0 }
{
}

Ray::Ray(Elite::FPoint3 p1, Elite::FPoint3 p2)
	: m_Oray(p1)
	, m_P2{p2}
{
	m_Direction = m_P2 - m_Oray;
}

Ray::Ray(uint32_t width, uint32_t height)
	: m_Direction{ 0.f,0.f,-1.f }
	, m_Width{ width }
	, m_Height{ height }
	, m_Oray{ 0,0,0 }
{
}

Elite::FPoint4 Ray::ScreenToWorld(uint32_t r, uint32_t c, float fov) const
{
	Elite::FPoint4 pos{};
	
	pos.x = float((2 * ((c + .5) / m_Width) - 1)) * (float(m_Width) / float(m_Height)) * fov;
	pos.y = float((1 - 2 * ((r + .5) / m_Height))) * fov;
	pos.z = -1.f;
	pos.w = 1.f;

	return  pos;
}

Elite::FVector3 Ray::GetDirection() const
{
	//m_Direction = Elite::GetNormalized(m_P2 - m_Oray);
	return m_Direction;
}

Elite::FPoint3 Ray::GetOrigin() const
{
	return m_Oray;
}

float Ray::GetTMax() const
{
	return m_TMax;
}

float Ray::GetTMin() const
{
	return m_TMin;
}

void Ray::SetDirection(const Elite::FVector3& newDir)
{
	m_Direction = newDir;
}

void Ray::SetOrigin(const Elite::FPoint3& newOrigin)
{
	m_Oray = newOrigin;
}

void Ray::SetTMax(float newTMax)
{
	m_TMax = newTMax;
}

void Ray::SetTMin(float newTMin)
{
	m_TMin = newTMin;
}