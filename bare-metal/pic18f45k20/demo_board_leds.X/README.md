# PIC18F4520 Bare-Metal

C firmware for the PIC18F4520 microcontroller.  
Built with MPLAB X IDE v6.30 and XC8 v3.10.

## Build
Verify if 2 files are included: main and other for example: main.c and 
hello_main.c
For include/exclude file click right button and selected
Verify if #define RUN_HELLO is uncommented
Open the project in MPLAB X and run **Build Project**.

## Program
Using pickit3 Programmer or old mplab v8.91.

## First Update GitHub
Create repository in the github.com
 git init -b main
 git add .
 git commit -m "xxxxxxxxxxxxxx"
 git remote add origin https://github.com/afrans/embedded-projects.git
 git push -u origin main
 
## Update GitHub
 git add .
 git commit -m "xxxxxxxxxxxxxx"
 git push -u origin main

## Up files: .gitignore
/source/*.c
/source/*.h
nbproject (exceto private)
.gitignore
