#ifndef SPCGANESHAENGINE_GBUNDLE_H
#define SPCGANESHAENGINE_GBUNDLE_H

#include <string>
#include <vector>

#include "glog.h"

namespace spcGaneshaEngine {

/// Finds resources
class GBundle {
public:
    GBundle(GLog& log);
    ~GBundle();
    
    std::string resourceFullPath(const std::string& resourceName);
    std::vector<uint8_t> readFile(const std::string& filename);
    
private:
    GLog& log;
};

}   //  namespace spcGaneshaEngine

#endif  //  SPCGANESHAENGINE_GBUNDLE_H
