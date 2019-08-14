WASD - move forward, left, backward, right
Arrow keys - pitch, yaw camera
IJKLOP - light movement
1,2 - toggle front face culling
3,4 - toggle wireframe mode
5,6,7 - toggle point, directional, spotlight
F - Toggling of Fog
H - Toggle Fog Density to be 0.0001f
J - Toggle Fog Density to be 1.f
G - Toggle Night
Z - Toggle FogColor to be pink
X - Toggle FogColor to be green
C - Toggle FogColor to be blue

Features: 
- Skyplane (With moving texture)
- Terrain normal and lighting
- Moving Water[Ocean and Pond](GPass.vertexshader code edited)
- Multitexturing (Terrain, Rocks[Object])
- Sprite Animation (Tabby Cat)
- Fog (Changes color between night and day, changes start position between night and day.[Start position for day is further than night])
- Can change from night to day. 
- Object Animations. Pink slime and Puddle Slime(looks like a blue ufo).

- Tree billboard
- Particles and billboard:
1. Water (Day), SnowFlake(Night) (Dropping from sky)
2. Water Splashing (Also a Sprite Animation) (Disappear when its night)
3. Sky Lantern above the Lake (Travels up to sky)
4. Apples dropping from the tree
5. Glow that spawns around the glow slime, will disappear into the Slime(Slime and Glow only appears at night)
- Shadows (Directional Light) [Moving Light from Left to Right on x axis]
- 2 Point Light (1 at Pink plort, 1 at Blue Puddle Slime)