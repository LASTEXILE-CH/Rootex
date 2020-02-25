#include "editor.h"

#include "core/renderer/rendering_device.h"

#include "framework/components/visual/visual_component_graph.h"
#include "framework/components/hierarchy_component.h"
#include "framework/systems/render_system.h"

void Editor::initialize(HWND hWnd)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	ImGui_ImplWin32_Init(hWnd);
	ImGui_ImplDX11_Init(RenderingDevice::GetSingleton()->getDevice(), RenderingDevice::GetSingleton()->getContext());
	ImGui::StyleColorsDark();
}

void Editor::begin(VisualComponentGraph* visualGraph)
{
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	ImGui::ShowDemoWindow();
}

void explore(const HierarchyComponent* component)
{
	bool x;
	static bool y = false;
	ImGui::Button(std::to_string(component->getOwner()->getID()).c_str());
	if (ImGui::IsItemClicked())
	{
		y = true;
		if (ImGui::BeginPopup("Info"))
		{
			ImGui::OpenPopup("Popup");
			ImGui::Text("This has %d components", component->getOwner()->getAllComponents().size());
			ImGui::EndPopup();
		}
	}

	for (auto&& child : component->getChildren())
	{
		explore(child->getComponent<HierarchyComponent>());
	}
}

void Editor::displayVisualGraph(VisualComponentGraph* graph)
{
	if (ImGui::Begin("Visual Graph"))
	{
		ImGui::SetNextWindowBgAlpha(1.0f);
		if (ImGui::Begin("Viewport"))
		{
			ImGui::Image(RenderingDevice::GetSingleton()->getRenderTextureShaderResourceView(), { 1280, 720 });
		}
		ImGui::End();
		//explore(graph->getRoot());
	}
	ImGui::End();
}

void Editor::end(VisualComponentGraph* visualGraph)
{
	RenderingDevice::GetSingleton()->setTextureRenderTarget();
	RenderSystem::GetSingleton()->render(visualGraph);
	RenderingDevice::GetSingleton()->setBackBufferRenderTarget();
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

Editor::~Editor()
{
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

Editor* Editor::GetSingleton()
{
	static Editor singleton;
	return &singleton;
}