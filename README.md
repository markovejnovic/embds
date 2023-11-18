# [ARCHIVED]

This project has become archived. I am not particularly interested in exploring
the possibilities of this project as my use-case has shifted to Zephyr which
already supports almost everything I need.

# embds

This repository is a collection of datastructures designed for embedded
development in C.

## rolling\_buffer.h

This file implements generics for creating type-safe rolling-buffer
implementations.

### Example Usage

```c
EMBDS_DEF_CBUFF_TYPE_STRUCT(my_type_t); // Normally goes into a .h
EMBDS_DEF_CBUFF_TYPE_INTERFACE(my_type_t); // Normally goes into a .h
EMBDS_IMPL_CBUFF_TYPE(my_type_t); // Normally goes into a .c

static my_type memory[5];
static embds_cbuff_my_type_t my_buffer = EMBDS_NEW_CBUFF(memory, 5);

int main() {
    assert(embds_cbuff_my_type_t_push(&my_buffer, my_type_instance)
           == EMBDS_CBUFF_ERROR_SUCCESS);

    my_type* destination;
    assert(embds_cbuff_my_type_t_pop(&my_buffer, &destination)
           == EMBDS_CBUFF_ERROR_SUCCESS);
    assert(destination == my_type_instance);

    assert(embds_cbuff_my_type_t_push(&my_buffer, my_type_instance)
           == EMBDS_CBUFF_ERROR_SUCCESS);
    embds_cbuff_my_type_t_peek(&my_buffer, 0, &destination);
    assert(destination == my_type_instance);

    return 0;
}
```
