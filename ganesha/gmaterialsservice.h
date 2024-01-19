#ifndef SPCGANESHAENGINE_GMATERIALSSERVICE_H
#define SPCGANESHAENGINE_GMATERIALSSERVICE_H

#include <string>
#include <map>
#include <vulkan/vulkan.h>

#include "gvulkanimage.h"
#include "gvulkandevice.h"
#include "gcommandservice.h"

namespace spcGaneshaEngine {

class GMaterialsServiceProtocol {
public:
    virtual GVULKANImage *createMaterial(const std::string& imageFilePath, GVULKANDevice& vulkanDevice) = 0;
    virtual std::map<std::string, GVULKANImage *>& getMaterialsMap() = 0;
    virtual GVULKANImage *findMaterial(const std::string& imageFilePath) = 0;
    virtual void destroyMaterials(VkDevice device) = 0;

};

class GMaterialsService: public GMaterialsServiceProtocol {
public:
    GMaterialsService(GCommandServiceProtocol *commandService);
    virtual ~GMaterialsService();

    GVULKANImage *createMaterial(const std::string& imageFilePath, GVULKANDevice& vulkanDevice);
    std::map<std::string, GVULKANImage *>& getMaterialsMap();
    GVULKANImage *findMaterial(const std::string& imageFilePath);
    void destroyMaterials(VkDevice device);

private:
    GCommandServiceProtocol *commandService;
    std::map<std::string, GVULKANImage *> materialsMap;
};

};  //  spcGaneshaEngine

#endif  //  SPCGANESHAENGINE_GMATERIALSSERVICE_H
