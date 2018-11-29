# Fun With HID

## Introduction

The aim of this project is to provide a fun embedded beginner project for the PJRC Teensy 3.1/3.2 that required no external hardware. This repo contains the code that turns a typical Teensy into an annoyance machine.

## How it works

The project uses platform IO and the mbed framework to enable the Teensy to emulate a USB HID (Human Interface Device), namely a keyboard/mouse combination. When plugged in to a computer, it has a configurable probability to run one, some, or all of the following functions every 15 seconds.

### Function 1

Presses the Alt-Tab key (configurable to Command-Tab for Macs)

### Function 2

Presses the Caps Lock key

### Function 3

Inputs a random number of random characters (typically 1-5)

### Function 4

Moves the mouse in a circle or a figure 8
