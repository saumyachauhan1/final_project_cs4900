# Multiplayer Box-Movement Game: Final Report

## Introduction

The Multiplayer Box-Movement Game is a simple yet engaging 3D game where players control a box and attempt to move it from a designated start point (Point A) to an endpoint (Point B). The game supports multiple players by synchronizing the movements and positions of the boxes across different instances of the game. This report provides an overview of the game’s design, implementation, and functionality.

## Game Overview

### Objective
The primary objective of the game is for the player to move their box from the start point (Point A) to the endpoint (Point B). Upon reaching Point B, the game displays a congratulatory message and resets the box to Point A, allowing the player to repeat the challenge. The game is designed to support multiple players, with each player’s actions synchronized across all game instances.

### Controls
The game uses keyboard inputs for controlling the box:
- **W**: Move forward
- **S**: Move backward
- **A**: Move left
- **D**: Move right
- **Q**: Move up
- **E**: Move down

### Networking and Synchronization
The game features a networking component that allows it to synchronize the positions of boxes across multiple instances. When a player moves their box, the new position is sent to the other game instances, ensuring that all players see the same game state.

## Implementation Details

### 3D Environment Setup
The game’s 3D environment consists of a simple plane with two marked points: Point A (the start) and Point B (the endpoint). The player-controlled box starts at Point A, and the player must navigate the box to Point B using the provided controls.

### Collision Detection and Win Condition
A key feature of the game is collision detection, which checks whether the player's box has reached Point B. This is implemented by measuring the distance between the box and the endpoint. If the box is within a threshold distance of 0.5 units from Point B, the game recognizes that the box has reached its destination, displays a congratulatory message, and resets the box to Point A.

### Networking
The game uses TCP sockets for networking, enabling real-time synchronization of box positions across different game instances. When a player moves their box, the updated position is serialized and sent to other instances, which then update their display to reflect the new position.

### Testing and Iteration
The game was tested in a multiplayer environment to ensure that synchronization is smooth and that the game state is consistent across all instances. Testing focused on verifying that movements are correctly transmitted and that the win condition (reaching Point B) works reliably in all instances.

## Conclusion

The Multiplayer Box-Movement Game successfully demonstrates the integration of 3D object manipulation, user input handling, collision detection, and networked multiplayer functionality in a simple yet effective way. Players can move their boxes in a shared environment, with each instance of the game reflecting the actions of all players in real-time. This project serves as a practical application of game development concepts, particularly in the areas of networking and synchronization.

### Future Enhancements
Future work could focus on adding more complex game mechanics, such as obstacles, scoring systems, and additional levels. Enhancing the graphical fidelity and introducing more sophisticated networking techniques, such as UDP or dedicated server-client architecture, could further improve the game’s performance and user experience. Additionally, expanding the multiplayer aspect to allow more players and introducing competitive or cooperative gameplay modes could add more dept

