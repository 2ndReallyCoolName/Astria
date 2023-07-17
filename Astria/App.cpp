#include "App.h"
#include "Box.h"
#include "Cone.h"
#include "Cylinder.h"
#include "Sphere.h"
#include "Sheet.h"
#include "SkinnedBox.h"
#include <memory>
#include "Surface.h"
#include "GDIPlusManager.h"
#include "imgui/imgui.h"
#include "TexturedCylinder.h"
#include "TexturedCone.h"
#include "TexturedSphere.h"
#include "AssImpModel.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

GDIPlusManager gdipm;


App::App()
	:
	wnd(1200, 800, "Astria"),
	light(wnd.Gfx())
{
	Assimp::Importer imp;
	auto model = imp.ReadFile("models\\spider.obj", aiProcess_Triangulate | aiProcess_JoinIdenticalVertices);

	class Factory {
	public:
		Factory(Graphics& gfx): gfx(gfx){}

		std::unique_ptr<Drawable> operator()() {
			const DirectX::XMFLOAT3 mat = { cdist(rng), cdist(rng), cdist(rng) };
			switch (typedist(rng))
			{
			case 0:
				return std::make_unique<Box>(
					gfx, rng, adist, ddist,
					odist, rdist, bdist, mat
					);
			case 1:
				return std::make_unique<Cylinder>(
					gfx, rng, adist, ddist, odist,
					rdist, latDist, longDist
					);

			case 2:
				return std::make_unique<Cone>(
					gfx, rng, adist, ddist, odist,
					rdist, latDist
					);
			case 3:
				return std::make_unique<Sphere>(
					gfx, rng, adist, ddist, odist,
					rdist, latDist, longDist
					);
			case 4:
				return std::make_unique<SkinnedBox>(
					gfx, rng, adist, ddist,
					odist, rdist
					);
			case 5:
				return std::make_unique<Sheet>(
					gfx, rng, adist, ddist,
					odist, rdist);
			case 6:
				return std::make_unique<TexturedCylinder>(
					gfx, rng, adist, ddist,
					odist, rdist, latDist, longDist);
			case 7:
				return std::make_unique<TexturedCone>(
					gfx, rng, adist, ddist,
					odist, rdist, longDist);
			case 8:
				return std::make_unique<TexturedSphere>(
					gfx, rng, adist, ddist,
					odist, rdist, latDist, longDist);
			case 9:
				return std::make_unique<AssImpModel>(
					gfx, rng, adist, ddist,
					odist, rdist, mat,  1.5f);
			default:
				assert(false && "impossible drawable option in factory");
				return {};
			}
		}

	private:
		Graphics& gfx;
		std::mt19937 rng{ std::random_device{}() };
		std::uniform_real_distribution<float> adist{ 0.0f, 3.1415f * 2.0f };
		std::uniform_real_distribution<float> ddist{0.0f, 3.1415f * 2.0f};
		std::uniform_real_distribution<float> odist{0.0f, 3.1415f * 0.3f};
		std::uniform_real_distribution<float> rdist{6.0f, 20.0f};
		std::uniform_real_distribution<float> bdist{ 0.5f, 3.0f };
		std::uniform_real_distribution<float> cdist{ 0.0f, 1.0f };
		std::uniform_int_distribution<int> latDist{ 10, 30 };
		std::uniform_int_distribution<int> longDist{ 10, 30 };
		std::uniform_int_distribution<int> typedist{ 9, 9};
	};

	drawables.reserve(nDrawables);
	std::generate_n(std::back_inserter(drawables), nDrawables, Factory{wnd.Gfx()});

	for (auto& pd : drawables)
	{
		if (auto pb = dynamic_cast<Box*>(pd.get()))
		{
			boxes.push_back(pb);
		}
	}

	/*if (boxes.size() > 0) {
		comboBoxIndex = 0;
	}*/

	wnd.Gfx().SetProjection(DirectX::XMMatrixPerspectiveLH(1.0f, 3.0f / 4.0f, 0.5f, 40.0f));
}

int App::Go()  
{
	while (true)
	{
		// process all messages pending, but to not block for new messages
		if (const auto ecode = Window::ProcessMessages())
		{
			// if return optional has value, means we're quitting so return exit code
			return *ecode;
		}
		DoFrame();
	}
}

App::~App()
{}

void App::DoFrame() 
{
	const auto dt = timer.Mark() * speed_factor;
	
	wnd.Gfx().BeginFrame(0.07f, 0.0f, 0.12f);
	wnd.Gfx().SetCamera(cam.GetMatrix());
	light.Bind(wnd.Gfx(), cam.GetMatrix());


	for (auto& d : drawables)
	{
		d->Update(wnd.kbd.KeyIsPressed(VK_SPACE) ? 0.0f : dt / 3);
		d->Draw(wnd.Gfx());
	}

	light.Draw(wnd.Gfx());


	SpawnSimulationWindow();
	
	//imgui window to control camera
	cam.SpawnControlWindow();
	light.SpawnControlWindow();

	SpawnBoxWindowManagerWindow();
	SpawnBoxWindows();

	wnd.Gfx().EndFrame();
} 


void App::SpawnSimulationWindow() noexcept
{
	if (ImGui::Begin("Simulation Speed"))
	{
		ImGui::SliderFloat("Speed Factor", &speed_factor, 0.0f, 6.0f, "%.4f", 3.2f);
		ImGui::Text("%.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::Text("Status: %s", wnd.kbd.KeyIsPressed(VK_SPACE) ? "PAUSED" : "RUNNING (hold spacebar to pause)");
	}
	ImGui::End();
}

void App::SpawnBoxWindowManagerWindow() noexcept
{
	if (ImGui::Begin("Boxes"))
	{
		using namespace std::string_literals;
		const auto preview = comboBoxIndex ? std::to_string(*comboBoxIndex) : "Choose a box..."s;
		if (ImGui::BeginCombo("Box Number", preview.c_str()))
		{
			for (int i = 0; i < boxes.size(); i++)
			{
				const bool selected = comboBoxIndex ? *comboBoxIndex == i : false;
				
				if (ImGui::Selectable(std::to_string(i).c_str(), selected))
				{
					comboBoxIndex = i;
				}
				if (selected)
				{
					ImGui::SetItemDefaultFocus();
				}
			}
			ImGui::EndCombo();
		}
		if (ImGui::Button("Spawn Control Window") && comboBoxIndex)
		{
			boxControlIds.insert(*comboBoxIndex);
			comboBoxIndex.reset();
		}
	}
	ImGui::End();
}

void App::SpawnBoxWindows() noexcept
{
	for (auto i = boxControlIds.begin(); i != boxControlIds.end(); )
	{
		if (!boxes[*i]->SpawnControlWindow(*i, wnd.Gfx()))
		{
			i = boxControlIds.erase(i);
		}
		else
		{
			i++;
		}
	}
}