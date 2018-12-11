/*******************************************************************************
* Copyright 2019 Intel Corporation
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*     http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*******************************************************************************/

#include <CL/sycl.hpp>

#include "common/c_types_map.hpp"
#include "common/engine.hpp"
#include "common/stream.hpp"
#include "common/utils.hpp"
#include "sycl/capi.hpp"
#include "sycl/sycl_engine.hpp"
#include "sycl/sycl_stream.hpp"

using namespace mkldnn::impl;

status_t mkldnn_stream_create_sycl(
        stream_t **stream, engine_t *engine, void *queue) {
    bool args_ok = true
            && !utils::any_null(stream, engine, queue)
            && engine->kind() == engine_kind::gpu;
    if (!args_ok)
        return status::invalid_arguments;

    auto *sycl_engine = utils::downcast<sycl::sycl_engine_base_t *>(engine);
    auto &sycl_queue = *static_cast<cl::sycl::queue *>(queue);
    return sycl_engine->create_stream(stream, sycl_queue);
}

status_t mkldnn_stream_get_sycl_queue(stream_t *stream,
        void **queue) {
    bool args_ok = true
        && !utils::any_null(queue, stream)
        && stream->engine()->backend_kind() == backend_kind::sycl;

    if (!args_ok)
        return status::invalid_arguments;

    auto sycl_stream = utils::downcast<sycl::sycl_stream_t*>(stream);
    auto &sycl_queue = sycl_stream->queue();
    *queue = static_cast<void *>(&sycl_queue);
    return status::success;
}
