#include "gtga.h"

namespace spcGaneshaEngine {

GTGA::GTGA(const std::string& filePath, TBool headersOnly) : identity(0) {
    TUInt dotPos = static_cast<TUInt>(filePath.rfind("."));
    std::string ext = &filePath[++dotPos];

    if (ext != "tga") {
        throw;
    }

    FILE* tga_file = fopen(filePath.c_str(), "rb");
    if (!tga_file) {
        throw;
    }

    if (!readHeaders(tga_file, header)) {
        throw;
    }

    if (!headersOnly) {
        uint32_t bytepp = 0;
        switch(header.fileHeader.imageType)
        {
            case TGA_TRUE_COLOR_IMAGE:
                bytepp = header.imageHeader.bitpp / 8;
            break;
        }

        TUInt data_size = header.imageHeader.width * header.imageHeader.height * bytepp;
        imageData = new uint8_t[data_size];
        memset(imageData, 0, data_size);
        if (!readData(imageData, tga_file, data_size, header)) {
            throw;
        }
    }

    fclose(tga_file);

    dotPos = static_cast<TUInt>(filePath.rfind("/"));
    std::string fileName = &filePath[++dotPos];
}

GTGA::~GTGA() {
    if (identity) {
        delete [] identity;
    }
    
    if (imageData) {
        delete imageData;
    }
}

uint8_t *GTGA::getImageData() {
    return imageData;
}
uint32_t GTGA::getWidth() {
    return header.imageHeader.width;
}

uint32_t GTGA::getHeight() {
    return header.imageHeader.height;
}

uint32_t GTGA::getBytepp() {
    return header.imageHeader.bitpp * 8;
}

TBool GTGA::readHeaders(FILE* tga_file, STGAHeader& tgaHeader) {
    if (!tga_file) {
        return false;
    }

    fread(&tgaHeader.fileHeader.identityLength, 1, 1, tga_file);
    fread(&tgaHeader.fileHeader.paleteType, 1, 1, tga_file);
    fread(&tgaHeader.fileHeader.imageType, 1, 1, tga_file);

    fread(&tgaHeader.paletteHeader.offset, 2, 1, tga_file);
    fread(&tgaHeader.paletteHeader.size, 2, 1, tga_file);
    fread(&tgaHeader.paletteHeader.bitpp, 1, 1, tga_file);

    fread(&tgaHeader.imageHeader.x_coord, 2, 1, tga_file);
    fread(&tgaHeader.imageHeader.y_coord, 2, 1, tga_file);
    fread(&tgaHeader.imageHeader.width, 2, 1, tga_file);
    fread(&tgaHeader.imageHeader.height, 2, 1, tga_file);
    fread(&tgaHeader.imageHeader.bitpp, 1, 1, tga_file);
    fread(&tgaHeader.imageHeader.image_descriptor, 1, 1, tga_file);

    if (tgaHeader.fileHeader.identityLength) {
        identity = new uint8_t[tgaHeader.fileHeader.identityLength];
        fread(identity, sizeof(uint8_t), tgaHeader.fileHeader.identityLength, tga_file);
    }

    return true;
}

TBool GTGA::readData(uint8_t* data, FILE* tga_file, const TUInt data_size, const STGAHeader& tgaHeader) {
    if (!tga_file) {
        return false;
    }

    fread(data, sizeof(uint8_t), data_size, tga_file);
    
    if (!RGB2BGR(data, tgaHeader)) {
        return false;
    }
    
    if (!flipOver(data, tgaHeader)) {
        return false;
    }

    return true;
}

TBool GTGA::RGB2BGR(uint8_t* data, const STGAHeader& tgaHeader) {
    uint8_t* tmpData = data;
    if (!tmpData) {
        return true;
    }

    TUInt bytepp = tgaHeader.imageHeader.bitpp / 8;
    for (TUInt i = 0; i < tgaHeader.imageHeader.height; i++)
    {
        for (TUInt j = 0; j < tgaHeader.imageHeader.width; j++)
        {
            TUInt index = bytepp * (i * tgaHeader.imageHeader.width + j);
            uint8_t tmp = tmpData[index];
            tmpData[index] = tmpData[index + 2];
            tmpData[index + 2] = tmp;
        }
    }

    return true;
}

TBool GTGA::flipOver(uint8_t* data, const STGAHeader& tgaHeader) {
    uint8_t* tmpData = data;
    if (!tmpData) {
        return true;
    }

    TUInt bytepp = tgaHeader.imageHeader.bitpp / 8;
    uint8_t* tmp = new uint8_t[bytepp * tgaHeader.imageHeader.width];
    for (TUInt i = 0; i < tgaHeader.imageHeader.height / 2; i++) {
        TUInt index = bytepp * i * tgaHeader.imageHeader.width;
        TUInt index2 = bytepp * (tgaHeader.imageHeader.height - i - 1) * tgaHeader.imageHeader.width;

        memcpy(tmp, &tmpData[index], bytepp * tgaHeader.imageHeader.width);
        memcpy(&tmpData[index], &tmpData[index2], bytepp * tgaHeader.imageHeader.width);
        memcpy(&tmpData[index2], tmp, bytepp * tgaHeader.imageHeader.width);
    }

    delete [] tmp;

    return true;
}

STGAHeader GTGA::createTGAHeader(const uint32_t width, const uint32_t height, const uint32_t bytepp) {
    STGAHeader header = {0};
    
    header.imageHeader.bitpp = 8 * bytepp;
    header.imageHeader.width = width;
    header.imageHeader.height = height;

    return header;
}

TBool GTGA::save(const std::string& filePath, const uint8_t *data, const uint32_t width, const uint32_t height, const uint32_t bytepp) {
    STGAHeader header = createTGAHeader(width, height, bytepp);

    TUInt sizer = width * height * bytepp;
    uint8_t* tempData = new uint8_t[sizer];
    memset(tempData, 0, sizer);
    memcpy(tempData, data, sizer);

    uint8_t byte_1 = 0;
    uint16_t byte_2 = 0;
    FILE* filo = fopen(filePath.c_str(), "wb");
    if (!filo) {
        return false;
    }

    fwrite(&byte_1, 1, 1, filo); //  identity length
    fwrite(&byte_1, 1, 1, filo); //  palete type
    byte_1 = 2;
    fwrite(&byte_1, 1, 1, filo); //  image type
    byte_1 = 0;
    fwrite(&byte_2, 2, 1, filo); //  palete offset
    fwrite(&byte_2, 2, 1, filo); //  palete size
    fwrite(&byte_1, 1, 1, filo); //  palete bpp
    fwrite(&byte_2, 2, 1, filo); //  x coord
    fwrite(&byte_2, 2, 1, filo); //  y coord

    byte_2 = width;
    fwrite(&byte_2, 2, 1, filo); //  image width
    byte_2 = height;
    fwrite(&byte_2, 2, 1, filo); //  image height

    byte_1 = 8 * bytepp;
    fwrite(&byte_1, 1, 1, filo); //  byte per pixel
    byte_1 = 0;
    fwrite(&byte_1, 1, 1, filo); //  image property
    
    if (!RGB2BGR(tempData, header)) {
        return false;
    }
    
    if (!flipOver(tempData, header)) {
        return false;
    }

    if (fwrite(tempData, sizer, 1, filo) != 1) { //  image data
        return false;
    }

    delete [] tempData;
    fclose(filo);

	return true;
}

}   //  namespace spcGaneshaEngine
