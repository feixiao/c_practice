## 快速入门

#### 使用

```shell
# crypt_and_hash <mode> <input filename> <output filename> <cipher> <mbedtls_md> <key>

# 加密
../build/mbedtls/mbedtls_aes 0 test.txt file.aes AES-128-CBC SHA1 hex:E76B2413958B00E193
# 解密
../build/mbedtls/mbedtls_aes 1 file.aes test_2.txt AES-128-CBC SHA1 hex:E76B2413958B00E193
```
