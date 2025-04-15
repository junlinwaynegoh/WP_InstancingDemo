#ifndef WP_SCENE_H
#define WP_SCENE_H
#include <memory>
#include <map>
#include <unordered_map>
#include <string>
#include <assert.h>
#include <iostream>
#include <optional>

struct WP_Scene
{
	bool isStartScene = false;

	virtual void PreSceneInitialisation() {}
	virtual void StartScene() {}
	virtual void RenderImGui() {}
	virtual void UpdateScene() {}
	virtual void EndScene() {}
	virtual void DeallocateScene() {}
};

class WP_SceneManager
{
private:
	std::unordered_map<std::string, std::unique_ptr<WP_Scene>> m_scenes;
	std::optional<std::unordered_map<std::string, std::unique_ptr<WP_Scene>>::iterator> m_currentScene;

	static std::unique_ptr<WP_SceneManager> s_sceneManager;

public:
	WP_SceneManager() {}
	~WP_SceneManager()
	{}

	static WP_SceneManager& GetInstance();

	static void CreateInstance();

	void AddScene(const std::string& _stringName,
		std::unique_ptr<WP_Scene>&& _scene);

	bool SelectScene(const std::string& _sceneName);

	void RunSceneSelect();

	bool CheckIfAnyValidScene();

	void RunPreSceneInitialisations();

	void StartScene();
	void RenderSceneImGui();
	void UpdateScene();
	void EndScene();
	void DeallocateScene();
};

#endif