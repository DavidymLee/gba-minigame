# Fly Swatter Game (GBA Mode 3)

**Author:** David Yoonmin Lee  
**Course:** CS 2110  
**Platform:** Game Boy Advance (GBA)  
**Language:** C (DMA Graphics, Embedded Systems)



## Game Overview

Fly Swatter is a **real-time reaction game** built for the Game Boy Advance.  
The player controls a swatter that moves across the screen using the directional buttons.  
A fly randomly spawns on the playfield — your goal is to **move the swatter over the fly and press the A button (X on keyboard)** to swat it.

Earn as many points as possible within **30 seconds!**



## Gameplay Summary

- **Objective:** Swat as many flies as you can before time runs out.  
- **Timer:** The game lasts 30 seconds.  
- **Win Condition:** 5 or more flies swatted → **Win Screen**  
- **Lose Condition:** Fewer than 5 flies → **Lose Screen**  
- **Restart:** Press **Select** at any time to reset.



## Technical Features

- **GBA Mode 3 graphics** with pixel-based rendering  
- **DMA (Direct Memory Access)** for fast image copying and background drawing  
- **State machine architecture** for transitions between START, PLAY, WIN, and LOSE screens  
- **Randomized fly spawning** and collision detection for hit registration  
- **Timer tracking** using the `vBlankCounter` variable  
- **Modular functions** for sprite drawing, erasing, and score display  
