#include <iostream>
#include <fstream>
#include <vector>
#include <ctime>

#include "turbojpeg.h"


typedef unsigned char BYTE;

void save_buffer_to_file(const char *filename, BYTE *buffer, unsigned long size) {
    FILE *outfile;
    if ((outfile = fopen(filename, "wb")) != NULL) {
        fwrite(buffer, size, 1, outfile);
        fclose(outfile);
    } else {
        fprintf(stderr, "can't open %s\n", filename);
        exit(1);
    }
}

/*
* Compress image buffer to a JPEG image in memory.
*
* @@input
*
* @param : [src] pointer to an image buffer that will be compressed.
* @param :
*
* @@output
* @param : [dst] pointer to an image buffer that will receive the compressed image.
           This variable should be passed in with NULL, and will be allocated by
           TurboJPEG(either by tjAlloc(),or by the Compress/Decompress) method
           So we need to use tjFree() to free memory allocated after we are done
           working on dst.
* @param : [dstLen] size of dst image buffer in bytes. This should be passed in with
           value 0.
*
* @@return
* @param void
*
* @@demo
*
        BYTE *dst = NULL;
        unsigned long dstLen = 0;
        tj_compress_jpeg_to_mem(
            ....,
            &dst,&dstLen
        )
*/
void tj_compress_jpeg_to_mem(
        /*IN*/BYTE *src, int width, int height, int pixelFormat, int subsamp, int quality, int flags,
        /*OUT*/BYTE **dst, unsigned long *dstLen
) {
    // NOTICE : we must use tjAlloc() and tjFree() to allocate dst buffer.
    // for compress, we let **tjCompress2** allocate dst buffer.
    // for decompress, we allocate dst buffer by ourself.

    tjhandle handle = tjInitCompress();
    tjCompress2(
            handle, src, width, 0/*pitch*/, height, pixelFormat,
            dst, dstLen, subsamp, quality, flags
    );

    tjDestroy(handle);
}

//void tj_compress_gray_jpeg_to_mem(
//        /*IN*/BYTE *src, int width, int height, int quality,
//        /*OUT*/BYTE **dst, unsigned long *dstLen
//) {
//    int pixelFormat = TJPF::TJPF_GRAY;
//    int subsamp = TJSAMP::TJSAMP_GRAY;
//    int flags = TJFLAG_FASTDCT;
//
//    tj_compress_jpeg_to_mem(
//            src, width, height, pixelFormat, subsamp, quality, flags,
//            dst, dstLen
//    );
//}

void tj_compress_gray_jpeg_to_file(
        /*IN*/BYTE *src, int width, int height, int quality,
        /*OUT*/const char *dst_filename
) {
    int pixelFormat = TJPF::TJPF_GRAY;
    int subsamp = TJSAMP::TJSAMP_GRAY;
    int flags = TJFLAG_FASTDCT;

    // (1) init dst memory buffer
    BYTE *dst = NULL; // memory allocated by TurboJPEG tjAlloc()
    unsigned long dstLen = 0;

    // (2) compress
    tj_compress_jpeg_to_mem(
            src, width, height, pixelFormat, subsamp, quality, flags,
            &dst, &dstLen
    );

    // (3) write buffer to file
    save_buffer_to_file(dst_filename, dst, dstLen);

    // (4) free memory allocated by TurboJPEG
    tjFree(dst);
}


/*
* Decompress image buffer to a JPEG image in memory.
*
* @@input
*
* @param : [src] pointer to an image buffer that will be compressed.
* @param :
*
* @@output
* @param : [dst] pointer to an image buffer that will receive the decompressed image.
           This variable should be passed in with NULL, and will be allocated in
           method by new[]. So we need to use delete[] to free memory allocated
           after we are done working on dst.
* @param : [dstLen] size of dst image buffer in bytes. This should be passed in with
            value 0.
*
* @@return
* @param void
*
* @@demo
*
        BYTE *dst = NULL;
        unsigned long dstLen = 0;
        tj_decompress_jpeg_from_mem(
        ....,
        &dst,&dstLen
        )
*/
void tj_decompress_jpeg_from_mem(
        /*IN*/BYTE *src, unsigned long srcLen, int tjPixelFormat, int flags,
        /*OUT*/BYTE **dst, unsigned long *dstLen, int *width, int *height, int *components, int *jpegSubsamp,
              int *jpegColorspace) {
    tjhandle handle = tjInitDecompress();
    tjDecompressHeader3(handle, src, srcLen, width, height, jpegSubsamp, jpegColorspace);

    (*components) = tjPixelSize[(TJPF) tjPixelFormat]; // 1 for GRAY,3 for RGB
    (*dstLen) = (*width) * (*height) * (*components);

    BYTE *tmp_dst = new BYTE[*dstLen]; /* Allocate out buffer */

    tjDecompress2(
            handle, src, srcLen,
            tmp_dst, *width, 0/*pitch*/, *height, tjPixelFormat, flags
    );
    tjDestroy(handle);

    (*dst) = tmp_dst; // pass dst out
}

//void tj_decompress_gray_jpeg_from_mem(
//        /*IN*/BYTE *src, unsigned long srcLen,
//        /*OUT*/BYTE **dst, unsigned long *dstLen, int *width, int *height, int *components
//) {
////    int pixelFormat = TJPF::TJPF_GRAY;
//    int pixelFormat = TJPF::TJPF_RGB;
//    int flags = TJFLAG_ACCURATEDCT;
//    int subsamp, colorspace; // no use for now  (3 TJSAMP::TJSAMP_GRAY, 2 TJCS::TJCS_GRAY)
//
//    tj_decompress_jpeg_from_mem(
//            src, srcLen, pixelFormat, flags,
//            dst, dstLen, width, height, components, &subsamp, &colorspace
//    );
//}

void tj_decompress_gray_jpeg_from_file(
        /*IN*/const char *src_filename,
        /*OUT*/BYTE **dst, unsigned long *dstLen, int *width, int *height, int *components
) {
    // (0) read src memory buffer from file
    std::ifstream ifs(src_filename, std::ios_base::binary | std::ios_base::in);
    ifs.seekg(0, std::ios::end);
    int len = ifs.tellg();
    printf("file len : %d\n", len);

    if (len < 0) {
        return;
    }

    uint64_t srcLen = len;
    ifs.seekg(0, std::ios::beg);

    // 读取文件数据
    BYTE *src = new BYTE[srcLen];
    ifs.read((char *) src, srcLen);

    // (2) decompress
    int pixelFormat = TJPF::TJPF_GRAY;
    int flags = TJFLAG_ACCURATEDCT;
    int subsamp, colorspace; // no use for now  (3 TJSAMP::TJSAMP_GRAY, 2 TJCS::TJCS_GRAY)

    tj_decompress_jpeg_from_mem(
            src, srcLen, pixelFormat, flags,
            dst, dstLen, width, height, components, &subsamp, &colorspace
    );

    // (3) free src memory buffer
    delete[] src;

    // (4) pass out dst buffer
}


//void test_compress(std::string &filename) {
//    int width = 2000;
//    int height = 1000;
//    int channel = 1;
//
//    unsigned long srcLen = width * height * channel;
//    unsigned char *src = new unsigned char[srcLen]; // new buffer
//    memset(src, 0, srcLen);
//    for (int i = 100; i < 300; i++) // row [height]
//    {
//        for (int j = 0; j < width; j++) // column [width]
//        {
//            *(src + width * i + j) = (char) 255;
//        }
//    }
//
//    //========================================================================
//    // compress to file
//    int quality = 90;
//    tj_compress_gray_jpeg_to_file(src, width, height, quality, filename.c_str());
//    //========================================================================
//
//    delete[] src;
//}

void test_decompress(std::string &srcFile, std::string &dstFile) {
    // (0) create memory src buffer
    const char *src_filename = srcFile.c_str();

    // (2) decompress
    BYTE *dst = NULL; // allocated inside **decompress** function by new[].
    unsigned long dstLen = 0;
    int width, height, components;

    tj_decompress_gray_jpeg_from_file(
            src_filename,
            &dst, &dstLen, &width, &height, &components
    );

    std::cout << dstLen << std::endl;
    std::cout << width << std::endl;
    std::cout << height << std::endl;
    std::cout << components << std::endl;

    // (3) use dst buffer
    //========================================================================
    // compress to file
    int quality = 100;
    const char *dst_filename = dstFile.c_str();
    tj_compress_gray_jpeg_to_file(dst, width, height, quality, dst_filename);
    //========================================================================

    // (4) free dst buffer
    delete[] dst;
}


//int IMAGE_COUNT = 10000;
//
//void test_compress_time() {
//    int width = 2000;
//    int height = 1000;
//    int channel = 1;
//
//    unsigned long srcLen = width * height * channel;
//    unsigned char *src = new unsigned char[srcLen]; // new buffer
//    memset(src, 0, srcLen);
//    for (int i = 100; i < 300; i++) // row [height]
//    {
//        for (int j = 0; j < width; j++) // column [width]
//        {
//            *(src + width * i + j) = (char) 255;
//        }
//    }
//
//    int quality = 90;
//    time_t start = time(NULL);
//
//    for (int i = 0; i < IMAGE_COUNT; i++) {
//        //========================================================================
//        // (1) init dst memory buffer
//        BYTE *dst = NULL; // memory allocated by TurboJPEG tjAlloc()
//        unsigned long dstLen = 0;
//
//        // (2) compress
//        tj_compress_gray_jpeg_to_mem(
//                src, width, height, quality,
//                &dst, &dstLen
//        );
//
//        // (3) free memory allocated by TurboJPEG
//        tjFree(dst);
//        //========================================================================
//    }
//
//    time_t end = time(NULL);
//    std::cout << "======================================" << std::endl;
//    double ms = (double) (end - start) * 1000;
//    std::cout << " use times = " << ms << "ms; avg = " << ms / IMAGE_COUNT << " ms; " << " #" << IMAGE_COUNT
//              << std::endl;
//    // avg = 4.9 ms  #10000 for jpeg
//    // avg = 4.5 ms  #10000 for turbojpeg
//    std::cout << "======================================" << std::endl;
//
//    delete[] src;
//}


int main(int argc, char *argv[]) {

    std::string file("/Users/frank/workspace/github/c_practice/jpeg/image/test.jpg");
    std::string file2("/Users/frank/workspace/github/c_practice/jpeg/image/new.jpeg");
//    test_compress(file);
    test_decompress(file, file2);
    return 0;
}