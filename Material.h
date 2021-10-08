#pragma once
#include "ERGBColor.h"
#include "EMath.h"
#include "Shape.h"

class Material
{
public:
	Material() = default;
	virtual ~Material() = default;
	Material(const Material&) = delete;
	Material(Material&&) noexcept = delete;
	Material& operator=(const Material&) = delete;
	Material& operator=(Material&&) noexcept = delete;
	virtual Elite::RGBColor Shade(const HitRecord& hitRecord, const Elite::FVector3& l, const Elite::FVector3& v) const = 0;
protected:
	Elite::RGBColor m_DiffuseColor{ 0,0,0 };
	float m_DiffuseReflectance{ 0 };
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
	Elite::RGBColor Shade(const HitRecord& hitRecord, const Elite::FVector3& l, const Elite::FVector3& v) const override;
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
	Elite::RGBColor Shade(const HitRecord& hitRecord, const Elite::FVector3& l, const Elite::FVector3& v) const override;
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
	Elite::RGBColor Shade(const HitRecord& hitRecord, const Elite::FVector3& l, const Elite::FVector3& v) const override;
private:

	bool m_IsMetal;
	Elite::RGBColor m_F0;
	float m_Roughness;
};
