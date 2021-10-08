#pragma once
#include "Light.h"
#include <vector>

class LightManager final
{
public:
	static LightManager* GetInstance();
	void ResetInstance();
	static LightManager* m_pInstance;

	Light* AddLight(Light* pLight);
	const std::vector<Light*>& GetLights() const;
private:
	LightManager() = default;
	std::vector<Light*> m_pLights;
};
