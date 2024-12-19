#pragma once

#include "logger.h"
#include "types.h"

#include <cstdio>
#include <cstdlib>

static inline void close_file(FILE *file) {
    fclose(file);
}

static FILE *open_file(const char *path, const char *modes) {
    FILE *file;
#ifdef _WIN32
    errno_t err = fopen_s(&file, path, modes);
    if (err) {
        if (file) close_file(file);
        file = nullptr;
    }
#else
    file = fopen(path, modes);
#endif /* ifdef WIN32 */
    return file;
}

// Reads contents of a file into a string
// This allocates and must be cleaned up
static const char *file_read_to_string(const char *path) {
    char *contents = nullptr;
#define finish()                                                                                                       \
    close_file(file);                                                                                                  \
    return contents;

    FILE *file = open_file(path, "rb");
    if (!file) {
        info("Failed to open file: %s", path);
        finish();
    }

    fseek(file, 0, SEEK_END);
    usize len = ftell(file);
    fseek(file, 0, SEEK_SET);

    contents = static_cast<char *>(calloc(len + 1 /* for null termination */, sizeof(char)));
    if (!contents) {
        info("Failed to allocate memory for shader code\n");
        finish();
    }

    usize bytes_read = fread(contents, 1, len, file);
    if (bytes_read != len) {
        info("Failed to read shader file\n");
    }
    contents[len] = '\0'; // null terminate
    return contents;
}
