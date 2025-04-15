#include <WP_Scene/WP_Scene.h>
#include <imgui.h>

std::unique_ptr<WP_SceneManager> WP_SceneManager::s_sceneManager;

WP_SceneManager& WP_SceneManager::GetInstance()
{
	if (!s_sceneManager)
	{
		CreateInstance();
	}
	return *s_sceneManager;
}

void WP_SceneManager::CreateInstance()
{
	assert(!s_sceneManager);

	s_sceneManager = std::make_unique<WP_SceneManager>();
}

void WP_SceneManager::AddScene(const std::string& _stringName,
	std::unique_ptr<WP_Scene>&& _scene)
{
	m_scenes[_stringName] = std::move(_scene);
}

bool WP_SceneManager::SelectScene(const std::string& _sceneName)
{
	for (std::unordered_map<std::string, std::unique_ptr<WP_Scene>>::iterator i = m_scenes.begin();
		i != m_scenes.end(); ++i)
	{
		if (i->first == _sceneName)
		{
			m_currentScene = i;
			return true;
		}
	}
	std::cerr << "no scenes of specified name " << _sceneName << std::endl;
	return false;
}

void WP_SceneManager::RunSceneSelect()
{
	ImGui::Begin("SceneSelect");

	for (auto& i : m_scenes)
	{
		if (ImGui::Button(i.first.c_str()))
		{
			SelectScene(i.first);
		}
	}
	ImGui::End();
}

bool WP_SceneManager::CheckIfAnyValidScene()
{
	return m_currentScene.has_value();
}

void WP_SceneManager::RunPreSceneInitialisations()
{
	for (auto& i : m_scenes)
	{
		i.second.get()->PreSceneInitialisation();
	}
}

void WP_SceneManager::StartScene()
{
	if (CheckIfAnyValidScene() && !m_currentScene.value()->second.get()->isStartScene)
	{
		m_currentScene.value()->second.get()->StartScene();
		m_currentScene.value()->second.get()->isStartScene = true;
	}
}

void WP_SceneManager::RenderSceneImGui()
{
	if (CheckIfAnyValidScene())
		m_currentScene.value()->second.get()->RenderImGui();
}

void WP_SceneManager::UpdateScene()
{
	if (CheckIfAnyValidScene())
		m_currentScene.value()->second.get()->UpdateScene();
}

void WP_SceneManager::EndScene()
{
	if (CheckIfAnyValidScene())
	{
		m_currentScene.value()->second.get()->EndScene();
		m_currentScene.value()->second.get()->isStartScene = false;
	}
}

void WP_SceneManager::DeallocateScene()
{
	for (auto& i : m_scenes)
	{
		i.second.get()->DeallocateScene();
	}
}