//External includes
#include "SDL.h"
#include "SDL_surface.h"

//Project includes
#include "ERenderer.h"
#include "ERGBColor.h"
#include "EMath.h"
#include "Scenegraph.h"
#include "LightManager.h"
#include <iostream>

Elite::Renderer::Renderer(SDL_Window* pWindow)
{
	//Initialize
	m_pWindow = pWindow;
	m_pFrontBuffer = SDL_GetWindowSurface(pWindow);
	int width, height = 0;
	SDL_GetWindowSize(pWindow, &width, &height);
	m_Width = static_cast<uint32_t>(width);
	m_Height = static_cast<uint32_t>(height);
	m_pBackBuffer = SDL_CreateRGBSurface(0, m_Width, m_Height, 32, 0, 0, 0, 0);
	m_pBackBufferPixels = (uint32_t*)m_pBackBuffer->pixels;
	m_Ray = { uint32_t(width), uint32_t(height) };
}

void Elite::Renderer::Render(Camera& camera)
{
	SDL_LockSurface(m_pBackBuffer);
	//
	Elite::FMatrix4 lookAtMatrix = camera.GetLookAtMatrix();
	//Loop over all the pixels
	for (uint32_t r = 0; r < m_Height; ++r)
	{
		for (uint32_t c = 0; c < m_Width; ++c)
		{
			FPoint3 sample = Elite::FPoint3(lookAtMatrix * m_Ray.ScreenToWorld(r,c,camera.GetFOV()));
			m_Ray.SetDirection(Elite::GetNormalized(sample - camera.GetPosition()));
			m_Ray.SetOrigin(camera.GetPosition());
			HitRecord hitRecord;
			HitObjects(m_Ray, hitRecord);

			Elite::RGBColor finalColor{ 0 ,0,0 };
			//
			if (hitRecord.IshitObject)
			{
				for (const Light* pLight : LightManager::GetInstance()->GetLights())
				{
					//getting the Hitpoint direction and its length
					Elite::FPoint3 hitPointWithOffset = hitRecord.hitPoint + (hitRecord.normal * 0.001f);
					Elite::FVector3 direction = pLight->GetDirection(hitPointWithOffset);
					float lengthDirection = Normalize(direction);

					bool isPointVisible = true;
					//checking for shadows
					if (m_ShadowsSwitch)
					{
						HitRecord tempHitRecord{};
						tempHitRecord.isLightRay = true;
						Ray lightRay;
						lightRay.SetTMin(0.001f);
						lightRay.SetTMax(lengthDirection);
						lightRay.SetOrigin(hitPointWithOffset);
						lightRay.SetDirection(direction);

						for (Shape* pShape : Scenegraph::GetInstance()->GetShapes())
						{
							if (pShape->Hit(lightRay, tempHitRecord))
							{
								isPointVisible = false;
							}
						}
					}
					Elite::RGBColor Ergb;
					if (isPointVisible)
					{
						float dotProduct = Dot(hitRecord.normal, direction);
						if (dotProduct >= 0)
						{
							//light contribution
							Ergb = pLight->GetIrradiance(hitRecord);
							// calculate v for shade
							Elite::FVector3 v = Elite::GetNormalized(Elite::FVector3(camera.GetPosition() - Elite::FVector3(hitPointWithOffset)));
							finalColor += Ergb * hitRecord.pMaterial->Shade(hitRecord, direction, v) *dotProduct;
							finalColor.MaxToOne();
						}
					}
				}
			}
			m_pBackBufferPixels[c + (r * m_Width)] = SDL_MapRGB(m_pBackBuffer->format,
				static_cast<uint8_t>(finalColor.r * 255),
				static_cast<uint8_t>(finalColor.g * 255),
				static_cast<uint8_t>(finalColor.b * 255));
		}
	}

	SDL_UnlockSurface(m_pBackBuffer);
	SDL_BlitSurface(m_pBackBuffer, 0, m_pFrontBuffer, 0);
	SDL_UpdateWindowSurface(m_pWindow);
}

bool Elite::Renderer::SaveBackbufferToImage() const
{
	return SDL_SaveBMP(m_pBackBuffer, "BackbufferRender.bmp");
}

void Elite::Renderer::HitObjects(const Ray& ray, HitRecord& hitRecord) const
{
	HitRecord tempHitRecord{};
	float tempTValue = FLT_MAX;

	for (Shape* shape : Scenegraph::GetInstance()->GetShapes())
	{
		if (shape->Hit(ray, tempHitRecord))
		{
			if (tempHitRecord.tValue < tempTValue)
			{
				tempTValue = tempHitRecord.tValue;
				hitRecord = tempHitRecord;
			}
		}
	}
}
void Elite::Renderer::SetSwitchBools(bool dirLight, bool pointLight1, bool pointLight2, bool irrad, bool shadows, bool BRDF)
{
	m_DirLightSwitch = dirLight;
	m_PointLight1Switch = pointLight1;
	m_PointLight2Switch = pointLight2;
	m_IrradianceSwitch = irrad;
	m_ShadowsSwitch = shadows;
	m_BRDFSwitch = BRDF;
}