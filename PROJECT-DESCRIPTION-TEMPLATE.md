# Lighthouse Night Scene

230/2021 - Anastasija Milojević 

A nighttime sea scene featuring a lighthouse and a moving boat. The scene uses directional, point and spot lighting. The boat
carries a point light, while the lighthouse uses a rotating spotlight with a visible beam. An event chain changes the scene lighting and turns on the lighthouse spotlight after specified time intervals.

## Controls

- W/A/S/D -> Move camera  
- Mouse -> Look around  
- ESC -> Close application
- P -> Turn boat point light on/off
- B -> Start/stop boat movement
- T -> Start event chain

## Features

### Fundamental:

- [x] Model with lighting
- [x] Two types of lighting with customizable colors and movement through GUI or ACTIONS
- [x] T --- AFTER_2_SECONDS---Triggers---> Directional light dims ---> AFTER_3_SECONDS---Triggers---> Lighthouse spotlight turns on

### Group A:

- [ ] Frame-buffers with post-processing   
- [ ] Off-screen Anti-Aliasing  
- [ ] Parallax Mapping
- [ ] Bloom with the use of HDR

### Group B:
- [ ] Deferred Shading  
- [ ] Point Shadows  
- [ ] SSAO

### Engine improvement:

- [ ] ...

## Models:

- [Boat model] (https://skfb.ly/oPTIs)
- [Lighthouse model] (https://skfb.ly/6SMQp)

## Textures

- [Sea water texture] (https://www.manytextures.com/texture/23/sea-water)
- [Night sky skybox] (https://lpc.opengameart.org/content/night-sky-skybox-generator)

