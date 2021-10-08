#pragma once
#include "EMath.h"
#include "ERGBColor.h"
#include "Shape.h"

class Triangle final : public Shape
{
public:

	enum class Cullmode
	{
		backCulling,
		frontCulling,
		noCulling
	};

	Triangle(const Elite::FPoint3& V0, const Elite::FPoint3& V1, const Elite::FPoint3& V2, Material* pMaterial, Cullmode cullmode);
	~Triangle() = default;
	Triangle(const Triangle&) = delete;
	Triangle(Triangle&&) noexcept = delete;
	Triangle& operator=(const Triangle&) = delete;
	Triangle& operator=(Triangle&&) noexcept = delete;
	bool Hit(const Ray& ray, HitRecord& hitRecord) const override;
	void Update(float deltaT) override;

private:
	Elite::FVector3 ToCenterAndBack(const Elite::FVector3& vector, bool toCenter) const;

	Elite::FVector3 m_V0, m_V1, m_V2;
	Elite::FVector3  m_A, m_B;
	Elite::FPoint3 m_Center;
	Cullmode m_Cullmode;
	float m_Angle;
	Elite::FVector3 m_Normal;
};
