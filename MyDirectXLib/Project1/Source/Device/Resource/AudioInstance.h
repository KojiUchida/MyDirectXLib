#pragma once
class AudioData;
struct IXAudio2;
struct IXAudio2SourceVoice;

//SourceVoice�Đ��⏕
class AudioInstance
{
public:
	//loopCount = 255�Ȃ疳�����[�v���܂��B
	AudioInstance(IXAudio2* pXAudio2, const AudioData& data, unsigned int loopCount = 0);
	~AudioInstance();

	//�Đ�
	void start();
	//�Đ���~
	void stop();
	//�Đ����~
	void pause();
	//���[�v�𒆎~(�Ō�܂ōĐ�����܂�)
	void stopLoop();

	//�Đ��I�����Ă��邩�ǂ���
	bool isEnd();

	//�{�����[���ݒ�(�{��)
	void setVolume(float volume);
	//�{�����[�����擾
	float getVolume();

private:
	IXAudio2SourceVoice* m_pSourceVoice;
};