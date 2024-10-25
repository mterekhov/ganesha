#ifndef SPCGANESHAENGINE_GSHADER_H
#define SPCGANESHAENGINE_GSHADER_H

#include <vulkan/vulkan.h>
#include <string>

namespace spcGaneshaEngine {

class GShader {
public:
    GShader(const std::string& shaderFileName);
    ~GShader();
    
    VkShaderModule getShaderModule();
    void setShaderModule(VkShaderModule newModule);
    std::string getShaderFileName();

private:
    const std::string shaderFileName;
    VkShaderModule shaderModule;
};

};  //  spcGaneshaEngine

#endif  //  SPCGANESHAENGINE_GSHADER_H
