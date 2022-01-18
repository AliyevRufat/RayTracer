#include "BRDF.h"

Elite::RGBColor BRDF::Lambert(float diffuseReflectance, const Elite::RGBColor& diffuseColor)
{
	return  (diffuseColor * diffuseReflectance) / float(E_PI);
}

Elite::RGBColor BRDF::Phong(float specularReflectance, float phongExponent, const Elite::FVector3& l, const Elite::FVector3& v, const Elite::FVector3& normal)
{
	Elite::FVector3 reflectV = l - 2 * (Elite::Dot(normal, l)) * normal;
	float cosAlpha = Elite::Dot(reflectV, v);
	float phong = (specularReflectance * pow(cosAlpha, phongExponent));
	return Elite::RGBColor(phong, phong, phong);
	//test for github
}

Elite::RGBColor BRDF::CookTorrance(const Elite::RGBColor& F, const Elite::FVector3& l, const Elite::FVector3& v, const Elite::FVector3& normal, float roughness)
{
	// calculations for D
	float correctRoughness = pow(roughness, 2);
	double roughnessPow2 = pow(correctRoughness, 2);
	Elite::FVector3 halfVector = (v + l) / (Elite::Magnitude(v + l));
	double dotOfNormalAndHalfVector = Elite::Dot(normal, halfVector);
	double D = roughnessPow2 / (E_PI * (pow(((pow(dotOfNormalAndHalfVector, 2)) * (roughnessPow2 - 1) + 1), 2)));

	// calculations for G
	double k = (pow((correctRoughness + 1), 2)) / 8;
	double gSmith = CalculateG(normal, v, k) * CalculateG(normal, l, k);

	// calculation of denominator
	float cookTorranceDenominator = (4 * (Elite::Dot(v, normal)) * (Elite::Dot(l, normal)));

	// cookTorrance
	return (F * float(D * gSmith)) / (cookTorranceDenominator);
}

double BRDF::CalculateG(const Elite::FVector3& normal, const Elite::FVector3& vector, double k)
{
	float dotOfNormalAndVecor = Elite::Dot(normal, vector);

	return dotOfNormalAndVecor / (dotOfNormalAndVecor * (1 - k) + k);
}