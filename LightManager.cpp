#include "LightManager.h"

LightManager* LightManager::m_pInstance{ nullptr };

void LightManager::ResetInstance()
{
	for (Light* l : m_pLights)
		delete l;
	delete m_pInstance;
	m_pInstance = nullptr;
}

LightManager* LightManager::GetInstance()
{
	if (!m_pInstance)
	{
		m_pInstance = new LightManager();
	}
	return m_pInstance;
}

Light* LightManager::AddLight(Light* pLight)
{
	m_pLights.push_back(pLight);
	return m_pLights[m_pLights.size() - 1];
}

const std::vector<Light*>& LightManager::GetLights() const
{
	return m_pLights;
}