#pragma once
#include "EMath.h"
#include "ERGBColor.h"
#include "Ray.h"

class Material;

struct HitRecord
{
	float tValue{};
	bool IshitObject{};
	Elite::FPoint3 hitPoint{};
	Elite::FVector3 normal{};
	Material* pMaterial{};
	bool isLightRay{ false };
};

class Shape
{
public:
	enum class ShapeType
	{
		plane,
		sphere,
		triangle,
		triangleMesh,
	};
	Shape() {};
	virtual ~Shape() = default;
	Shape(const Shape&) = delete;
	Shape(Shape&&) noexcept = delete;
	Shape& operator=(const Shape&) = delete;
	Shape& operator=(Shape&&) noexcept = delete;

	virtual void Update(float deltaT) {};

	virtual bool Hit(const Ray& ray, HitRecord& hitRecord) const = 0;
	virtual bool TestIntersection(const Ray& ray, HitRecord& hitRecord) const { return false; };
	ShapeType GetShapeType() const { return m_ShapeType; };

	Material* GetMaterial() const { return m_pMaterial; };
	//bool m_IsRefractive = false;
protected:
	Material* m_pMaterial = nullptr;
	ShapeType m_ShapeType;
};
