#pragma once
#include <vector>
#include <unordered_map>
#include <typeindex>

#include <assert.h>

#include "Types.h"

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
		m_ComponentArray[m_Index] = { std::forward<Args>(args)... };

		m_EntityToComponentMap[ent] = m_Index;
		m_ComponentToEntityMap[m_Index] = ent;

		++m_Index;
	}

	void RemoveTComponent(Entity ent) {
		
		ComponentID removedIndex = m_EntityToComponentMap[ent];
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
		return m_ComponentArray[m_EntityToComponentMap[ent]];
	}

	//For testing
	std::vector<std::pair<Entity, T&>> GetAllComponents(){
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

		m_ComponentTypeIds.insert({ typeInd, m_CurrComponentId });
		m_ComponentTypeArraysMap.insert({ typeInd, CreateScope<ComponentArray<T>>() });

		++m_CurrComponentId;
	}

	template<typename T>
	ComponentID GetComponentID() {
		std::type_index componentInd = typeid(T);
		assert(m_ComponentTypeIds.find(componentInd) != m_ComponentTypeIds.end() && "Component Type is not registered!");

		return m_ComponentTypeIds[componentInd];
	}

	template<typename T>
	ComponentArray<T>& GetComponentArray() {
		std::type_index componentInd = typeid(T);
		assert(m_ComponentTypeArraysMap.find(componentInd) != m_ComponentTypeArraysMap.end() && "Trying to Get a Non-Registered Component's Array!");
		return *static_cast<ComponentArray<T>*>(m_ComponentTypeArraysMap[componentInd].get());
	}

	template<typename T, typename... Args>
	void AddComponent(Entity ent, Args&&...args) {
		std::type_index componentInd = typeid(T);
		assert(m_ComponentTypeIds.find(componentInd) != m_ComponentTypeIds.end() && "Component Type is not registered!");

		GetComponentArray<T>().AddTComponent(std::forward<Args>(args)...);
	}

	template<typename T>
	void RemoveComponent(Entity ent) {
		std::type_index componentInd = typeid(T);
		GetComponentArray<T>().RemoveTComponent(ent);
	}

	template<typename T>
	T& GetComponent(Entity ent) {
		std::type_index componentInd = typeid(T);
		return GetComponentArray<T>().GetTComponent(ent);
	}

	template<typename T>
	bool SigHasComponent(Signature sig) {
		std::type_index typeInd = typeid(T);
		return (sig[m_ComponentTypeIds[typeInd]]);
	}

	void DestroyEntComponents(Entity ent, Signature entSig) {
		for (auto& [typeInd, compId] : m_ComponentTypeIds) {
			if (entSig[compId]) m_ComponentTypeArraysMap[typeInd]->EntityDestroyed(ent);
		}
	}

private:
	std::unordered_map<std::type_index, ComponentID> m_ComponentTypeIds{};
	std::unordered_map<std::type_index, Scope<IComponentArray>> m_ComponentTypeArraysMap{};
	ComponentID m_CurrComponentId = 0;
};