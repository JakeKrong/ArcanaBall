#pragma once
#include <vector>
#include <unordered_map>
#include <typeindex>

#include <assert.h>
#include <stdexcept>

#include "Types.h"
#include "Component.h"

//Interface for ComponentArray
class IComponentArray {
public:
	virtual ~IComponentArray() = default;
	virtual void EntityDestroyed(Entity ent) = 0;
};

template<typename T>
class ComponentArray : public IComponentArray {
public:
	ComponentArray() {
		m_ComponentArray.resize(ENTITY_CAP);
	}

	template<typename... Args>
	void AddTComponent(Entity ent, Args&&...args) {
		assert(!m_EntityToComponentMap.contains(ent) && "Adding a component that is already held by entity!");

		auto existing = m_EntityToComponentMap.find(ent);
		if (existing != m_EntityToComponentMap.end()) {
			m_ComponentArray[existing->second] = { std::forward<Args>(args)... };
			return;
		}

		m_ComponentArray[m_Index] = { std::forward<Args>(args)... };

		m_EntityToComponentMap[ent] = m_Index;
		m_ComponentToEntityMap[m_Index] = ent;

		++m_Index;
	}

	void RemoveTComponent(Entity ent) {
		assert(m_EntityToComponentMap.contains(ent) && "Removing a component unassigned to an entity!");

		auto removed = m_EntityToComponentMap.find(ent);
		if (removed == m_EntityToComponentMap.end()) return;

		ComponentID removedIndex = removed->second;
		ComponentID lastIndex = m_Index - 1;

		if (removedIndex != lastIndex) {
			m_ComponentArray[removedIndex] = std::move(m_ComponentArray[lastIndex]);

			Entity lastEntity = m_ComponentToEntityMap[lastIndex];
			m_EntityToComponentMap[lastEntity] = removedIndex;
			m_ComponentToEntityMap[removedIndex] = lastEntity;
		}

		m_EntityToComponentMap.erase(ent);
		m_ComponentToEntityMap.erase(lastIndex);

		--m_Index;
	}

	void EntityDestroyed(Entity ent) override {
		if (m_EntityToComponentMap.find(ent) != m_EntityToComponentMap.end()) {
			RemoveTComponent(ent);
		}
	}

	T& GetTComponent(Entity ent) {
		auto it = m_EntityToComponentMap.find(ent);
		assert(it != m_EntityToComponentMap.end() && "Trying to get a component unassigned to an entity");

		if (it == m_EntityToComponentMap.end()) throw std::out_of_range("GetTComponent: entity holds no component of this type!");

		return m_ComponentArray[it->second];
	}

	std::vector<std::pair<Entity, T&>> GetAllTEntityComponent(){ //For testing only
		std::vector<std::pair<Entity, T&>> entComponent;
		
		for (auto& [ent, idx] : m_EntityToComponentMap) {
			entComponent.emplace_back(ent, m_ComponentArray[idx]);
		}
		return entComponent;
	}

private:
	std::vector<T> m_ComponentArray;
	std::unordered_map<Entity, ComponentID> m_EntityToComponentMap;
	std::unordered_map<ComponentID, Entity> m_ComponentToEntityMap;
	ComponentID m_Index = 0;
};

class ComponentManager {
public:
	ComponentManager() {};

	template<typename T>
	void RegisterComponent() {
		std::type_index typeInd = typeid(T);

		assert(m_ComponentTypeIds.find(typeInd) == m_ComponentTypeIds.end() && "Component Type already registered!");
		assert(m_CurrComponentId < COMPONENT_CAP && "Entity has surpassed allowed component cap!");

		m_ComponentTypeIds.insert({ typeInd, m_CurrComponentId });
		m_ComponentTypeArraysMap.insert({ typeInd, CreateScope<ComponentArray<T>>() });

		++m_CurrComponentId;
	}

	template<typename T>
	ComponentID GetComponentID() {
		auto it = m_ComponentTypeIds.find(typeid(T));
		assert(it != m_ComponentTypeIds.end() && "Component Type is not registered!");

		if (it == m_ComponentTypeIds.end()) throw std::out_of_range("GetComponentID: component type is not registered!");

		return it->second;
	}

	template<typename T>
	ComponentArray<T>& GetComponentArray() {
		auto it = m_ComponentTypeArraysMap.find(typeid(T));
		assert(it != m_ComponentTypeArraysMap.end() && "Trying to Get a Non-Registered Component's Array!");

		if (it == m_ComponentTypeArraysMap.end()) throw std::out_of_range("GetComponentArray: component type is not registered!");

		return *static_cast<ComponentArray<T>*>(it->second.get());
	}

	template<typename T, typename... Args>
	void AddComponent(Entity ent, Args&&...args) {
		std::type_index componentInd = typeid(T);
		assert(m_ComponentTypeIds.find(componentInd) != m_ComponentTypeIds.end() && "Component Type is not registered!");

		GetComponentArray<T>().AddTComponent(ent, std::forward<Args>(args)...);
	}

	template<typename T>
	void RemoveComponent(Entity ent) {
		GetComponentArray<T>().RemoveTComponent(ent);
	}

	template<typename T>
	T& GetComponent(Entity ent) {
		return GetComponentArray<T>().GetTComponent(ent);
	}

	template<typename T>
	bool SigHasComponent(Signature sig) {
		return sig[GetComponentID<T>()];
	}

	void DestroyEntComponents(Entity ent, Signature entSig) {
		for (auto& [typeInd, compId] : m_ComponentTypeIds) {
			if (entSig[compId]) m_ComponentTypeArraysMap.at(typeInd)->EntityDestroyed(ent);
		}
	}

private:
	std::unordered_map<std::type_index, ComponentID> m_ComponentTypeIds{};
	std::unordered_map<std::type_index, Scope<IComponentArray>> m_ComponentTypeArraysMap{};
	ComponentID m_CurrComponentId = 0;
};