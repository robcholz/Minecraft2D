# Minecraft2D

- Features: Original soundtracks and textures used(All the resources are updated to 1.19.4).
- The game is designed to be compatible with vanilla Minecraft resourcepacks(it will support resourcepacks).
  ![menu.png](docs%2Freadme%2Fmenu_aug_2023.png)

## Installations

### CMake: ```3.24 and above```

### SFML: ```2.5.1```

### Boost: ```1.81.0```

### C++: ```C++20```

## Build

### Windows

### OSX

```sh
git clone https://github.com/robcholz/Minecraft2D.git
cd Minecraft2D
mkdir build&&cd build
cmake -G "Unix Makefiles" ..
make
cd ..
./bin/MinecraftClient #run the game
```

### Linux


## Progress

- [x] Logger
- [x] Multi-language support
- [x] Widget GUI components
- [x] Sound manager
- [x] Peripherals support
- [x] Binary map file loading

- [x] Block
- [x] Save
- [x] World generation
- [ ] Decoration - in progress
- [ ] Gravity - waiting to be fixed
- [x] Entity
- [x] In-game GUI
- [x] Player
- [ ] Multiplayer
- [ ] Particle Effects
- [ ] Fullscreen mode
- [ ] Cross-platform


## Dependencies
- [Bitsery](https://github.com/fraillt/bitsery)
- [Json](https://github.com/nlohmann/json)
- [MemoryPool](https://github.com/appshift/mempool) (repo is private now)
- [ObjectPool](https://github.com/bitshifter/objectpool)
- [Plist](https://github.com/animetrics/PlistCpp)
- [Logger](https://github.com/SergiusTheBest/plog)
