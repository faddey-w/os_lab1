#include <fstream>
#include <iostream>
#include <algorithm>


#include "bmp_io.h"


using namespace lab1::bmp;


void to_redscale(pixel&);


int main(int argc, char **argv) {
    const char *infname, *outfname;
    if (argc > 2) {
        infname = argv[1];
        outfname = argv[2];
    } else {
        outfname = "result.bmp";
        if (argc > 1) {
            infname = argv[1];
        } else {
            infname = "input.bmp";
        }
    }
    try {
        std::ifstream infile;
        infile.open(infname, std::ios::binary);
        if (!infile.is_open()) {
            std::cerr << "Could not open input file\n";
            return 1;
        }

        BMPImage img(infile);
        infile.close();

        for(int row = 0; row < img.height(); ++row) {
            for(int col = 0; col < img.width(); ++col) {
                to_redscale(img.at(row, col));
            }
        }


        std::ofstream outfile;
        outfile.open(outfname, std::ios::binary);
        if (!outfile.is_open()) {
            std::cerr << "Could not open output file\n";
            return 2;
        }
        img.save(outfile);
        outfile.close();
    } catch (BMPReadError& err) {
        std::cerr << "Error on reading image: " << err.what() << std::endl;
        return 3;
    }
    return 0;
}

void to_redscale(pixel& px) {
    int tmp = 2*(px.red+px.green+px.blue) / 3;
    px.red = BYTE(std::min(tmp, 255));
    px.green = BYTE(std::max(tmp-255, 0));
    px.blue = BYTE(std::max(tmp-255, 0));
}

