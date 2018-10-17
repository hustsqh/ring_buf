#ifndef __RING_BUF_H__
#define __RING_BUF_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

typedef struct{
    uint32_t size;
    uint32_t readIdx;
    uint32_t writeIdx;
    char buf[0];
}ring_buf_t;


ring_buf_t *ring_buf_create(size_t size);
int ring_buf_destory(ring_buf_t *rb);
size_t get_data_size(ring_buf_t *rb);
size_t get_left_size(ring_buf_t *rb);
size_t ring_buf_write_data(ring_buf_t *rb, const char *data, size_t len);
size_t ring_buf_read_data(ring_buf_t *rb, char *data, size_t len);
size_t ring_buf_read_data_only(ring_buf_t *rb, char *data, size_t len);

#ifdef __cplusplus
}
#endif

#endif
