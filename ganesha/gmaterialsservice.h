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
    virtual void init() = 0;
    virtual void destroy() = 0;
    
    virtual GVULKANImage *createMaterial(const std::string& imageFilePath) = 0;
    virtual std::map<std::string, GVULKANImage *>& getMaterialsMap() = 0;
    virtual GVULKANImage *findMaterial(const std::string& imageFilePath) = 0;

};

class GMaterialsService: public GMaterialsServiceProtocol {
public:
    GMaterialsService(GCommandServiceProtocol *commandService, GVULKANDevice& vulkanDevice);
    virtual ~GMaterialsService();

    void init() override;
    void destroy() override;

    GVULKANImage *createMaterial(const std::string& imageFilePath) override;
    std::map<std::string, GVULKANImage *>& getMaterialsMap() override;
    GVULKANImage *findMaterial(const std::string& imageFilePath) override;

private:
    GCommandServiceProtocol *commandService;
    GVULKANDevice& vulkanDevice;
    std::map<std::string, GVULKANImage *> materialsMap;
};

};  //  spcGaneshaEngine

#endif  //  SPCGANESHAENGINE_GMATERIALSSERVICE_H
