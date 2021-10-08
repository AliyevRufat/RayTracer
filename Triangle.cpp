#include "Triangle.h"
#include <iostream>

Triangle::Triangle(const Elite::FPoint3& V0, const Elite::FPoint3& V1, const Elite::FPoint3& V2, Material* pMaterial, Cullmode cullmode)
	:m_A{ V1 - V0 }
	, m_B{ V2 - V0 }
	, m_V0{ V0 }
	, m_V1{ V1 }
	, m_V2{ V2 }
	, m_Cullmode{ cullmode }
	, m_Angle{ 45.0f }
	, m_Normal{ Elite::Cross(m_A, m_B) }
{
	m_Center = Elite::FPoint3((Elite::FVector3(V0) + Elite::FVector3(V1) + Elite::FVector3(V2)) / 3.f);
	m_pMaterial = pMaterial;
	m_ShapeType = ShapeType::triangle;
}

void Triangle::Update(float deltaT)
{
	//going to world origin before rotation
	m_V0 = ToCenterAndBack(m_V0, true);
	m_V1 = ToCenterAndBack(m_V1, true);
	m_V2 = ToCenterAndBack(m_V2, true);
	//rotation
	Elite::FMatrix4 rotation = Elite::MakeRotationY(Elite::ToRadians(m_Angle) * deltaT);
	Elite::FMatrix4 translation = Elite::MakeTranslation(Elite::FVector3(m_Center));
	Elite::FMatrix4 RT = rotation * translation;
	Elite::FVector4 tempV0{}, tempV1{}, tempV2{};
	tempV0 = RT * Elite::FVector4(m_V0, 1);
	tempV1 = RT * Elite::FVector4(m_V1, 1);
	tempV2 = RT * Elite::FVector4(m_V2, 1);
	//going back to the original pos
	m_V0 = ToCenterAndBack(Elite::FVector3(tempV0), false);
	m_V1 = ToCenterAndBack(Elite::FVector3(tempV1), false);
	m_V2 = ToCenterAndBack(Elite::FVector3(tempV2), false);
	//recalculating
	m_A = m_V1 - m_V0;
	m_B = m_V2 - m_V0;
	m_Normal = Elite::Cross(m_A, m_B);
}

bool Triangle::Hit(const Ray& ray, HitRecord& hitRecord) const
{
	Elite::FVector3 normal = m_Normal;
	//
	if (Elite::Dot(normal, ray.GetDirection()) == 0)
	{
		return false;
	}
	//we hit

	Elite::FVector3 L = Elite::FVector3(m_Center) - Elite::FVector3(ray.GetOrigin()); // get vector from ray origin to center
	float tValue = Elite::Dot(L, normal) / Elite::Dot(ray.GetDirection(), normal); // get the t value

	if (tValue < ray.GetTMin() || tValue > ray.GetTMax())
	{
		return false;
	}
	//within range

	Elite::FPoint3 hitPoint = ray.GetOrigin() + tValue * ray.GetDirection();

	Elite::FVector3 c = {};
	Elite::FVector3 edge = {};
	Elite::FVector3 pointToSide = {};

	//first edge
	edge = m_V1 - m_V0;
	pointToSide = Elite::FPoint3(hitPoint) - Elite::FPoint3(m_V0);
	c = Elite::Cross(edge, pointToSide);

	if (Elite::Dot(normal, c) < 0.0f)
	{
		return false;
	}
	//second edge
	edge = m_V2 - m_V1;
	pointToSide = Elite::FPoint3(hitPoint) - Elite::FPoint3(m_V1);
	c = Elite::Cross(edge, pointToSide);

	if (Elite::Dot(normal, c) < 0.0f)
	{
		return false;
	}
	//third edge
	edge = m_V0 - m_V2;
	pointToSide = Elite::FPoint3(hitPoint) - Elite::FPoint3(m_V2);
	c = Elite::Cross(edge, pointToSide);

	if (Elite::Dot(normal, c) < 0.0f)
	{
		return false;
	}

	Cullmode cullmode = m_Cullmode;
	//switch the cullings
	if (hitRecord.isLightRay)
	{
		if (cullmode == Cullmode::backCulling)
		{
			cullmode = Cullmode::frontCulling;
		}
		else if (cullmode == Cullmode::frontCulling)
		{
			cullmode = Cullmode::backCulling;
		}
	}

	switch (cullmode)
	{
	case Triangle::Cullmode::frontCulling:
		if (Elite::Dot(normal, ray.GetDirection()) < 0)
		{
			return false;
		}
		break;
	case Triangle::Cullmode::backCulling:
		if (Elite::Dot(normal, ray.GetDirection()) > 0)
		{
			return false;
		}
		break;
	}
	Elite::Normalize(normal);

	hitRecord.normal = normal;
	hitRecord.IshitObject = true;
	hitRecord.tValue = tValue;
	hitRecord.hitPoint = hitPoint;
	hitRecord.pMaterial = m_pMaterial;
	return true;
}

Elite::FVector3 Triangle::ToCenterAndBack(const Elite::FVector3& vector, bool toCenter) const
{
	if (toCenter)
	{
		return vector - Elite::FVector3(m_Center);
	}
	else
	{
		return vector + Elite::FVector3(m_Center);
	}
}