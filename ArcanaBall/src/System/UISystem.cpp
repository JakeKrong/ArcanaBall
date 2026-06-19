#include "UISystem.h"

#include "Registry.h"
#include "Button.h"

#include "GameStateEvent.h"
#include "MenuNavEvent.h"
#include "AudioEvent.h"

#include <iostream>

void UISystem::Update(const InputState& input){
	if (!m_Registry || m_Entities.size() == 0) {
		return;
	}

	if (!(input.mouseClicked || input.mouseReleased || input.mouseHeld)) {
		return;
	}
	
	auto& buttonCompArr = m_Registry->GetComponentArray<Button>();
	auto& transCompArr = m_Registry->GetComponentArray<Transform>();

	if (input.mouseClicked) {
		for (const Entity ent : m_Entities) {
			Transform& transComp = transCompArr.GetTComponent(ent);

			sf::FloatRect clickArea(sf::Vector2f(input.mousePos), { 1, 1 });

			if (clickArea.findIntersection({ transComp.position, transComp.size })) {
				Button& buttonComp = buttonCompArr.GetTComponent(ent);
				//Perform click
				if (input.mouseClicked && buttonComp.isEnabled) {
					switch (buttonComp.buttonAction) {
					case ButtonAction::StartGame:
						m_Registry->GetEventQueue().Publish<GameStateEvent>({ GameStateEvent::Type::StartGame, buttonComp.buttonPayload });
						break;
					case ButtonAction::ExitGame:
						m_Registry->GetEventQueue().Publish<GameStateEvent>(GameStateEvent::Type::EndGame);
						break;
					case ButtonAction::ResumeGame:
						m_Registry->GetEventQueue().Publish<GameStateEvent>(GameStateEvent::Type::ContinueGame);
						break;
					case ButtonAction::MainMenu:
						m_Registry->GetEventQueue().Publish<GameStateEvent>(GameStateEvent::Type::MainMenu);
						break;
					case ButtonAction::RestartGame:
						m_Registry->GetEventQueue().Publish<GameStateEvent>({ GameStateEvent::Type::StartGame, buttonComp.buttonPayload });
						break;
					case ButtonAction::CloseMenuOverlay:
						m_Registry->GetEventQueue().Publish<MenuNavEvent>({ MenuNavEvent::NavScreen::MainMenu });
						break;
					case ButtonAction::SelectLevel:
						m_Registry->GetEventQueue().Publish<MenuNavEvent>({ MenuNavEvent::NavScreen::SelectLevel });
						break;
					case ButtonAction::OpenGuide:
						m_Registry->GetEventQueue().Publish<MenuNavEvent>({ MenuNavEvent::NavScreen::GameGuide });
						break;
					case ButtonAction::AdjustVolume:
					{
						buttonComp.checkHold = true;
						float minX = 1035, maxX = 1185;
						transComp.position.x = std::clamp(input.mousePos.x, minX, maxX);
						float newVolume = (transComp.position.x - minX) / ((maxX - minX) / 2) * DefaultVolumeSetting;
						m_Registry->GetEventQueue().Publish<VolumeChangedEvent>(newVolume);
						break;
					}
					default:
						break;
					}
					m_Registry->GetEventQueue().Publish<AudioEvent>(AudioAsset::UI_ButtonClick);
				}
			}
		}
	}
	else if (input.mouseReleased) {
		for (const Entity ent : m_Entities) {
			buttonCompArr.GetTComponent(ent).checkHold = false;
		}
	}
	else if (input.mouseHeld){
		for (const Entity ent : m_Entities) {
			Button& buttonComp = buttonCompArr.GetTComponent(ent);
			if (buttonComp.checkHold) {
				switch (buttonComp.buttonAction) {
				case(ButtonAction::AdjustVolume):
				{
					Transform& transComp = m_Registry->GetEntityComponent<Transform>(ent);
					float minX = 1035, maxX = 1185;
					transComp.position.x = std::clamp(input.mousePos.x, minX, maxX);
					float newVolume = (transComp.position.x - minX) / ((maxX - minX) /2) * DefaultVolumeSetting;
					m_Registry->GetEventQueue().Publish<VolumeChangedEvent>(newVolume);
					break;
				}
				default:
					break;
				}
			}
		}
	}
}

void UISystem::SetButtonsEnabled(bool enabled) {
	auto& buttonCompArr = m_Registry->GetComponentArray<Button>();
	for (Entity ent : m_Entities) {
		buttonCompArr.GetTComponent(ent).isEnabled = enabled;
	}
}