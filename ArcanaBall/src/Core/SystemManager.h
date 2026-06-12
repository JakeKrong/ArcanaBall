#pragma once
#include <unordered_map>
#include <typeindex>
#include <assert.h>

#include "Types.h"
#include "ISystem.h"

class SystemManager {
public:
	SystemManager() {};
	~SystemManager() {};

	template<typename T>
	T& RegisterSys() {
		std::type_index sysInd = typeid(T);
		assert(m_SysSignatureMap.find(sysInd) == m_SysSignatureMap.end() && "Registering an already registered system!");

		auto registeredSystem = CreateScope<T>();
		T& systemRef = *registeredSystem;

		m_SysSignatureMap.insert({ sysInd, Signature{} });
		m_SystemMap.insert({ sysInd, std::move(registeredSystem) });

		return systemRef;
	}

	template<typename T>
	void SetSignature(Signature sig) {
		std::type_index sysInd = typeid(T);
		assert(m_SysSignatureMap.find(sysInd) != m_SysSignatureMap.end() && "Setting signature for an unregistered system!");
		m_SysSignatureMap[sysInd] = sig;
	}

	inline void EntitySigChanged(Entity ent, Signature oldSig, Signature newSig) {
		for (auto& [systemInd, sysSig] : m_SysSignatureMap) {

			if ((oldSig & sysSig) != sysSig) {
				if ((newSig & sysSig) == sysSig) m_SystemMap[systemInd]->AddEntity(ent);
			}
			else {
				if ((newSig & sysSig) != sysSig) m_SystemMap[systemInd]->RemoveEntity(ent);
			}
		}
	}

	inline void EntityDestroyed(Entity ent, Signature entSig) {
		for (auto& [systemInd, sysSig] : m_SysSignatureMap) {
			if ((entSig & sysSig) == sysSig) {
				m_SystemMap[systemInd]->RemoveEntity(ent);
			}
		}
	}

private:
	std::unordered_map<std::type_index, Signature> m_SysSignatureMap;
	std::unordered_map<std::type_index, Scope<ISystem>> m_SystemMap;
};