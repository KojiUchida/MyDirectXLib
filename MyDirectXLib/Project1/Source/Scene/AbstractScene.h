#pragma once
#include <string>

//�V�[���p���ۃN���X
class AbstractScene
{
public:
	AbstractScene() {};
	virtual ~AbstractScene() {};

	virtual void init() = 0;
	virtual void update() = 0;
	virtual void shutdown() = 0;

	virtual std::string nextScene() = 0;
	virtual bool isEnd() = 0;
};

