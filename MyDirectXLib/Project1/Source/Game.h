#pragma once

class SceneManager;

//�Q�[���N���X
class Game
{
public:
	Game();
	~Game();

	void init();		//����������
	void update();		//�X�V����
	void draw();		//�`�揈��
	void shutdown();	//�I������

private:
	SceneManager* m_pSceneManager;
};

