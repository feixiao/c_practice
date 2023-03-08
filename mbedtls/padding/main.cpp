#include <iostream>
#include "mbedtls/aes.h"
#include "mbedtls/cipher.h"

void test();



//  在采用aes加密或者其他加密方式时，我们会接触到填充方式的概念，一般有ZeroPadding、PKCS5Padding与PKCS7Padding方式。
//  ZeroPadding 数据长度不对齐时使用0填充，否则不填充。
//  PKCS7Padding <1>已对齐，填充一个长度为blockSize且每个字节均为blockSize的数据。
//  <2>未对齐，需要补充的字节个数为n，则填充一个长度为n且每个字节均为n的数据。
//  这里针对aes加密来说，blockSize就是16。PKCS5Padding，PKCS7Padding的子集，只是块大小固定为8字节。

int main() {
    test();
    return 0;

}


void test() {
    unsigned char key[] = "12345678";
	unsigned char iv[] =  "87654321";
	static char data[1024]= "abcdefgh";
    static unsigned char input_buf[1024] = {0};
    static unsigned char output_buf[1024] = {0};
    size_t output_len;
    printf("\n data = %s\n",data);

    //  加密
	mbedtls_cipher_context_t ctx = {0};

	mbedtls_cipher_init(&ctx);
	mbedtls_cipher_setup(&ctx,
	mbedtls_cipher_info_from_values(MBEDTLS_CIPHER_ID_AES, 128,MBEDTLS_MODE_CBC));
	mbedtls_cipher_set_padding_mode(&ctx, MBEDTLS_PADDING_PKCS7);
	mbedtls_cipher_setkey(&ctx, key, 128, MBEDTLS_ENCRYPT);
	mbedtls_cipher_set_iv(&ctx, iv, 16);
	mbedtls_cipher_reset(&ctx);
    int x=  strlen(data);
	int len =  x + (16 - x% 16);
	int data1 =  16 - (x % 16);     //aes 加密要求16对齐，所以aes是没有pkcs5padding加密的，因为pkcs5padding要求8对齐
    memset(input_buf,data1,sizeof(input_buf));
    memcpy(input_buf,data,strlen(data));
    printf( "\n len = %d\n",len );
	mbedtls_cipher_update(&ctx, input_buf, len, output_buf, &output_len);
	printf("\noutput_len = %d \n",output_len);
	int inlen = output_len;
	mbedtls_cipher_finish(&ctx, output_buf + output_len, &output_len);
	printf("\noutput_len = %d \n",output_len);
	inlen+=output_len;
    mbedtls_cipher_free(&ctx);


    // 解密
     static unsigned char dec_buf[1024] = {0};
	 mbedtls_cipher_context_t ctx1;
	 mbedtls_cipher_init(&ctx1);
	 mbedtls_cipher_setup(&ctx1,mbedtls_cipher_info_from_values(MBEDTLS_CIPHER_ID_AES, 128,MBEDTLS_MODE_CBC));
	 mbedtls_cipher_set_padding_mode(&ctx1, MBEDTLS_PADDING_PKCS7);
	 mbedtls_cipher_setkey(&ctx1, key, 128, MBEDTLS_DECRYPT);
	 unsigned char deciv[] =  "87654321";
	 mbedtls_cipher_set_iv(&ctx1, deciv, 16);
	 mbedtls_cipher_reset(&ctx1);
	size_t OLEN1 = 0;
	mbedtls_cipher_update(&ctx1, output_buf, inlen, dec_buf, &OLEN1);
	int total_len = OLEN1;
	mbedtls_cipher_finish(&ctx1, dec_buf + OLEN1, &OLEN1);
	total_len += OLEN1;
	dec_buf[total_len] = 0;
	printf("\n aa = %s , size : %d\n", dec_buf, total_len);
	mbedtls_cipher_free(&ctx1);


}