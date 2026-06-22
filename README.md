\# ArcanaBall



ArcanaBall is a creative interpretation brick-breaker game built using \*\*C++23\*\* and \*\*SFML 3.0\*\*. The game blends classic arcade breakout mechanics with a tactical layer of magical elements and status reactions. Players infuse their projectiles with elemental forces to trigger explosive chain reactions, offering a unique strategy-driven twist to the classic genre.



\## Core Gameplay \& Objectives



\* \*\*The Main Objective:\*\* Destroy every block present on the stage while carefully keeping the ball from falling off the bottom of the screen.

\* \*\*Tactical Attributes:\*\* Different block types populate each stage, featuring unique durability, resistances, and distinct elemental vulnerabilities.

\* \*\*Elemental Reactions:\*\* Infuse the ball with magical properties using the paddle. Strike blocks to trigger powerful combination reactions (such as \*Ice Shatter\* or \*Lightning Cross\*) to decimate various grid layouts efficiently before running out of lives.



\---



\## Architectural Features (Custom ECS Engine)



ArcanaBall runs on an optimized, data-oriented Entity Component System (ECS) built from scratch:



\* \*\*Registry \& Contiguous Storage:\*\* Maximizes CPU cache locality by storing components contiguously in memory arrays.

\* \*\*Constant Time O(1) Operations:\*\* The entity manager utilizes a bitset signature mapping paired with an ID recycling queue to achieve deterministic component access, creation, and destruction.

\* \*\*Spatial Partitioning Broad-Phase:\*\* Collision detection maps entities into an optimized 2D grid matrix to drastically reduce intersection checks.

\* \*\*Decoupled Event Queue:\*\* System communication is strictly decoupled via a type-safe, deferred event queue to avoid iterator invalidations and preserve memory safety across game states.



\---



\## Getting Started



\### Prerequisites



The project handles its own external library dependencies automatically. You only need the following installed on your host machine:

\* \*\*Visual Studio\*\* (with the \*Desktop development with C++\* workload checked)

\* Alternatively, a standalone compiler supporting \*\*C++23\*\* and \*\*CMake 3.24+\*\*

\* \*\*Git\*\* (required by CMake to automatically pull and fetch dependencies)



\---



\### Building the Project



\#### Option 1: Via Visual Studio (Recommended)

1\. Extract `ArcanaBall.zip`.

2\. Open Visual Studio and select \*\*Open a local folder\*\*.

3\. Choose the extracted `ArcanaBall` directory. Visual Studio will automatically detect the `CMakeLists.txt` and generate the cache (fetching SFML in the background).

4\. Select `ArcanaBall.exe` from the startup item dropdown and click \*\*Build / Run\*\*.



\#### Option 2: Via Command Line

1\. Extract `ArcanaBall.zip` and open a terminal inside the directory.

2\. Generate the build configuration (this step automatically fetches the SFML source):

&#x20;  ```bash

&#x20;  mkdir build

&#x20;  cd build

&#x20;  cmake ..

