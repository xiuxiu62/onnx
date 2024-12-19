#include "core/file.h"
#include "core/types.h"

#include <string>
#include <vcruntime_string.h>

#define MAGIC "ONNX"

enum class DataType {
    Float = 1,
    U8 = 2,
    I8 = 3,
    I32 = 6,
    I64 = 7,
};

struct Tensor {
    DataType data_type;
    u32 dim_count;
    u64 *dims;
    void *data;
    usize data_size;
};

// Representing operations
struct Node {
    char *op_type;
    char **input_names;
    int input_count;
    char **output_names;
    int output_count;
};

struct Graph {
    Node *nodes;
    Tensor *tensors;
    usize node_count, tensor_count;
};

struct Model {
    const char *version;
    const char *producer;
    Graph graph;
};

void write_bytes(FILE *file, const void *data, usize size) {
    const u8 *bytes = static_cast<const u8 *>(data);
    for (usize i = 0; i < size; i++) {
        fputc(bytes[i], file);
    }
}

void read_bytes(FILE *file, void *data, usize size) {
    u8 *bytes = static_cast<u8 *>(data);
    for (usize i = 0; i < size; i++) {
        bytes[i] = fgetc(file);
    }
}

void serialize_tensor(FILE *file, const Tensor &tensor) {
    write_bytes(file, &tensor.data_type, sizeof(DataType));

    write_bytes(file, &tensor.dim_count, sizeof(i32));
    for (usize i = 0; i < tensor.dim_count; i++) {
        write_bytes(file, &tensor.dims[i], sizeof(i64));
    }

    write_bytes(file, &tensor.data_size, sizeof(usize));
    write_bytes(file, tensor.data, tensor.data_size);
}

void deserialize_tensor(FILE *file, Tensor &tensor) {
    read_bytes(file, &tensor.data_type, sizeof(DataType));

    read_bytes(file, &tensor.dim_count, sizeof(i32));
    tensor.dims = (u64 *)malloc(tensor.dim_count * sizeof(i64));
    for (int i = 0; i < tensor.dim_count; i++) {
        read_bytes(file, &tensor.dims[i], sizeof(i64));
    }

    read_bytes(file, &tensor.data_size, sizeof(usize));
    tensor.data = malloc(tensor.data_size);
    read_bytes(file, tensor.data, tensor.data_size);
}

bool save_model(const char *filename, const Model &model) {
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

bool load_model(const char *filename, Model &model) {
    FILE *file = open_file(filename, "rb");
    if (!file) {
        error("Error opening file for reading\n");
        return NULL;
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

void free_tensor(Tensor &tensor) {
    free(tensor.dims);
    free(tensor.data);
}

void free_model(Model &model) {
    free(const_cast<char *>(model.version));
    free(const_cast<char *>(model.producer));

    for (usize i = 0; i < model.graph.tensor_count; i++) {
        free_tensor(model.graph.tensors[i]);
    }
    free(model.graph.tensors);
}

int main() {
    char *version = static_cast<char *>(malloc(strlen("1.0") + 1));
    strcpy(version, "1.0");
    char *producer = static_cast<char *>(malloc(strlen("example") + 1));
    strcpy(producer, "example");

    u64 *dims = static_cast<u64 *>(malloc(sizeof(u64) * 2));
    dims[0] = 2;
    dims[1] = 3;

    f32 *data = static_cast<f32 *>(malloc(sizeof(f32) * 6));
    for (usize i = 0; i < 6; i++) {
        data[i] = static_cast<f32>(i + 1);
    }

    Tensor *tensors = static_cast<Tensor *>(malloc(sizeof(Tensor)));
    *tensors = {
        .data_type = DataType::Float,
        .dim_count = 2,
        .dims = dims,
        .data = data,
        .data_size = 24,
    };

    Model model = {
        .version = version,
        .producer = producer,
        .graph = {.tensors = tensors, .node_count = 0, .tensor_count = 1},
    };

    save_model("test.onnx", model);
    free_model(model);
    memset(&model, 0, sizeof(Model));

    load_model("test.onnx", model);
    free_model(model);

    return 0;
}
