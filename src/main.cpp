#include "logger.h"
#include "model.hpp"
#include "stream.hpp"
#include "tensor.hpp"
#include "types.h"

#include <cstdlib>
#include <cstring>

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
        .data_type = DataType::F32,
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

    serialize_model("models/test.onnx", model);
    free_model(model);
    memset(&model, 0, sizeof(Model));

    deserialize_model("models/test.onnx", model);
    free_model(model);

    return 0;
}
