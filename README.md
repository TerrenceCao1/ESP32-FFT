# ESP32-FFT

Ultra-lightweight FFT (Fast Fourier Transform) implementation for the ESP32 microcontroler

## Overview

**ESP32-FFT** is tailored for ESP32-based projects, aiming to have:

- Low Memory Footprint
- Sufficient speed/efficiency for small microcontrollers
- Straightforward and user friendly API
- Compatibility with ESP-IDF/Arduino

Perfect for **[Audio Visualization](github.com/TerrenceCao1/AudioVis)**, **Signal Analysis**, or any application in need of frequency decomposition on a microcontroller.

## Features

- Radix-2 Cooley-Tukey FFT - Lightweight
- Pure C implementation (no other dependencies)
- Configurable FFT size (as long as it's a power of 2)

## Installation/Build

```bash
git clone https://github.com/TerrenceCao1/ESP32-FFT.git ./ESP32-FFT
mv ./ESP32-FFT /your/project/directory/components
```

Include in your CMake, or Makefile, or just remember it when you gcc.

```c
//put this at top of file that it's needed
#include "FFT.h"
```

## THINGS I NEEDA WORK ON

> Some README/Github/Project Stuff

- Create an examples folder
- Write a README section on **Usage**
- Run performance tests/limitations

> It's a little slower than the commercial ones for sale so these are some optimizations I will do:

- Bit-Reversal Reordering
- Precalculating Twiddle values
- Adding radix-4/8
- Precoding Trivial Recursion Cases
