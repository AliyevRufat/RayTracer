#pragma once
#include "Shape.h"
#include "Plane.h"
#include "Sphere.h"
#include "Triangle.h"
#include "TriangleMesh.h"
#include <vector>

class Scenegraph final
{
public:

	static Scenegraph* GetInstance();
	void ResetInstance();
	static Scenegraph* m_pInstance;

	void Update(float deltaT);
	void AddObjectToGraph(Shape* pShape);

	const std::vector<Shape*>& GetShapes() const;
private:
	Scenegraph() = default;
	//
	std::vector<Shape*> m_pShapes;
};
