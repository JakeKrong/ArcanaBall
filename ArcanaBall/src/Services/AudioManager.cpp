#include "AudioManager.h"
#include "AudioEvent.h"
#include "Types.h"

#include <print>

AudioManager::AudioManager(){
	m_AudioVolume = DefaultVolumeSetting;
}

void AudioManager::PreloadAudio() {
	auto loadAudio = [this](AudioAsset asset, const std::string& filePath) {
		sf::SoundBuffer buffer;
		if (buffer.loadFromFile(m_SoundBasePath + filePath + ".ogg")) {
			m_Buffers.emplace(asset, std::move(buffer));
		}
		else {
			std::println("[Audio Manager] Failed to load audio from {}!", filePath);
		}
	};

	//UI
	loadAudio(AudioAsset::UI_ButtonClick, "Button_Click");

	//FX
	loadAudio(AudioAsset::FX_Slowmo, "Slow_Mo");
	loadAudio(AudioAsset::FX_StoneBreak, "Stone_Break");
	loadAudio(AudioAsset::FX_BrickHit, "Brick_Hit");
	loadAudio(AudioAsset::FX_BrickBreak, "Brick_Break");
	loadAudio(AudioAsset::FX_WoodHit, "Wood_Hit");
	loadAudio(AudioAsset::FX_WoodBreak, "Wood_Break");
	loadAudio(AudioAsset::FX_SteelHit, "Steel_Hit");
	loadAudio(AudioAsset::FX_SteelBreak, "Steel_Break");

	//Elements
	loadAudio(AudioAsset::Elem_InfFire, "Fire_Infuse");
	loadAudio(AudioAsset::Elem_InfIce, "Ice_Infuse");
	loadAudio(AudioAsset::Elem_InfLightning, "Lightning_Infuse");
	loadAudio(AudioAsset::Elem_IceShatter, "IceShatter");
	loadAudio(AudioAsset::Elem_Overload, "Overload");
	loadAudio(AudioAsset::Elem_LightningCross, "LightningCross");
}

AudioID AudioManager::PlayAudio(AudioAsset asset, bool loop) {
	AudioID audioId = 0; //Set as uninitialised audio ID first (to leverage NRVO)

	if (!m_Buffers.contains(asset)) {
		return audioId;
	}

	if (m_ActiveChannels.size() >= MAX_AUDIO_CHANNELS) {
		return audioId;
	}

	audioId = m_NextId++;

	m_ActiveChannels.emplace_back(ActiveChannel{ audioId, sf::Sound(m_Buffers.at(asset)) });

	ActiveChannel& targetChannel = m_ActiveChannels.back();
	targetChannel.sound.setVolume(m_AudioVolume);
	targetChannel.sound.setLooping(loop);
	targetChannel.sound.play();

	return audioId;
}

void AudioManager::StopAudio(AudioID audioId) {
	if (!audioId) return;

	for (auto& channel : m_ActiveChannels) {
		if (channel.id == audioId) {
			channel.sound.stop();
			break;
		}
	}
}

void AudioManager::StopAllAudio() {
	for (auto& channel : m_ActiveChannels) {
		channel.sound.stop();
	}
	m_ActiveChannels.clear();
}

void AudioManager::UpdateChannels(EventQueue& eventQ) {

	//Check volume changed event
	for (auto event : eventQ.GetTEvents<VolumeChangedEvent>()) {
		m_AudioVolume = event->newVolume;
		SetMusicVolume(m_AudioVolume);
	}

	//Play queued audio events
	for (auto event : eventQ.GetTEvents<AudioEvent>()) {
		PlayAudio(event->asset, event->loop);
	}

	//Clear up Finished Audio
	std::erase_if(m_ActiveChannels, [](ActiveChannel& channel) {
		return channel.sound.getStatus() == sf::Sound::Status::Stopped;
		});
};

void AudioManager::PlayMusic(const std::string& filePath, bool loop) {
	m_Music.stop();

	if (!m_Music.openFromFile(m_SoundBasePath + filePath + ".ogg")) {
		std::println("[Audio Manager] Failed to stream track from {}!", filePath);
		return;
	}
	m_Music.setVolume(m_AudioVolume);
	m_Music.setLooping(loop);
	m_Music.play();
}

void AudioManager::StopMusic() {
	m_Music.stop();
}

void AudioManager::SetMusicVolume(float volume) {
	m_Music.setVolume(volume);
}

float AudioManager::getCurVolume() { return m_AudioVolume; }