
#About

This repository is an attempt to develop 3D-engine on basis of VULKAN API for MacOS platform in framework format

# Some coding rules

- Every service should have fully abstract class as it's interface
- Everywhere services should be declared through it's interface classes
- Every service should eat GVULKANDevice reference in it's constructor
- Every service should have init()/destroy() methods

# Layers, Events and Application

Every application should interact with engine through GApplication ancestor. The main purpose of GApplication is to establish connection between different system events and engine. In constructor of GApplication ancestor you should list layers which will be involved in interaction.

GLayer ancestror is some kind of logic which will be changing content. For example separate layer to process window resize or keyboard-mouse events. 

# Resources

As main format for geometry resources I selected gltf https://www.khronos.org/api/index_2017/Gltf