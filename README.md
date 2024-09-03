# Multiplayer Box-Movement Game

## Overview
The Multiplayer Box-Movement Game is a simple 3D game where players control a box and attempt to move it from a designated start point (Point A) to an endpoint (Point B). The game supports multiple players, with real-time synchronization of movements across different game instances. Players use keyboard controls to navigate the box in a 3D space, and the goal is to reach Point B. Upon reaching the endpoint, the game displays a congratulatory message and resets the box to the start position.

## Features
- **3D Environment**: A simple 3D plane with marked start (Point A) and end (Point B) points.
- **Player Controls**: Use the following keys to control the box:
  - **W**: Move forward
  - **S**: Move backward
  - **A**: Move left
  - **D**: Move right
  - **Q**: Move up
  - **E**: Move down
- **Collision Detection**: The game checks if the player box has reached the endpoint.
- **Networking**: Supports multiple players with synchronized movements across different game instances.
- **Win Condition**: When a player reaches Point B, a congratulatory message is displayed, and the box resets to Point A.

## Installation

### Prerequisites
- C++ compiler (supporting C++11 or later)
- CMake (if building with CMake)
- OpenGL development libraries
- IrrKlang sound engine
- Networking libraries (e.g., Winsock for Windows)

### Build Instructions
1. **Clone the repository into the NewModule folder:**
   ```bash
   git clone https://github.com/saumyachauhan1/final_project_cs4900.git
   cd final_project_cs4900/
2. Run CMAKE GUI.bat  
