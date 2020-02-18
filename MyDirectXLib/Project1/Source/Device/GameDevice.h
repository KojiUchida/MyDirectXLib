#pragma once

class Renderer;
class MeshManager;
class AudioManager;
class FontManager;

class GameDevice
{
public:
	static void initialize();
	static void update();
	static void shutdown();

	static Renderer* getRenderer();
	static MeshManager* getMeshManager();
	static AudioManager* getAudioManager();
	static FontManager* getFontManager();

private:
	GameDevice();
	~GameDevice();

	//�R�s�[�֎~
	GameDevice(const GameDevice& other) = delete;
	GameDevice& operator = (const GameDevice& other) = delete;

private:
	static GameDevice* instance;

	Renderer* m_pRenderer;
	MeshManager* m_pMeshManager;
	AudioManager* m_pAudioManager;
	FontManager* m_pFontManager;
};