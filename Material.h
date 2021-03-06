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
	virtual Elite::RGBColor Shade(const HitRecord& hitRecord, const Elite::FVector3& l, const Elite::FVector3& v, bool& isRefractive, const Elite::FVector3& incidentRay) const = 0;
	virtual void SetCurrShape(Shape* pShape) {};
	bool HitObjects(const Ray& castRay, Shape* thisObject, Shape*& closestObject, HitRecord& hitRecord) const;
	void SetIndexOfRefraction(double ior) { m_IndexOfRef = ior; }

	//Datamembers
	Elite::RGBColor m_DiffuseColor{ 0,0,0 };
	float m_DiffuseReflectance{ 0 };
	Shape* m_pShape;
	double m_IndexOfRef = 1;
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
	Elite::RGBColor Shade(const HitRecord& hitRecord, const Elite::FVector3& l, const Elite::FVector3& v, bool& isRefractive, const Elite::FVector3& incidentRay) const override;
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
	Elite::RGBColor Shade(const HitRecord& hitRecord, const Elite::FVector3& l, const Elite::FVector3& v, bool& isRefractive, const Elite::FVector3& incidentRay) const override;
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
	Elite::RGBColor Shade(const HitRecord& hitRecord, const Elite::FVector3& l, const Elite::FVector3& v, bool& isRefractive, const Elite::FVector3& incidentRay) const override;
private:

	bool m_IsMetal;
	Elite::RGBColor m_F0;
	float m_Roughness;
};

class Material_Refractive final : public Material
{
public:
	Material_Refractive(const Elite::RGBColor& diffuseColor);
	void SetCurrShape(Shape* pShape) override { m_pShape = pShape; };
	~Material_Refractive() = default;
	Material_Refractive(const Material_Refractive&) = delete;
	Material_Refractive(Material_Refractive&&) noexcept = delete;
	Material_Refractive& operator=(const Material_Refractive&) = delete;
	Material_Refractive& operator=(Material_Refractive&&) noexcept = delete;
	Elite::RGBColor Shade(const HitRecord& hitRecord, const Elite::FVector3& l, const Elite::FVector3& v, bool& isRefractive, const Elite::FVector3& incidentRay) const override;
private:
	Elite::FVector3 GetRefractedRay(const HitRecord& hitRecord, const Elite::FVector3& incidentRay) const;
};

class Material_Reflective final : public Material
{
public:
	Material_Reflective(const Elite::RGBColor& diffuseColor);
	void SetCurrShape(Shape* pShape) override { m_pShape = pShape; };
	~Material_Reflective() = default;
	Material_Reflective(const Material_Reflective&) = delete;
	Material_Reflective(Material_Reflective&&) noexcept = delete;
	Material_Reflective& operator=(const Material_Reflective&) = delete;
	Material_Reflective& operator=(Material_Reflective&&) noexcept = delete;
	Elite::RGBColor Shade(const HitRecord& hitRecord, const Elite::FVector3& l, const Elite::FVector3& v, bool& isRefractive, const Elite::FVector3& incidentRay) const override;
};