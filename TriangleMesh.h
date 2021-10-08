#pragma once
#include <list>
#include "Shape.h"
#include <string>
#include <fstream>
#include <iostream>
#include "Scenegraph.h"
#include "Material.h"
#include "MaterialManager.h"

class TriangleMesh final : public Shape
{
public:
	TriangleMesh(const std::string& fileName, Material* pMat);
	~TriangleMesh() = default;
	TriangleMesh(const TriangleMesh&) = delete;
	TriangleMesh(TriangleMesh&&) noexcept = delete;
	TriangleMesh& operator=(const TriangleMesh&) = delete;
	TriangleMesh& operator=(TriangleMesh&&) noexcept = delete;
	void Update(float deltaT) override {};
	bool Hit(const Ray& ray, HitRecord& hitRecord) const override;
	void ReadObjFile();
private:

	const std::string m_FileName;
	std::vector<Elite::FPoint3> m_VertexList = {};
	std::vector<uint32_t> m_IDList = {};
};
