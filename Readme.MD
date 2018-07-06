# Space Ranger

![Game Screenshot](https://image.ibb.co/nh1GUy/Screenshot_23.png)

A space ship battle game developed with pure OpenGL on C++.


## Environment Setup

- Solution is created with Visual Studio 2017.

- It's assumed that OpenGL, FreeGLUT and Windows SDK 8.1 is already installed correctly on the system. This guide can be followed to get your dev. environment ready: http://www.cs.uky.edu/~cheng/cs633/OpenGLInstallGuideWindows.pdf

- You should add .dll path manually into VS environment. To do that, go to Project Properties -> Configuration Properties -> Debugging -> Environment and set it to PATH=%PATH%;$(ProjectDir)\lib.

- Project must be compiled on x86 mode. (Release mode is preferred for performance)

- If a standalone exe is needed, media folder and fmod.dll & fmod64.dll should be on same directory with exe file.


## Features

This game includes other NPC ships, procedurally created spiral shaped universe, and a blaster shooting minigame as extra features. 


NPC Ships: 

- They're spawned randomly on universe, and will expire after sometime. Until a NPC ship expires, it will shoot its blaster randomly, and can hit player or other ships with this blaster. Since no AI is implemented, their shooting is fully random. Their movements are also random. 

- Ships can crash into planets, and destroy themselves by this way. Since NPC movements are fully random, they will mostly hit planets or the sun of universe on middle. (but it's still fun to play with them) 


Player Ships: 

- Player has two different ships initially. They're fully independent. Player has a ability to shoot blaster to destroy other NPC ships( D key ). A explosion sound effect will play if player hits another ship with blaster. 

- If player crashes  inti another NPC ship, NPC blaster or a planet, the ship will be destroyed with a smooth animation. Player still can switch to other ship and keep playing if it's not destroyed. 

- Ships has lighting parameters for random various materials.

- Camera shake will occur as ship's linear velocity increases.


Surface Matrix: 

- A 2d matrix-shaped surface can be seen in game world. This surface will refresh itself with new position of player if player comes close to bounds. So it will look like an infinite surface to player camera by this way. Also, this will improve performance drastically since we're not drawing whole surface at one time. 


Player Movement: 

- All velocity transitions are smoothed with linear interpolation. So, stopping, rotating or speeding will be more smooth. 


Universe Generation: 

- Galaxy's planet places are generated with Euler's spiral formula. Some planets will have a ring, and will spawn with different colors / sizes. Additionally, some stars are placed on distant places to center. 

- The only light source in scene is the sun on middle. This sphere's normals are inverted, so it's outer face will be lit instead of inner side ( light source is inside of the sphere) 

- Additionally, spheres are face culled. Their inner faces won't rendered.


Sound Effects:

- Sound effects are handled with FMOD.

- Ship engine sound effect will increase with linear velocity of current ship.

- Explosion sound effects will play if player hits another ship with it's blaster

- Other sound effects for ambience and blaster shooting


## Controls

- d -> Shoot laser

- z -> Stop each movement of ship

- a -> Increase linear velocity (keeping pressed down will continuously increase it)

- s -> Decrease linear velocity (keeping pressed down will continuously decrease it)

- l -> Increase rightwards angular velocity (keeping pressed down will continuously increase it)

- k -> Decrease rightwards velocity (keeping pressed down will continuously decrease it)

- b -> Swap player ships

- t -> Third Person View

- f -> First Person View

- w -> Top View

- q -> Quit from game

- Left Mouse Button -> (If paused) Continue game / Pause Game

- Right Mouse Button -> (If paused) Advance game by 1 frame / Pause Game


## Licence

Copyright (C) 2017-2018 Doğa Can YANIKOĞLU

This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with this program. If not, see http://www.gnu.org/licenses/

## Contributing

Contributors are encouraged to fork this repository and issue pull requests.
