#include "model.hpp"
#include "tensor.hpp"
#include "types.h"

#include <cstdlib>
#include <cstring>

int main() {
    // Create version and producer strings
    char *version = static_cast<char *>(malloc(strlen("1.0") + 1));
    strcpy(version, "1.0");
    char *producer = static_cast<char *>(malloc(strlen("example") + 1));
    strcpy(producer, "example");

    // Create a tensor
    u64 *dims = static_cast<u64 *>(malloc(sizeof(u64) * 2));
    dims[0] = 2;
    dims[1] = 3;

    f32 *data = static_cast<f32 *>(malloc(sizeof(f32) * 6));
    for (usize i = 0; i < 6; i++) {
        data[i] = static_cast<f32>(i + 1);
    }

    Tensor *tensors = static_cast<Tensor *>(malloc(sizeof(Tensor)));
    *tensors = {
        .data_type = DataType::F32,
        .dim_count = 2,
        .dims = dims,
        .data = data,
        .data_size = 24,
        .owns_data = true,
    };

    // Create a node (example: ReLU node)
    Node *nodes = static_cast<Node *>(malloc(sizeof(Node)));

    // Set node name
    nodes->name = static_cast<char *>(malloc(strlen("relu1") + 1));
    strcpy(nodes->name, "relu1");

    // Set domain (empty for default ONNX domain)
    nodes->domain = static_cast<char *>(malloc(1));
    nodes->domain[0] = '\0';

    // Set doc string
    nodes->doc_string = static_cast<char *>(malloc(strlen("ReLU activation") + 1));
    strcpy(nodes->doc_string, "ReLU activation");

    // Set operation type
    nodes->op_type = OpType::Relu;

    // Set input names
    nodes->input_count = 1;
    nodes->input_names = static_cast<char **>(malloc(sizeof(char *)));
    nodes->input_names[0] = static_cast<char *>(malloc(strlen("input") + 1));
    strcpy(nodes->input_names[0], "input");

    // Set output names
    nodes->output_count = 1;
    nodes->output_names = static_cast<char **>(malloc(sizeof(char *)));
    nodes->output_names[0] = static_cast<char *>(malloc(strlen("output") + 1));
    strcpy(nodes->output_names[0], "output");

    // No attributes for ReLU
    nodes->attributes = nullptr;
    nodes->attribute_count = 0;

    // Create and populate model
    Model model = {
        .version = version,
        .producer = producer,
        .graph = {.nodes = nodes, .tensors = tensors, .node_count = 1, .tensor_count = 1},
    };

    // Serialize, free, then deserialize
    serialize_model("models/test.onnx", model);
    free_model(model);
    memset(&model, 0, sizeof(Model));

    deserialize_model("models/test.onnx", model);
    free_model(model);

    return 0;
}

// int main() {
//     char *version = static_cast<char *>(malloc(strlen("1.0") + 1));
//     strcpy(version, "1.0");
//     char *producer = static_cast<char *>(malloc(strlen("example") + 1));
//     strcpy(producer, "example");

//     u64 *dims = static_cast<u64 *>(malloc(sizeof(u64) * 2));
//     dims[0] = 2;
//     dims[1] = 3;

//     f32 *data = static_cast<f32 *>(malloc(sizeof(f32) * 6));
//     for (usize i = 0; i < 6; i++) {
//         data[i] = static_cast<f32>(i + 1);
//     }

//     Tensor *tensors = static_cast<Tensor *>(malloc(sizeof(Tensor)));
//     *tensors = {
//         .data_type = DataType::F32,
//         .dim_count = 2,
//         .dims = dims,
//         .data = data,
//         .data_size = 24,
//         .owns_data = true,
//     };

//     Model model = {
//         .version = version,
//         .producer = producer,
//         .graph = {.tensors = tensors, .node_count = 0, .tensor_count = 1},
//     };

//     serialize_model("models/test.onnx", model);
//     free_model(model);
//     memset(&model, 0, sizeof(Model));

//     deserialize_model("models/test.onnx", model);
//     free_model(model);

//     return 0;
// }
