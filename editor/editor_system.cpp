#include "editor_system.h"

#include "core/random.h"
#include "core/renderer/rendering_device.h"
#include "core/renderer/material_library.h"
#include "core/resource_loader.h"
#include "core/resource_files/lua_text_resource_file.h"
#include "core/input/input_manager.h"
#include "framework/scene_loader.h"
#include "framework/systems/render_system.h"
#include "framework/systems/render_ui_system.h"
#include "framework/systems/script_system.h"
#include "framework/systems/ui_system.h"
#include "framework/systems/physics_system.h"
#include "editor_application.h"
#include "main/window.h"

#include "imgui_stdlib.h"
#include "ImGuizmo.h"

#define DOCUMENTATION_LINK String("https://rootex.readthedocs.io/")

Vector<String> Split(const String& s, char delim)
{
	Vector<String> elems;
	std::stringstream ss(s);
	std::string item;
	while (std::getline(ss, item, delim))
	{
		elems.push_back(item);
	}
	return elems;
}

bool EditorSystem::initialize(const JSON::json& systemData)
{
	BIND_EVENT_MEMBER_FUNCTION(EditorEvents::EditorSaveBeforeQuit, EditorSystem::saveBeforeQuit);
	BIND_EVENT_MEMBER_FUNCTION(EditorEvents::EditorSaveAll, EditorSystem::saveAll);
	BIND_EVENT_MEMBER_FUNCTION(EditorEvents::EditorAutoSave, EditorSystem::autoSave);
	BIND_EVENT_MEMBER_FUNCTION(EditorEvents::EditorCreateNewScene, EditorSystem::createNewScene);
	BIND_EVENT_MEMBER_FUNCTION(EditorEvents::EditorCreateNewMaterial, EditorSystem::createNewMaterial);

	m_Scene.reset(new SceneDock());
	m_Output.reset(new OutputDock());
	m_Toolbar.reset(new ToolbarDock());
	m_Viewport.reset(new ViewportDock(systemData["viewport"]));
	m_Inspector.reset(new InspectorDock());
	m_FileViewer.reset(new FileViewer());

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable | ImGuiConfigFlags_NavEnableGamepad | ImGuiConfigFlags_NavEnableKeyboard;
	io.ConfigDockingWithShift = true;
	io.FontAllowUserScaling = true;

	ImFontConfig fontConfig;
	fontConfig.PixelSnapH = true;
	fontConfig.OversampleH = 1;
	m_EditorFont = io.Fonts->AddFontFromFileTTF("editor/assets/fonts/DroidSans/DroidSans.ttf", 18.0f, &fontConfig);
	{
		static const ImWchar iconsRanges[] = { ICON_MIN_ROOTEX, ICON_MAX_ROOTEX, 0 };
		ImFontConfig iconsConfig;
		iconsConfig.MergeMode = true;
		iconsConfig.PixelSnapH = true;
		io.Fonts->AddFontFromMemoryCompressedBase85TTF(FONT_ICON_BUFFER_NAME_ROOTEX, 18.0f, &iconsConfig, iconsRanges);
	}

	m_EditorFontItalic = io.Fonts->AddFontFromFileTTF("editor/assets/fonts/Cousine/Cousine-Italic.ttf", 18.0f, &fontConfig);
	{
		static const ImWchar iconsRanges[] = { ICON_MIN_ROOTEX, ICON_MAX_ROOTEX, 0 };
		ImFontConfig iconsConfig;
		iconsConfig.MergeMode = true;
		iconsConfig.PixelSnapH = true;
		io.Fonts->AddFontFromMemoryCompressedBase85TTF(FONT_ICON_BUFFER_NAME_ROOTEX, 18.0f, &iconsConfig, iconsRanges);
	}
	m_EditorFontBold = io.Fonts->AddFontFromFileTTF("editor/assets/fonts/DroidSans/DroidSans-Bold.ttf", 18.0f, &fontConfig);
	{
		static const ImWchar iconsRanges[] = { ICON_MIN_ROOTEX, ICON_MAX_ROOTEX, 0 };
		ImFontConfig iconsConfig;
		iconsConfig.MergeMode = true;
		iconsConfig.PixelSnapH = true;
		io.Fonts->AddFontFromMemoryCompressedBase85TTF(FONT_ICON_BUFFER_NAME_ROOTEX, 18.0f, &iconsConfig, iconsRanges);
	}
	m_EditorFontMonospace = io.Fonts->AddFontFromFileTTF("editor/assets/fonts/Cousine/Cousine-Regular.ttf", 18.0f, &fontConfig);
	{
		static const ImWchar iconsRanges[] = { ICON_MIN_ROOTEX, ICON_MAX_ROOTEX, 0 };
		ImFontConfig iconsConfig;
		iconsConfig.MergeMode = true;
		iconsConfig.PixelSnapH = true;
		io.Fonts->AddFontFromMemoryCompressedBase85TTF(FONT_ICON_BUFFER_NAME_ROOTEX, 18.0f, &iconsConfig, iconsRanges);
	}
	ImGui_ImplWin32_Init(Application::GetSingleton()->getWindow()->getWindowHandle());
	ImGui_ImplDX11_Init(RenderingDevice::GetSingleton()->getDevice(), RenderingDevice::GetSingleton()->getContext());
	ImGui::StyleColorsDark();

	{
		ImGuiStyle& style = ImGui::GetStyle();
		style.WindowPadding = ImVec2(15, 15);
		style.WindowRounding = 5.0f;
		style.FramePadding = ImVec2(5, 5);
		style.FrameRounding = 4.0f;
		style.ItemSpacing = ImVec2(12, 6);
		style.ItemInnerSpacing = ImVec2(8, 4);
		style.IndentSpacing = 25.0f;
		style.ScrollbarSize = 15.0f;
		style.ScrollbarRounding = 9.0f;
		style.GrabMinSize = 5.0f;
		style.GrabRounding = 3.0f;

		ImVec4* colors = ImGui::GetStyle().Colors;
		colors[ImGuiCol_Text] = ImVec4(0.80f, 0.80f, 0.83f, 1.00f);
		colors[ImGuiCol_TextDisabled] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
		colors[ImGuiCol_WindowBg] = ImVec4(0.05f, 0.05f, 0.07f, 1.00f);
		colors[ImGuiCol_ChildBg] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
		colors[ImGuiCol_PopupBg] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
		colors[ImGuiCol_Border] = ImVec4(0.23f, 0.23f, 0.23f, 1.00f);
		colors[ImGuiCol_BorderShadow] = ImVec4(0.68f, 0.63f, 0.63f, 1.00f);
		colors[ImGuiCol_FrameBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
		colors[ImGuiCol_FrameBgHovered] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
		colors[ImGuiCol_FrameBgActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
		colors[ImGuiCol_TitleBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
		colors[ImGuiCol_TitleBgActive] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
		colors[ImGuiCol_TitleBgCollapsed] = ImVec4(1.00f, 0.98f, 0.95f, 0.75f);
		colors[ImGuiCol_MenuBarBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
		colors[ImGuiCol_ScrollbarBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
		colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
		colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
		colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
		colors[ImGuiCol_CheckMark] = ImVec4(0.20f, 0.68f, 0.42f, 0.83f);
		colors[ImGuiCol_SliderGrab] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
		colors[ImGuiCol_SliderGrabActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
		colors[ImGuiCol_Button] = ImVec4(0.12f, 0.43f, 0.33f, 0.88f);
		colors[ImGuiCol_ButtonHovered] = ImVec4(0.20f, 0.68f, 0.42f, 0.83f);
		colors[ImGuiCol_ButtonActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
		colors[ImGuiCol_Header] = ImVec4(0.20f, 0.68f, 0.42f, 0.58f);
		colors[ImGuiCol_HeaderHovered] = ImVec4(0.20f, 0.68f, 0.42f, 0.83f);
		colors[ImGuiCol_HeaderActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
		colors[ImGuiCol_Separator] = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
		colors[ImGuiCol_SeparatorHovered] = ImVec4(0.20f, 0.68f, 0.42f, 0.83f);
		colors[ImGuiCol_SeparatorActive] = ImVec4(0.36f, 0.75f, 0.10f, 1.00f);
		colors[ImGuiCol_ResizeGrip] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
		colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
		colors[ImGuiCol_ResizeGripActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
		colors[ImGuiCol_Tab] = ImVec4(0.18f, 0.57f, 0.58f, 0.86f);
		colors[ImGuiCol_TabHovered] = ImVec4(0.20f, 0.68f, 0.42f, 0.83f);
		colors[ImGuiCol_TabActive] = ImVec4(0.20f, 0.68f, 0.42f, 1.00f);
		colors[ImGuiCol_TabUnfocused] = ImVec4(0.14f, 0.41f, 0.42f, 0.46f);
		colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.14f, 0.41f, 0.42f, 1.00f);
		colors[ImGuiCol_DockingPreview] = ImVec4(0.00f, 1.00f, 0.21f, 0.70f);
		colors[ImGuiCol_DockingEmptyBg] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
		colors[ImGuiCol_PlotLines] = ImVec4(0.40f, 0.39f, 0.38f, 0.63f);
		colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
		colors[ImGuiCol_PlotHistogram] = ImVec4(0.40f, 0.39f, 0.38f, 0.63f);
		colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
		colors[ImGuiCol_TableHeaderBg] = ImVec4(0.19f, 0.19f, 0.20f, 1.00f);
		colors[ImGuiCol_TableBorderStrong] = ImVec4(0.31f, 0.31f, 0.35f, 1.00f);
		colors[ImGuiCol_TableBorderLight] = ImVec4(0.23f, 0.23f, 0.25f, 1.00f);
		colors[ImGuiCol_TableRowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
		colors[ImGuiCol_TableRowBgAlt] = ImVec4(1.00f, 1.00f, 1.00f, 0.06f);
		colors[ImGuiCol_TextSelectedBg] = ImVec4(0.25f, 1.00f, 0.00f, 0.48f);
		colors[ImGuiCol_DragDropTarget] = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
		colors[ImGuiCol_NavHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
		colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
		colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
		colors[ImGuiCol_ModalWindowDimBg] = ImVec4(1.00f, 0.98f, 0.95f, 0.73f);
	}

	return true;
}

void EditorSystem::update(float deltaMilliseconds)
{
	ZoneScoped;
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	ImGuizmo::BeginFrame();

	pushRegularFont();

	drawDefaultUI(deltaMilliseconds);
	m_Scene->draw(deltaMilliseconds);
	m_Toolbar->draw(deltaMilliseconds);
	m_Viewport->draw(deltaMilliseconds);
	m_Inspector->draw(deltaMilliseconds);
	m_FileViewer->draw(deltaMilliseconds);
	m_Output->draw(deltaMilliseconds);

	popFont();

	RenderingDevice::GetSingleton()->setMainRT();
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	RenderingDevice::GetSingleton()->setOffScreenRTVDSV();
}

EditorSystem::EditorSystem()
    : System("EditorSystem", UpdateOrder::Editor, true)
{
}

EditorSystem::~EditorSystem()
{
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

void EditorSystem::drawDefaultUI(float deltaMilliseconds)
{
	ZoneScoped;
	static bool optFullscreenPersistant = true;
	bool optFullscreen = optFullscreenPersistant;
	static ImGuiDockNodeFlags dockspaceFlags = ImGuiDockNodeFlags_None;

	ImGuiWindowFlags windowFlags = ImGuiWindowFlags_MenuBar;
	if (optFullscreen)
	{
		ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->Pos);
		ImGui::SetNextWindowSize(viewport->Size);
		ImGui::SetNextWindowViewport(viewport->ID);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 2.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);

		windowFlags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
		windowFlags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
	}

	if (dockspaceFlags & ImGuiDockNodeFlags_PassthruCentralNode)
		windowFlags |= ImGuiWindowFlags_NoBackground;

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

	static String loadingScene;

	ImGui::Begin("Rootex Editor", nullptr, windowFlags);
	{
		ImGui::PopStyleVar();

		if (optFullscreen)
		{
			ImGui::PopStyleVar(2);
		}

		ImGuiID dockspaceId = ImGui::GetID("Rootex Editor Dockspace");
		ImGui::DockSpace(dockspaceId, ImVec2(0.0f, 0.0f), dockspaceFlags);

		if (ImGui::BeginMenuBar())
		{
			static String newSceneName;
			static String openSceneName;
			static String newMaterialName;
			static String newMaterialType;
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::BeginMenu("Create Material"))
				{
					if (ImGui::BeginCombo("Material Type", newMaterialType.c_str()))
					{
						for (auto& [materialType, materialCreators] : MaterialLibrary::GetMaterialDatabase())
						{
							if (ImGui::Selectable(materialType.c_str(), ""))
							{
								newMaterialType = materialType;
							}
						}
						ImGui::EndCombo();
					}
					ImGui::InputText("Material Name", &newMaterialName, ImGuiInputTextFlags_AlwaysInsertMode);
					if (!newMaterialName.empty() && !newMaterialType.empty() && ImGui::Button("Create"))
					{
						Vector<String> newMaterialInfo = { "game/assets/materials/" + newMaterialName + ".rmat", newMaterialType };
						EventManager::GetSingleton()->call(EditorEvents::EditorCreateNewMaterial, newMaterialInfo);
					}

					ImGui::EndMenu();
				}
				static String newScript;
				if (ImGui::BeginMenu("Create Script"))
				{
					ImGui::InputText("Script Name", &newScript, ImGuiInputTextFlags_AlwaysInsertMode);
					if (!newScript.empty() && ImGui::Button("Create"))
					{
						newScript = "game/assets/scripts/" + newScript + ".lua";
						if (!OS::IsExists(newScript))
						{
							InputOutputFileStream file = OS::CreateFileName(newScript);
							String defaultScript = ResourceLoader::CreateLuaTextResourceFile("rootex/assets/scripts/empty.lua")->getString();
							file.write(defaultScript.c_str(), strlen(defaultScript.c_str()));
							file.close();
							PRINT("Successfully created script: " + newScript);
							newScript = "";
						}
						else
						{
							WARN("Script already exists: " + newScript);
						}
					}
					ImGui::EndMenu();
				}
				ImGui::Separator();
				if (ImGui::BeginMenu("Create Scene"))
				{
					ImGui::InputText("Scene Name", &newSceneName, ImGuiInputTextFlags_AlwaysInsertMode);
					if (!newSceneName.empty() && ImGui::Button("Create"))
					{
						if (SceneLoader::GetSingleton()->getCurrentScene())
						{
							m_MenuAction = "Save";
							m_PopupCause = "create";
						}
						else
						{
							EventManager::GetSingleton()->call(EditorEvents::EditorCreateNewScene, newSceneName);
							loadingScene = "game/assets/scenes/" + newSceneName + ".scene.json";
						}
					}
					ImGui::EndMenu();
				}
				if (ImGui::BeginMenu("Open Scene"))
				{
					for (auto&& levelName : OS::GetFilesInDirectory("game/assets/scenes/"))
					{
						if (ImGui::MenuItem(levelName.generic_string().c_str()))
						{
							if (SceneLoader::GetSingleton()->getCurrentScene())
							{
								openSceneName = levelName.generic_string();
								m_MenuAction = "Save";
								m_PopupCause = "open";
							}
							else
							{
								loadingScene = levelName.generic_string();
							}
						}
					}
					ImGui::EndMenu();
				}
				if (ImGui::MenuItem("Save Scene", "", false, (bool)SceneLoader::GetSingleton()->getCurrentScene()))
				{
					EventManager::GetSingleton()->call(EditorEvents::EditorSaveAll);
				}
				if (ImGui::MenuItem("Preferences"))
				{
					EventManager::GetSingleton()->call(EditorEvents::EditorOpenFile, ApplicationSettings::GetSingleton()->getTextFile()->getPath().string());
				}
				ImGui::Separator();
				if (ImGui::MenuItem("Quit", ""))
				{
					EventManager::GetSingleton()->call(EditorEvents::EditorSaveBeforeQuit, 0);
				}

				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Assets"))
			{
				if (ImGui::MenuItem("Build Fonts"))
				{
					PRINT("Building fonts...");
					OS::Execute("start \"\" \"" + OS::GetAbsolutePath("build_fonts.bat").string() + "\"");
					PRINT("Built fonts");
				}
				if (ImGui::BeginMenu("Resources"))
				{
					int id = 0;
					for (auto& [type, files] : ResourceLoader::GetResources())
					{
						for (auto& fileHandle : files)
						{
							Ref<ResourceFile> file = fileHandle.lock();
							if (!file)
							{
								continue;
							}

							ImGui::PushID(id);
							if (file->isDirty())
							{
								if (ImGui::Button(ICON_ROOTEX_REFRESH "##Refresh Asset"))
								{
									file->reimport();
								}
								ImGui::SameLine();
								ImGui::TextColored(getWarningColor(), "%s", file->getPath().generic_string().c_str());
							}
							else
							{
								ImGui::Text(file->getPath().generic_string().c_str());
							}
							ImGui::PopID();
							id++;
						}
					}
					ImGui::EndMenu();
				}
				ImGui::EndMenu();
			}
			static bool styleEditor = false;
			if (ImGui::BeginMenu("View"))
			{
				if (ImGui::Checkbox("Wireframe Mode", &m_WireframeMode))
				{
					if (m_WireframeMode)
					{
						RenderSystem::GetSingleton()->enableWireframeRasterizer();
					}
					else
					{
						RenderSystem::GetSingleton()->resetDefaultRasterizer();
					}
				}

				ImGui::Checkbox("Style Editor", &styleEditor);

				bool fullscreen = Extract<bool>(EventManager::GetSingleton()->returnCall(RootexEvents::WindowGetScreenState));
				if (ImGui::Checkbox("Full Screen", &fullscreen))
				{
					EventManager::GetSingleton()->deferredCall(RootexEvents::WindowToggleFullscreen);
				}
				if (ImGui::BeginMenu("Windows"))
				{
					ImGui::Checkbox("Toolbar", &m_Toolbar->getSettings().m_IsActive);
					ImGui::Checkbox("Output", &m_Output->getSettings().m_IsActive);
					ImGui::Checkbox("Scene", &m_Scene->getSettings().m_IsActive);
					ImGui::Checkbox("Viewport", &m_Viewport->getSettings().m_IsActive);
					ImGui::Checkbox("Inspector", &m_Inspector->getSettings().m_IsActive);
					ImGui::EndMenu();
				}
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Scene"))
			{
				if (SceneLoader::GetSingleton()->getCurrentScene())
				{
					SceneLoader::GetSingleton()->getCurrentScene()->getSettings().draw();
				}
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Documentation"))
			{
				if (ImGui::BeginMenu("Guides"))
				{
					static Vector<FilePath> guidesFiles = OS::GetAllFilesInDirectory("docs/guides/");
					for (auto& file : guidesFiles)
					{
						if (ImGui::MenuItem(file.generic_string().c_str()))
						{
							EventManager::GetSingleton()->call(EditorEvents::EditorOpenFile, file.generic_string());
						}
					}
					ImGui::EndMenu();
				}

				if (ImGui::BeginMenu("Engine"))
				{
					static Vector<FilePath> engineDocFiles = OS::GetAllFilesInDirectory("docs/engine/");
					for (auto& file : engineDocFiles)
					{
						if (ImGui::MenuItem(file.generic_string().c_str()))
						{
							EventManager::GetSingleton()->call(EditorEvents::EditorOpenFile, file.generic_string());
						}
					}
					ImGui::EndMenu();
				}

				if (ImGui::BeginMenu("Engine API"))
				{
					static Vector<FilePath> apiFiles = OS::GetAllFilesInDirectory("docs/api/");
					for (auto& file : apiFiles)
					{
						if (ImGui::MenuItem(file.generic_string().c_str()))
						{
							EventManager::GetSingleton()->call(EditorEvents::EditorOpenFile, file.generic_string());
						}
					}
					ImGui::EndMenu();
				}
				if (ImGui::MenuItem("Lua API"))
				{
					m_MenuAction = "Lua API Documentation";
				}

				ImGui::Separator();

				if (ImGui::MenuItem("Online Documentation " ICON_ROOTEX_EXTERNAL_LINK))
				{
					OS::Execute("start " + DOCUMENTATION_LINK);
				}

				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("About"))
			{
				if (ImGui::MenuItem("About Rootex Editor"))
				{
					m_MenuAction = "About Rootex Editor";
				}
				if (ImGui::BeginMenu("Open Source Licenses"))
				{
					for (auto&& library : Application::GetSingleton()->getLibrariesPaths())
					{
						if (ImGui::MenuItem(library.filename().string().c_str(), ""))
						{
							m_MenuAction = library.string();
						}
					}
					ImGui::EndMenu();
				}
				ImGui::EndMenu();
			}

			if (styleEditor)
			{
				ImGui::Begin("Style Editor");
				ImGui::ShowStyleEditor();
				ImGui::End();
			}

			if (m_MenuAction != "")
			{
				ImGui::OpenPopup(m_MenuAction.c_str());
			}

			ImGui::SetNextWindowSize({ ImGui::GetContentRegionMax().x / 2, ImGui::GetContentRegionMax().y / 2 });
			if (ImGui::BeginPopupModal("Lua API Documentation", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
			{
				static String searchingString;
				ImGui::InputText("Search", &searchingString, ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_AlwaysInsertMode | ImGuiInputTextFlags_CallbackHistory);
				ImGui::SameLine();
				if (ImGui::Button("Close"))
				{
					ImGui::CloseCurrentPopup();
					m_MenuAction = "";
				}

				static String openedLuaClass;
				if (ImGui::ListBoxHeader("##Lua Classes", { 0, ImGui::GetContentRegionAvail().y }))
				{
					for (auto& [key, value] : LuaInterpreter::GetSingleton()->getLuaState().registry())
					{
						if (key.is<String>())
						{
							sol::object luaClass = value;
							String typeName = key.as<String>();
							if (luaClass.is<sol::table>() && luaClass.as<sol::table>()["__type"] != sol::nil)
							{
								typeName = luaClass.as<sol::table>()["__type"]["name"];
							}

							bool shouldMatch = false;
							for (int i = 0; i < typeName.size(); i++)
							{
								shouldMatch |= typeName.compare(i, searchingString.size(), searchingString) == 0;
							}

							if (searchingString.empty() || shouldMatch)
							{
								if (ImGui::MenuItem(typeName.c_str()))
								{
									openedLuaClass = key.as<String>();
								}
							}
						}
					}
					ImGui::ListBoxFooter();
				}
				ImGui::SameLine();
				if (ImGui::ListBoxHeader("##Class Description", ImGui::GetContentRegionAvail()))
				{
					if (!openedLuaClass.empty())
					{
						sol::object luaClass = LuaInterpreter::GetSingleton()->getLuaState().registry()[openedLuaClass];
						String typeName = openedLuaClass;
						if (luaClass.is<sol::table>() && luaClass.as<sol::table>()["__type"] != sol::nil)
						{
							typeName = luaClass.as<sol::table>()["__type"]["name"];
						}

						EditorSystem::GetSingleton()->pushBoldFont();
						ImGui::Text("%s", typeName.c_str());
						EditorSystem::GetSingleton()->popFont();

						ImGui::SetNextItemOpen(true);
						showDocumentation(typeName, luaClass);
					}
					ImGui::ListBoxFooter();
				}

				ImGui::EndPopup();
			}

			if (ImGui::BeginPopupModal("Save", 0, ImGuiWindowFlags_AlwaysAutoResize))
			{
				ImGui::SetNextWindowSize({ ImGui::GetWindowWidth(), ImGui::GetWindowHeight() });
				ImGui::Text(String("Do you want to save " + SceneLoader::GetSingleton()->getCurrentScene()->getFullName() + "?").c_str());
				if (ImGui::Button("Save"))
				{
					if (m_PopupCause == "quit")
					{
						saveAll(nullptr);
						EventManager::GetSingleton()->call(RootexEvents::QuitEditorWindow);
					}
					else if (m_PopupCause == "create")
					{
						saveAll(nullptr);
						EventManager::GetSingleton()->call(EditorEvents::EditorCreateNewScene, newSceneName);
						loadingScene = "game/assets/scenes/" + newSceneName + ".scene.json";
						ImGui::CloseCurrentPopup();
						m_MenuAction = "";
					}
					else if (m_PopupCause == "open")
					{
						saveAll(nullptr);
						loadingScene = openSceneName;
						ImGui::CloseCurrentPopup();
						m_MenuAction = "";
					}
				}
				ImGui::SameLine();

				if (ImGui::Button("Don't Save"))
				{
					if (m_PopupCause == "quit")
					{
						EventManager::GetSingleton()->call(RootexEvents::QuitEditorWindow);
					}
					else if (m_PopupCause == "create")
					{
						EventManager::GetSingleton()->call(EditorEvents::EditorCreateNewScene, newSceneName);
						loadingScene = "game/assets/scenes/" + newSceneName + ".scene.json";
						ImGui::CloseCurrentPopup();
						m_MenuAction = "";
					}
					else if (m_PopupCause == "open")
					{
						loadingScene = openSceneName;
						ImGui::CloseCurrentPopup();
						m_MenuAction = "";
					}
				}
				ImGui::SameLine();

				if (ImGui::Button("Cancel"))
				{
					ImGui::CloseCurrentPopup();
					m_MenuAction = "";
				}
				ImGui::EndPopup();
			}

			if (ImGui::BeginPopup("About Rootex Editor"))
			{
				static Ref<ImageResourceFile> engineImage = ResourceLoader::CreateImageResourceFile("rootex/assets/rootex.png");
				static Ref<ImageResourceFile> editorImage = ResourceLoader::CreateImageResourceFile("editor/assets/editor.png");

				ImGui::BeginGroup();
				ImGui::Image(engineImage->getTexture()->getTextureResourceView(), { 200, 200 });
				ImGui::Text("Rootex Engine");
				ImGui::EndGroup();

				ImGui::SameLine();

				ImGui::BeginGroup();
				ImGui::Image(editorImage->getTexture()->getTextureResourceView(), { 200, 200 });
				ImGui::Text("Rootex Editor");
				ImGui::EndGroup();

				ImGui::Text(String("Rootex Engine and Rootex Editor developed by SDSLabs. Built on " + OS::GetBuildDate() + " at " + OS::GetBuildTime() + "\n" + "Source available at https://www.github.com/sdslabs/rootex").c_str());

				static Ref<TextResourceFile> license = ResourceLoader::CreateTextResourceFile("LICENSE");
				ImGui::Text("%s", license->getString().c_str());
				m_MenuAction = "";
				ImGui::EndPopup();
			}
			if (ImGui::BeginPopup("Proprietary Licenses"))
			{
				ImGui::Text("To be added");
				ImGui::EndPopup();
			}

			ImGui::SetNextItemWidth(ImGui::GetContentRegionAvailWidth());
			for (auto&& library : Application::GetSingleton()->getLibrariesPaths())
			{
				if (ImGui::BeginPopup(library.string().c_str(), ImGuiWindowFlags_AlwaysAutoResize))
				{
					Ref<TextResourceFile> license = ResourceLoader::CreateTextResourceFile(library.string() + "/LICENSE");
					ImGui::Text("%s", license->getString().c_str());
					m_MenuAction = "";
					ImGui::EndPopup();
				}
			}
			ImGui::EndMenuBar();
		}
	}

	static Atomic<int> progress;
	static float currentProgress = 0.0f;
	static int totalProgress = -1;

	if (!loadingScene.empty() && totalProgress == -1)
	{
		ImGui::OpenPopup("Load Scene");
		totalProgress = SceneLoader::GetSingleton()->preloadScene(loadingScene, progress);
		currentProgress = 0.0f;
	}

	if (ImGui::BeginPopupModal("Load Scene", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
	{
		ImGui::Text("Loading scene: %s", loadingScene.c_str());

		ImGui::SetNextItemWidth(ImGui::GetContentRegionAvailWidth());
		float target = progress.load() / (float)totalProgress;
		float velocity = (target - currentProgress) * Random::Float();
		currentProgress += velocity * 0.01f;
		ImGui::ProgressBar(currentProgress);

		if (totalProgress == progress)
		{
			EventManager::GetSingleton()->call(EditorEvents::EditorCloseScene);
			SceneLoader::GetSingleton()->loadPreloadedScene(loadingScene, {});
			SetWindowText(GetActiveWindow(), ("Rootex Editor: " + loadingScene).c_str());
			totalProgress = -1;
			progress = 0;
			loadingScene = "";
			ImGui::CloseCurrentPopup();
		}

		ImGui::EndPopup();
	}

	ImGui::End();
}

static HashMap<int, String> LuaTypeNames = {
	{ LUA_TNONE, "none" },
	{ LUA_TNIL, "lua_nil" },
	{ LUA_TSTRING, "string" },
	{ LUA_TNUMBER, "number" },
	{ LUA_TTHREAD, "thread" },
	{ LUA_TBOOLEAN, "boolean" },
	{ LUA_TFUNCTION, "function" },
	{ LUA_TUSERDATA, "userdata" },
	{ LUA_TLIGHTUSERDATA, "lightuserdata" },
	{ LUA_TTABLE, "table" },
	{ -0xFFFF, "poly" }
};

void EditorSystem::showDocumentation(const String& name, const sol::table& table)
{
	if (ImGui::TreeNodeEx(name.c_str(), ImGuiTreeNodeFlags_CollapsingHeader))
	{
		ImGui::Indent();
		for (auto [key, value] : table)
		{
			if (key.is<String>() && key.as<String>() != name && key.as<String>() != "class_check" && key.as<String>() != "class_cast")
			{
				if (value.is<sol::table>())
				{
					showDocumentation(name + "." + key.as<String>(), value.as<sol::table>());
				}
				else
				{
					String tag = LuaTypeNames[(int)value.get_type()];
					String prefix = name;
					String suffix;
					if (value.is<sol::function>())
					{
						suffix = "()";
					}
					else if (value.is<String>())
					{
						suffix = " = " + value.as<String>();
					}

					pushItalicFont();
					ImGui::TextColored(getSuccessColor(), "%s", tag.c_str());
					ImGui::SameLine();
					ImGui::TextColored(getWarningColor(), "%s", prefix.c_str());
					popFont();

					ImGui::SameLine();

					pushBoldFont();
					ImGui::MenuItem((" . " + key.as<String>() + suffix).c_str());
					popFont();
				}
			}
		}
		ImGui::Unindent();
	}
}

Variant EditorSystem::saveAll(const Event* event)
{
	if (SceneLoader::GetSingleton()->getCurrentScene())
	{
		MaterialLibrary::SaveAll();
		SceneLoader::GetSingleton()->saveScene(SceneLoader::GetSingleton()->getCurrentScene());
		PRINT("Successfully saved current scene: " + SceneLoader::GetSingleton()->getCurrentScene()->getFullName());
	}
	else
	{
		PRINT("No level is open. Did not save current scene");
	}
	return true;
}

Variant EditorSystem::autoSave(const Event* event)
{
	PRINT("Auto-saving current scene...");
	saveAll(nullptr);
	return true;
}

Variant EditorSystem::saveBeforeQuit(const Event* event)
{
	if (SceneLoader::GetSingleton()->getCurrentScene())
	{
		m_MenuAction = "Save";
		m_PopupCause = "quit";
	}
	else
	{
		EventManager::GetSingleton()->call(RootexEvents::QuitEditorWindow);
	}
	return true;
}

Variant EditorSystem::createNewScene(const Event* event)
{
	const String& sceneName = Extract<String>(event->getData());
	const String& newScenePath = "game/assets/scenes/" + sceneName + ".scene.json";
	if (OS::IsExists(newScenePath))
	{
		WARN("Found a level with the same name: " + newScenePath);
		return true;
	}

	Ref<Scene> newScene = Scene::CreateEmptyAtPath(newScenePath);
	newScene->setName(sceneName);
	SceneLoader::GetSingleton()->saveScene(newScene.get());
	return true;
}

Variant EditorSystem::createNewMaterial(const Event* event)
{
	const Vector<String>& materialInfo = Extract<Vector<String>>(event->getData());
	MaterialLibrary::CreateNewMaterialFile(materialInfo[0], materialInfo[1]);
	return true;
}

EditorSystem* EditorSystem::GetSingleton()
{
	static EditorSystem singleton;
	return &singleton;
}

ImColor ColorToImColor(Color& c)
{
	return ImColor(c.x, c.y, c.z, c.w);
}
