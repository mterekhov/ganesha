
#About

This repository is an attempt to develop 3D-engine on basis of VULKAN API for MacOS platform in framework format

# Some coding rules

- Every service should have fully abstract class as it's interface
- Everywhere services should be declared through it's interface classes
- Every service should eat GVULKANDevice reference in it's constructor
- Every service should have init()/destroy() methods
