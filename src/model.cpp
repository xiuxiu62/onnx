#include "model.hpp"

#include "common.hpp"
#include "file.h"
#include "io.hpp"
#include "tensor.hpp"

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

    // Write graph node and tensor counts
    write_bytes(file, &model.graph.node_count, sizeof(usize));
    write_bytes(file, &model.graph.tensor_count, sizeof(usize));

    // Write nodes
    for (usize i = 0; i < model.graph.node_count; i++) {
        serialize_node(file, model.graph.nodes[i]);
    }

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

    // Read Graph node and tensor counts
    read_bytes(file, &model.graph.node_count, sizeof(usize));
    read_bytes(file, &model.graph.tensor_count, sizeof(usize));

    // Read Nodes
    model.graph.nodes = static_cast<Node *>(malloc(model.graph.node_count * sizeof(Node)));
    for (usize i = 0; i < model.graph.node_count; i++) {
        deserialize_node(file, model.graph.nodes[i]);
    }

    // Read Tensors
    model.graph.tensors = static_cast<Tensor *>(malloc(model.graph.tensor_count * sizeof(Tensor)));
    for (usize i = 0; i < model.graph.tensor_count; i++) {
        deserialize_tensor(file, model.graph.tensors[i]);
    }

    close_file(file);
    return true;
}

void free_model(Model &model) {
    // Free version and producer strings
    free(const_cast<char *>(model.version));
    free(const_cast<char *>(model.producer));

    // Free nodes
    for (usize i = 0; i < model.graph.node_count; i++) {
        free_node(model.graph.nodes[i]);
    }
    free(model.graph.nodes);

    // Free tensors
    for (usize i = 0; i < model.graph.tensor_count; i++) {
        free_tensor(model.graph.tensors[i]);
    }
    free(model.graph.tensors);
}

void create_model(Model &model, const char *version, const char *producer) {
    model = {
        .version = duplicate_string(version),
        .producer = duplicate_string(producer),
        .graph =
            {
                .nodes = nullptr,
                .tensors = nullptr,
                .node_count = 0,
                .tensor_count = 0,
            },
    };
}

Node &add_node(Model &model, const char *name, OpType type) {
    model.graph.nodes = static_cast<Node *>(
        resize_array(model.graph.nodes, model.graph.node_count, model.graph.node_count + 1, sizeof(Node)));

    Node &node = model.graph.nodes[model.graph.node_count];
    node.name = duplicate_string(name);
    node.domain = duplicate_string(""); // Default empty domain
    node.doc_string = duplicate_string("");
    node.op_type = type;
    node.input_names = nullptr;
    node.output_names = nullptr;
    node.input_count = 0;
    node.output_count = 0;
    node.attributes = nullptr;
    node.attribute_count = 0;

    model.graph.node_count++;
    return node;
}

bool remove_node(Model &model, const char *name) {
    for (usize i = 0; i < model.graph.node_count; i++) {
        Node &node = model.graph.nodes[i];
        if (strcmp(node.name, name) == 0) {
            free_node(node);

            // Shift remaining nodes
            for (usize j = i; j < model.graph.node_count - 1; j++) {
                model.graph.nodes[j] = model.graph.nodes[j + 1];
            }

            model.graph.node_count--;
            if (model.graph.node_count > 0) {
                model.graph.nodes = static_cast<Node *>(
                    resize_array(model.graph.nodes, model.graph.node_count + 1, model.graph.node_count, sizeof(Node)));
            } else {
                free(model.graph.nodes);
                model.graph.nodes = nullptr;
            }
            return true;
        }
    }
    return false;
}

bool add_input(Node &node, const char *input_name) {
    node.input_names = (char **)resize_array(node.input_names, node.input_count, node.input_count + 1, sizeof(char *));
    node.input_names[node.input_count] = duplicate_string(input_name);
    node.input_count++;
    return true;
}

bool add_output(Node &node, const char *output_name) {
    node.output_names =
        static_cast<char **>(resize_array(node.output_names, node.output_count, node.output_count + 1, sizeof(char *)));
    node.output_names[node.output_count] = duplicate_string(output_name);
    node.output_count++;
    return true;
}

Attribute &add_attribute(Node &node, const char *name, AttributeType type) {
    node.attributes = static_cast<Attribute *>(
        resize_array(node.attributes, node.attribute_count, node.attribute_count + 1, sizeof(Attribute)));

    Attribute &attr = node.attributes[node.attribute_count];
    attr.name = duplicate_string(name);
    attr.type = type;
    memset(&attr.value, 0, sizeof(AttributeValue));

    node.attribute_count++;
    return attr;
}

Tensor &add_tensor(Model &model, DataType type, const u64 *dims, u32 dim_count) {
    model.graph.tensors = static_cast<Tensor *>(
        resize_array(model.graph.tensors, model.graph.tensor_count, model.graph.tensor_count + 1, sizeof(Tensor)));

    Tensor &tensor = model.graph.tensors[model.graph.tensor_count];
    tensor.data_type = type;
    tensor.dim_count = dim_count;

    tensor.dims = (u64 *)malloc(dim_count * sizeof(u64));
    memcpy(tensor.dims, dims, dim_count * sizeof(u64));

    // Calculate total size
    usize total_size = 1;
    for (u32 i = 0; i < dim_count; i++) {
        total_size *= dims[i];
    }

    tensor.data_size = total_size * data_type_size(type);
    tensor.data = malloc(tensor.data_size);
    tensor.owns_data = true;

    model.graph.tensor_count++;
    return tensor;
}

bool remove_tensor(Model &model, usize index) {
    if (index >= model.graph.tensor_count) return false;

    free_tensor(model.graph.tensors[index]);

    for (usize i = index; i < model.graph.tensor_count - 1; i++) {
        model.graph.tensors[i] = model.graph.tensors[i + 1];
    }

    model.graph.tensor_count--;
    if (model.graph.tensor_count > 0) {
        model.graph.tensors = (Tensor *)resize_array(model.graph.tensors, model.graph.tensor_count + 1,
                                                     model.graph.tensor_count, sizeof(Tensor));
    } else {
        free(model.graph.tensors);
        model.graph.tensors = NULL;
    }
    return true;
}
