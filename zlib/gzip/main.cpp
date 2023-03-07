#include <stdio.h>
#include <stdlib.h>
#include <zlib.h>
#include <errno.h>
#include <string.h>
#include <iostream>

/* The name of the test file to read. */

static const char * file_name = "test.txt.gz";


int gzcompress(Bytef *data, uLong ndata, Bytef *zdata, uLong *nzdata);
int gzdecompress(Byte *zdata, uLong nzdata, Byte *data, uLong *ndata);

/* Size of the block of memory to use for reading. */

#define LENGTH 0x1000

using namespace std;

// g++ -Wall main.cpp -o gzip -lz
int main ()
{
    gzFile file;
    file = gzopen (file_name, "r");
    if (! file) {
        fprintf (stderr, "gzopen of '%s' failed: %s.\n", file_name,
                 strerror (errno));
            exit (EXIT_FAILURE);
    }
    while (1) {
        int err;                    
        int bytes_read;
        unsigned char buffer[LENGTH];
        bytes_read = gzread (file, buffer, LENGTH - 1);
        buffer[bytes_read] = '\0';
        printf ("%s", buffer);
        if (bytes_read < LENGTH - 1) {
            if (gzeof (file)) {
                break;
            }
            else {
                const char * error_string;
                error_string = gzerror (file, & err);
                if (err) {
                    fprintf (stderr, "Error: %s.\n", error_string);
                    exit (EXIT_FAILURE);
                }
            }
        }
    }

    std::string src("12345678900987654321");
    Byte buf[1024] = {"\0"};
    uLong size = 1024;
    int ret = gzcompress((Byte*)src.c_str(), src.size(), buf, &size);
    if(ret != 0) {
        printf("gzcompress failed, ret=%d\n", ret);
        return -1;
    }


    Byte newBuf[1024] = {"\0"};
    uLong newSize = 1024;
    ret = gzdecompress(buf, size, newBuf, &newSize);
    if(ret != 0) {
        printf("gzdecompress failed, ret=%d\n", ret);
        return -1;
    }

    printf("dec : %s\n", newBuf);
    gzclose (file);
    return 0;
}




/* Compress gzip data */
/* data 原数据 ndata 原数据长度 zdata 压缩后数据 nzdata 压缩后长度 */
int gzcompress(Bytef *data, uLong ndata,
 Bytef *zdata, uLong *nzdata)
{
 z_stream c_stream;
 int err = 0;

 if (data && ndata > 0) {
  c_stream.zalloc = NULL;
  c_stream.zfree = NULL;
  c_stream.opaque = NULL;
  //只有设置为MAX_WBITS + 16才能在在压缩文本中带header和trailer
  if (deflateInit2(&c_stream, Z_DEFAULT_COMPRESSION, Z_DEFLATED,
   MAX_WBITS + 16, 8, Z_DEFAULT_STRATEGY) != Z_OK) return -1;
  c_stream.next_in = data;
  c_stream.avail_in = ndata;
  c_stream.next_out = zdata;
  c_stream.avail_out = *nzdata;
  while (c_stream.avail_in != 0 && c_stream.total_out < *nzdata) {
   if (deflate(&c_stream, Z_NO_FLUSH) != Z_OK) return -1;
  }
  if (c_stream.avail_in != 0) return c_stream.avail_in;
  for (;;) {
   if ((err = deflate(&c_stream, Z_FINISH)) == Z_STREAM_END) break;
   if (err != Z_OK) return -1;
  }
  if (deflateEnd(&c_stream) != Z_OK) return -1;
  *nzdata = c_stream.total_out;
  return 0;
 }
 return -1;
}


/* Uncompress gzip data */
/* zdata 数据 nzdata 原数据长度 data 解压后数据 ndata 解压后长度 */
int gzdecompress(Byte *zdata, uLong nzdata, Byte *data, uLong *ndata)
{
 int err = 0;
 z_stream d_stream = { 0 }; /* decompression stream */
 static char dummy_head[2] = {
  0x8 + 0x7 * 0x10,
  (((0x8 + 0x7 * 0x10) * 0x100 + 30) / 31 * 31) & 0xFF,
 };
 d_stream.zalloc = NULL;
 d_stream.zfree = NULL;
 d_stream.opaque = NULL;
 d_stream.next_in = zdata;
 d_stream.avail_in = 0;
 d_stream.next_out = data;
 //只有设置为MAX_WBITS + 16才能在解压带header和trailer的文本
 if (inflateInit2(&d_stream, MAX_WBITS + 16) != Z_OK) return -1;
 //if(inflateInit2(&d_stream, 47) != Z_OK) return -1;
 while (d_stream.total_out < *ndata && d_stream.total_in < nzdata) {
  d_stream.avail_in = d_stream.avail_out = 1; /* force small buffers */
  if ((err = inflate(&d_stream, Z_NO_FLUSH)) == Z_STREAM_END) break;
  if (err != Z_OK) {
   if (err == Z_DATA_ERROR) {
    d_stream.next_in = (Bytef*)dummy_head;
    d_stream.avail_in = sizeof(dummy_head);
    if ((err = inflate(&d_stream, Z_NO_FLUSH)) != Z_OK) {
     return -1;
    }
   }
   else return -1;
  }
 }
 if (inflateEnd(&d_stream) != Z_OK) return -1;
 *ndata = d_stream.total_out;
 return 0;
}