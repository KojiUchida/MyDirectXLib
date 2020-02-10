#include "AudioSource.h"
#include "Device\GameDevice.h"
#include "Device\Resource\AudioManager.h"
#include "Device\Resource\AudioInstance.h"

AudioSource::AudioSource(GameObject * m_pUser)
	: AbstractComponent(m_pUser),
	m_Distance(0.0f),
	m_pAudioInstance(nullptr),
	m_AudioName("")
{
}

AudioSource::~AudioSource()
{
	if (m_pAudioInstance != nullptr)
		delete m_pAudioInstance;
}

void AudioSource::onStart()
{
}

void AudioSource::onUpdate()
{

}

void AudioSource::play(int loopCount)
{
	//�������ݒ肳��Ă��Ȃ�������return
	if (m_pAudioInstance == nullptr ||
		m_AudioName.empty()) return;

	if (m_pAudioInstance->isEnd())
		//�Đ����I����Ă�����V�����Đ�
		setAudio(m_AudioName, loopCount);
	else
		//�����łȂ��ꍇ�͓r������
		m_pAudioInstance->start();
}

void AudioSource::stop()
{
	//�������ݒ肳��Ă��Ȃ�������return
	if (m_pAudioInstance == nullptr) return;

	delete m_pAudioInstance;
	m_pAudioInstance = nullptr;
}

void AudioSource::pause()
{
	//�������ݒ肳��Ă��Ȃ�������return
	if (m_pAudioInstance == nullptr) return;

	m_pAudioInstance->pause();
}

void AudioSource::setAudio(std::string audioName, int loopCount)
{
	m_AudioName = audioName;

	//�������Đ����̏ꍇ�X�g�b�v
	if (m_pAudioInstance != nullptr)
	{
		m_pAudioInstance->stop();
		delete m_pAudioInstance;
	}

	//������ݒ�
	m_pAudioInstance =
		new AudioInstance(
			GameDevice::getAudioManager()->getXAudio2(),
			GameDevice::getAudioManager()->getAudioData(audioName),
			loopCount
		);
}

void AudioSource::setVolume(float volume)
{
	//�������ݒ肳��Ă��Ȃ�������return
	if (m_pAudioInstance == nullptr) return;

	m_pAudioInstance->setVolume(volume);
}

float AudioSource::getVolume()
{
	//�������ݒ肳��Ă��Ȃ�������return
	if (m_pAudioInstance == nullptr) return 0.0f;

	return m_pAudioInstance->getVolume();
}
