# GraphicsCortex
GraphicsCortex is a custom OpenGL and Physx based 3D graphics API to be used in the game engine and physics simulation applications. It aims to provide a platform to create high-performance physics simulations with realistic graphics. With tools to communicate between python scripts, it enables the training of deep learning agents in these environments. Since it is first created to train autonomous driving models, it has special tools to create vehicle simulations. Training neural networks is a compute resource heavy proceess itself, so the simulation used for it must be cheap in gpu usage and video memory, also be very fast. GraphicsCortex is developed to use the least amount of resources for simulation and rendering while providing the most amount of control to the user.

## Some Example Renders of Current Progress
### Lighting
![2022-06-17 03-23-26 (1)](https://user-images.githubusercontent.com/89701935/174199340-15e9d44f-4cc2-4c9e-bab4-3ad225bdc8f5.gif)
![2022-06-19 17-45-08](https://user-images.githubusercontent.com/89701935/174487262-86feab70-94c1-49bc-ad7a-2d9fef564669.gif)

### Normal Maps and Skybox
![2022-07-05 16-04-29](https://user-images.githubusercontent.com/89701935/177335628-fe006ca0-ab11-4886-b1b2-5b64be168c82.gif)

### Reflections
![2022-08-06 05-57-15 (1)](https://user-images.githubusercontent.com/89701935/183231156-bcd62505-c9f0-4050-827f-fcfc570db6c3.gif)
<image src="https://user-images.githubusercontent.com/89701935/179228243-30309a4f-569a-4a00-9861-ab79d5b5cc62.png" height=320 width=420>

### Other Renders
![sculpture](https://user-images.githubusercontent.com/89701935/202374528-6e2604d9-e2a3-4d29-9881-330640898932.png)

### Vehicles
  
![GraphicsCortexVehicleDemo(1)](https://user-images.githubusercontent.com/89701935/213333098-6b8915c9-c221-4070-954e-df2309116cc5.gif)

  
[Youtube Examples](https://www.youtube.com/watch?v=4sZXiHZbVMU&ab_channel=Furkan%C3%9Cnsal "Youtube Examples")

## Features (work in progress)

  ### Currently Used Libraries:
|    Library    |         Functionality           |
|:-------------:|:-------------------------------:|
|    **glew**   |OpenGL|
|    **GLFW**   |Window|
|    **glm**    |Math|
|   **assimp**  |3D Mesh Loading|
| **stb_image** |Image Read/Write/Compression/Decompression|
|   **physx**   |Physics Simulation|
|   **redis**   |In-Memory Communication Between Other Languages|
|  **FreeType** |Font File Loading|

## GraphicsCortex Roadmap for 2024

### Utils
Splines\
Python API\
CUDA/OpenCL connection?

### UI/UX
Editor\
Docking\
Object Hierarchy\
Prefabs\
Component System\
Mouse Controls\
Serialization\
Deserialization\
Hot code reloading

### Rendering
Shader Attribute and Layout detection\
Mesh Configuration\
PBR and BRDF\
RenderPasses and Shadows\
Implementing Vulkan\
Vulkan RTX Path Tracing\
Skeletal Animations\
Compute Shaders

### Physics
Transition to PhysX 5.1\
Scene Queries\
Collusion Detection\
Physics Visualization

### Advanced
Liquid Simulations\
Smoke Sİmulations\
Volumetric Rendering\
Cloth Simulations

## AlecTED simulation requirements
### Step 1:
A scene with anolog controllable vehicle from python, obstacles, target area.\ 
(missing: analog control, standarized python api, collusion detection)

### Step 2:
A scene in city with traffic and pedestrians
