#include "Scenegraph.h"
#include "Shape.h"
#include <iostream>

Scenegraph* Scenegraph::m_pInstance{ nullptr };

void Scenegraph::ResetInstance()
{
	if (m_pShapes.size() <= 0)
	{
		return;
	}

	for (size_t i = 0; i < m_pShapes.size(); i++)
	{
		if (m_pShapes[i]->GetShapeType() != Shape::ShapeType::triangleMesh)
		{
			delete m_pShapes[i];
		}
	}

	delete m_pInstance;
	m_pInstance = nullptr;
}

Scenegraph* Scenegraph::GetInstance()
{
	if (!m_pInstance)
	{
		m_pInstance = new Scenegraph();
	}
	return m_pInstance;
}

void Scenegraph::Update(float deltaT)
{
	for (Shape* p : m_pShapes)
		p->Update(deltaT);
}

void Scenegraph::AddObjectToGraph(Shape* pShape)
{
	m_pShapes.push_back(pShape);
}

const std::vector<Shape*>& Scenegraph::GetShapes() const
{
	return m_pShapes;
}