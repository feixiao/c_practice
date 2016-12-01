    #include <stdlib.h>  
    #include <string.h>  
    #include <stdio.h>  
    #include <zlib.h>  
    

    // gcc -Wall zip.c -o zip_test -lz

    int main(int argc, char* argv[])  
    {  
        char text[] = "zlib compress and uncompress test";  
        uLong tlen = strlen(text) + 1;  /* 需要把字符串的结束符也一并处理 */  
        char* buf = NULL;  
        uLong blen;  
      
        /* 计算缓冲区大小，并为其分配内存 */  
        blen = compressBound(tlen); /* 压缩后的长度是不会超过blen的 */  
        if((buf = (char*)malloc(sizeof(char) * blen)) == NULL)  
        {  
            printf("no enough memory!\n");  
            return -1;  
        }  
      
        /* 压缩 */  
        if(compress(buf, &blen, text, tlen) != Z_OK)  
        {  
            printf("compress failed!\n");  
            return -1;  
        }  
      
        /* 解压缩 */  
        if(uncompress(text, &tlen, buf, blen) != Z_OK)  
        {  
            printf("uncompress failed!\n");  
            return -1;  
        }  
      
        /* 打印结果，并释放内存 */  
        printf("%s", text);  
        if(buf != NULL)  
        {  
            free(buf);  
            buf = NULL;  
        }  
      
        return 0;  
    }  