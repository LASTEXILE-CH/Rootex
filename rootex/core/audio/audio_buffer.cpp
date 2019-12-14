#include "audio_buffer.h"
#include "audio_system.h"

AudioBuffer::AudioBuffer(AudioResourceFile* audioFile)
    : m_AudioFile(audioFile)
{
}

AudioBuffer::~AudioBuffer()
{
}

ResourceFile* AudioBuffer::getAudioFile()
{
	return m_AudioFile;
}