# Multiplayer Box-Movement Game

## Abstract
This project aims to develop a simple multiplayer game where players control boxes and move them from a designated start point (Point A) to an endpoint (Point B) in a 3D space. The game will incorporate basic 3D rendering, real-time user input handling, and networked multiplayer functionality. Players will navigate their boxes using keyboard inputs, and each player's movements will be synchronized across multiple game instances to create a shared game environment.


1. **3D Environment Setup**: 
   - Created a simple 3D plane using existing models and textures. Points A and B will be represented by distinct objects placed at specific coordinates.
   - The player-controlled box is a movable object within this environment.

2. **Player Controls**:
   - Implemented keyboard input handling to allow players to move the box in six directions: forward, backward, left, right, up, and down. 
   - Movement will be handled in small increments to ensure precise control.

3. **Networking and Synchronization**:
   - Implemented networking functionality using TCP sockets to synchronize the box’s position across multiple instances of the game.
   - Whenever a player moves their box, the new position will be sent to other players, ensuring that all instances reflect the same game state.

4. **Collision Detection and Win Condition**:
   - Implemented basic collision detection to determine when a box has reached Point B.
   - Once a box reaches the endpoint, the game will display a congratulatory message and reset the box to the start position.

5. **Testing and Iteration**:
   - Tested the game in a multiplayer setting to ensure smooth synchronization and responsiveness.
   - Iterate on the controls, networking code, and game logic based on testing feedback.

**3rd Party Components**:
- No additional third-party components are required beyond what has been covered in class. The project will utilize existing libraries and frameworks available within the development environment.

