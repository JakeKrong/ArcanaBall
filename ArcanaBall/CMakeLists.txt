# CMakeList.txt : CMake project for ArcanaBall, include source and define
# project specific logic here.

SET (GAME_SOURCES
 "src/ArcanaBall.h"
 "src/Core/Types.h"
 "src/Core/EntityManager.h" "src/Core/EntityManager.cpp" 
 "src/Core/ComponentManager.h"  
 "src/Core/SystemManager.h" "src/System/ISystem.h" 
 "src/Core/Registry.h" "src/Core/Registry.cpp"
 "src/Core/EventQueue.h" 
 "src/Core/Game.h" "src/Core/Game.cpp"

 "src/Components/Transform.h" "src/Components/Renderable.h"  
 
 "src/Services/InputManager.h" "src/Services/InputManager.cpp" 
 
 "src/State/IState.h" "src/Components/Button.h" "src/State/MainMenuState.h" "src/State/MainMenuState.cpp")

# Add source to this project's executable.
add_executable (ArcanaBall "src/ArcanaBall.cpp" ${GAME_SOURCES})

SET (INCLUDE_DIRS
	${CMAKE_CURRENT_SOURCE_DIR}/src
	${CMAKE_CURRENT_SOURCE_DIR}/src/Core
	${CMAKE_CURRENT_SOURCE_DIR}/src/System
	${CMAKE_CURRENT_SOURCE_DIR}/src/Components
	${CMAKE_CURRENT_SOURCE_DIR}/src/Services
	{CMAKE_CURRENT_SOURCE_DIR}/src/State
)

target_include_directories(ArcanaBall PRIVATE ${INCLUDE_DIRS})

target_link_libraries(ArcanaBall PRIVATE 
	SFML::Graphics
	SFML::Window
	SFML::System
	SFML::Audio
)