Space Ranger by Doða Can YANIKOÐLU
-----------------------------------------------------

Contents
--------
i. Environment Setup
ii. Extra Features
iii. Game Rules
iv. Controls
v. Bugs & Known Issues


i. Environment Setup
--------------------
-> Just launch project by opening "HW3.vcxproj". Project should be builded on x86 - Release mode. (It's important
that project should be launched on "Release", code should be optimized for getting optimal performance)

-> If it's needed to create a new project from scratch: fmod_vc.lib & fmod64_vc.lib libraries should be included in linker. Additionally; FMOD folder, media folder, fmod.dll, and fmod64.dll should be in project root directory.

-> If a standalone exe is needed: media folder, fmod.dll, and fmod64.dll should be on same directory with exe file.

ii. Extra Features
------------------
This game includes other NPC ships, procedurally created spiral shaped universe, and a blaster shooting minigame as extra features. 

NPC Ships: 
-> They're spawned randomly on universe, and will expire after sometime. Until a NPC ship expires, it will shoot its blaster randomly, and can hit player or other ships with this blaster. Since no AI is implemented, their shooting is fully random. Their movements are also random. 
-> Ships can crash into planets, and destroy themselves by this way. Since NPC movements are fully random, they will mostly hit planets or the sun of universe on middle. (but it's still fun to play with them) 

Player Ships: 
-> Player has two different ships initially. They're fully independent. Player has a ability to shoot blaster to destroy other NPC ships( D key ). A explosion sound effect will play if player hits another ship with blaster. 
-> If player crashes  inti another NPC ship, NPC blaster or a planet, the ship will be destroyed with a smooth animation. Player still can switch to other ship and keep playing if it's not destroyed. 
-> Ships has lighting parameters for random various materials.
-> Camera shake will occur as ship's linear velocity increases.

Surface Matrix: 
-> A 2d matrix-shaped surface can be seen in game world. This surface will refresh itself with new position of player if player comes close to bounds. So it will look like an infinite surface to player camera by this way. Also, this will improve performance drastically since we're not drawing whole surface at one time. 

Player Movement: 
-> All velocity transitions are smoothed with linear interpolation. So, stopping, rotating or speeding will be more smooth. 

Universe Generation: 
-> Galaxy's planet places are generated with Euler's spiral formula. Some planets will have a ring, and will spawn with different colors / sizes. Additionally, some stars are placed on distant places to center. 
-> The only light source in scene is the sun on middle. This sphere's normals are inverted, so it's outer face will be lit instead of inner side ( light source is inside of the sphere) 
-> Additionally, spheres are face culled. Their inner faces won't rendered.

Sound Effects:
-> Sound effects are handled with FMOD.
-> Ship engine sound effect will increase with linear velocity of current ship.
-> Explosion sound effects will play if player hits another ship with it's blaster
-> Other sound effects for ambience and blaster shooting

iii. Game Rules
-------------------
-> If player hits to a NPC blaster, player's ship is destroyed
-> If player hits to a planet or sun, player's ship is destroyed
-> If player hits another ship with its blaster, this ship is destroyed (player can destroy his/her second ship too)
-> If player and another ship crashes, both of them are destroyed.
-> Same rules are applicable for NPC ships.


iv. Controls
------------
z -> Stop each movement of ship
a -> Increase linear velocity (keeping pressed down will continuously increase it)
s -> Decrease linear velocity (keeping pressed down will continuously decrease it)
l -> Increase rightwards angular velocity (keeping pressed down will continuously increase it)
k -> Decrease rightwards velocity (keeping pressed down will continuously decrease it)
b -> Swap player ships
t -> Third Person View
f -> First Person View
w -> Top View
q -> Quit from game
Left Mouse Button -> (If paused) Continue game / Pause Game
Right Mouse Button -> (If paused) Advance game by 1 frame / Pause Game


v. Bugs & Known Issues
-----------------------
-> Hitting other ships with blaster is a bit hard due to sizes and speeds of NPC ships.
-> Controls are a bit weird because of instructions in homework paper, rotating the ship can be hard.
-> Rarely camera view can focus somewhere different than ship's destroyed parts when player's ship is destroyed
-> Top view shows the ships very small, because of the sizes of planets.