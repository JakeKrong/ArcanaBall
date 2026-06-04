# CMakeList.txt : CMake project for ArcanaBall, include source and define
# project specific logic here.

SET (GAME_SOURCES
 "Core/Types.h"
 "Core/EntityManager.h" "Core/EntityManager.cpp" "Core/ComponentManager.h"  
 "Components/Transform.h")

SET (INCLUDE_DIRS
	${CMAKE_CURRENT_SOURCE_DIR}/Core
	${CMAKE_CURRENT_SOURCE_DIR}/Components
)

# Add source to this project's executable.
add_executable (ArcanaBall "ArcanaBall.cpp" "ArcanaBall.h" ${GAME_SOURCES})

target_include_directories(ArcanaBall PRIVATE ${INCLUDE_DIRS})

target_link_directories(ArcanaBall PRIVATE 
	sfml-graphics
)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET ArcanaBall PROPERTY CXX_STANDARD 20)
endif()