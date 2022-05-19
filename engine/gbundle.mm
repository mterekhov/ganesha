#import <Foundation/Foundation.h>

#include "gbundle.h"

namespace spcGaneshaEngine {

std::string GBundle::resourceFullPath(const std::string& resourceName) {
    NSString *resourcePath = [NSString stringWithCString:resourceName.c_str()
                                                encoding:NSUTF8StringEncoding];
    return [[NSBundle.mainBundle pathForResource: resourcePath
                                          ofType: nil] UTF8String];
}

std::vector<uint8_t> GBundle::readFile(const std::string& filename) {
    FILE *file = fopen(filename.c_str(), "rb");
    if (file == 0) {
        printf("GaneshaEngine: no chance to read file %s\n", filename.c_str());
        return std::vector<uint8_t>();
    }
    
    fseek(file, 0L, SEEK_END);
    size_t fileSize = ftell(file);
    std::vector<uint8_t> buffer(fileSize);
    fseek(file, 0L, SEEK_SET);
    fread(buffer.data(), fileSize, 1, file);
    fclose(file);
    
    return buffer;
}

}   //  namespace spcGaneshaEngine
