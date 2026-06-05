#pragma once
#include <queue>
#include "Types.h"

//*****************************************************************************//
// Entity Manager
// Tracks available Entities [ID] to be assigned in queue (m_AvailableEntites)
// and their respective signatures (m_EntSignature, by index)
// ------------------------------------------------------------------------------
// > CreateEntity	- Pop ID from queue and assign to created Entity
// > Destroy Entity - Push ID back into queue and reset Entity's signature
// > GetEntSignature- Get Entity's signature (using index)
// > SetSignature	- Update Entity's signature
//*****************************************************************************//

class EntityManager {
public:
	EntityManager();
	~EntityManager();

	Entity PopEntity();
	void PushEntity(Entity);
	Signature GetEntSignature(Entity);
	void SetSignature(Entity, Signature);

private:
	std::queue<Entity> m_AvailableEntities;
	std::vector<Signature> m_EntSignatures;
};