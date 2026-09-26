#define CATCH_CONFIG_FAST_COMPILE
#include <catch2/catch_test_macros.hpp>

#include "Core/EntityManager.h"
#include "Core/ComponentManager.h"
#include "Core/SystemManager.h"
#include "Core/Registry.h"
#include "System/ISystem.h"
#include "System/LifetimeSystem.h"
#include "Core/Types.h"

TEST_CASE("EntityManager basic operations", "[EntityManager]") {
	EntityManager em;

	Entity a = em.PopEntity();
	Entity b = em.PopEntity();

	REQUIRE(a != b);
	// Set a signature bit and verify it is stored
	Signature sig;
	sig.set(2, true);
	em.SetSignature(a, sig);
	REQUIRE(em.GetEntSignature(a)[2] == true);

	// Kill drops the live flag straight away but leaves the signature and the ID alone
	REQUIRE(em.IsEntityLive(a));
	em.KillEntity(a);
	REQUIRE_FALSE(em.IsEntityLive(a));
	REQUIRE(em.GetEntSignature(a)[2] == true);

	// Recycle is what actually clears the signature and returns the ID to the pool
	em.RecycleEntity(a);
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

	// Removing via EntityDestroyed should remove component entries.
	// DestroyEntComponents takes a Signature, so the component's bit has to be set in one.
	Signature destroySig;
	destroySig.set(cm.GetComponentID<TestComp>(), true);

	cm.DestroyEntComponents(e1, destroySig);
	auto list2 = cm.GetComponentArray<TestComp>().GetAllTEntityComponent();

	REQUIRE(list2.size() == 1);
	REQUIRE(list2[0].first == e2);
	REQUIRE(list2[0].second.val == 99); //Survivor's data must be intact after the swap-and-pop
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
	auto ents = sys.GetEntities();
	REQUIRE(ents.size() == 1);
	REQUIRE(ents[0] == 123);

	// Entity leaves when signature no longer matches
	sm.EntitySigChanged(123, newSig, Signature{});
	auto ents2 = sys.GetEntities();
	REQUIRE(ents2.empty());
}

TEST_CASE("Entity removal leaves a durable flag on the system", "[SystemManager]") {
	// CollisionSystem's block grid used to be repaired by a deferred BlockDestroyed event,
	// which ClearEvents discarded if the system did not run on the frame it landed (pause,
	// game-over screen). The flag that replaced it has to survive however many frames pass
	// before the system next runs.
	struct TestSys : public ISystem {
		bool RemovalPending() const { return m_EntitiesRemoved; }
		void ConsumeRemoval() { m_EntitiesRemoved = false; }
	};

	SystemManager sm;
	TestSys& sys = sm.RegisterSys<TestSys>();

	Signature sysSig;
	sysSig.set(0, true);
	sm.SetSignature<TestSys>(sysSig);

	Signature matching;
	matching.set(0, true);

	sm.EntitySigChanged(7, Signature{}, matching);
	REQUIRE(sys.GetEntities().size() == 1);
	REQUIRE_FALSE(sys.RemovalPending()); //An append must not look like a removal

	sm.EntityDestroyed(7, matching);
	REQUIRE(sys.GetEntities().empty());
	REQUIRE(sys.RemovalPending());

	// Still set after any number of frames in which the system never got to run
	REQUIRE(sys.RemovalPending());

	sys.ConsumeRemoval();
	REQUIRE_FALSE(sys.RemovalPending());

	// Destroying an entity this system never held must not raise the flag
	sm.EntityDestroyed(99, matching);
	REQUIRE_FALSE(sys.RemovalPending());
}

TEST_CASE("Destroy kills immediately but reaps later", "[Registry]") {
	struct Marker { int val = 0; };

	Registry reg;
	reg.RegisterComponent<Marker>();

	Entity a = reg.CreateEntity();
	Entity b = reg.CreateEntity();
	reg.AddComponentToEntity<Marker>(a, 11);
	reg.AddComponentToEntity<Marker>(b, 22);

	reg.DestroyEntity(a);

	// Dead the instant it is destroyed, so every IsEntityLive guard stays correct
	REQUIRE_FALSE(reg.IsEntityLive(a));
	REQUIRE(reg.IsEntityLive(b));

	// ...but the component is still readable until the reap, so references held by a
	// system part-way through its update do not dangle
	REQUIRE(reg.GetEntityComponent<Marker>(a).val == 11);
	REQUIRE(reg.GetAllComponents<Marker>().size() == 2);

	// The ID must not come back before it is reaped
	REQUIRE(reg.CreateEntity() != a);

	reg.ReapDestroyed();

	REQUIRE(reg.GetAllComponents<Marker>().size() == 1);
	REQUIRE(reg.GetAllComponents<Marker>()[0].first == b);
	REQUIRE(reg.GetEntityComponent<Marker>(b).val == 22); //Survivor intact after swap-and-pop
}

TEST_CASE("Repeat destroy queues the entity for reaping once", "[Registry]") {
	Registry reg;
	Entity a = reg.CreateEntity();

	reg.DestroyEntity(a);
	reg.DestroyEntity(a);
	reg.DestroyEntity(a);
	reg.ReapDestroyed();

	// A double-reaped ID would sit in the pool twice and be handed out to two live entities
	REQUIRE(reg.CreateEntity() != reg.CreateEntity());
}

TEST_CASE("Lifetime retires an entity after its last collision pass", "[LifetimeSystem]") {
	Registry reg;
	reg.RegisterComponent<Lifetime>();

	LifetimeSystem& sys = reg.RegisterSystem<LifetimeSystem>();
	sys.SetRegistry(&reg);

	Signature sig;
	sig.set(reg.GetComponentID<Lifetime>());
	reg.SetSystemSignature<LifetimeSystem>(sig);

	Entity onePass = reg.CreateEntity();
	Entity twoPass = reg.CreateEntity();
	reg.AddComponentToEntity<Lifetime>(onePass, 1);
	reg.AddComponentToEntity<Lifetime>(twoPass, 2);

	// A reaction collider is spawned with 1 and must survive to be seen by exactly one
	// collision pass, so the first decrement is what retires it.
	sys.Update();
	reg.ReapDestroyed();
	REQUIRE_FALSE(reg.IsEntityLive(onePass));
	REQUIRE(reg.IsEntityLive(twoPass));

	sys.Update();
	reg.ReapDestroyed();
	REQUIRE_FALSE(reg.IsEntityLive(twoPass));
}
