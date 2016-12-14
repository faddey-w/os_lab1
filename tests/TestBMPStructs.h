#ifndef OS_LAB1_TESTBMPSTRUCTS_H
#define OS_LAB1_TESTBMPSTRUCTS_H


#include <cxxtest/TestSuite.h>

#include <src/bmp_io.h>


class TestBMPStructs : public CxxTest::TestSuite {
public:

    void test_file_header_size() {
        TSM_ASSERT(sizeof(lab1::BitMapFileHeader), sizeof(lab1::BitMapFileHeader) == 14);
    }

    void test_core_info_header_size() {
        TSM_ASSERT(sizeof(lab1::BitMapCoreInfo), sizeof(lab1::BitMapCoreInfo) == 12);
    }

    void test_v3_info_header_size() {
        TSM_ASSERT(sizeof(lab1::BitMapV3Info), sizeof(lab1::BitMapV3Info) == 40);
    }

    void test_v4_info_header_size() {
        TSM_ASSERT(sizeof(lab1::BitMapV4Info), sizeof(lab1::BitMapV4Info) == 108);
    }

    void test_v5_info_header_size() {
        TSM_ASSERT(sizeof(lab1::BitMapV5Info), sizeof(lab1::BitMapV5Info) == 124);
    }

};


#endif //OS_LAB1_TESTBMPSTRUCTS_H
