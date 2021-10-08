#include "MaterialManager.h"
#include "Material.h"
#include <iostream>

MaterialManager::MaterialManager()
{
}

MaterialManager::~MaterialManager()
{
	for (size_t i = 0; i < m_pMaterials.size(); i++)
	{
		delete m_pMaterials[i];
	}
}

Material* MaterialManager::AddMaterial(Material* pMaterial)
{
	m_pMaterials.push_back(pMaterial);
	return m_pMaterials[m_pMaterials.size() - 1];
}