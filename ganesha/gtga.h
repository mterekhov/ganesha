#ifndef SPCGANESHAENGINE_GTGA_H
#define SPCGANESHAENGINE_GTGA_H

#include <stdio.h>

#include "ganeshatypes.h"

namespace spcGaneshaEngine {

/// Length of comments in tga file
typedef uint8_t TIdentityLength;

/// Palette type in tga file
typedef uint8_t TPaletteType;

/// Type of image in tga file
typedef uint8_t TImageType;

/// Palette offset in tga file
typedef uint16_t TPaletteOffset;

/// Palette size in tga file
typedef uint16_t TPaletteSize;

/// @struct SFileHeader
/// Header of tga file
struct SFileHeader {
    /// Length of comments
    TIdentityLength identityLength;
    
    /// Palette type
    TPaletteType paleteType;
    
    /// Type of image
    TImageType imageType;
};

/// @struct SPaletteHeader
/// Palette header of tga file
struct SPaletteHeader {
    /// Offset of palette
    TPaletteOffset offset;
    
    /// Size of palette
    TPaletteSize size;
    
    /// Bits per pixel
    uint32_t bitpp;
};

/// @struct SImageHeader
/// Image header
struct SImageHeader {
    /// X coord
    uint16_t x_coord;
    
    /// Y coord
    uint16_t y_coord;
    
    /// Width
    uint32_t width;
    
    /// Height
    uint32_t height;
    
    /// Bits per pixel
    uint32_t bitpp;
    
    /// ???
    uint8_t image_descriptor;
};

struct STGAHeader {
    SFileHeader fileHeader;
    SImageHeader imageHeader;
    SPaletteHeader paletteHeader;
};

/// Size of SFileHeader structure in bytes
#define FILE_HEADER_SIZE 3

/// Size of SPaletteHeader structure in bytes
#define PALETTE_HEADER_SIZE 5

/// Size of SImageHeader structure in bytes
#define IMAGE_HEADER_SIZE 10

/**
    Type of image in TGA file format
*/
enum EImageType {
    /// No image data in file
    TGA_NOT_IMAGE_DATA,

    /// Palette image
    TGA_PALETTE_IMAGE,

    /// True-color image
    TGA_TRUE_COLOR_IMAGE,

    /// Gray-scale image
    TGA_BLACK_WHITE_IMAGE,

    /// Palette image with run-length encoding
    TGA_RLE_PALETTE_IMAGE = 9,

    /// True-color image with run-length encoding
    TGA_RLE_TRUE_COLOR_IMAGE,

    /// Gray-scale image with run-length encoding
    TGA_RLE_BLACK_WHITE_IMAGE
};

/**
    @class GTGA
    API to work with TGA files format
*/
class GTGA {
public:
    /// Default constructor
    GTGA(const std::string& filePath, TBool headersOnly = false);

    /// Default destructor
	virtual ~GTGA();

    TBool save(const std::string& filePath, const uint8_t *data, const uint32_t width, const uint32_t height, const uint32_t bytepp);
    uint8_t *getImageData();
    uint32_t getWidth();
    uint32_t getHeight();
    uint32_t getBytepp();

private:
    STGAHeader header = {0};

    /// Comments in tga file. Usually it is absent
    uint8_t* identity;
    uint8_t* imageData;

    TBool readHeaders(FILE* tga_file, STGAHeader& tgaHeader);
    TBool readData(uint8_t* data, FILE* tga_file, const TUInt data_size, const STGAHeader& tgaHeader);
    
    /// Destroy all the data
	void clearData();
    
    TBool flipOver(uint8_t* data, const STGAHeader& tgaHeader);
    TBool RGB2BGR(uint8_t* data, const STGAHeader& tgaHeader);
    
    STGAHeader createTGAHeader(const uint32_t width, const uint32_t height, const uint32_t bytepp);
};

}   //  namespace spcGaneshaEngine

#endif  //  SPCGANESHAENGINE_GTGA_H
