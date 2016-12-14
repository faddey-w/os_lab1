#ifndef OS_LAB1_BMP_IO_H
#define OS_LAB1_BMP_IO_H


#include <string>
#include <stdexcept>
#include <ios>
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
        BYTE red;
        BYTE green;
        BYTE blue;
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

        BMPImage(std::istream& stream);

        void save(std::ostream& stream) const;

        pixel& at(int row, int col);

        int width() const { return info.bitmap_width; }
        int height() const { return info.bitmap_height; }

    };

};};

#endif //OS_LAB1_BMP_IO_H
