#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ring_buf.h"

#define MAX(a,b) ((a) >= (b))? (a):(b)
#define MIN(a,b) ((a) <= (b))? (a):(b)

ring_buf_t *ring_buf_create(size_t size)
{
    ring_buf_t *rb = NULL;
    
    if(size == 0){
        return NULL;
    }

    rb = (ring_buf_t *)malloc(sizeof(ring_buf_t) + size);
    if(!rb){
        return rb;
    }

    memset(rb, 0, sizeof(ring_buf_t) + size);
    rb->size = size;
    rb->readIdx = 0;
    rb->writeIdx = 0;

    return rb;
}


int ring_buf_destory(ring_buf_t *rb)
{
    if(!rb){
        return -1;
    }

    free(rb);

    return 0;
}


size_t get_data_size(ring_buf_t *rb)
{
    if(rb->writeIdx >= rb->readIdx){
        return rb->writeIdx - rb->readIdx;
    }

    return (rb->size - rb->readIdx + rb->writeIdx);
}

size_t get_left_size(ring_buf_t *rb)
{
    return (rb->size - get_data_size(rb) -1);
}

size_t ring_buf_write_data(ring_buf_t *rb, const char *data, size_t len)
{
    size_t cp_right = 0;
    size_t cp_left = 0;
    size_t lef_size = 0;
    
    if(!data || !len){
        return 0;
    }

    lef_size = get_left_size(rb);

    if(lef_size < len){
        return 0;
    }

    cp_right = MIN(rb->size - rb->writeIdx, lef_size);
    if(len <= cp_right){
        memcpy(rb->buf + rb->writeIdx, data, len);
        rb->writeIdx += len;
        if(rb->writeIdx == rb->size){
            rb->writeIdx = 0;
        }
    }else{
        cp_left = len - cp_right;
        memcpy(rb->buf + rb->writeIdx, data, cp_right);
        memcpy(rb->buf, data + cp_right, cp_left);
        rb->writeIdx = cp_left;
    }

    return len;
}


static size_t ring_buf_read_data_inner(ring_buf_t *rb, char *data, size_t len, int pop)
{
    size_t cp_right = 0;
    size_t cp_left = 0;
    
    if(!data || !len){
        return 0;
    }

    if(rb->writeIdx >= rb->readIdx){
        cp_right = MIN(rb->writeIdx - rb->readIdx, len);
        memcpy(data, rb->buf + rb->readIdx, cp_right);
        if(pop){
            rb->readIdx += cp_right;
        }
        return cp_right;
    }else{
        cp_right = rb->size - rb->readIdx;
        if(len <= cp_right){
            memcpy(data, rb->buf + rb->readIdx, len);
            if(pop){
                rb->readIdx += len;
                if(rb->readIdx == rb->size){
                    rb->readIdx = 0;
                }
            }
            return len;
        }else{
            memcpy(data, rb->buf + rb->readIdx, cp_right);
            cp_left = MIN(len - cp_right, rb->writeIdx);
            memcpy(data + cp_right, rb->buf, cp_left);
            if(pop){
                rb->readIdx = cp_left;
            }
            return cp_right + cp_left;
        }
    }
}

size_t ring_buf_read_data(ring_buf_t *rb, char *data, size_t len)
{
    return ring_buf_read_data_inner(rb, data, len, 1);
}

size_t ring_buf_read_data_only(ring_buf_t *rb, char *data, size_t len)
{
    return ring_buf_read_data_inner(rb, data, len, 0);
}


