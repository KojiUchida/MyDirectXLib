#pragma once
#include <string>
#include "Component\AbstractComponent.h"

class AudioInstance;

//�����Đ��p�R���|�[�l���g
class AudioSource
	: public AbstractComponent
{
public:
	AudioSource(GameObject* m_pUser);
	~AudioSource();

	virtual void onStart() override;
	virtual void onUpdate() override;

	//�Đ�(���[�v�񐔂�255�̏ꍇ�������[�v�B�r������Đ������ꍇ�͖�������܂��B)
	void play(int loopCount = 0);
	//�Đ���~
	void stop();
	//�Đ����f
	void pause();

	//������ݒ�(���[�v�񐔂�255�̏ꍇ�������[�v)
	void setAudio(std::string audioName, int loopCount = 0);

	//�{�����[���ݒ�(1.0f�Œʏ�)
	void setVolume(float volume);
	//�{�����[�����擾
	float getVolume();

	//�s�b�`�ݒ�(1.0f�Œʏ�)
	void setPitch(float pitch);
	//�s�b�`���擾
	float getPitch();

private:
	//float m_Distance;					//��������������͈�
	std::string m_AudioName;			//�����A�Z�b�g��
	AudioInstance* m_pAudioInstance;	//�����Đ��⏕
};