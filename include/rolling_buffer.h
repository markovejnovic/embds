/**
 * This header defines generics used for implementing type-safe rolling-buffers
 * in C.
 *
 * Example Usage:
 *
 * EMBDS_DEF_CBUFF_TYPE_STRUCT(my_type_t);
 * EMBDS_DEF_CBUFF_TYPE_INTERFACE(my_type_t);
 * EMBDS_IMPL_CBUFF_TYPE(my_type_t);
 *
 * static my_type memory[5];
 * static embds_cbuff_my_type_t my_buffer = EMBDS_NEW_CBUFF(memory, 5);
 *
 * int main() {
 *     assert(embds_cbuff_my_type_t_push(&my_buffer, my_type_instance)
 *            == EMBDS_CBUFF_ERROR_SUCCESS);
 *
 *     my_type* destination;
 *     assert(embds_cbuff_my_type_t_pop(&my_buffer, &destination)
 *            == EMBDS_CBUFF_ERROR_SUCCESS);
 *     assert(destination == my_type_instance);
 *
 *     assert(embds_cbuff_my_type_t_push(&my_buffer, my_type_instance)
 *            == EMBDS_CBUFF_ERROR_SUCCESS);
 *     embds_cbuff_my_type_t_peek(&my_buffer, 0, &destination);
 *     assert(destination == my_type_instance);
 *
 *     return 0;
 * }
 *
 * Copyright 2022 Marko Vejnovic
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */

#ifndef __EMBDS_CIRCULAR_BUFFER_H__
#define __EMBDS_CIRCULAR_BUFFER_H__

#include <stdlib.h>

typedef enum {
    EMBDS_CBUFF_ERROR_SUCCESS = 0,
    EMBDS_CBUFF_ERROR_OUT_OF_BOUNDS,
} embds_cbuff_ret_t;

#define __EMBDS_CBUFF_BASE_STRUCT \
    size_t head_idx; \
    size_t tail_idx; \
    const size_t sz;

#define EMBDS_DEF_CBUFF_TYPE_STRUCT(type) \
    typedef struct { \
        type * const data_buf; \
        __EMBDS_CBUFF_BASE_STRUCT \
    } embds_cbuff_##type##_t; \

#define EMBDS_NEW_CBUFF(data_ptr, sz) { \
        .data_buf = data_ptr, \
        .head_idx = 0, \
        .tail_idx = 0, \
        .sz = sz \
    }

#define EMBDS_DEF_CBUFF_TYPE_INTERFACE(type) \
    embds_cbuff_ret_t embds_cbuff_##type##_push( \
            embds_cbuff_##type##_t* const cbuff, \
            type data); \
\
    embds_cbuff_ret_t embds_cbuff_##type##_pop( \
            embds_cbuff_##type##_t* const cbuff, \
            type * const data); \
\
    embds_cbuff_ret_t embds_cbuff_##type##_peek( \
            embds_cbuff_##type##_t* const cbuff, \
            const size_t idx, type * const data);

#define EMBDS_IMPL_CBUFF_TYPE(type) \
    embds_cbuff_ret_t embds_cbuff_##type##_push( \
            embds_cbuff_##type##_t* const cbuff, \
            type data) { \
        const size_t next = (cbuff->head_idx + 1) % cbuff->sz; \
\
        if (next == cbuff->tail_idx) { \
            return EMBDS_CBUFF_ERROR_OUT_OF_BOUNDS; \
        } \
\
        cbuff->data_buf[cbuff->head_idx] = data; \
        cbuff->head_idx = next; \
\
        return EMBDS_CBUFF_ERROR_SUCCESS; \
    } \
\
    embds_cbuff_ret_t embds_cbuff_##type##_pop( \
            embds_cbuff_##type##_t* const cbuff, \
            type * const data) { \
        if (cbuff->head_idx == cbuff->tail_idx) { \
            return EMBDS_CBUFF_ERROR_OUT_OF_BOUNDS; \
        } \
\
        const size_t next = (cbuff->tail_idx + 1) % cbuff->sz; \
        if (data != NULL) { \
            *data = cbuff->data_buf[cbuff->tail_idx]; \
        } \
        cbuff->tail_idx = next; \
\
        return EMBDS_CBUFF_ERROR_SUCCESS; \
    } \
\
    embds_cbuff_ret_t embds_cbuff_##type##_peek( \
            embds_cbuff_##type##_t* const cbuff, \
            const size_t idx, type * const data) { \
        if (idx >= cbuff->sz) { \
            return EMBDS_CBUFF_ERROR_OUT_OF_BOUNDS; \
        } \
\
        const size_t real_idx = (cbuff->tail_idx + idx) % cbuff->sz; \
        *data = cbuff->data_buf[cbuff->tail_idx]; \
\
        return EMBDS_CBUFF_ERROR_SUCCESS; \
    }

#endif // __EMBDS_CIRCULAR_BUFFER__
