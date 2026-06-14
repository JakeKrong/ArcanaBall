#include "UISystem.h"

#include "Registry.h"
#include "Button.h"

#include "GameStateEvent.h"
#include "AudioEvent.h"

void UISystem::Update(const InputState& input){
	if (!m_Registry || m_Entities.size() == 0) {
		return;
	}

	if (!(input.mouseClicked || input.mouseReleased)) {
		return;
	}
	
	auto& buttonCompArr = m_Registry->GetComponentArray<Button>();
	auto& transCompArr = m_Registry->GetComponentArray<Transform>();

	for (const Entity ent : m_Entities) {
		Transform& transComp = transCompArr.GetTComponent(ent);

		sf::FloatRect clickArea(sf::Vector2f(input.mousePos), { 1, 1 });

		if (clickArea.findIntersection({ transComp.position, transComp.size })) {
			Button& buttonComp = buttonCompArr.GetTComponent(ent);
			//Perform click
			if (buttonComp.isOnPress && input.mouseClicked) {
				switch (buttonComp.buttonAction) {
				case ButtonAction::StartGame:
					m_Registry->GetEventQueue().Publish<GameStateEvent>(GameStateEvent::Type::StartGame);
					break;
				case ButtonAction::ExitGame:
					m_Registry->GetEventQueue().Publish<GameStateEvent>(GameStateEvent::Type::EndGame);
					break;
				default:
					break;
				}
				m_Registry->GetEventQueue().Publish<AudioEvent>(AudioAsset::UI_ButtonClick);
			}
			//Perform release
			else if (!buttonComp.isOnPress && input.mouseReleased){

			}
		}
	}
}