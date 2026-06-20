# CMakeList.txt : CMake project for ArcanaBall, include source and define
# project specific logic here.

SET (GAME_SOURCES
 "src/Core/Types.h"
 "src/Core/EntityManager.h" "src/Core/EntityManager.cpp" 
 "src/Core/ComponentManager.h"  
 "src/Core/SystemManager.h" "src/System/ISystem.h" 
 "src/Core/Registry.h" "src/Core/Registry.cpp"
 "src/Core/EventQueue.h" 
 "src/Core/Game.h" "src/Core/Game.cpp"
 "src/Core/StateManager.h" "src/Core/StateManager.cpp"
 "src/Core/Prefabs.h" "src/Core/Prefabs.cpp" 
 "src/Core/AudioAsset.h"

 "src/Components/Component.h" "src/Components/Transform.h" "src/Components/Renderable.h"
 "src/Components/Button.h" "src/Components/Physics.h" "src/Components/StatusEffect.h" 
 "src/Components/Collider.h" "src/Components/Block.h" "src/Components/Child.h" "src/Components/AnimationData.h"
 
 "src/Services/InputManager.h" "src/Services/InputManager.cpp"
 "src/Services/TextureManager.h" "src/Services/TextureManager.cpp"
 "src/Services/AudioManager.h" "src/Services/AudioManager.cpp"
 "src/Services/FileReader.cpp" "src/Services/FileReader.h"
 "src/System/BlockSystem.h" "src/System/BlockSystem.cpp" 

 "src/System/Systems.h" 
 "src/System/RenderSystem.h" "src/System/RenderSystem.cpp" 
 "src/System/UISystem.h" "src/System/UISystem.cpp" "src/System/CollisionSystem.h" "src/System/CollisionSystem.cpp"
 "src/System/PhysicsSystem.h" "src/System/PhysicsSystem.cpp"
 "src/System/HierarchySystem.h" "src/System/HierarchySystem.cpp"
 "src/System/AnimationSystem.h" "src/System/AnimationSystem.cpp"

 "src/State/IState.h"
 "src/State/MainMenuState.h" "src/State/MainMenuState.cpp"
 "src/State/PlayingState.h" "src/State/PlayingState.cpp"

 "src/Events/BaseEvent.h"
 "src/Events/GameStateEvent.h" "src/Events/SpawnEntityEvent.h"
 "src/Events/AudioEvent.h" "src/Events/PhysicsEvent.h"
 "src/Events/BlockCollisionEvent.h" "src/Events/EntityDestroyedEvent.h"
 "src/Events/MenuNavEvent.h" "src/Services/FontManager.h" "src/Services/FontManager.cpp")

# Add source to this project's executable.
add_executable (ArcanaBall "src/ArcanaBall.cpp" ${GAME_SOURCES})

SET (INCLUDE_DIRS
	${CMAKE_CURRENT_SOURCE_DIR}/src
	${CMAKE_CURRENT_SOURCE_DIR}/src/Core
	${CMAKE_CURRENT_SOURCE_DIR}/src/System
	${CMAKE_CURRENT_SOURCE_DIR}/src/Components
	${CMAKE_CURRENT_SOURCE_DIR}/src/Services
	${CMAKE_CURRENT_SOURCE_DIR}/src/State
	${CMAKE_CURRENT_SOURCE_DIR}/src/Events
)

target_include_directories(ArcanaBall PRIVATE ${INCLUDE_DIRS})

target_link_libraries(ArcanaBall PRIVATE 
	SFML::Graphics
	SFML::Window
	SFML::System
	SFML::Audio
)