#pragma once
#include "Imgui/imgui.h"
#include "Imgui/imgui_impl_win32.h"
#include "Imgui/imgui_impl_dx12.h"

class ImguiManager
{

public:

	static ImguiManager* GetInstance()
	{
		static ImguiManager instance;
		return &instance;

	};

	void Init();
	void Render();


private:

	

	bool show_demo_window = true;
	bool show_another_window = false;
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
};

