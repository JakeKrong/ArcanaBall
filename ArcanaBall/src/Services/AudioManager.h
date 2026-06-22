#pragma once

#include <string>
#include <unordered_map>
#include <SFML/Audio/Sound.hpp>
#include <SFML/Audio/SoundBuffer.hpp>
#include <SFML/Audio/Music.hpp>

#include "AudioAsset.h"
#include "EventQueue.h"
#include "Types.h"

using AudioID = uint16_t;

struct ActiveChannel {
    AudioID id = 0;
    sf::Sound sound;
};

class AudioManager {
public:
    AudioManager();

    void PreloadAudio();

    AudioID PlayAudio(AudioAsset, bool loop = false);
    void StopAudio(AudioID id);
    void StopAllAudio();

    void UpdateChannels(EventQueue&);

    void PlayMusic(const std::string&, bool loop = true);
    void StopMusic();
    void SetMusicVolume(float);

    float getCurVolume();

private:
    static constexpr size_t MAX_AUDIO_CHANNELS = 64;

    std::unordered_map<AudioAsset, sf::SoundBuffer> m_Buffers;
    std::list<ActiveChannel> m_ActiveChannels;

    AudioID m_NextId = 1;
    sf::Music m_Music;
    float m_AudioVolume;

#if BuildForPlayable
    std::string m_SoundBasePath = "assets/audio/";
#else 
    std::string m_SoundBasePath = "../../../../ArcanaBall/assets/audio/";
#endif
};