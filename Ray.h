#pragma once
#include "EMath.h"
#include "ERGBColor.h"

class Ray final
{
public:
	Ray(uint32_t width, uint32_t height);
	Ray();
	~Ray() = default;
	Elite::FPoint4 ScreenToWorld(uint32_t r, uint32_t c, float fov) const;
	//getters
	Elite::FVector3 GetDirection() const;
	Elite::FPoint3 GetOrigin() const;
	float GetTMax()const;
	float GetTMin()const;
	//setters
	void SetDirection(const Elite::FVector3& newDir);
	void SetOrigin(const Elite::FPoint3& newOrigin);
	void SetTMax(float newTMax);
	void SetTMin(float newTMin);
private:
	Elite::FPoint3 m_Oray;
	Elite::FVector3 m_Direction;
	float m_TMin = 0.0001f;
	float m_TMax = FLT_MAX;
	uint32_t m_Width;
	uint32_t m_Height;
};