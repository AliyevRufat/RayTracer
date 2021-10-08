#include "Camera.h"
#include "EMath.h"
#include <iostream>

Camera::Camera(const Elite::FVector3& position)
	:m_Position{ position }
	, m_Angle{ 45.0f }
	, m_Fov{ tanf(Elite::ToRadians(m_Angle / 2.f)) }
{
}

float Camera::GetFOV() const
{
	return m_Fov;
}

Elite::FMatrix4 Camera::GetLookAtMatrix()
{
	Normalize(m_Forward);
	m_Right = Cross(m_WorldUp, m_Forward);
	Normalize(m_Right);
	m_Up = Cross(m_Forward, m_Right);
	Normalize(m_Up);

	Elite::FMatrix4 onb = Elite::FMatrix4
	{
		Elite::FVector4(m_Right),
		Elite::FVector4(m_Up),
		Elite::FVector4(m_Forward),
		Elite::FVector4(Elite::FVector3(m_Position), 1.f)
	};

	return  onb;
}

void Camera::ForwardTranslation(float distance)
{
	m_Position += m_Forward * distance;
}

void Camera::RightTranslation(float distance)
{
	m_Position += m_Right * distance;
}

void Camera::UpTranslation(float distance)
{
	m_Position += m_Up * distance; // check
}

void Camera::Pitch(float angle)
{
	Elite::FMatrix3 xRot = MakeRotation(Elite::ToRadians(angle), m_Right);
	m_Forward = Inverse(Transpose(xRot)) * m_Forward;
}

void Camera::Yaw(float angle)
{
	Elite::FMatrix3 yRot = MakeRotation(Elite::ToRadians(angle), m_Up);
	m_Forward = Inverse(Transpose(yRot)) * m_Forward;
}

Elite::FPoint3 Camera::GetPosition() const
{
	return m_Position;
}