#include "common.hpp"

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
