#pragma once

#include <mbedtls/aes.h>
#include <mbedtls/ctr_drbg.h>
#include <mbedtls/entropy.h>
#include <string>

using namespace std;

#define IV_SIZE 16
#define HASH_VALUE_SIZE_IN_BYTES 32
#define ENCRYPTION_KEY_SIZE 128
#define ENCRYPTION_KEY_SIZE_IN_BYTES (ENCRYPTION_KEY_SIZE / 8)
class Encryptor
{
  private:
    mbedtls_aes_context m_aescontext;
    bool m_encrypt;
    string m_password;

    // initialization vector
    unsigned char m_original_iv[IV_SIZE];
    unsigned char m_operating_iv[IV_SIZE];

    // key for encrypting  data
    unsigned char m_encryption_key[ENCRYPTION_KEY_SIZE_IN_BYTES];

  public:
    Encryptor();
    int initialize(bool encrypt, unsigned char*key);
    int encrypt_block(
        bool encrypt,
        unsigned char* input_buf,
        unsigned char** output_buf,
        size_t size,
	      size_t *out_data_len);

        
    void close();
};