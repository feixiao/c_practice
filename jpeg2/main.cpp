#include <iostream>

#include <iostream>
#include <fstream>
#include <ctime>

#include "jpeglib.h"

typedef unsigned char BYTE;

// https://blog.51cto.com/u_15127510/4384916

/*
 *	Descr: 读取jpeg文件的data内容（取出文件头信息）
 *	@param input_filename ：jpeg文件路径（含文件名）输入
 *	@param output_buffer ：返回指向数据地址的指针
 *	@param
 *	@Author :Libin
 *	@return :成功返回0,失败返回-1
*/
int read_jpeg_file(const char *input_filename, char **output_buffer)
{
    struct jpeg_decompress_struct cinfo;
    struct jpeg_error_mgr jerr;
    FILE *input_file;
    FILE *output_file;
    JSAMPARRAY buffer;
    int row_width;

    unsigned char *rowdata = NULL;

    cinfo.err = jpeg_std_error(&jerr);


    if ((input_file = fopen(input_filename, "rb")) == NULL) {
        fprintf(stderr, "can't open %s\n", input_filename);
        return -1;
    }


    // 1、Initialization of JPEG compression objects
    jpeg_create_decompress(&cinfo);

    /* Specify data source for decompression */
    jpeg_stdio_src(&cinfo, input_file);

    /* 1.设置读取jpg文件头部，Read file header, set default decompression parameters */
    (void) jpeg_read_header(&cinfo, TRUE);

    /* 2.开始解码数据 Start decompressor */
    (void) jpeg_start_decompress(&cinfo);

    row_width = cinfo.output_width * cinfo.output_components;



    /* 3.跳过读取的头文件字节Make a one-row-high sample array that will go away when done with image */
    buffer = (*cinfo.mem->alloc_sarray)
            ((j_common_ptr) &cinfo, JPOOL_IMAGE, row_width, 1);

    printf("Get image width:%d,height:%d\n",row_width,cinfo.output_height);
    *output_buffer = (unsigned char *)malloc(row_width * cinfo.output_height);
    memset(*output_buffer, 0, row_width * cinfo.output_height);
    rowdata = *output_buffer;


    /* 4.Process data由左上角从上到下行行扫描 */
    while (cinfo.output_scanline < cinfo.output_height) {
        (void) jpeg_read_scanlines(&cinfo, buffer, 1);

        memcpy(rowdata, *buffer, row_width);
        rowdata += row_width;
    }

    //free(output_buffer);
    (void) jpeg_finish_decompress(&cinfo);
    printf("Get iamge data ,buffer size=%d\n",strlen((const char* ) *output_buffer));
    jpeg_destroy_decompress(&cinfo);

    /* Close files, if we opened them */
    fclose(input_file);

    return 0;
}


bool CompressJPEG(
        /*IN*/BYTE *src, int width, int height, int depth,
        /*OUT*/BYTE **dst, unsigned long *dstLen
)
{
    // NOTICE: dst space must be created outside before passing in.
    struct jpeg_compress_struct jcs;
    struct jpeg_error_mgr jem;
    jcs.err = jpeg_std_error(&jem);

    jpeg_create_compress(&jcs);
    jpeg_mem_dest(&jcs, dst, dstLen);
    jcs.image_width = width;
    jcs.image_height = height;
    jcs.input_components = depth;
    jcs.in_color_space = JCS_RGB;

    jpeg_set_defaults(&jcs);
    jpeg_set_quality(&jcs, 80, true);

    jcs.jpeg_color_space = JCS_YCbCr;
    jcs.comp_info[0].h_samp_factor = 2;
    jcs.comp_info[0].v_samp_factor = 2;

    jpeg_start_compress(&jcs, TRUE);
    JSAMPROW row_pointer[1];
    int row_stride = jcs.image_width*jcs.num_components;
    while (jcs.next_scanline<jcs.image_height)
    {
        row_pointer[0] = &src[jcs.next_scanline*row_stride];
        jpeg_write_scanlines(&jcs, row_pointer, 1);
    }
    jpeg_finish_compress(&jcs);
    jpeg_destroy_compress(&jcs);

    return true;
}

bool DeompressJPEG(
        /*IN*/BYTE *src, unsigned long srcLen,
        /*OUT*/BYTE **dst, unsigned long *dstLen, int *width, int *height, int *depth
)
{
    // NOTICE: dst space will be created inside.
    struct jpeg_decompress_struct cinfo;
    struct jpeg_error_mgr jerr;

    cinfo.err=jpeg_std_error(&jerr);
    jpeg_create_decompress(&cinfo);

    jpeg_mem_src(&cinfo,src,srcLen);
    jpeg_read_header(&cinfo,TRUE);

    jpeg_start_decompress(&cinfo);
    (*width) = cinfo.output_width;
    (*height) = cinfo.output_height;
    (*depth) = cinfo.num_components;
    (*dstLen) = (*width)*(*height)*(*depth);
    BYTE *tmp_dst = new BYTE[*dstLen];

    JSAMPROW row_pointer[1];
    int row_stride = cinfo.image_width*cinfo.num_components;
    while (cinfo.output_scanline<cinfo.output_height)
    {
        row_pointer[0] = &tmp_dst[cinfo.output_scanline*row_stride];
        jpeg_read_scanlines(&cinfo,row_pointer,1);
    }
    jpeg_finish_decompress(&cinfo);
    jpeg_destroy_decompress(&cinfo);
    *dst = tmp_dst;

    return true;
}

void compress_jpeg_to_file(
        /*IN*/BYTE *src,int width, int height, int components, int color_space,int quality,
        /*OUT*/const char *dst_filename
)
{
    /* This struct contains the JPEG compression parameters and pointers to
    * working space (which is allocated as needed by the JPEG library).
    * It is possible to have several such structures, representing multiple
    * compression/decompression processes, in existence at once.  We refer
    * to any one struct (and its associated working data) as a "JPEG object".
    */
    struct jpeg_compress_struct cinfo;
    /* This struct represents a JPEG error handler.  It is declared separately
    * because applications often want to supply a specialized error handler
    * (see the second half of this file for an example).  But here we just
    * take the easy way out and use the standard error handler, which will
    * print a message on stderr and call exit() if compression fails.
    * Note that this struct must live as long as the main JPEG parameter
    * struct, to avoid dangling-pointer problems.
    */
    struct jpeg_error_mgr jerr;
    /* More stuff */
    FILE *outfile;                /* target file */
    JSAMPROW row_pointer[1];      /* pointer to JSAMPLE row[s] */
    int row_stride;               /* physical row width in image buffer */

    /* Step 1: allocate and initialize JPEG compression object */

    /* We have to set up the error handler first, in case the initialization
    * step fails.  (Unlikely, but it could happen if you are out of memory.)
    * This routine fills in the contents of struct jerr, and returns jerr's
    * address which we place into the link field in cinfo.
    */
    cinfo.err = jpeg_std_error(&jerr);
    /* Now we can initialize the JPEG compression object. */
    jpeg_create_compress(&cinfo);

    /* Step 2: specify data destination (eg, a file) */
    /* Note: steps 2 and 3 can be done in either order. */

    /* Here we use the library-supplied code to send compressed data to a
    * stdio stream.  You can also write your own code to do something else.
    * VERY IMPORTANT: use "b" option to fopen() if you are on a machine that
    * requires it in order to write binary files.
    */
    if ((outfile = fopen(dst_filename, "wb")) == NULL) {
        fprintf(stderr, "can't open %s\n", dst_filename);
        exit(1);
    }
    jpeg_stdio_dest(&cinfo, outfile);

    /* Step 3: set parameters for compression */

    /* First we supply a description of the input image.
    * Four fields of the cinfo struct must be filled in:
    */
    cinfo.image_width = width;      /* image width and height, in pixels */
    cinfo.image_height = height;
    cinfo.input_components = components;           /* # of color components per pixel */
    cinfo.in_color_space = (J_COLOR_SPACE)color_space;       /* colorspace of input image */
    /* Now use the library's routine to set default compression parameters.
    * (You must set at least cinfo.in_color_space before calling this,
    * since the defaults depend on the source color space.)
    */
    jpeg_set_defaults(&cinfo);
    /* Now you can set any non-default parameters you wish to.
    * Here we just illustrate the use of quality (quantization table) scaling:
    */
    jpeg_set_quality(&cinfo, quality, TRUE /* limit to baseline-JPEG values */);

    /* Step 4: Start compressor */

    /* TRUE ensures that we will write a complete interchange-JPEG file.
    * Pass TRUE unless you are very sure of what you're doing.
    */
    jpeg_start_compress(&cinfo, TRUE);

    /* Step 5: while (scan lines remain to be written) */
    /*           jpeg_write_scanlines(...); */

    /* Here we use the library's state variable cinfo.next_scanline as the
    * loop counter, so that we don't have to keep track ourselves.
    * To keep things simple, we pass one scanline per call; you can pass
    * more if you wish, though.
    */
    row_stride = cinfo.image_width * cinfo.input_components; /* JSAMPLEs per row in image_buffer */

    while (cinfo.next_scanline < cinfo.image_height) {
        /* jpeg_write_scanlines expects an array of pointers to scanlines.
        * Here the array is only one element long, but you could pass
        * more than one scanline at a time if that's more convenient.
        */
        row_pointer[0] = &src[cinfo.next_scanline * row_stride];
        (void)jpeg_write_scanlines(&cinfo, row_pointer, 1);
    }

    /* Step 6: Finish compression */

    jpeg_finish_compress(&cinfo);
    /* After finish_compress, we can close the output file. */
    fclose(outfile);

    /* Step 7: release JPEG compression object */

    /* This is an important step since it will release a good deal of memory. */
    jpeg_destroy_compress(&cinfo);

    /* And we're done! */
}

void compress_jpeg_to_mem(
        /*IN*/BYTE *src,int width, int height, int components, int color_space,int quality,
        /*OUT*/BYTE **dst, unsigned long *dstLen
)
{
    /* This struct contains the JPEG compression parameters and pointers to
    * working space (which is allocated as needed by the JPEG library).
    * It is possible to have several such structures, representing multiple
    * compression/decompression processes, in existence at once.  We refer
    * to any one struct (and its associated working data) as a "JPEG object".
    */
    struct jpeg_compress_struct cinfo;
    /* This struct represents a JPEG error handler.  It is declared separately
    * because applications often want to supply a specialized error handler
    * (see the second half of this file for an example).  But here we just
    * take the easy way out and use the standard error handler, which will
    * print a message on stderr and call exit() if compression fails.
    * Note that this struct must live as long as the main JPEG parameter
    * struct, to avoid dangling-pointer problems.
    */
    struct jpeg_error_mgr jerr;
    /* More stuff */
    //FILE *outfile;                /* target file */
    JSAMPROW row_pointer[1];      /* pointer to JSAMPLE row[s] */
    int row_stride;               /* physical row width in image buffer */

    /* Step 1: allocate and initialize JPEG compression object */

    /* We have to set up the error handler first, in case the initialization
    * step fails.  (Unlikely, but it could happen if you are out of memory.)
    * This routine fills in the contents of struct jerr, and returns jerr's
    * address which we place into the link field in cinfo.
    */
    cinfo.err = jpeg_std_error(&jerr);
    /* Now we can initialize the JPEG compression object. */
    jpeg_create_compress(&cinfo);

    /* Step 2: specify data destination (eg, a file) */
    /* Note: steps 2 and 3 can be done in either order. */
    //jpeg_stdio_dest(&cinfo, outfile);
    jpeg_mem_dest(&cinfo, dst, dstLen);

    /* Step 3: set parameters for compression */

    /* First we supply a description of the input image.
    * Four fields of the cinfo struct must be filled in:
    */
    cinfo.image_width = width;      /* image width and height, in pixels */
    cinfo.image_height = height;
    cinfo.input_components = components;           /* # of color components per pixel */
    cinfo.in_color_space = (J_COLOR_SPACE)color_space;       /* colorspace of input image */
    /* Now use the library's routine to set default compression parameters.
    * (You must set at least cinfo.in_color_space before calling this,
    * since the defaults depend on the source color space.)
    */
    jpeg_set_defaults(&cinfo);
    /* Now you can set any non-default parameters you wish to.
    * Here we just illustrate the use of quality (quantization table) scaling:
    */
    jpeg_set_quality(&cinfo, quality, TRUE /* limit to baseline-JPEG values */);

    /* Step 4: Start compressor */

    /* TRUE ensures that we will write a complete interchange-JPEG file.
    * Pass TRUE unless you are very sure of what you're doing.
    */
    jpeg_start_compress(&cinfo, TRUE);

    /* Step 5: while (scan lines remain to be written) */
    /*           jpeg_write_scanlines(...); */

    /* Here we use the library's state variable cinfo.next_scanline as the
    * loop counter, so that we don't have to keep track ourselves.
    * To keep things simple, we pass one scanline per call; you can pass
    * more if you wish, though.
    */
    row_stride = cinfo.image_width * cinfo.input_components; /* JSAMPLEs per row in image_buffer */

    while (cinfo.next_scanline < cinfo.image_height) {
        /* jpeg_write_scanlines expects an array of pointers to scanlines.
        * Here the array is only one element long, but you could pass
        * more than one scanline at a time if that's more convenient.
        */
        row_pointer[0] = &src[cinfo.next_scanline * row_stride];
        (void)jpeg_write_scanlines(&cinfo, row_pointer, 1);
    }

    /* Step 6: Finish compression */

    jpeg_finish_compress(&cinfo);
    /* After finish_compress, we can close the output file. */
    //fclose(outfile);

    /* Step 7: release JPEG compression object */

    /* This is an important step since it will release a good deal of memory. */
    jpeg_destroy_compress(&cinfo);

    /* And we're done! */
}

int    decompress_jpeg_from_file(
        /*IN*/const char *src_filename,
        /*OUT*/BYTE **dst, unsigned long *dstLen, int *width, int *height, int *components,int *color_space)
{
    /* This struct contains the JPEG decompression parameters and pointers to
    * working space (which is allocated as needed by the JPEG library).
    */
    struct jpeg_decompress_struct cinfo;
    /* We use our private extension JPEG error handler.
    * Note that this struct must live as long as the main JPEG parameter
    * struct, to avoid dangling-pointer problems.
    */
    struct jpeg_error_mgr jerr;
    /* More stuff */
    FILE *infile;                 /* source file */
    int row_stride;               /* physical row width in output buffer */

    /* In this example we want to open the input file before doing anything else,
    * so that the setjmp() error recovery below can assume the file is open.
    * VERY IMPORTANT: use "b" option to fopen() if you are on a machine that
    * requires it in order to read binary files.
    */

    if ((infile = fopen(src_filename, "rb")) == NULL) {
        fprintf(stderr, "can't open %s\n", src_filename);
        return 0;
    }

    /* Step 1: allocate and initialize JPEG decompression object */

    /* We set up the normal JPEG error routines, then override error_exit. */
    cinfo.err = jpeg_std_error(&jerr);
    /* Now we can initialize the JPEG decompression object. */
    jpeg_create_decompress(&cinfo);

    /* Step 2: specify data source (eg, a file) */

    jpeg_stdio_src(&cinfo, infile);

    /* Step 3: read file parameters with jpeg_read_header() */

    (void)jpeg_read_header(&cinfo, TRUE);
    /* We can ignore the return value from jpeg_read_header since
    *   (a) suspension is not possible with the stdio data source, and
    *   (b) we passed TRUE to reject a tables-only JPEG file as an error.
    * See libjpeg.txt for more info.
    */

    /* Step 4: set parameters for decompression */

    /* In this example, we don't need to change any of the defaults set by
    * jpeg_read_header(), so we do nothing here.
    */

    /* Step 5: Start decompressor */

    (void)jpeg_start_decompress(&cinfo);
    /* We can ignore the return value since suspension is not possible
    * with the stdio data source.
    */

    /* We may need to do some setup of our own at this point before reading
    * the data.  After jpeg_start_decompress() we have the correct scaled
    * output image dimensions available, as well as the output colormap
    * if we asked for color quantization.
    * In this example, we need to make an output work buffer of the right size.
    */
    /* JSAMPLEs per row in output buffer */
    row_stride = cinfo.output_width * cinfo.output_components;
    /* Make a one-row-high sample array that will go away when done with image */

    /* Set output fields */
    (*width) = cinfo.output_width;
    (*height) = cinfo.output_height;
    (*components) = cinfo.num_components;
    (*color_space) = cinfo.jpeg_color_space;
    (*dstLen) = (*width)*(*height)*(*components);
    BYTE *tmp_dst = new BYTE[*dstLen]; /* Allocate out buffer */

    JSAMPROW row_pointer[1];
    row_stride = cinfo.output_width*cinfo.num_components;
    /* Step 6: while (scan lines remain to be read) */
    /*           jpeg_read_scanlines(...); */

    /* Here we use the library's state variable cinfo.output_scanline as the
    * loop counter, so that we don't have to keep track ourselves.
    */
    while (cinfo.output_scanline < cinfo.output_height) {
        /* jpeg_read_scanlines expects an array of pointers to scanlines.
        * Here the array is only one element long, but you could ask for
        * more than one scanline at a time if that's more convenient.
        */
        row_pointer[0] = &tmp_dst[cinfo.output_scanline*row_stride];
        (void)jpeg_read_scanlines(&cinfo, row_pointer, 1);
    }
    (*dst) = tmp_dst; // assign to outside dst

    /* Step 7: Finish decompression */

    (void)jpeg_finish_decompress(&cinfo);
    /* We can ignore the return value since suspension is not possible
    * with the stdio data source.
    */

    /* Step 8: Release JPEG decompression object */

    /* This is an important step since it will release a good deal of memory. */
    jpeg_destroy_decompress(&cinfo);

    /* After finish_decompress, we can close the input file.
    * Here we postpone it until after no more JPEG errors are possible,
    * so as to simplify the setjmp error logic above.  (Actually, I don't
    * think that jpeg_destroy can do an error exit, but why assume anything...)
    */
    fclose(infile);

    /* At this point you may want to check to see whether any corrupt-data
    * warnings occurred (test whether jerr.pub.num_warnings is nonzero).
    */

    /* And we're done! */
    return 1;
}

int    decompress_jpeg_from_mem(
        /*IN*/BYTE *src, unsigned long srcLen,
        /*OUT*/BYTE **dst, unsigned long *dstLen, int *width, int *height, int *components, int *color_space)
{
    /* This struct contains the JPEG decompression parameters and pointers to
    * working space (which is allocated as needed by the JPEG library).
    */
    struct jpeg_decompress_struct cinfo;
    /* We use our private extension JPEG error handler.
    * Note that this struct must live as long as the main JPEG parameter
    * struct, to avoid dangling-pointer problems.
    */
    struct jpeg_error_mgr jerr;
    /* More stuff */
    //FILE *infile;                 /* source file */
    int row_stride;               /* physical row width in output buffer */

    /* In this example we want to open the input file before doing anything else,
    * so that the setjmp() error recovery below can assume the file is open.
    * VERY IMPORTANT: use "b" option to fopen() if you are on a machine that
    * requires it in order to read binary files.
    */

    /*
    if ((infile = fopen(src_filename, "rb")) == NULL) {
        fprintf(stderr, "can't open %s\n", src_filename);
        return 0;
    }
    */

    /* Step 1: allocate and initialize JPEG decompression object */

    /* We set up the normal JPEG error routines, then override error_exit. */
    cinfo.err = jpeg_std_error(&jerr);
    /* Now we can initialize the JPEG decompression object. */
    jpeg_create_decompress(&cinfo);

    /* Step 2: specify data source (eg, a file) */

    //jpeg_stdio_src(&cinfo, infile);
    jpeg_mem_src(&cinfo, src, srcLen);

    /* Step 3: read file parameters with jpeg_read_header() */

    (void)jpeg_read_header(&cinfo, TRUE);
    /* We can ignore the return value from jpeg_read_header since
    *   (a) suspension is not possible with the stdio data source, and
    *   (b) we passed TRUE to reject a tables-only JPEG file as an error.
    * See libjpeg.txt for more info.
    */

    /* Step 4: set parameters for decompression */

    /* In this example, we don't need to change any of the defaults set by
    * jpeg_read_header(), so we do nothing here.
    */

    /* Step 5: Start decompressor */

    (void)jpeg_start_decompress(&cinfo);
    /* We can ignore the return value since suspension is not possible
    * with the stdio data source.
    */

    /* We may need to do some setup of our own at this point before reading
    * the data.  After jpeg_start_decompress() we have the correct scaled
    * output image dimensions available, as well as the output colormap
    * if we asked for color quantization.
    * In this example, we need to make an output work buffer of the right size.
    */
    /* JSAMPLEs per row in output buffer */
    row_stride = cinfo.output_width * cinfo.output_components;
    /* Make a one-row-high sample array that will go away when done with image */

    /* Set output fields */
    (*width) = cinfo.output_width;
    (*height) = cinfo.output_height;
    (*components) = cinfo.num_components;
    (*color_space) = cinfo.jpeg_color_space;
    (*dstLen) = (*width)*(*height)*(*components);
    BYTE *tmp_dst = new BYTE[*dstLen]; /* Allocate out buffer */

    JSAMPROW row_pointer[1];
    row_stride = cinfo.output_width*cinfo.num_components;
    /* Step 6: while (scan lines remain to be read) */
    /*           jpeg_read_scanlines(...); */

    /* Here we use the library's state variable cinfo.output_scanline as the
    * loop counter, so that we don't have to keep track ourselves.
    */
    while (cinfo.output_scanline < cinfo.output_height) {
        /* jpeg_read_scanlines expects an array of pointers to scanlines.
        * Here the array is only one element long, but you could ask for
        * more than one scanline at a time if that's more convenient.
        */
        row_pointer[0] = &tmp_dst[cinfo.output_scanline*row_stride];
        (void)jpeg_read_scanlines(&cinfo, row_pointer, 1);
    }
    (*dst) = tmp_dst; // assign to outside dst

    /* Step 7: Finish decompression */

    (void)jpeg_finish_decompress(&cinfo);
    /* We can ignore the return value since suspension is not possible
    * with the stdio data source.
    */

    /* Step 8: Release JPEG decompression object */

    /* This is an important step since it will release a good deal of memory. */
    jpeg_destroy_decompress(&cinfo);

    /* After finish_decompress, we can close the input file.
    * Here we postpone it until after no more JPEG errors are possible,
    * so as to simplify the setjmp error logic above.  (Actually, I don't
    * think that jpeg_destroy can do an error exit, but why assume anything...)
    */
    //fclose(infile);

    /* At this point you may want to check to see whether any corrupt-data
    * warnings occurred (test whether jerr.pub.num_warnings is nonzero).
    */

    /* And we're done! */
    return 1;
}

void test_compress_to_file()
{
    int width = 2000;
    int height = 1000;
    int channel = 1;

    int nImgSize = width * height * channel;
    unsigned char * pRawImage = new unsigned char[nImgSize]; // new buffer
    memset(pRawImage, 0, nImgSize);
    for (int i = 100; i < 300; i++) // row [height]
    {
        for (int j = 0; j < width; j++) // column [width]
        {
            *(pRawImage + width * i + j) = (char)255;
        }
    }

    // do work.
    compress_jpeg_to_file(
            pRawImage,width,height,channel,J_COLOR_SPACE::JCS_GRAYSCALE,90,
            "../image/compress/to_file.jpg"
    );

    delete[] pRawImage;
}

void test_compress_to_mem()
{
    int width = 2000;
    int height = 1000;
    int channel = 1;

    int nImgSize = width * height * channel;
    unsigned char * pRawImage = new unsigned char[nImgSize]; // new buffer
    memset(pRawImage, 0, nImgSize);
    for (int i = 100; i < 300; i++) // row [height]
    {
        for (int j = 0; j < width; j++) // column [width]
        {
            *(pRawImage + width * i + j) = (char)255;
        }
    }

    // (1) new dst space outside **compress** function
    // pOutBuffer's nImgSize must > lOutSize to contain valid dst buffer.
    unsigned char *pOutBuffer = new unsigned char[nImgSize]; // new dst buffer
    unsigned long lOutSize = 0;

    // (2) compress to mem
    compress_jpeg_to_mem(
            pRawImage, width, height, channel, J_COLOR_SPACE::JCS_GRAYSCALE, 90,
            &pOutBuffer, &lOutSize
    );

    std::cout << "orginal size = " << nImgSize << std::endl;
    std::cout << "compress size = "<<lOutSize << std::endl; // 30403

    // (3) write mem buffer to file
    const char* dst_filename = "../image/compress/mem_to_file.jpg";
    FILE *outfile;
    if ((outfile = fopen(dst_filename, "wb")) == NULL) {
        fprintf(stderr, "can't open %s\n", dst_filename);
        exit(1);
    }
    fwrite(pOutBuffer,lOutSize,1,outfile);
    fclose(outfile);

    delete[] pOutBuffer;

    delete[] pRawImage;
}

void test_decompress_from_file(std::string& sFile, std::string& dFile)
{
    const char *src_filename = sFile.c_str();
    BYTE *dst = NULL; // raw image buffer allocated inside **decompress** function
    unsigned long dstLen;
    int width, height, channel,color_space;

    // allocate dst inside function
    decompress_jpeg_from_file(
            src_filename,
            &dst,&dstLen,&width,&height,&channel,&color_space
    );

    std::cout << dstLen << std::endl;
    std::cout << width << std::endl;
    std::cout << height << std::endl;
    std::cout << channel << std::endl;
    std::cout << color_space << std::endl;

    // use raw image buffer
    // do work.
    compress_jpeg_to_file(
            dst, width, height, channel, color_space, 100,
            dFile.c_str()
    );

    // free allocated memory
    if (dst != NULL)
    {
        delete[] dst;
    }
}

void test_decompress_from_mem()
{
    // (0) create memory src buffer
    char* src_filename = "../image/compress/to_file.jpg";
    std::ifstream ifs(src_filename, std::ios_base::binary | std::ios_base::in);
    ifs.seekg(0, std::ios::end);
    uint64_t size = ifs.tellg();
    ifs.seekg(0, std::ios::beg);

    //std::vector<char> buffer(size);
    //ifs.read(&buffer.front(), size);
    BYTE *src = new BYTE[size];
    ifs.read((char*)src, size);

    // (1) decompress from mem
    BYTE *dst = NULL; // raw image buffer allocated inside **decompress** function
    unsigned long dstLen;
    int width, height, channel, color_space;

    // allocate dst inside function
    decompress_jpeg_from_mem(
            src,size,
            &dst, &dstLen, &width, &height, &channel, &color_space
    );

    std::cout << dstLen << std::endl;
    std::cout << width << std::endl;
    std::cout << height << std::endl;
    std::cout << channel << std::endl;
    std::cout << color_space << std::endl;

    // (2) use raw image buffer
    // do work.
    compress_jpeg_to_file(
            dst, width, height, channel, J_COLOR_SPACE::JCS_GRAYSCALE, 90,
            "../image/compress/decompress_from_mem_and_then_to_file.jpg"
    );
    // free dst allocated memory
    if (dst != NULL)
    {
        delete[] dst;
    }

    // free src memory buffer
    delete[] src;
}


int IMAGE_COUNT = 10000;
void test_compress_time()
{
    int width = 2000;
    int height = 1000;
    int channel = 1;

    unsigned long srcLen = width * height * channel;
    unsigned char * src = new unsigned char[srcLen]; // new buffer
    memset(src, 0, srcLen);
    for (int i = 100; i < 300; i++) // row [height]
    {
        for (int j = 0; j < width; j++) // column [width]
        {
            *(src + width * i + j) = (char)255;
        }
    }

    int quality = 90;
    time_t start = time(NULL);

    for (int i = 0; i < IMAGE_COUNT; i++)
    {
        // (1) new dst space outside **compress** function
        // pOutBuffer's nImgSize must > lOutSize to contain valid dst buffer.
        unsigned char *pOutBuffer = new unsigned char[srcLen]; // new dst buffer
        unsigned long lOutSize = 0;

        // (2) compress to mem
        compress_jpeg_to_mem(
                src, width, height, channel, J_COLOR_SPACE::JCS_GRAYSCALE, 90,
                &pOutBuffer, &lOutSize
        );

        // (3) free memory
        delete[] pOutBuffer;
    }

    time_t end = time(NULL);
    std::cout << "======================================" << std::endl;
    double ms = (double)(end - start) * 1000;
    std::cout << " use times = " << ms << "ms; avg = " << ms / IMAGE_COUNT << " ms; " << " #" << IMAGE_COUNT << std::endl;
    // avg = 4.9 ms  #10000 for jpeg
    // avg = 4.5 ms  #10000 for turbojpeg
    std::cout << "======================================" << std::endl;


    delete[] src;
}

void test_decompress_time()
{

}

int main(int argc, char* argv[])
{
    // 30403, 30153
    //test_compress_to_file();
    //test_compress_to_mem();

    std::string file("/Users/frank/workspace/github/c_practice/jpeg2/image/test.jpg");
    std::string file2("/Users/frank/workspace/github/c_practice/jpeg2/image/new.jpeg");
    test_decompress_from_file(file, file2);
    //test_decompress_from_mem();

//    test_compress_time();
    return 0;
}