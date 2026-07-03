# ArcanaBall

<img width="426" height="240" alt="ArcanaBall_Showcase" src="https://github.com/user-attachments/assets/0569babd-e9a0-4e13-9de9-00c63efc67fe" />


**ArcanaBall** is a creative interpretation of the classic brick-breaker genre built with **C++23** and **SFML 3.0**. The game combines traditional arcade gameplay with tactical elemental magic, allowing players to infuse their projectiles with magical attributes that trigger devastating elemental reactions.

---

## 📥 Download
**[Click here to download the latest version (v1.0)](https://github.com/JakeKrong/ArcanaBall/releases/tag/v1.0)**

1. Download the `ArcanaBall.zip` file.
2. Extract all files to a folder.
3. Run `ArcanaBall.exe` in the root folder.

## 🎮 Core Gameplay

Your objective is simple:

- Destroy every block in the stage.
- Prevent the ball from falling off the bottom of the screen.
- Use elemental abilities strategically to clear levels efficiently.

### ✨ Elemental Combat

The paddle can imbue the ball with magical elements. Different block types possess unique durability, resistances, and elemental weaknesses.

Combining elements creates powerful reactions, including:

- ❄️ **Ice Shatter**
- ⚡ **Lightning Cross**
- 🔥 **Overload**

Mastering these reactions is the key to clearing difficult stages before running out of lives.

---

## 🏗️ Engine Architecture

ArcanaBall is powered by a custom-built, data-oriented Entity Component System (ECS) designed for performance and scalability.

### Registry & Contiguous Storage

Components are stored contiguously in memory to maximize CPU cache locality and iteration performance.

### O(1) Entity Operations

The entity manager uses:

- Bitset component signatures
- ID recycling queues

This provides deterministic **O(1)** entity creation, destruction, and component lookup.

### Spatial Partitioning

Collision detection uses an optimized 2D grid broad-phase to significantly reduce unnecessary intersection tests.

### Deferred Event Queue

Systems communicate through a type-safe deferred event queue, preventing iterator invalidation and maintaining memory safety during gameplay.

---

# 🚀 Getting Started

## Prerequisites

The project automatically downloads external dependencies during configuration.

You'll need:

- **Visual Studio** with the **Desktop development with C++** workload

**or**

- A C++23-compatible compiler
- **CMake 3.24+**

Additionally:

- **Git** (required by CMake to fetch SFML automatically)

---

# 🔨 Building

## Option 1 — Visual Studio (Recommended)

1. Extract `ArcanaBall.zip`.
2. Open **Visual Studio**.
3. Select **Open a local folder**.
4. Choose the extracted `ArcanaBall` directory.
5. Visual Studio will detect the `CMakeLists.txt` and automatically configure the project.
6. Wait for CMake to finish downloading SFML.
7. Select **ArcanaBall.exe** as the startup item.
8. Click **Build** or press **F5** to run.

---

## Option 2 — Command Line

Open a terminal inside the project directory.

### Configure

```bash
mkdir build
cd build

cmake ..
```

### Build

```bash
cmake --build . --config Release
```

### Run

```bash
./ArcanaBall
```

> On Windows with Visual Studio generators, the executable will typically be located in:
>
> ```text
> build/Release/ArcanaBall.exe
> ```

---

## 🛠️ Technologies

- C++23
- SFML 3.0
- CMake 3.24+

---

## 📁 Project Structure

```text
ArcanaBall/
├── assets/
├── src/
├── include/
├── CMakeLists.txt
└── README.md
```

---

## 📜 License

This project is intended for educational and portfolio purposes.
