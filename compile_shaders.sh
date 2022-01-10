# Type a script or drag a script file from your workspace to insert its path.
echo ">>> compiling shaders"
echo ">>> compiling shader.vert..."
vulkansdk-macos-1.2.198.1/macOS/bin/glslc resources/shaders/shader.vert -o resources/shaders/vert.spv
echo ">>> compiling shader.frag"
vulkansdk-macos-1.2.198.1/macOS/bin/glslc resources/shaders/shader.frag -o resources/shaders/frag.spv
echo ">>> finished"
