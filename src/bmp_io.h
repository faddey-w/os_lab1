#ifndef OS_LAB1_BMP_IO_H
#define OS_LAB1_BMP_IO_H


#include <string>
#include <stdexcept>
#include <ios>
#include <cassert>
#include <vector>


namespace lab1 { namespace bmp {

    typedef unsigned char BYTE;
    typedef unsigned short WORD;
    typedef unsigned int DWORD;
    typedef int LONG;

#pragma pack(push, 1)
    struct BitMapFileHeader {
        BYTE signature[2];
        DWORD file_size;
        DWORD reserved;
        DWORD data_offset;
    };
    struct BitMapCoreInfo {
        DWORD struct_size;
        WORD bitmap_width;
        WORD bitmap_height;
        WORD should_be_1;
        WORD bit_count;
    };
    struct BitMapV3Info {
        DWORD struct_size;
        LONG bitmap_width;
        LONG bitmap_height;
        WORD should_be_1;
        WORD bit_count;
        DWORD compression;
        DWORD data_size;
        LONG ppm[2];
        DWORD clr_table_size;
        DWORD clr_table_cells_used;
    };
    struct BitMapV4Info : public BitMapV3Info {
        DWORD red_mask;
        DWORD green_mask;
        DWORD blue_mask;
        DWORD alpha_mask;
        DWORD color_space;
        BYTE color_space_data[48];
    };
    struct BitMapV5Info : BitMapV4Info {
        BYTE unused_by_us[16];
    };

    struct pixel {
        BYTE blue;
        BYTE green;
        BYTE red;
        BYTE alpha;
    };

#pragma pack(pop)

    class BMPReadError : public std::runtime_error {
    public:
        BMPReadError(const std::string& msg): std::runtime_error(msg) {};
    };


    class BMPImage {
        BitMapFileHeader header;
        BitMapV5Info info;
        std::vector<std::vector<pixel> > data;

    public:

        BMPImage(std::istream& stream) {
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

        void save(std::ostream& stream) const {
            stream.write(reinterpret_cast<const char*>(&header), sizeof(header));
            stream.write(reinterpret_cast<const char*>(&info), sizeof(info));
            for(int line = 0; line < info.bitmap_height; line++) {
                const pixel* raw_data = data[line].data();
                stream.write(reinterpret_cast<const char*>(raw_data),
                             sizeof(pixel) * data[line].size());
            }
        }

    };

};};

#endif //OS_LAB1_BMP_IO_H
