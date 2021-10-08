#include "Material.h"
#include "BRDF.h"
Material_Lambert::Material_Lambert(float diffuseReflectance, const  Elite::RGBColor& diffuseColor)
{
	m_DiffuseColor = { diffuseColor };
	m_DiffuseReflectance = { diffuseReflectance };
}

Elite::RGBColor Material_Lambert::Shade(const HitRecord& hitRecord, const Elite::FVector3& l, const Elite::FVector3& v) const
{
	return BRDF::Lambert(m_DiffuseReflectance, m_DiffuseColor);
}

Material_LambertPhong::Material_LambertPhong(float diffuseReflectance, const Elite::RGBColor& diffuseColor, float specularReflectance, float phongExponent)
	: m_PhongExponent{ phongExponent }
	, m_SpecularReflectance{ specularReflectance }
{
	m_DiffuseColor = diffuseColor;
	m_DiffuseReflectance = diffuseReflectance;
}

Elite::RGBColor Material_LambertPhong::Shade(const HitRecord& hitRecord, const Elite::FVector3& l, const Elite::FVector3& v) const
{
	return BRDF::Lambert(m_DiffuseReflectance, m_DiffuseColor) +
		BRDF::Phong(m_SpecularReflectance, m_PhongExponent, l, v, hitRecord.normal);
}

Material_CookTorrance::Material_CookTorrance(float diffuseReflectance, const Elite::RGBColor& diffuseColor, bool isMetal, const Elite::RGBColor& F0, float roughness)
	:m_Roughness{ roughness }
{
	m_F0 = (isMetal) ? F0 : (Elite::RGBColor(0.04f, 0.04f, 0.04f));

	m_IsMetal = isMetal;
	m_DiffuseColor = diffuseColor;
	m_DiffuseReflectance = diffuseReflectance;
}

Elite::RGBColor Material_CookTorrance::Shade(const HitRecord& hitRecord, const Elite::FVector3& l, const Elite::FVector3& v) const
{
	Elite::FVector3 halfVector = (v + l) / (Elite::Magnitude(v + l));
	Elite::RGBColor F = m_F0 + (Elite::RGBColor(1, 1, 1) - m_F0) * (pow(1 - (Elite::Dot(halfVector, v)), 5));

	Elite::RGBColor kd = (m_IsMetal) ? Elite::RGBColor(0, 0, 0) : (Elite::RGBColor(1, 1, 1) - F);

	return (kd * BRDF::Lambert(m_DiffuseReflectance, m_DiffuseColor) + BRDF::CookTorrance(F, l, v, hitRecord.normal, m_Roughness));
}