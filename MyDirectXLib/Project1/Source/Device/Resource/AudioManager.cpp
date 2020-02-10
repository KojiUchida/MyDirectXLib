#include "AudioManager.h"
#include <xaudio2.h>
#include "AudioData.h"

#pragma comment(lib, "xaudio2.lib")

AudioManager::AudioManager()
{
	UINT32 flags = 0;
#ifdef _DEBUG
	//�f�o�b�O���Ȃ�f�o�b�O�p�I�v�V������t����
	flags |= XAUDIO2_DEBUG_ENGINE;
#endif
	//XAudio2�쐬
	XAudio2Create(&m_pXAudio2, flags);
	//MasteringVoice�쐬
	m_pXAudio2->CreateMasteringVoice(&m_pMasteringVoice);
}

AudioManager::~AudioManager()
{
	//�ǂݍ��񂾃f�[�^��S�ĉ��
	for (auto data : m_AudioDataList)
	{
		delete data.second;
	}
	m_pMasteringVoice->DestroyVoice();
	m_pXAudio2->Release();
	m_pXAudio2 = nullptr;
}

IXAudio2 * AudioManager::getXAudio2() const
{
	return m_pXAudio2;
}

IXAudio2MasteringVoice * AudioManager::getMasteringVoice() const
{
	return m_pMasteringVoice;
}

void AudioManager::loadWavFile(std::string key, std::wstring filePath)
{
	AudioData* pAudioData = new AudioData();
	pAudioData->init(filePath);
	m_AudioDataList.emplace(key, pAudioData);
}

const AudioData & AudioManager::getAudioData(std::string key) const
{
	return *m_AudioDataList.at(key);
}
