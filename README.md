# enderbin
*Linux File System Game*

![linux-only](https://img.shields.io/badge/linux-only-brightgreen)
![works-on](https://img.shields.io/badge/works%20on-my%20mashine%E2%84%A2-orange)
![fortnite-sucks](https://img.shields.io/badge/fortnite-sucks-blue)

enderbin is a small game you can play inside your file system. Your goal is to find and run the enderbin. Every time the binary is executed, it loses one life and teleports to a different nearby folder. If it reaches zero lives, you win!

## Setup
Download and build:
```
git clone https://github.com/slerpyyy/enderbin.git
cd enderbin
make
```
First hit:
```
./ender
```

## Why does this exist?
I wrote this for a friend of mine, who is new to Linux and not used to navigating the file system from inside the terminal. This was made to challenge their abilities.

## How does it keep track of its health?
Every time the program copies itself into a different directory, it scans through its own machine code, finds the health constant and replaces it. This way the program is fully self-contained, so you don't have to worry about any additional config files. If it somehow manages to jump onto a USB stick, you can eject it, plug it into a different machine and keep playing there without having to set up anything.

The way this is currently implemented, the program actually deletes itself while it's still running, which might be a problem on some machines, but I'm working on it.

## Extra credits
In case you couldn't tell by the name, this project is heavily inspired by the [Enderman](https://minecraft.gamepedia.com/Enderman) as found in the game Minecraft.
