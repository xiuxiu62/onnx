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

// void serialize_model(Stream &stream, const Model &model);
// void deserialize_model(Stream &stream, Model &model);
bool serialize_model(const char *filename, const Model &model);
bool deserialize_model(const char *filename, Model &model);
void free_model(Model &model);
// bool save_model(const char *filename, const Model &model);
// bool load_model(const char *filename, Model &model);
