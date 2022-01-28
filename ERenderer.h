/*=============================================================================*/
// Copyright 2017-2019 Elite Engine
// Authors: Matthieu Delaere
/*=============================================================================*/
// ERenderer.h: class that holds the surface to render to, does traverse the pixels
// and traces the rays using a tracer
/*=============================================================================*/
#ifndef ELITE_RAYTRACING_RENDERER
#define	ELITE_RAYTRACING_RENDERER

#include <cstdint>
#include "Shape.h"
#include "Camera.h"
#include "Ray.h"
#include "Light.h"
#include "Material.h"
#include "TriangleMesh.h"

struct SDL_Window;
struct SDL_Surface;

namespace Elite
{
	class Renderer final
	{
	public:
		Renderer(SDL_Window* pWindow);
		~Renderer() = default;

		Renderer(const Renderer&) = delete;
		Renderer(Renderer&&) noexcept = delete;
		Renderer& operator=(const Renderer&) = delete;
		Renderer& operator=(Renderer&&) noexcept = delete;

		void Render(Camera& camera);
		bool SaveBackbufferToImage() const;
		//toggle
		void SetSwitchBools(bool dirLight, bool pointLight1, bool pointLight2, bool irrad, bool shadows, bool BRDF);
		static Camera* m_Camera;
	private:
		SDL_Window* m_pWindow = nullptr;
		SDL_Surface* m_pFrontBuffer = nullptr;
		SDL_Surface* m_pBackBuffer = nullptr;
		uint32_t* m_pBackBufferPixels = nullptr;
		uint32_t m_Width = 0;
		uint32_t m_Height = 0;
		Ray m_Ray;
		//toggles
		bool m_DirLightSwitch, m_PointLight1Switch, m_PointLight2Switch, m_IrradianceSwitch, m_ShadowsSwitch, m_BRDFSwitch;
		//Methods
		void HitObjects(const Ray& ray, HitRecord& hitRecord) const;
	};
}

#endif