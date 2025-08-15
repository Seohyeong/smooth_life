# 🧬 SmoothLife & Game of Life in C

This project is a C-based implementation of two fascinating cellular automata:

- **Conway's Game of Life** — the classic zero-player game simulating life-like evolution on a grid.
- **SmoothLife** — a continuous generalization of Game of Life, designed by Stephan Rafler, where cells evolve based on smooth transitions and convolution-based rules.

It was built as a hands-on exercise to learn the C programming language and explore algorithmic simulation.

🎥 Demo Video: [SmoothLife Demo](assets/smoothlife_demo.mov)

---

## 🧠 What Is SmoothLife?

SmoothLife extends Conway’s discrete rules to a continuous domain:
- Instead of binary alive/dead states, cells have values between 0 and 1.
- Evolution is governed by convolution kernels and smooth transition functions.
- It allows for more fluid, organic patterns — like gliders that morph and flow.

For more background, check out [SmoothLife on SourceForge](https://sourceforge.net/projects/smoothlife/).

---

## File Structure

smooth_life/
├── game_of_life.c       # Classic Game of Life implementation
└── main.c               # SmoothLife simulation