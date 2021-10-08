#include "TriangleMesh.h"

TriangleMesh::TriangleMesh(const std::string& fileName, Material* pMat)
	:m_FileName{ fileName }
{
	m_pMaterial = { pMat };
	m_ShapeType = ShapeType::triangleMesh;
}

bool TriangleMesh::Hit(const Ray& ray, HitRecord& hitRecord) const
{
	HitRecord tempHitRecord = {};
	tempHitRecord.isLightRay = hitRecord.isLightRay;
	float tempTValue = FLT_MAX;

	for (uint64_t index = 0; index < m_IDList.size(); index += 3)
	{
		const Elite::FPoint3 i0 = m_VertexList[m_IDList[index]];
		const Elite::FPoint3 i1 = m_VertexList[m_IDList[index + 1]];
		const Elite::FPoint3 i2 = m_VertexList[m_IDList[index + 2]];

		Triangle triangle{ i0, i1, i2, m_pMaterial, Triangle::Cullmode::backCulling };

		if (triangle.Hit(ray, tempHitRecord) && (tempHitRecord.tValue < tempTValue))
		{
			tempTValue = tempHitRecord.tValue;
			hitRecord = tempHitRecord;
		}
	}
	return hitRecord.IshitObject;
}

void TriangleMesh::ReadObjFile()
{
	std::ifstream input;
	std::string oneLine;
	std::string catchString;
	std::vector<float> catchFloat;

	input.open(m_FileName, std::ios::in | std::ios::binary);
	if (input.is_open())
	{
		while (std::getline(input, oneLine))
		{
			if (oneLine[0] == 'v')
			{
				for (size_t i = 0; i < oneLine.size(); i++)
				{
					if (oneLine[i] == '.')
					{
						if (oneLine[i - 2] == '-')
						{
							for (size_t j = i - 2; j < i + 5; j++)
							{
								catchString += oneLine[j];
							}
						}
						else
						{
							for (size_t j = i - 1; j < i + 5; j++)
							{
								catchString += oneLine[j];
							}
						}
						catchFloat.push_back(std::stof(catchString));
						catchString = ' ';

						if (catchFloat.size() == 3)
						{
							m_VertexList.push_back(Elite::FPoint3{ catchFloat[0],catchFloat[1],catchFloat[2] });
							catchFloat.clear();
						}
					}
				}
				catchString = ' ';

				//another way to do it

				//std::string firstFloat;
				//std::string secondFloat;
				//std::string thirdFloat;
				//
				//for (size_t i = 2; i < 10; i++)
				//{
				//	firstFloat += oneLine[i];
				//}
				//for (size_t i = 10; i < 17; i++)
				//{
				//	secondFloat += oneLine[i];
				//}
				//for (size_t i = 17; i < oneLine.size(); i++)
				//{
				//	thirdFloat += oneLine[i];
				//}
				//
				//m_VertexList.push_back(Elite::FVector3(std::stof(firstFloat), std::stof(secondFloat), std::stof(thirdFloat)));
			}
			if (oneLine[0] == 'f')
			{
				std::string firstInt;
				std::string secondInt;
				std::string thirdInt;

				int firstWS = 0;
				int secondWS = 0;
				int thirdWS = 0;

				for (int i = 1; i < oneLine.size(); i++)
				{
					if (oneLine[i] == ' ')
					{
						firstWS = i;
						break;
					}
				}
				for (int i = firstWS + 1; i < oneLine.size(); i++)
				{
					if (oneLine[i] == ' ')
					{
						secondWS = i;
						break;
					}
				}
				for (int i = secondWS + 1; i < oneLine.size(); i++)
				{
					if (oneLine[i] == ' ')
					{
						thirdWS = i;
						break;
					}
				}
				// cut the strings and put in the string floats
				for (size_t i = firstWS; i < secondWS; i++)
				{
					firstInt += oneLine[i];
				}
				for (size_t i = secondWS; i < thirdWS; i++)
				{
					secondInt += oneLine[i];
				}
				for (size_t i = thirdWS; i < oneLine.size(); i++)
				{
					thirdInt += oneLine[i];
				}

				m_IDList.push_back(std::stoi(firstInt) - 1);
				m_IDList.push_back(std::stoi(secondInt) - 1);
				m_IDList.push_back(std::stoi(thirdInt) - 1);
			}
		}
		input.close();
	}
}