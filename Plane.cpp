#include "Plane.h"
#include <iostream>

Plane::Plane(const Elite::FPoint3& originPlane, const Elite::FVector3& normal, Material* pMaterial)
	: m_Oplane{ originPlane }
	, m_Normal{ normal }
{
	m_pMaterial = pMaterial;
	m_ShapeType = ShapeType::plane;
}

bool Plane::Hit(const Ray& ray, HitRecord& hitRecord) const
{
	float denom = Dot(ray.GetDirection(), m_Normal);
	//
	if (denom != 0)
	{
		if (abs(denom) > ray.GetTMin() && abs(denom) < ray.GetTMax())
		{
			float t = Dot((m_Oplane - ray.GetOrigin()), m_Normal) / denom;

			hitRecord.tValue = t;
			if (hitRecord.tValue > ray.GetTMin() && hitRecord.tValue < ray.GetTMax())
			{
				if (t >= 0)
				{
					hitRecord.normal = m_Normal;
					hitRecord.pMaterial = m_pMaterial;
					hitRecord.hitPoint = ray.GetOrigin() + hitRecord.tValue * ray.GetDirection();
					hitRecord.IshitObject = true;
					return true;
				}
			}
		}
	}
	return false;
}