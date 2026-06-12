#pragma once

#include <vector>
#include <unordered_map>
#include <typeindex>
#include <memory>

#include "BaseEvent.h"

class EventQueue {
public:
	template<typename T>
	void Publish(const T& event) {
		m_EventsMap[typeid(T)].push_back(std::make_unique<T>(event));
	}

	template<typename T>
	std::vector<T*> GetTEvents() {
		std::vector<T*> events;
		auto it = m_EventsMap.find(typeid(T));

		if (it != m_EventsMap.end()) {
			for (auto& eventPtr : it->second) {
				events.push_back(static_cast<T*>(eventPtr.get()));
			}
		}
		return events;
	}

	inline void ClearEvents() { m_EventsMap.clear(); }

private:
	std::unordered_map<std::type_index, std::vector<std::unique_ptr<BaseEvent>>> m_EventsMap;
};