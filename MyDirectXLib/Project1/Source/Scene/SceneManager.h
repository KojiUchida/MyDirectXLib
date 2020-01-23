#pragma once
#include <string>
#include <unordered_map>

class AbstractScene;

//�V�[���Ǘ��N���X
class SceneManager
{
public:
	SceneManager();
	~SceneManager();

	//�X�V����
	void update();
	//�`�揈��
	void draw();

	//�V�[���Ǘ��ɃV�[���ǉ�
	void addScene(std::string key, AbstractScene* pAbstractScene);
	//�V�[���ύX
	void changeScene(std::string key);

private:
	//�R�s�[�R���X�g���N�^�֎~
	SceneManager(const SceneManager&) = delete;
	//����֎~
	SceneManager& operator = (const SceneManager&) = delete;

private:
	//���݂̃V�[����
	std::string m_CurrentScene;
	//�V�[���Ǘ��pmap
	std::unordered_map<std::string, AbstractScene*> m_Scenes;
};

