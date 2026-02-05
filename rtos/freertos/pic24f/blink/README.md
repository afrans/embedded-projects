# <rtos - blink>

RTOS-based embedded firmware for Microchip PIC24F.
Project focuses on task scheduling, inter-task communication and
hardware abstraction.

## FreeRTOS-Kernel-10.3.0
- https://github.com/FreeRTOS/FreeRTOS-Kernel/releases/tag/V10.3.0

## Target
- MCU: PIC24FJ128GA306
- RTOS: FreeRTOS
- Language: C

## Toolchain
- IDE: MPLAB X v<5.35>
- Compiler: XC16 v<2.10>
- Programmer: PICkit3

## Build
- Configuration: Release
- Output: `dist/default/production/<blink.production>.hex`

## Notes
- FreeRTOS configured per project
- Only HEX file is versioned

## First Update GitHub
- Create repository in the github.com
```bash
 git init -b main
 git add .
 git commit -m "embedded-projects projects"
 git remote add origin https://github.com/afrans/embedded-projects.git
 git push -u origin main
```

## Update GitHub
```bash
git add .
git commit -m "embedded-projects projects"
git push -u origin main
```