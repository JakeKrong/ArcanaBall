#pragma once

#include <vector>
#include <unordered_map>
#include <typeindex>
#include <memory>
#include <variant>


// Can move event to separate .h file if becomes bloated
class BaseEvent {
	virtual ~BaseEvent() = default;
};

class TempEvent : public BaseEvent {

};

class SpawnEntityEvent : public BaseEvent {
	enum class Type { Enemy, BloodEffect, Etc };
	Type type;

	std::variant<int, float> payload; //Insert data for each class type
};

class EventQueue {
public:
	template<typename T>
	void Publish(const T& event) {
		m_EventsMap[typeid(T)].push_back(std::make_unique<T>(event));
	}

	template<typename T>
	std::vector<T*> GetEventQueue() {
		std::vector<T*> events;
		auto it = m_EventsMap.find(typeid(T));

		if (it != m_EventsMap.end()) {
			for (auto& eventPtr : it->second) {
				events.push_back(static_cast<T*>(eventPtr.get()));
			}
		}
		return events;
	}

	void ClearEvents() { m_EventsMap.clear(); }

private:
	std::unordered_map<std::type_index, std::vector<std::unique_ptr<BaseEvent>>> m_EventsMap;
};