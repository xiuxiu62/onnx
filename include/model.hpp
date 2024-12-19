#pragma once

#include "node.hpp"
#include "tensor.hpp"

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

bool serialize_model(const char *filename, const Model &model);
bool deserialize_model(const char *filename, Model &model);
void free_model(Model &model);

void create_model(Model &model, const char *version, const char *producer);
Node &add_node(Model &model, const char *name, OpType type);
bool remove_node(Model &model, const char *name);
bool add_input(Node &node, const char *input_name);
bool add_output(Node &node, const char *output_name);
Attribute &add_attribute(Node &node, const char *name, AttributeType type);
Tensor &add_tensor(Model &model, DataType type, const u64 *dims, u32 dim_count);
bool remove_tensor(Model &model, usize index);
