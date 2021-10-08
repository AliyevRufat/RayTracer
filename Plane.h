#pragma once
#include "EMath.h"
#include "ERGBColor.h"
#include "Shape.h"

class Material;

class Plane final : public Shape
{
public:
	Plane(const Elite::FPoint3& originPlane, const Elite::FVector3& normal, Material* pMaterial);
	~Plane() = default;
	Plane(const Plane&) = delete;
	Plane(Plane&&) noexcept = delete;
	Plane& operator=(const Plane&) = delete;
	Plane& operator=(Plane&&) noexcept = delete;

	bool Hit(const Ray& ray, HitRecord& hitRecord) const override;
private:
	Elite::FPoint3 m_Oplane;
	Elite::FVector3 m_Normal;
};
