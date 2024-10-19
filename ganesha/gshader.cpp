#include "gshader.h"

namespace spcGaneshaEngine {

GShader::GShader(const std::string& shaderFileName) : shaderFileName(shaderFileName), shaderModule(VK_NULL_HANDLE) {
}

GShader::~GShader() {
}

VkShaderModule GShader::getShaderModule() {
    return shaderModule;
}

void GShader::setShaderModule(VkShaderModule newModule) {
    shaderModule = newModule;
}

std::string GShader::getShaderFileName() {
    return shaderFileName;
}

};
