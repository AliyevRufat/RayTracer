#pragma once
#include "ERGBColor.h"
#include "EMath.h"
#include "Shape.h"
#include <vector>

class Material
{
public:
	Material() = default;
	virtual ~Material() = default;
	Material(const Material&) = delete;
	Material(Material&&) noexcept = delete;
	Material& operator=(const Material&) = delete;
	Material& operator=(Material&&) noexcept = delete;
	virtual Elite::RGBColor Shade(const HitRecord& hitRecord, const Elite::FVector3& l, const Elite::FVector3& v, bool& isShade, const Elite::FVector3& incidentRay) const = 0;
	virtual void SetAllShapes(std::vector<Shape*> pAllShapes) {};
	virtual void SetCurrShape(Shape* pShape) {};
	bool CastRay(const Ray& castRay, std::vector<Shape*> objectList,
		Shape* thisObject,
		Shape*& closestObject,
		HitRecord& hitRecord) const;

	Elite::RGBColor m_DiffuseColor{ 0,0,0 };
	float m_DiffuseReflectance{ 0 };
	Shape* m_pShape;
	std::vector<Shape*> m_pAllShapes;
	static double m_IndexOfRef;
};

class Material_Lambert final : public Material
{
public:

	Material_Lambert(float diffuseReflectance, const Elite::RGBColor& diffuseColor);
	~Material_Lambert() = default;
	Material_Lambert(const Material_Lambert&) = delete;
	Material_Lambert(Material_Lambert&&) noexcept = delete;
	Material_Lambert& operator=(const Material_Lambert&) = delete;
	Material_Lambert& operator=(Material_Lambert&&) noexcept = delete;
	Elite::RGBColor Shade(const HitRecord& hitRecord, const Elite::FVector3& l, const Elite::FVector3& v, bool& isShade, const Elite::FVector3& incidentRay) const override;
};

class Material_LambertPhong final : public Material
{
public:

	Material_LambertPhong(float diffuseReflectance, const Elite::RGBColor& diffuseColor, float specularReflectance, float phongExponent);
	~Material_LambertPhong() = default;
	Material_LambertPhong(const Material_LambertPhong&) = delete;
	Material_LambertPhong(Material_LambertPhong&&) noexcept = delete;
	Material_LambertPhong& operator=(const Material_LambertPhong&) = delete;
	Material_LambertPhong& operator=(Material_LambertPhong&&) noexcept = delete;
	Elite::RGBColor Shade(const HitRecord& hitRecord, const Elite::FVector3& l, const Elite::FVector3& v, bool& isShade, const Elite::FVector3& incidentRay) const override;
private:
	float m_SpecularReflectance;
	float m_PhongExponent;
};

class Material_CookTorrance final : public Material
{
public:

	Material_CookTorrance(float diffuseReflectance, const Elite::RGBColor& diffuseColor, bool isMetal, const  Elite::RGBColor& F0, float roughness);
	~Material_CookTorrance() = default;
	Material_CookTorrance(const Material_CookTorrance&) = delete;
	Material_CookTorrance(Material_CookTorrance&&) noexcept = delete;
	Material_CookTorrance& operator=(const Material_CookTorrance&) = delete;
	Material_CookTorrance& operator=(Material_CookTorrance&&) noexcept = delete;
	Elite::RGBColor Shade(const HitRecord& hitRecord, const Elite::FVector3& l, const Elite::FVector3& v, bool& isShade, const Elite::FVector3& incidentRay) const override;
private:

	bool m_IsMetal;
	Elite::RGBColor m_F0;
	float m_Roughness;
};

class Material_Refractive final : public Material
{
public:
	Material_Refractive(const Elite::RGBColor& diffuseColor);
	void SetAllShapes(std::vector<Shape*> pAllShapes) override { m_pAllShapes = pAllShapes; };
	void SetCurrShape(Shape* pShape) override { m_pShape = pShape; };
	~Material_Refractive() = default;
	Material_Refractive(const Material_Refractive&) = delete;
	Material_Refractive(Material_Refractive&&) noexcept = delete;
	Material_Refractive& operator=(const Material_Refractive&) = delete;
	Material_Refractive& operator=(Material_Refractive&&) noexcept = delete;
	Elite::RGBColor Shade(const HitRecord& hitRecord, const Elite::FVector3& l, const Elite::FVector3& v, bool& isShade, const Elite::FVector3& incidentRay) const override;

};