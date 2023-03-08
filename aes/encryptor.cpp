#include "encryptor.h"
#include <string.h>

Encryptor::Encryptor() : m_encrypt(true)
{
    unsigned char iv[IV_SIZE] = {0xb2,
                                 0x4b,
                                 0xf2,
                                 0xf7,
                                 0x7a,
                                 0xc5,
                                 0xec,
                                 0x0c,
                                 0x5e,
                                 0x1f,
                                 0x4d,
                                 0xc1,
                                 0xae,
                                 0x46,
                                 0x5e,
                                 0x75};
    memcpy(m_original_iv, iv, IV_SIZE);
}

int Encryptor::initialize( bool encrypt, unsigned char*key )
{
    int ret = 0;

    m_encrypt = encrypt;
    memset((void*)m_encryption_key, 0, ENCRYPTION_KEY_SIZE_IN_BYTES); 
    memcpy(m_encryption_key,key,ENCRYPTION_KEY_SIZE_IN_BYTES);

    // initialize aes context
    mbedtls_aes_init(&m_aescontext);

    if (encrypt)
        ret = mbedtls_aes_setkey_enc(
            &m_aescontext, m_encryption_key, ENCRYPTION_KEY_SIZE);
    else
        ret = mbedtls_aes_setkey_dec(
            &m_aescontext, m_encryption_key, ENCRYPTION_KEY_SIZE);

    if (ret != 0)
    {
        printf("mbedtls_aes_setkey_dec failed with %d", ret);
        goto exit;
    }
    // init iv
    memcpy(m_operating_iv, m_original_iv, IV_SIZE);
exit:
    return ret;
}

int Encryptor::encrypt_block(
    bool encrypt,
    unsigned char* input_buf,
    unsigned char** output_buf,
    size_t size,
    size_t *out_data_len)
{
    unsigned char output[1024] = {0};
    unsigned char* output_data = (unsigned char*)malloc(1024); // 不能定义为数组，strlen
    memset(output_data, 0, 1024);
    unsigned char input_str[1024]={0};


    memcpy(input_str,input_buf,size);

    if(encrypt==MBEDTLS_AES_ENCRYPT){
        // pad to multiple of 16 for AES
        size_t enc_len = size;      // add a byte (after the padding zeros) to hold the number of padbytes needed
        int pad = 16 - (enc_len % 16);
        memset(input_str+size,pad,pad);     //  填充需要pad的数据内容到最后
	    
        enc_len += pad;

        printf("input %s , size: %d , pad : %d , enc_len : %d \n", input_buf, size, pad, enc_len);
        int ret = 0;
	    ret = mbedtls_aes_crypt_cbc( &m_aescontext,
                        MBEDTLS_AES_ENCRYPT,
                        enc_len,
                        m_operating_iv,
                        input_str,
                        output );// Initialization vector is updated after use
        if (ret != 0)
        {
                printf("mbedtls_aes_crypt_cbc failed with %d", ret);
        } else {
                // memcpy(output_data,output,strlen((const char*)output));
		        // *output_buf = output_data;
                // *out_data_len = (int) strlen((const char*)output);

                memcpy(output_data,output,enc_len);
		        *output_buf = output_data;
                *out_data_len = enc_len;

                printf("pad : %d , enc_len : %d , out_data_len : %d, strlen : %d\n",
                     pad, enc_len, *out_data_len, strlen((char*)*output_buf));

	    }
    } else { //decryption
        int ret=0;
        ret = mbedtls_aes_crypt_cbc( &m_aescontext,
                        MBEDTLS_AES_DECRYPT,
                        size,
                        m_operating_iv,
                        (const unsigned char*)input_buf,
                        output_data);

        if (ret != 0)
        {
                printf("mbedtls_aes_crypt_cbc failed with %d", ret);
        } else {

            int len = strlen((char*)output_data);
            int pad =  output_data[len - 1];

            // remove padding
            memset(output_data + len - pad, 0, pad);
		    *output_buf = output_data;
		    *out_data_len = len - pad;
        }
    }
    return 0;

/*
    unsigned char output[128] = {0};
    unsigned char output2[128] = {0};
    mbedtls_aes_crypt_cbc( &m_aescontext, MBEDTLS_AES_ENCRYPT, strlen((const char*)input_buf), m_operating_iv, input_buf, output );
    memcpy(m_operating_iv,m_original_iv,IV_SIZE);
   
    //decryption - test only 
    mbedtls_aes_setkey_dec( &m_aescontext, m_encryption_key, ENCRYPTION_KEY_SIZE );
    mbedtls_aes_crypt_cbc( &m_aescontext, MBEDTLS_AES_DECRYPT, strlen((const char*)output), m_operating_iv, output, output2 );
    unsigned char* output_data = (unsigned char*)oe_host_malloc(16);
    memcpy(output_data,output2,strlen((const char*)output2));
    *output_buf = output_data;
    *out_data_len = (int)strlen((const char*)output2);
    return 1;
*/
}

void Encryptor::close()
{
    // free aes context
    mbedtls_aes_free(&m_aescontext);
    printf("encryptor::close\n\n");
}