#pragma once
#include "EMath.h"
#include "ERGBColor.h"
#include "Shape.h"

class Sphere final : public Shape
{
public:
	Sphere(const Elite::FPoint3& originSphere, float radius, Material* pMaterial);
	~Sphere() = default;
	Sphere(const Sphere&) = delete;
	Sphere(Sphere&&) noexcept = delete;
	Sphere& operator=(const Sphere&) = delete;
	Sphere& operator=(Sphere&&) noexcept = delete;

	bool Hit(const Ray& ray, HitRecord& hitRecord) const override;

private:
	Elite::FPoint3 m_Origin;
	float m_Radius;
};
