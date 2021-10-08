#pragma once
#include "EMath.h"

class Camera final
{
public:
	Camera(const Elite::FVector3& position);
	~Camera() = default;
	Camera(const Camera&) = delete;
	Camera(Camera&&) noexcept = delete;
	Camera& operator=(const Camera&) = delete;
	Camera& operator=(Camera&&) noexcept = delete;
	Elite::FMatrix4 GetLookAtMatrix();
	float GetFOV() const;
	Elite::FPoint3 GetPosition() const;
	void ForwardTranslation(float distance);
	void RightTranslation(float distance);
	void UpTranslation(float distance);
	void Pitch(float angle);
	void Yaw(float angle);
private:
	//Datamembers
	Elite::FVector3  m_Forward{ 0.f, 0.f, 1.f }; //z axis
	Elite::FVector3		  m_Up{ 0.f, 1.f, 0.f }; //y axis
	Elite::FVector3    m_Right{ 1.f, 0.f, 0.f }; //x axis
	const Elite::FVector3 m_WorldUp{ 0.f,1.f,0.f };
	Elite::FPoint3 m_Position;
	float m_Angle;
	float m_Fov;
};
