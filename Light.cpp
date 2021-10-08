#include "Light.h"
#include <iostream>

Light::Light(const Elite::FVector3& position, const Elite::RGBColor& color, float intensity, LightType lightType)
	:m_Position{ position }
	, m_Color{ color }
	, m_Intensity{ intensity }
	, m_LightType{ lightType }
{
}

Elite::RGBColor Light::GetIrradiance(const HitRecord& hitRecord) const
{
	Elite::RGBColor irradiance;
	if (m_LightType == LightType::pointLight)
	{
		irradiance = m_Color * (m_Intensity / pow((Elite::Magnitude(m_Position - Elite::FVector3(hitRecord.hitPoint))), 2));
	}
	else
	{
		irradiance = m_Color * m_Intensity;
	}
	return irradiance;
}

const Elite::FVector3 Light::GetDirection(const Elite::FPoint3& hitPoint) const
{
	return (m_Position - Elite::FVector3(hitPoint));
}