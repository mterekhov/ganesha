#ifndef SPCGANESHAENGINE_GBUNDLE_H
#define SPCGANESHAENGINE_GBUNDLE_H

#include <string>
#include <vector>

namespace spcGaneshaEngine {

/// Finds resources
class GBundle {
public:
    std::string resourceFullPath(const std::string& resourceName);
    std::vector<uint8_t> readFile(const std::string& filename);
};

}   //  namespace spcGaneshaEngine

#endif  //  SPCGANESHAENGINE_GBUNDLE_H
