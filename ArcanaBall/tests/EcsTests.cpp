#define CATCH_CONFIG_FAST_COMPILE
#include <catch2/catch_test_macros.hpp>

#include "Core/EntityManager.h"
#include "Core/ComponentManager.h"
#include "Core/SystemManager.h"
#include "System/ISystem.h"
#include "Core/Types.h"

TEST_CASE("EntityManager basic operations", "[EntityManager]") {
	EntityManager em;

	Entity a = em.PopEntity();
	Entity b = em.PopEntity();

	REQUIRE(a != b);
	// Set a signature bit and verify it is stored, then PushEntity resets it
	Signature sig;
	sig.set(2, true);
	em.SetSignature(a, sig);
	REQUIRE(em.GetEntSignature(a)[2] == true);

	em.PushEntity(a);
	REQUIRE(em.GetEntSignature(a).none());
}

TEST_CASE("ComponentManager add/get/remove components", "[ComponentManager]") {
	struct TestComp { int val = 0; };

	ComponentManager cm;
	cm.RegisterComponent<TestComp>();

	Entity e1 = 10;
	Entity e2 = 20;

	cm.AddComponent<TestComp>(e1, 42);
	cm.AddComponent<TestComp>(e2, 99);

	auto& comp1 = cm.GetComponent<TestComp>(e1);
	auto& comp2 = cm.GetComponent<TestComp>(e2);

	REQUIRE(comp1.val == 42);
	REQUIRE(comp2.val == 99);

	auto list = cm.GetComponentArray<TestComp>().GetAllTEntityComponent();
	REQUIRE(list.size() == 2);
	// Validate contents exist for e1 and e2
	bool foundE1 = false, foundE2 = false;
	for (auto& [ent, comp] : list) {
		if (ent == e1) foundE1 = (comp.val == 42);
		if (ent == e2) foundE2 = (comp.val == 99);
	}
	REQUIRE(foundE1);
	REQUIRE(foundE2);

	// Removing via EntityDestroyed should remove component entries
	cm.DestroyEntComponents(e1, cm.GetComponentID<TestComp>());
	auto list2 = cm.GetComponentArray<TestComp>().GetAllTEntityComponent();
	REQUIRE(list2.size() <= 1);
}

TEST_CASE("SystemManager entity signature change handling", "[SystemManager]") {
	struct TestSys : public ISystem {};
	SystemManager sm;

	TestSys& sys = sm.RegisterSys<TestSys>();

	Signature sysSig;
	sysSig.set(0, true);
	sm.SetSignature<TestSys>(sysSig);

	// Entity enters system when signature matches
	Signature oldSig; // empty
	Signature newSig;
	newSig.set(0, true);

	sm.EntitySigChanged(123, oldSig, newSig);
	auto ents = sys.returnEntities();
	REQUIRE(ents.size() == 1);
	REQUIRE(ents[0] == 123);

	// Entity leaves when signature no longer matches
	sm.EntitySigChanged(123, newSig, Signature{});
	auto ents2 = sys.returnEntities();
	REQUIRE(ents2.empty());
}