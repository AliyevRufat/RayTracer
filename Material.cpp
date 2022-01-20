#include "Material.h"
#include "BRDF.h"
#include "Camera.h"
#include "Scenegraph.h"
#include "Light.h"
#include "LightManager.h"
#include "ERenderer.h"

Material_Lambert::Material_Lambert(float diffuseReflectance, const  Elite::RGBColor& diffuseColor)
{
	m_DiffuseColor = { diffuseColor };
	m_DiffuseReflectance = { diffuseReflectance };
}

Elite::RGBColor Material_Lambert::Shade(const HitRecord& hitRecord, const Elite::FVector3& l, const Elite::FVector3& v, bool& isShade,const Elite::FVector3& incidentRay) const
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

Elite::RGBColor Material_LambertPhong::Shade(const HitRecord& hitRecord, const Elite::FVector3& l, const Elite::FVector3& v, bool& isShade, const Elite::FVector3& incidentRay) const
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

Elite::RGBColor Material_CookTorrance::Shade(const HitRecord& hitRecord, const Elite::FVector3& l, const Elite::FVector3& v, bool& isShade, const Elite::FVector3& incidentRay) const
{
	Elite::FVector3 halfVector = (v + l) / (Elite::Magnitude(v + l));
	Elite::RGBColor F = m_F0 + (Elite::RGBColor(1, 1, 1) - m_F0) * (pow(1 - (Elite::Dot(halfVector, v)), 5));

	Elite::RGBColor kd = (m_IsMetal) ? Elite::RGBColor(0, 0, 0) : (Elite::RGBColor(1, 1, 1) - F);

	return (kd * BRDF::Lambert(m_DiffuseReflectance, m_DiffuseColor) + BRDF::CookTorrance(F, l, v, hitRecord.normal, m_Roughness));
}

Material_Refractive::Material_Refractive(const Elite::RGBColor& diffuseColor)
{
	m_DiffuseColor = diffuseColor;
}

Elite::RGBColor Material_Refractive::Shade(const HitRecord& hitRecord, const Elite::FVector3& l, const Elite::FVector3& v, bool& isShade, const Elite::FVector3& incidentRay) const
{
	Elite::RGBColor trnColor = {0,0,0};
	
	//
	Elite::FVector3 p = incidentRay;
	auto tempNormal = hitRecord.normal;
	double r = 1.0 / m_IndexOfRef;
	double c = Elite::Dot(tempNormal, p);

	if (c < 0.0)
	{
		c = -Elite::Dot(tempNormal, p);
	}

	Elite::FVector3 refractedVector = r * p + (r * c - sqrtf(1.0 - pow(r, 2.0) * (1.0 - pow(c, 2.0)))) * tempNormal;

	//the refracted ray
	Ray refractedRay(hitRecord.hitPoint + (refractedVector * 0.01), hitRecord.hitPoint + refractedVector);

	//second intersection test
	Shape* closestObject = nullptr;
	HitRecord closestHitRecord;
	HitRecord newHitRecord;
	bool testIntersection = m_pShape->HitRefractive(refractedRay, newHitRecord);
	bool intersectionFound = false;
	Ray finalRay;

	if (testIntersection)
	{
		Elite::FVector3 p2 = refractedRay.GetDirection();
		p2 = Elite::GetNormalized(p2);
		auto tempNormal2 = newHitRecord.normal;
		double r2 = 1.0;
		double c2 = Elite::Dot(tempNormal2, p2);
		if (c2 < 0.0)
		{
			c2 = -Elite::Dot(tempNormal2, p2);
		}
		Elite::FVector3 refractedVector2 = r2 * p2 + (r2 * c2 - sqrtf(1.0 - pow(r2, 2.0) * (1.0 - pow(c2, 2.0)))) * tempNormal2;

		//refracter ray
		Ray refractedRay2(newHitRecord.hitPoint + (refractedVector2 * 0.01), newHitRecord.hitPoint + refractedVector2);

		intersectionFound = HitObjects(refractedRay2, m_pShape, closestObject, closestHitRecord);
		finalRay = refractedRay2;
	}
	else
	{
		intersectionFound = HitObjects(refractedRay, m_pShape, closestObject, closestHitRecord);
		finalRay = refractedRay;
	}


	Elite::RGBColor matColor = { 0,0,0 };

	if (intersectionFound)
	{
		if (closestObject->GetMaterial())
		{
			for (size_t i = 0; i < LightManager::GetInstance()->GetLights().size(); i++)
			{
				auto pLight = LightManager::GetInstance()->GetLights()[i];
				//getting the Hitpoint direction and its length
				Elite::FPoint3 hitPointWithOffset = closestHitRecord.hitPoint + (closestHitRecord.normal * 0.001f);
				Elite::FVector3 direction = pLight->GetDirection(hitPointWithOffset);
				float lengthDirection = Normalize(direction);

				bool isPointVisible = true;
				//checking for shadows
				HitRecord tempHitRecord{};
				tempHitRecord.isLightRay = true;
				Ray lightRay;
				lightRay.SetTMin(0.001f);
				lightRay.SetTMax(lengthDirection);
				lightRay.SetOrigin(hitPointWithOffset);
				lightRay.SetDirection(direction);

				for (Shape* pShape : Scenegraph::GetInstance()->GetShapes())
				{
					if (pShape->Hit(lightRay, tempHitRecord))
					{
						isPointVisible = false;
					}
				}
				Elite::RGBColor Ergb;
				if (isPointVisible)
				{
					float dotProduct = Dot(closestHitRecord.normal, direction);
					if (dotProduct >= 0)
					{
						//light contribution
						Ergb = pLight->GetIrradiance(closestHitRecord);
						// calculate v for shade
						Elite::FVector3 v = Elite::GetNormalized(Elite::FVector3(Elite::Renderer::m_Camera->GetPosition() - Elite::FVector3(hitPointWithOffset)));
						bool temp = false;
						matColor += Ergb * closestHitRecord.pMaterial->Shade(closestHitRecord, direction, v, temp, incidentRay) * dotProduct;
						matColor.MaxToOne();
					}
				}
				else
				{
					matColor = { 0,0,0 };
				}
			}
		}
	}
	isShade = true;
	trnColor = matColor + m_DiffuseColor * 0.1f;
	return trnColor;
}

bool Material::HitObjects(const Ray& castRay, Shape* thisObject, Shape*& closestObject, HitRecord& hitRecord) const
{
	double minDist = 1e6;
	bool intersectionFound = false;

	HitRecord tempHitRecord{};
	float tempTValue = FLT_MAX;

	for (Shape* shape : Scenegraph::GetInstance()->GetShapes())
	{
		if (shape->Hit(castRay, tempHitRecord))
		{
			if (shape != thisObject)
			{
				if (tempHitRecord.tValue < tempTValue)
				{
					tempTValue = tempHitRecord.tValue;
					hitRecord = tempHitRecord;
					intersectionFound = true;
					closestObject = shape;
				}
			}
		}
	}
	return intersectionFound;
}
