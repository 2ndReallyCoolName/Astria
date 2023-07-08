#include "App.h"
#include "Box.h"
#include "Pyramid.h"
#include "Pipe.h"
#include "Ball.h"
#include "Sheet.h"
#include "SkinnedBox.h"
#include <memory>
#include "Surface.h"
#include "GDIPlusManager.h"
#include "imgui/imgui.h"


GDIPlusManager gdipm;


App::App()
	:
	wnd(1200, 800, "Astria"),
	light(wnd.Gfx())
{

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
				return std::make_unique<Pipe>(
					gfx, rng, adist, ddist, odist,
					rdist, latDist, longDist
					);
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
		std::uniform_int_distribution<int> latDist{ 10, 20 };
		std::uniform_int_distribution<int> longDist{ 7, 20 };
		std::uniform_int_distribution<int> typedist{ 0, 1};
	};

	drawables.reserve(nDrawables);
	std::generate_n(std::back_inserter(drawables), nDrawables, Factory{wnd.Gfx()});

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

	// imgui window to control simulation speed
	if (ImGui::Begin("Simulation Speed"))
	{
		ImGui::SliderFloat("Speed Factor", &speed_factor, 0.0f, 4.0f);
		ImGui::Text("%.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::Text("Status: %s", wnd.kbd.KeyIsPressed(VK_SPACE) ? "PAUSED" : "RUNNING (hold spacebar to pause)");
	}
	ImGui::End();
	
	//imgui window to control camera
	cam.SpawnControlWindow();
	light.SpawnControlWindow();

	wnd.Gfx().EndFrame();
} 