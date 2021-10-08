#pragma once
#include "EMath.h"
#include "ERGBColor.h"
#include "Shape.h"
#include "Ray.h"

class Light final
{
public:

	enum class LightType
	{
		pointLight,
		directionalLight
	};

	Light(const Elite::FVector3& position, const Elite::RGBColor& color, float intensity, LightType lightType);

	~Light() = default;

	Light(const Light&) = delete;
	Light(Light&&) noexcept = delete;
	Light& operator=(const Light&) = delete;
	Light& operator=(Light&&) noexcept = delete;
	Elite::RGBColor GetIrradiance(const HitRecord& hitRecord) const;
	const Elite::FVector3 GetDirection(const Elite::FPoint3& hitPoint) const;

private:
	LightType m_LightType;
	Elite::FVector3 m_Position;
	Elite::RGBColor m_Color;
	float m_Intensity;
};
