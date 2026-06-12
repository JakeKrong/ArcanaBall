#pragma once
#include "Types.h"
#include "EntityManager.h"
#include "ComponentManager.h"
#include "SystemManager.h"
#include "EventQueue.h"

class Registry {
public:
	Registry();

//Disable copying, only allow moving
	Registry(const Registry&) = delete;
	Registry(Registry&&) noexcept;

	void ResetManagers();

// *** Entity functions ***//
	Entity CreateEntity();
	void DestroyEntity(Entity);

// *** Component functions ***//
	template<typename T>
	void RegisterComponent() {
		m_CompManager->RegisterComponent<T>();
	}

	template<typename T, typename...Args>
	void AddComponentToEntity(Entity ent, Args&&... args) {
		Signature oldEnttSig = m_EntManager->GetEntSignature(ent);
		m_CompManager->AddComponent<T>(ent, std::forward<Args>(args)...);

		Signature newEnttSig = oldEnttSig; 
		newEnttSig.set(m_CompManager->GetComponentID<T>(), true);

		m_EntManager->SetSignature(ent, newEnttSig);
		m_SysManager->EntitySigChanged(ent, oldEnttSig, newEnttSig);
	}

	template<typename T>
	void RemoveComponentFromEntity(Entity ent) {
		Signature oldEnttSig = m_EntManager->GetEntSignature(ent);
		m_CompManager->RemoveComponent<T>(ent);
		
		Signature newEnttSig = oldEnttSig;
		newEnttSig.set(m_CompManager->GetComponentID<T>(), false);

		m_EntManager->SetSignature(ent, newEnttSig);
		m_SysManager->EntitySigChanged(ent, oldEnttSig, newEnttSig);
	}

	template<typename T>
	ComponentID GetComponentID() {
		return m_CompManager->GetComponentID<T>();
	}

	template<typename T>
	T& GetEntityComponent(Entity ent) {
		return m_CompManager->GetComponent<T>(ent);
	}

	template<typename T>
	ComponentArray<T>& GetComponentArray() {
		return m_CompManager->GetComponentArray<T>();
	}

	template<typename T>
	bool EntityHasComponent(Entity ent) {
		Signature enttSig = m_EntManager->GetEntSignature(ent);
		ComponentID compId = m_CompManager->GetComponentID<T>();
		return (enttSig[compId]);
	}

	//(Ideally) for testing only
	template<typename T>
	std::vector<std::pair<Entity, T&>>GetAllComponents() {
		return m_CompManager->GetComponentArray<T>().GetAllTEntityComponent();
	}

// *** System functions ***//
	template<typename T>
	T& RegisterSystem() {
		return m_SysManager->RegisterSys<T>();
	}

	template<typename T>
	void SetSystemSignature(Signature sig) {
		m_SysManager->SetSignature<T>(sig);
	}

// *** Event Queue functions ***//
	EventQueue& GetEventQueue();

private:
	Scope<EntityManager> m_EntManager;
	Scope<ComponentManager> m_CompManager;
	Scope<SystemManager> m_SysManager;
	Scope<EventQueue> m_EventQ;
};