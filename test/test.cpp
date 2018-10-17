#include <stdio.h>
#include <string.h>
#include <stdlib.h>



#include <gtest/gtest.h>
#include "../ring_buf.h"

#define MAX_BUF_LEN 50

TEST(ring_buf, create){
    ring_buf_t *rb = ring_buf_create(MAX_BUF_LEN);
    EXPECT_TRUE((rb != NULL));
    int ret = 0;
    ret = ring_buf_destory(rb);
    EXPECT_EQ(ret, 0);
}

TEST(ring_buf, data_size){
    ring_buf_t *rb = ring_buf_create(MAX_BUF_LEN);
    EXPECT_EQ(get_left_size(rb), MAX_BUF_LEN -1);
    EXPECT_EQ(get_data_size(rb), 0);
    EXPECT_EQ(ring_buf_destory(rb), 0);
}


TEST(ring_buf, write_data){
    ring_buf_t *rb = ring_buf_create(MAX_BUF_LEN);
    const char *str = "0123456789";
    
    EXPECT_EQ(ring_buf_write_data(rb, str, strlen(str)), strlen(str));
    EXPECT_EQ(get_data_size(rb), strlen(str));
    EXPECT_EQ(get_left_size(rb), MAX_BUF_LEN - strlen(str) -1);
    EXPECT_EQ(ring_buf_destory(rb), 0);
}

TEST(ring_buf, read_data){
    ring_buf_t *rb = ring_buf_create(MAX_BUF_LEN);
    const char *str = "0123456789";
    char buf[20] = {0};
    
    EXPECT_EQ(ring_buf_write_data(rb, str, strlen(str)), strlen(str));
    EXPECT_EQ(ring_buf_read_data(rb,buf,sizeof(buf)), strlen(str));
    EXPECT_EQ(strcmp(buf, str), 0);
    EXPECT_EQ(get_left_size(rb), MAX_BUF_LEN -1);
    EXPECT_EQ(get_data_size(rb), 0);
    EXPECT_EQ(ring_buf_destory(rb), 0);
}

TEST(ring_buf, read_write_aroud){
    ring_buf_t *rb = ring_buf_create(MAX_BUF_LEN);
    const char *str = "0123456789";
    int write_n = 0, read_n = 0;
    char *read_buf = NULL;
    int i = 0;

    write_n = (MAX_BUF_LEN / strlen(str))/2 + 1;
    read_n = (MAX_BUF_LEN / strlen(str))/2;

    for(i = 0; i < write_n; i++){
        EXPECT_EQ(ring_buf_write_data(rb, str, strlen(str)), strlen(str));
    }
    EXPECT_EQ(get_left_size(rb), MAX_BUF_LEN - write_n * strlen(str) -1);
    EXPECT_EQ(get_data_size(rb), write_n * strlen(str));

    read_buf = (char *)malloc(read_n *strlen(str) + 1);
    memset(read_buf, 0, read_n *strlen(str) + 1);
    EXPECT_EQ(ring_buf_read_data(rb, read_buf, read_n *strlen(str)), read_n *strlen(str));
    printf("read data:%s\n", read_buf);

    
    EXPECT_EQ(get_left_size(rb), MAX_BUF_LEN - (write_n - read_n) * strlen(str) -1);
    EXPECT_EQ(get_data_size(rb), (write_n - read_n)* strlen(str));

    for(i = 0; i < write_n; i++){
        EXPECT_EQ(ring_buf_write_data(rb, str, strlen(str)), strlen(str));
    }
    EXPECT_EQ(get_left_size(rb), MAX_BUF_LEN - (2 * write_n - read_n) * strlen(str) -1);
    EXPECT_EQ(get_data_size(rb), (2 * write_n - read_n)* strlen(str));

    free(read_buf);
    int read_len = (2 * write_n - read_n)* strlen(str);
    read_buf = (char *)malloc(read_len + 1);

    memset(read_buf, 0, read_len + 1);
    EXPECT_EQ(ring_buf_read_data_only(rb, read_buf, read_len), read_len);
    EXPECT_EQ(get_left_size(rb), MAX_BUF_LEN - read_len - 1);
    EXPECT_EQ(get_data_size(rb), read_len);
    printf("read data:%s\n", read_buf);
    
    memset(read_buf, 0, read_len + 1);
    EXPECT_EQ(ring_buf_read_data(rb, read_buf, read_len), read_len);
    EXPECT_EQ(get_left_size(rb), MAX_BUF_LEN - 1);
    EXPECT_EQ(get_data_size(rb), 0);
    printf("read data:%s\n", read_buf);

    free(read_buf);
}







