#ifndef UTILS_H
#define UTILS_H
#include <stddef.h>

typedef struct SizedBuffer {
    size_t size;
    char *buffer;
} SizedBuffer;

#define DEFINE_DYNAMIC_ARRAY(Type) \
typedef struct DynamicArray_##Type { \
    size_t capacity; \
    size_t occupied; \
    Type *buffer; \
} DynamicArray_##Type

typedef struct CliOptions {
    char *startupCmd;
    char *configPath;
} CliOptions;

#endif
