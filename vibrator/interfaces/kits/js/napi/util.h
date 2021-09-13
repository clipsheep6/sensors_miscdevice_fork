/*
 * Copyright 2021 Institute of Software Chinese Academy of Sciences, ISRC
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef NAPI_UTIL_H
#define NAPI_UTIL_H

#include <napi/native_common.h>
#include <napi/native_node_api.h>
#include <string>

#define GET_PARAMS(env, info, num) \
    size_t argc = num;             \
    napi_value argv[num];          \
    napi_value thisVar;            \
    void* data;                    \
    napi_get_cb_info(env, info, &argc, argv, &thisVar, &data)

static inline napi_status util_value_to_stdstring(napi_env env, napi_value data,
    std::string *str)
{
    napi_status status;
    size_t size, size2;

    status = napi_get_value_string_utf8(env, data, nullptr, 0, &size);
    if (status != napi_ok)
        return status;

    str->resize(size);
    status = napi_get_value_string_utf8(env, data, &(*str)[0], size + 1, &size2);
    if (status != napi_ok)
        return status;

    if (size2 != size)
        str->resize(size2);
    return napi_ok;
}

static inline napi_status util_get_property_ref(napi_env env, napi_value object,
    const char *key, napi_ref *data)
{
    napi_status status;
    napi_value value;

    status = napi_get_named_property(env, object, key, &value);
    if (status != napi_ok) {
        return status;
    }
    return napi_create_reference(env, value, 1, data);
}

static inline napi_status util_get_property_stdstring(napi_env env, napi_value object,
    const char *key, std::string *data)
{
    napi_status status;
    napi_value value;

    status = napi_get_named_property(env, object, key, &value);
    if (status != napi_ok)
        return status;
    return util_value_to_stdstring(env, value, data);
}

static inline napi_status util_set_property_stdstring(napi_env env, napi_value object,
    const char *key, const std::string &data)
{
    napi_status status;
    napi_value value;

    status = napi_create_string_utf8(env, data.c_str(), data.size(), &value);
    if (status != napi_ok)
        return status;
    return napi_set_named_property(env, object, key, value);
}

static inline napi_status util_set_property_int32(napi_env env, napi_value object, const char *key, int32_t data)
{
    napi_status status;
    napi_value value;

    status = napi_create_int32(env, data, &value);
    if (status != napi_ok)
        return status;
    return napi_set_named_property(env, object, key, value);
}

#endif
