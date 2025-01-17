#pragma once

#include "common/common.h"
#include "entity.h"
#include "core/input/input_manager.h"

typedef unsigned int SceneID;

#define ROOT_SCENE_ID 1

struct SceneSettings
{
	ResourceCollection preloads;
	SceneID camera = ROOT_SCENE_ID;
	SceneID listener = ROOT_SCENE_ID;
	HashMap<String, InputScheme> inputSchemes;
	String startScheme = {};

	void drawSceneSelectables(Scene* scene, SceneID& toSet);
	void drawInputScheme(InputDescription& floatInput);
	void draw();
};

void to_json(JSON::json& j, const SceneSettings& s);
void from_json(const JSON::json& j, SceneSettings& s);

class Scene
{
public:
	enum class ImportStyle
	{
		/// If scene is not imported but created raw inside this scene
		Local,
		/// If scene is linked to another scene file
		External
	};

private:
	static Vector<Scene*> s_Scenes;

	SceneID m_ID;
	String m_Name;
	String m_FullName;
	ImportStyle m_ImportStyle;
	/// Contains the current file name if local, else contains the linked scene file
	String m_SceneFile;
	Ptr<Entity> m_Entity;
	SceneSettings m_Settings;

	Scene* m_ParentScene = nullptr;
	Vector<Ptr<Scene>> m_ChildrenScenes;

	bool checkCycle(Scene* child);

public:
	static void ResetNextID();

	static Ptr<Scene> Create(const JSON::json& sceneData, bool isACopy = false);
	static Ptr<Scene> CreateFromFile(const String& sceneFile);
	static Ptr<Scene> CreateEmpty();
	static Ptr<Scene> CreateEmptyAtPath(const String& sceneFile);
	static Ptr<Scene> CreateEmptyWithEntity();
	static Ptr<Scene> CreateRootScene();

	static Vector<Scene*> FindScenesByName(const String& name);
	static Scene* FindSceneByID(const SceneID& id);
	static const Vector<Scene*>& FindAllScenes();

	Scene(SceneID id, const String& name, const SceneSettings& settings, ImportStyle importStyle, const String& sceneFile);
	~Scene();

	Scene* findScene(SceneID scene);
	void reimport();

	void onLoad();
	bool snatchChild(Scene* child);
	bool addChild(Ptr<Scene>& child);
	bool removeChild(Scene* toRemove);

	void setEntity(Ptr<Entity>& entity) { m_Entity = std::move(entity); }
	void setName(const String& name);

	JSON::json getJSON() const;
	Vector<Ptr<Scene>>& getChildren() { return m_ChildrenScenes; }
	SceneID getID() const { return m_ID; }
	ImportStyle getImportStyle() const { return m_ImportStyle; }
	String getScenePath() const { return m_SceneFile; }
	Scene* getParent() const { return m_ParentScene; }
	Entity* getEntity() const { return m_Entity.get(); }
	const String& getName() const { return m_Name; }
	const String& getFullName() const { return m_FullName; }
	SceneSettings& getSettings() { return m_Settings; }
};
