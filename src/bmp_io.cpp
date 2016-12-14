#include <string>
#include <iostream>

#include "bmp_io.h"

namespace lab1 { namespace bmp {

    BMPImage::BMPImage(std::istream& stream) {
        stream.read(reinterpret_cast<char*>(&header), sizeof(header));
        if (header.signature[0] != 0x42 || header.signature[1] != 0x4d) {
            throw BMPReadError("Not a BMP file");
        }

        long long file_pos = stream.tellg();
        DWORD info_size;
        stream.read(reinterpret_cast<char*>(&info_size), sizeof(DWORD));
        if (info_size != sizeof(BitMapV5Info)) {
            throw BMPReadError("Only 124-byte headers are supported");
        }
        stream.seekg(file_pos);

        stream.read(reinterpret_cast<char*>(&info), sizeof(info));
        if (info.bit_count != 32) {
            throw BMPReadError("Only 32-bit images are supported");
        }
        if (info.compression != 3) {
            throw BMPReadError("Supported compression mode: 2-D array with masked channels.");
        }

        if (info.red_mask != 0xFF000000
            || info.green_mask != 0x00FF0000
            || info.blue_mask != 0x0000FF00) {
            throw BMPReadError("Unsupported channels bit mask");
        }

        pixel buf[info.bitmap_width];
        for(int line = 0; line < info.bitmap_height; line++) {
            // we do not add any padding to buffer size
            // because the line's length always aligned to 4 bytes
            // (in our case)
            stream.read(reinterpret_cast<char*>(&buf), sizeof(buf));
            data.push_back(std::vector<pixel>(
                buf, buf+info.bitmap_width
            ));
        }
    }

    void BMPImage::save(std::ostream& stream) const {
        stream.write(reinterpret_cast<const char*>(&header), sizeof(header));
        stream.write(reinterpret_cast<const char*>(&info), sizeof(info));
        for(int line = 0; line < info.bitmap_height; line++) {
            const pixel* raw_data = data[line].data();
            stream.write(reinterpret_cast<const char*>(raw_data),
                         sizeof(pixel) * data[line].size());
        }
    }

    pixel &BMPImage::at(int row, int col) {
        return data[row][col];
    }

}};
