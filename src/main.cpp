#include "logger.h"
#include "model.hpp"

int main() {
    Model model;
    create_model(model, "1.0", "xiu");

    Node &relu = add_node(model, "relu1", OpType::Relu);
    add_input(relu, "input");
    add_output(relu, "output");

    u64 dims[] = {2, 3};
    Tensor &tensor = add_tensor(model, DataType::F32, dims, 2);

    serialize_model("example.onnx", model);
    free_model(model);
    Model model_2;
    deserialize_model("example.onnx", model_2);
    info("%s", model_2.graph.nodes[0].input_names[0]);

    return 0;
}
