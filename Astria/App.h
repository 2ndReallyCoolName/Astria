#pragma once
#include "Window.h"
#include "AstriaTimer.h"
#include "ImguiManager.h"
#include "Camera.h"
#include "PointLight.h"
#include <set>

class App
{
public:
	App();
	// master frame / message loop
	int Go();
	~App();
private:
	void DoFrame();
	void SpawnSimulationWindow() noexcept;
	void SpawnBoxWindowManagerWindow() noexcept;
	void SpawnBoxWindows() noexcept;
private:
	ImguiManager imgui;
	Window wnd;
	AstriaTimer timer;
	std::vector<std::unique_ptr<class Drawable>> drawables;
	std::vector<class Box*> boxes;
	static constexpr size_t nDrawables = 20;
	float speed_factor = 1.0f;
	Camera cam;
	PointLight light;
	std::optional<int> comboBoxIndex;
	std::set<int> boxControlIds;
};