# ArcanaBall

<img width="426" height="240" alt="ArcanaBall gameplay" src="https://github.com/user-attachments/assets/0569babd-e9a0-4e13-9de9-00c63efc67fe" />

A brick breaker with an elemental twist, written in C++23 with SFML 3. The paddle can infuse the ball with fire, ice or lightning. Each block type reacts differently to each element, and hitting an infused block with a second element sets off a reaction that damages the blocks around it.

The game runs on a small custom entity component system (ECS), described under [Engine](#engine).

## Download

A prebuilt Windows version is on the [v1.0 release page](https://github.com/JakeKrong/ArcanaBall/releases/tag/v1.0). Download `ArcanaBall.zip`, extract it and run `ArcanaBall.exe` from the extracted folder.

## How to play

Clear every block on the stage without letting the ball fall past the paddle. You get three balls per stage.

| Input         | Action |
| Mouse         | Move the paddle |
| Left click    | Launch the ball |
| Q / W / E     | Infuse the paddle with fire / ice / lightning. Press the same key again to remove it. |
| Esc | Pause |

The ball takes on the paddle's element while it sits on the paddle and whenever it bounces off it.

Every hit does one point of damage. What an element does to a block depends on the block's resistance to it:
- Vulnerable: the block takes damage and holds the element.
- Susceptible: the block takes damage but doesn't hold the element.
- Immune: the block takes no damage.

Hitting a block that holds one element with a different element triggers a reaction:
| Fire + Ice | Ice Shatter | Damages the blocks surrounding it |
| Fire + Lightning | Overload | Explodes into the blocks directly above, below and to either side |
| Ice + Lightning | Lightning Cross | Sends bolts out along the block's row and column |

(Refer to in-game "How-to-play" guide for more info)

Each reaction also carries an element (ice, fire and lightning respectively), so it can infuse the blocks it hits or set off another reaction.

## Building

Requirements:

- A C++23 compiler. I developed this on Windows with MSVC (Visual Studio 2022 17.7 or newer), and haven't tried other compilers or platforms.
- CMake 3.24 or newer.
- Git. CMake uses it to download SFML 3.0.0 and Catch2 the first time you configure.

### Visual Studio

1. Open the repository with File > Open > Folder. Visual Studio reads `CMakePresets.json` and configures the project, which takes a while the first time because it downloads and builds SFML.
2. Pick the x64 Debug or x64 Release configuration and set `ArcanaBall.exe` as the startup item.
3. Press F5.

### Command line

```bash
cmake -S . -B build
cmake --build build --config Release
```

The build puts a copy of the `assets` folder next to the executable. The game looks for `assets` in the current working directory, so run it from the executable's folder. With the Visual Studio generator (the default on Windows) that's `build/ArcanaBall/Release`:

```bash
cd build/ArcanaBall/Release
./ArcanaBall.exe
```

With a single-configuration generator such as Ninja, the executable is in `build/ArcanaBall` instead.

### Tests

The ECS has Catch2 unit tests, built as `ArcanaBall_tests` next to the game:

```bash
ctest --test-dir build/ArcanaBall -C Release
```

## Engine

The ECS lives in `src/Core`.

Entities are 16-bit IDs handed out from a fixed pool and recycled after they're destroyed. Each entity has a bitset signature that records which components it has.

Each component type is stored in its own packed array. Removing a component moves the last element into the gap, so the array never has holes.

Systems declare which components they need. Whenever an entity gains or loses a component, the registry checks its new signature and adds it to or removes it from the matching systems.

Destroying an entity marks it dead immediately, but the actual cleanup (removing its components, taking it out of systems and returning its ID to the pool) waits until every system has finished updating for the frame. That way no system has its entity list change while it's looping over it.

Systems talk to each other through a typed event queue that is cleared every frame. They run in a fixed order, and each event is published by a system that runs before the one that reads it.

A few gameplay details built on top of that:

- Blocks sit on a fixed 10 x 8 grid, which doubles as the collision broad phase: the ball is only tested against blocks in the grid cells it overlaps. Collision responses are looked up by the pair of collider types involved (ball and block, ball and paddle, and so on).
- Physics runs on a fixed 144 Hz timestep.
- Blocks, the ball, effects and UI pieces are assembled from components by the functions in `Prefabs.cpp`.

### Level format

Levels are read from `assets/LevelData.txt` when the game starts:

```text
[Level 1]
0000000000
0101102220
0222011020
0101111020
0210101110
0100210020
0222001120
0000000000
```

Each level is a `[Level N]` header followed by 8 rows of 10 digits: `0` is empty, `1` stone, `2` brick, `3` wood and `4` steel. Lines starting with `#` are ignored, and a level with fewer than 8 rows is skipped with a warning. The menus currently expect exactly four levels.

## Project layout

```text
ArcanaBall/
├── CMakeLists.txt        Fetches SFML and Catch2
├── CMakePresets.json
└── ArcanaBall/
    ├── CMakeLists.txt    Game and test targets
    ├── assets/           Textures, audio, fonts and LevelData.txt
    ├── src/
    │   ├── Core/         ECS, game loop, state manager, prefabs
    │   ├── Components/
    │   ├── System/
    │   ├── Events/
    │   ├── Services/     Textures, audio, fonts, input, level loading
    │   └── State/        Main menu and gameplay states
    └── tests/            ECS unit tests
```

## License

This project is for educational and portfolio purposes.
