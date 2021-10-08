#pragma once
#include "ERGBColor.h"
#include "EMath.h"

class BRDF final
{
public:
	BRDF() = default;
	~BRDF() = default;
	BRDF(const BRDF&) = delete;
	BRDF(BRDF&&) noexcept = delete;
	BRDF& operator=(const BRDF&) = delete;
	BRDF& operator=(BRDF&&) noexcept = delete;

	//
	static Elite::RGBColor Lambert(float diffuseReflectance, const Elite::RGBColor& diffuseColor);
	static Elite::RGBColor Phong(float specularReflectance, float phongExponent, const Elite::FVector3& l, const Elite::FVector3& v, const Elite::FVector3& normal);
	static Elite::RGBColor CookTorrance(const Elite::RGBColor& F, const Elite::FVector3& l, const Elite::FVector3& v, const Elite::FVector3& normal, float roughness);

	static double CalculateG(const Elite::FVector3& normal, const Elite::FVector3& vector, double k);
};
