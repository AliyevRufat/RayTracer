//External includes
#include "vld.h"
#include "SDL.h"
#include "SDL_surface.h"
#undef main

//Standard includes
#include <iostream>

//Project includes
#include "ETimer.h"
#include "ERenderer.h"
#include "Scenegraph.h"
#include "Camera.h"
#include "MaterialManager.h"
#include "LightManager.h"
#include "TriangleMesh.h"
void ShutDown(SDL_Window* pWindow)
{
	SDL_DestroyWindow(pWindow);
	SDL_Quit();
}

int main(int argc, char* args[])
{
	//Unreferenced parameters
	(void)argc;
	(void)args;

	//Create window + surfaces
	SDL_Init(SDL_INIT_VIDEO);

	const uint32_t width = 640;
	const uint32_t height = 480;
	SDL_Window* pWindow = SDL_CreateWindow(
		"RayTracer - **Rufat Aliyev**",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		width, height, 0);

	if (!pWindow)
		return 1;

	//Initialize "framework"
	Elite::Timer* pTimer = new Elite::Timer();
	Elite::Renderer* pRenderer = new Elite::Renderer(pWindow);
	//---------------------------------------------------------------------------------------
	//init Cam and MatManager
	Camera camera = Elite::FVector3{ 0.f,3.5f,10.f };
	MaterialManager materialManager;
	//vars
	float deltaTime{ 0.f };
	bool isLeftButtonPressed = false;
	bool isRightButtonPressed = false;
	float movementSpeed = 1.5f;
	float rotationSpeed = 2.f;
	//point2f vars
	Elite::FPoint2 prevMousePosition{ 0,0 };
	Elite::FPoint2 newMousePosition{ 0 ,0 };
	//switch bools
	bool directionLightSwitch{ true };
	bool pointLight1Switch{ true };
	bool pointLight2Switch{ true };
	bool irradianceSwitch{ true };
	bool shadowsSwitch{ true };
	bool BRDFSwitch{ true };
	//---------------------------------------------------------------------------------------
	//Start loop
	pTimer->Start();
	float printTimer = 0.f;
	bool isLooping = true;
	bool takeScreenshot = false;
	//-------------------------------------------Switch this bool to change the scene from normal to bunny-----------------------------------------------------
	bool renderBunny = false;

	//Material* triangleMeshMat = new Material_Lambert(0.9f, Elite::RGBColor(1.0f, 1.f, 0.8f));
	//TriangleMesh* triangleMesh = new TriangleMesh("lowpoly_bunny.obj", triangleMeshMat);
	//Material* triangleMat = new Material_Lambert(0.9f, Elite::RGBColor(1.0f, 1.f, 0.8f));

	//if (renderBunny)
	//{
	//	//---------Initialize Triangle Mesh-------
	//	triangleMesh->ReadObjFile();
	//	Scenegraph::GetInstance()->AddObjectToGraph(triangleMesh);
	//}
	//---------Initialize all the Shapes-------
	//planes
	Material* matPlane = new Material_Lambert(0.9f, Elite::RGBColor(1.f, 0.85f, 0.75f));
	Scenegraph::GetInstance()->AddObjectToGraph(new Plane(Elite::FPoint3(0, 0.f, -7), Elite::FVector3(0.f, 0.f, 1.f), matPlane));
	Scenegraph::GetInstance()->AddObjectToGraph(new Plane(Elite::FPoint3(0.f, 0.f, 0.f), Elite::FVector3(0.f, 1.f, 0.f), matPlane));
	Scenegraph::GetInstance()->AddObjectToGraph(new Plane(Elite::FPoint3(-5.f, 0.f, 0.f), Elite::FVector3(1.f, 0.f, 0.f), matPlane));
	Scenegraph::GetInstance()->AddObjectToGraph(new Plane(Elite::FPoint3(5.f, 0.f, 0.f), Elite::FVector3(-1.f, 0.f, 0.f), matPlane));
	if (!renderBunny)
	{
		std::vector<Shape*> allShapes;
		//////////spheres
		//Scenegraph::GetInstance()->AddObjectToGraph(new Sphere(Elite::FPoint3(-2.5f, 4.0f, 0.0f), 1.0f, materialManager.AddMaterial(new Material_CookTorrance(1.0f, Elite::RGBColor(0.6f, 0.6f, 0.6f), false, Elite::RGBColor(0.f, 0.f, 0.f), 1.0f))));
		//Scenegraph::GetInstance()->AddObjectToGraph(new Sphere(Elite::FPoint3(0.0f, 4.0f, 0.0f), 1.0f, materialManager.AddMaterial(new Material_CookTorrance(1.0f, Elite::RGBColor(0.6f, 0.6f, 0.6f), false, Elite::RGBColor(0.f, 0.f, 0.f), 0.4f))));
		//Scenegraph::GetInstance()->AddObjectToGraph(new Sphere(Elite::FPoint3(2.5f, 4.0f, 0.0f), 1.0f, materialManager.AddMaterial(new Material_CookTorrance(1.0f, Elite::RGBColor(0.6f, 0.6f, 0.6f), false, Elite::RGBColor(0.f, 0.f, 0.f), 0.1f))));
		auto materialRefractive = materialManager.AddMaterial(new Material_Refractive(Elite::RGBColor(1.0f,0.5f,0.5f)));
		auto sphere = new Sphere(Elite::FPoint3(-2.5f, 1.5f, 0.0f), 1.0f, materialRefractive);
		materialRefractive->SetCurrShape(sphere);
		allShapes.push_back(sphere);
		Scenegraph::GetInstance()->AddObjectToGraph(sphere);
		sphere = new Sphere(Elite::FPoint3(-2.5f, 1.5f, -3.0f), 1.0f, materialManager.AddMaterial(new Material_CookTorrance(1.0f, Elite::RGBColor(0.6f, 0.6f, 0.6f), true, Elite::RGBColor(0.910f, 0.778f, 0.423f), 0.7f)));
		allShapes.push_back(sphere);
		Scenegraph::GetInstance()->AddObjectToGraph(sphere);
		//Scenegraph::GetInstance()->AddObjectToGraph(new Sphere(Elite::FPoint3(2.5f, 1.5f, 0.0f), 1.0f, materialManager.AddMaterial(new Material_CookTorrance(1.0f, Elite::RGBColor(0.6f, 0.6f, 0.6f), true, Elite::RGBColor(0.910f, 0.778f, 0.423f), 0.1f))));
		////triangles
		//Scenegraph::GetInstance()->AddObjectToGraph(new Triangle(Elite::FPoint3(-0.75f, 7.5f, 0.0f), Elite::FPoint3(-0.75f, 6.0f, 0.0f), Elite::FPoint3(0.75f, 6.0f, 0.0f), triangleMat, Triangle::Cullmode::backCulling));
		//Scenegraph::GetInstance()->AddObjectToGraph(new Triangle(Elite::FPoint3(1.75f, 7.5f, 0.0f), Elite::FPoint3(1.75f, 6.0f, 0.0f), Elite::FPoint3(3.25f, 6.0f, 0.0f), triangleMat, Triangle::Cullmode::frontCulling));
		//Scenegraph::GetInstance()->AddObjectToGraph(new Triangle(Elite::FPoint3(-3.25f, 7.5f, 0.0f), Elite::FPoint3(-3.25f, 6.0f, 0.0f), Elite::FPoint3(-1.75f, 6.0f, 0.0f), triangleMat, Triangle::Cullmode::noCulling));
		materialRefractive->SetAllShapes(allShapes);
	}

	//---------Initialize all the Lights-------
	LightManager::GetInstance()->AddLight(new Light(Elite::FVector3{ 0,5.0f,-4.0f }, Elite::RGBColor{ 1.f,1.f,1.f }, 20.0f, Light::LightType::pointLight));
	//LightManager::GetInstance()->AddLight(new Light(Elite::FVector3{ 0,2.5f,4.f }, Elite::RGBColor{ 1.f,1.f,1.f }, 20.0f, Light::LightType::pointLight));
	LightManager::GetInstance()->AddLight(new Light(Elite::FVector3{ -4,10,6.f }, Elite::RGBColor{ 1.f,1.f,1.f }, 1.5f, Light::LightType::directionalLight));
	//-----------------------------------------
	while (isLooping)
	{
		deltaTime = pTimer->GetElapsed();

		//--------- Get input events ---------
		SDL_Event e;

		while (SDL_PollEvent(&e))
		{
			switch (e.type)
			{
			case SDL_QUIT:
				isLooping = false;
				break;
			case SDL_KEYUP:
				if (e.key.keysym.scancode == SDL_SCANCODE_X)
					takeScreenshot = true;
				break;
			case SDL_MOUSEBUTTONDOWN:
				if (e.button.button == SDL_BUTTON_LEFT)
				{
					isLeftButtonPressed = true;
				}
				if (e.button.button == SDL_BUTTON_RIGHT)
				{
					isRightButtonPressed = true;
				}
				break;
			case SDL_MOUSEMOTION:
				newMousePosition.x = float(e.motion.x);
				newMousePosition.y = float(e.motion.y);
				//delta mouse pos
				Elite::FPoint2 deltaMousePos{ newMousePosition.x - prevMousePosition.x ,newMousePosition.y - prevMousePosition.y };

				if (isLeftButtonPressed && isRightButtonPressed)
				{
					// if LMB and RMB pressed at the same time and dragged on Y axis(translate y)
					camera.UpTranslation(movementSpeed * deltaTime * deltaMousePos.y);
				}
				else if (isLeftButtonPressed && !isRightButtonPressed)
				{
					// if LMB dragged on Y axis (translate z)
					camera.ForwardTranslation(movementSpeed * deltaTime * deltaMousePos.y);

					// if LMB dragged on X axis (rotate y)
					camera.Yaw(rotationSpeed * deltaTime * deltaMousePos.x);
				}
				else if (!isLeftButtonPressed && isRightButtonPressed)
				{
					// if RMB dragged on X axis (rotate y)
					camera.Yaw(rotationSpeed * deltaTime * deltaMousePos.x);
					// if RMB dragged on Y axis (rotate x)
					camera.Pitch(rotationSpeed * deltaTime * deltaMousePos.y);
				}

				prevMousePosition.x = float(e.motion.x);
				prevMousePosition.y = float(e.motion.y);
				break;
			case SDL_MOUSEBUTTONUP:
				if (e.button.button == SDL_BUTTON_LEFT)
				{
					isLeftButtonPressed = false;
				}
				if (e.button.button == SDL_BUTTON_RIGHT)
				{
					isRightButtonPressed = false;
				}
				break;
			case SDL_KEYDOWN:
				switch (e.key.keysym.sym)
				{
				case SDLK_j:
					directionLightSwitch = !directionLightSwitch;
					break;
				case SDLK_k:
					pointLight1Switch = !pointLight1Switch;
					break;
				case SDLK_l:
					pointLight2Switch = !pointLight2Switch;
					break;
				case SDLK_z:
					shadowsSwitch = !shadowsSwitch;
					break;
				case SDLK_t:
				{
					if (irradianceSwitch && BRDFSwitch)
					{
						BRDFSwitch = false;
					}
					else if (irradianceSwitch && !BRDFSwitch)
					{
						BRDFSwitch = true;
						irradianceSwitch = false;
					}
					else if (!irradianceSwitch && BRDFSwitch)
					{
						irradianceSwitch = true;
						BRDFSwitch = true;
					}
				}
				break;
				}
			}
		}
		Scenegraph::GetInstance()->Update(deltaTime);
		//movement cam
		const Uint8* pStates = SDL_GetKeyboardState(nullptr);

		if (pStates[SDL_SCANCODE_W])
		{
			camera.ForwardTranslation(deltaTime * -movementSpeed);
		}
		else if (pStates[SDL_SCANCODE_S])
		{
			camera.ForwardTranslation(deltaTime * movementSpeed);
		}
		if (pStates[SDL_SCANCODE_A])
		{
			camera.RightTranslation(deltaTime * -movementSpeed);
		}
		else if (pStates[SDL_SCANCODE_D])
		{
			camera.RightTranslation(deltaTime * movementSpeed);
		}

		//--------- Render ---------
		pRenderer->Render(camera);// , triangleMesh, renderBunny);
		//switchting the lights and brdfs etc
		pRenderer->SetSwitchBools(directionLightSwitch, pointLight1Switch, pointLight2Switch, irradianceSwitch, shadowsSwitch, BRDFSwitch);

		//--------- Timer ---------
		pTimer->Update();
		printTimer += pTimer->GetElapsed();
		if (printTimer >= 1.f)
		{
			printTimer = 0.f;
			std::cout << "FPS: " << pTimer->GetFPS() << std::endl;
		}

		//Save screenshot after full render
		if (takeScreenshot)
		{
			if (!pRenderer->SaveBackbufferToImage())
				std::cout << "Screenshot saved!" << std::endl;
			else
				std::cout << "Something went wrong. Screenshot not saved!" << std::endl;
			takeScreenshot = false;
		}
	}
	pTimer->Stop();

	//Shutdown "framework"
	delete pRenderer;
	delete pTimer;
	delete matPlane;
	//delete triangleMat;
	//delete triangleMeshMat;
	Scenegraph::GetInstance()->ResetInstance();
	LightManager::GetInstance()->ResetInstance();
	//delete triangleMesh;

	ShutDown(pWindow);
	return 0;
}