#include "onnx/common.hpp"

#include "io.hpp"
#include "types.h"

#include <cstdlib>
#include <cstring>

void serialize_string(FILE *file, const char *str) {
    if (str == nullptr) {
        usize len = 0;
        write_bytes(file, &len, sizeof(usize));
        return;
    }

    usize len = strlen(str) + 1;
    write_bytes(file, &len, sizeof(usize));
    write_bytes(file, str, len);
}

void deserialize_string(FILE *file, char *&str) {
    usize len;
    read_bytes(file, &len, sizeof(usize));
    if (len == 0) {
        str = nullptr;
        return;
    }
    str = static_cast<char *>(malloc(len));
    read_bytes(file, str, len);
}

void serialize_string_array(FILE *file, char **data, u32 count) {
    write_bytes(file, &count, sizeof(u32));
    for (usize i = 0; i < count; i++) {
        serialize_string(file, data[i]);
    }
}

void deserialize_string_array(FILE *file, char **&data, u32 count) {
    read_bytes(file, &count, sizeof(u32));
    if (count == 0) {
        data = nullptr;
        return;
    }

    data = static_cast<char **>(malloc(count * sizeof(char *)));
    for (usize i = 0; i < count; i++) {
        deserialize_string(file, data[i]);
    }
}

char *duplicate_string(const char *str) {
    if (!str) return NULL;
    char *dup = (char *)malloc(strlen(str) + 1);
    strcpy(dup, str);
    return dup;
}

void *resize_array(void *arr, usize old_count, usize new_count, usize stride) {
    void *new_arr = malloc(new_count * stride);
    if (arr && old_count > 0) {
        memcpy(new_arr, arr, old_count * stride);
        free(arr);
    }
    return new_arr;
}
