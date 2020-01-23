#include "SceneManager.h"
#include "AbstractScene.h"

SceneManager::SceneManager()
{
}

SceneManager::~SceneManager()
{
	for (auto pScene : m_Scenes)
	{
		delete &pScene;
	}
	m_Scenes.clear();
}

void SceneManager::update()
{
	if (m_CurrentScene.empty())
		return;

	m_Scenes.at(m_CurrentScene)->update();
}

void SceneManager::draw()
{
	if (m_CurrentScene.empty())
		return;

	m_Scenes.at(m_CurrentScene)->draw();
}

void SceneManager::addScene(std::string key, AbstractScene * pAbstractScene)
{
	m_Scenes.emplace(key, pAbstractScene);
}

void SceneManager::changeScene(std::string key)
{
	//�Â��V�[���̏I������
	m_Scenes.at(m_CurrentScene)->shutdown();

	//���݂̃V�[�������X�V
	m_CurrentScene = key;

	//���̊J�n����
	m_Scenes.at(m_CurrentScene)->init();
}
