#pragma once

#include <Windows.h>
#include <optional>

#include "common/common.h"
#include "core/renderer/directx/rendering_device.h"

class GameWindow
{
private:
	WNDCLASSEX m_WindowClass = { 0 };
	LPCSTR m_ClassName;
	HINSTANCE m_AppInstance;
	HWND m_WindowHandler;
	Ptr<RenderingDeviceD3D> m_GraphicsHandler = nullptr;

public:
	GameWindow(int xOffset, int yOffset, int width, int height, const String& title);
	GameWindow(const GameWindow&) = delete;
	GameWindow& operator=(const GameWindow&) = delete;

	~GameWindow();

	int gameLoop();
	static LRESULT CALLBACK handleWindowsMessage(HWND windowHandler, UINT msg, WPARAM wParam, LPARAM lParam);
	RenderingDeviceD3D* getRenderingDevice();
	std::optional<int> GameWindow::ProcessMessages();
};