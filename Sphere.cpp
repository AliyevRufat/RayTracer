#include "Sphere.h"

Sphere::Sphere(const Elite::FPoint3& originSphere, float radius, Material* pMaterial)
	:m_Origin{ originSphere }
	, m_Radius{ radius }
{
	m_pMaterial = { pMaterial };
	m_ShapeType = ShapeType::sphere;
}

bool Sphere::Hit(const Ray& ray, HitRecord& hitRecord) const
{
	float discriminant{};
	float a = Dot(ray.GetDirection(), ray.GetDirection());
	float b = 2 * Dot(ray.GetDirection(), (ray.GetOrigin() - m_Origin));
	float c = Dot((ray.GetOrigin() - m_Origin), (ray.GetOrigin() - m_Origin)) - pow(m_Radius, 2);

	discriminant = b * b - 4 * a * c;
	//hitRecord.color = m_Color;

	if (discriminant > 0)
	{
		float squaredDiscriminant = sqrt(discriminant);

		hitRecord.tValue = (-b - squaredDiscriminant) / (2 * a);
		if (hitRecord.tValue > ray.GetTMin() && hitRecord.tValue < ray.GetTMax())
		{
			if (hitRecord.tValue < ray.GetTMin())
			{
				//t =  (-b + squaredDiscriminant) / (2 * a); // for rendering inside of sphere // optional
				return false;
			}

			hitRecord.pMaterial = m_pMaterial;
			hitRecord.hitPoint = ray.GetOrigin() + hitRecord.tValue * ray.GetDirection();
			hitRecord.IshitObject = true;
			hitRecord.normal = Elite::GetNormalized(hitRecord.hitPoint - m_Origin);

			return true;
		}
	}
	return false;
}

bool Sphere::HitRefractive(const Ray& ray, HitRecord& hitRecord) const
{
	float discriminant{};

	auto dir = ray.GetOrigin() - ray.GetP2();
	dir = Elite::GetNormalized(dir);

	float a = 1;
	float b = 2 * Dot(ray.GetOrigin() - m_Origin, dir);
	float c = Dot(ray.GetOrigin() - m_Origin, ray.GetOrigin() - m_Origin) - 1;

	discriminant = b * b - 4 * a * c;
	if (discriminant > 0)
	{
		float numSQRT = sqrt(discriminant);

		hitRecord.tValue = (-b - numSQRT) / (2 * a);
		auto t1 = (-b + numSQRT) / (2 * a);
		auto t2 = (-b - numSQRT) / (2 * a);

		if ((t1 < 0.0) && (t2 < 0.0))
		{
			return false;
		}
		
		if (hitRecord.tValue < ray.GetTMin())
		{
			return false;
		}

		hitRecord.pMaterial = m_pMaterial;
		hitRecord.hitPoint = ray.GetOrigin() + hitRecord.tValue * ray.GetDirection();
		hitRecord.IshitObject = true;
		hitRecord.normal = Elite::GetNormalized(hitRecord.hitPoint - m_Origin);

		return true;
	}
	return false;
}
