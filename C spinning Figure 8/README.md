# C Spinning Figure 8

A terminal-based animation that renders a rotating 3D figure-8 shape using ASCII characters.

## Description

This program creates a visually appealing 3D figure-8 animation in the terminal using ASCII art. The figure-8 is constructed from two connected toroids (donut shapes) and displays with lighting effects to enhance the 3D appearance. The animation continuously rotates, giving a dynamic view of the shape.

## Features

- Smooth 3D rotation of a figure-8 shape
- ASCII character-based rendering with shading
- Interactive controls:
  - Press 'P' to pause/resume the animation
  - Press 'Q' to quit the program
- Terminal optimizations to prevent mouse interaction and ensure smooth rendering

## Technical Details

- Written in C
- Uses mathematical transformations to create a 3D effect in a 2D terminal
- Implements a figure-8 by connecting two toroids
- Handles terminal settings for non-blocking input
- Calculates lighting based on surface normals

## Compilation

A Makefile is included for easy compilation:

```
make
```

This will create the executable in the `bin` directory.

## Running the Program

After compilation, run the program with:

```
./bin/figure8
```

## Requirements

- C compiler (GCC recommended)
- POSIX-compliant terminal
- Math library (`-lm` linker flag)

## How It Works

The program uses mathematical principles to:
1. Generate two toroids positioned to form a figure-8
2. Apply rotation transformations on 3D coordinates
3. Project the 3D points onto the 2D terminal screen
4. Calculate lighting based on surface normals
5. Render the projection using ASCII characters for different lighting levels
