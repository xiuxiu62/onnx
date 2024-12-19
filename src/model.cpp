#include "model.hpp"

#include "file.h"
#include "io.hpp"

#include <cstring>

#define MAGIC "ONNX"

bool serialize_model(const char *filename, const Model &model) {
    FILE *file = open_file(filename, "wb");
    if (!file) {
        error("Error opening file for writing\n");
        return false;
    }

    // Write magic number
    write_bytes(file, MAGIC, strlen(MAGIC));

    // Write IR version
    usize version_len = strlen(model.version);
    write_bytes(file, &version_len, sizeof(usize));
    write_bytes(file, model.version, version_len);

    // Write producer name
    usize producer_len = strlen(model.producer);
    write_bytes(file, &producer_len, sizeof(usize));
    write_bytes(file, model.producer, producer_len);

    // Write graph
    write_bytes(file, &model.graph.node_count, sizeof(usize));
    write_bytes(file, &model.graph.tensor_count, sizeof(usize));

    // Write tensors
    for (usize i = 0; i < model.graph.tensor_count; i++) {
        serialize_tensor(file, model.graph.tensors[i]);
    }

    close_file(file);
    return true;
}

bool deserialize_model(const char *filename, Model &model) {
    FILE *file = open_file(filename, "rb");
    if (!file) {
        error("Error opening file for reading\n");
        return false;
    }

    // Read and verify magic number
    char magic[5] = {0};
    read_bytes(file, magic, 4);
    if (strcmp(magic, MAGIC) != 0) {
        printf("Invalid ONNX file format\n");
        close_file(file);
        return false;
    }

    // Read IR Version
    usize version_len;
    read_bytes(file, &version_len, sizeof(usize));
    model.version = static_cast<char *>(malloc(version_len + 1));
    auto ir_version = const_cast<char *>(model.version);
    read_bytes(file, ir_version, version_len);
    ir_version[version_len] = '\0';

    // Read Producer name
    usize producer_len;
    read_bytes(file, &producer_len, sizeof(usize));
    model.producer = static_cast<char *>(malloc(producer_len + 1));
    auto producer_name = const_cast<char *>(model.producer);
    read_bytes(file, producer_name, producer_len);
    producer_name[producer_len] = '\0';

    // Read Graph
    read_bytes(file, &model.graph.node_count, sizeof(usize));
    read_bytes(file, &model.graph.tensor_count, sizeof(usize));

    // Read Tensors
    model.graph.tensors = static_cast<Tensor *>(malloc(model.graph.tensor_count * sizeof(Tensor)));
    for (usize i = 0; i < model.graph.tensor_count; i++) {
        Tensor &tensor = model.graph.tensors[i];
        deserialize_tensor(file, tensor);
        memcpy(&model.graph.tensors[i], &tensor, sizeof(tensor));
    }

    close_file(file);

    return true;
}

void free_model(Model &model) {
    free(const_cast<char *>(model.version));
    free(const_cast<char *>(model.producer));

    for (usize i = 0; i < model.graph.tensor_count; i++) {
        free_tensor(model.graph.tensors[i]);
    }
    free(model.graph.tensors);
}
