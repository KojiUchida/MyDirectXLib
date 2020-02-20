#include "AudioInstance.h"
#include "WAVFileReader.h"
#include "AudioData.h"
#include <xaudio2.h>

AudioInstance::AudioInstance(IXAudio2 * pXAudio2, const AudioData & data, unsigned int loopCount)
{
	pXAudio2->CreateSourceVoice(&m_pSourceVoice, data.getWAVData().wfx);

	//�Đ��p�o�b�t�@
	XAUDIO2_BUFFER buffer = { 0 };
	buffer.AudioBytes = data.getWAVData().audioBytes;
	buffer.pAudioData = data.getWAVData().startAudio;
	buffer.Flags = XAUDIO2_END_OF_STREAM;
	//���[�v����ݒ�
	buffer.LoopCount = loopCount;

	//�o�b�t�@���\�[�X�{�C�X�ɐݒ�
	m_pSourceVoice->SubmitSourceBuffer(&buffer);
}

AudioInstance::~AudioInstance()
{
	//�Đ����Ȃ��~����
	if (!isEnd()) stop();

	m_pSourceVoice->DestroyVoice();
}

void AudioInstance::start()
{
	m_pSourceVoice->Start();
}

void AudioInstance::stop()
{
	m_pSourceVoice->Stop();
}

void AudioInstance::pause()
{
	m_pSourceVoice->Stop(XAUDIO2_PLAY_TAILS);
}

void AudioInstance::stopLoop()
{
	m_pSourceVoice->ExitLoop();
}

bool AudioInstance::isEnd()
{
	XAUDIO2_VOICE_STATE voiceState;
	m_pSourceVoice->GetState(&voiceState);
	return voiceState.BuffersQueued == 0;
}

void AudioInstance::setVolume(float volume)
{
	m_pSourceVoice->SetVolume(volume);
}

float AudioInstance::getVolume()
{
	float volume = 0.0f;
	m_pSourceVoice->GetVolume(&volume);
	return volume;
}

void AudioInstance::setPitch(float pitch)
{
	m_pSourceVoice->SetFrequencyRatio(pitch);
}

float AudioInstance::getPitch()
{
	float pitch = 0.0f;
	m_pSourceVoice->GetFrequencyRatio(&pitch);
	return pitch;
}
