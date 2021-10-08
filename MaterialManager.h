#pragma once
#include <vector>
#include "ERGBColor.h"

class Material;

class MaterialManager final
{
public:
	explicit MaterialManager();
	~MaterialManager();
	MaterialManager(const MaterialManager&) = delete;
	MaterialManager& operator=(const MaterialManager&) = delete;
	MaterialManager(MaterialManager&&) = delete;
	MaterialManager& operator=(MaterialManager&&) = delete;
	//
	Material* AddMaterial(Material* pMaterial);
private:
	std::vector<Material*> m_pMaterials;
};
