#include <iostream>

#include "encryptor.h"


int main () {
    Encryptor e1;
    std::string key("12345678");

    int ret = e1.initialize(true, (unsigned char*)key.c_str());
    if(ret != 0) {
        printf("initialize failed, ret = %d\n", ret);
        return -1;
    }

    std::string text("一个普通的山村穷小子，偶然之下，跨入到一个江湖小门派，虽然资质平庸，但依靠自身努力和合理算计最后修炼成仙的故事。");

    unsigned char * output_buf = nullptr;
    size_t out_data_len = 0;
    ret = e1.encrypt_block(true, (unsigned char*)text.c_str(), &output_buf, (size_t)text.size(), &out_data_len);
    if(ret != 0) {
        printf("encrypt_block failed, ret = %d\n", ret);
        return -1;
    }

    Encryptor e2;
    ret = e2.initialize(false, (unsigned char*)key.c_str());
    if(ret != 0) {
        printf("initialize failed, ret = %d\n", ret);
        return -1;
    }

    unsigned char * output_buf2 = nullptr;
    size_t out_data_len2 = 0;
    ret = e2.encrypt_block(false, (unsigned char*)output_buf, &output_buf2, out_data_len, &out_data_len2);
    if(ret != 0) {
        printf("encrypt_block failed, ret = %d\n", ret);
        return -1;
    }

    printf("output_buf2 : %s \n", output_buf2);
    return 0;
}