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

#ifndef NAPI_ASYNCWORKDATA_H
#define NAPI_ASYNCWORKDATA_H

#include <napi/native_common.h>
#include <napi/native_node_api.h>

class AsyncWorkData
{
public:
    AsyncWorkData(napi_env env)
    {
        this->env = env;
    }
    ~AsyncWorkData()
    {
        for (unsigned int i = 0; i < sizeof(callbacks) / sizeof(napi_ref); ++i) {
            if (callbacks[i]) {
                napi_delete_reference(env, callbacks[i]);
            }
        }
        if (async_work) {
            napi_delete_async_work(env, async_work);
        }
    }
    bool parse_callback_args(size_t argc, napi_value argv[], napi_value *retval)
    {
        napi_value cb[3] = { nullptr, nullptr, nullptr };
        napi_valuetype type;

        NAPI_ASSERT_BASE(env, argc <= 3, "Wrong number of arguments", false);
        for (unsigned int i = 0; i < argc; ++i) {
            NAPI_CALL_BASE(env, napi_typeof(env, argv[i], &type), false);
            if (type == napi_undefined || type == napi_null) {
                continue;
            }
            NAPI_ASSERT_BASE(env, type == napi_function, "Wrong argument type. Function expected.", false);
            cb[i] = argv[i];
        }
        for (unsigned int i = 0; i < argc; ++i) {
            if (cb[i]) {
                NAPI_CALL_BASE(env, napi_create_reference(env, cb[i], 1, &callbacks[i]), false);
            }
        }
        for (unsigned int i = 0; i < sizeof(callbacks) / sizeof(napi_ref); ++i) {
            if (callbacks[i]) {
                *retval = nullptr;
                return true;
            }
        }
        NAPI_CALL_BASE(env, napi_create_promise(env, &deferred, retval), false);
        return true;
    }

    void notify_multiple_argument(size_t argc, napi_value argv[], const char *names[], bool resolved = true)
    {
        napi_value param;
        // promise mode
        if (!deferred) {
            goto callbackMode;
        }
        if (napi_create_object(env, &param) != napi_ok) {
            goto callbackMode;
        }
        for (size_t i = 0; i < argc; ++i) {
            if (napi_set_named_property(env, param, names[i], argv[i]) != napi_ok) {
                goto callbackMode;
            }
        }
        if (resolved) {
            napi_resolve_deferred(env, deferred, param);
        } else {
            napi_reject_deferred(env, deferred, param);
        }
    callbackMode:
        // callback mode
        napi_value undefined;
        if (napi_get_undefined(env, &undefined) != napi_ok) {
            return;
        }
        auto make_callback = [this, undefined](napi_ref ref_function, size_t argc, napi_value argv[])
        {
            napi_value cb, call_result;
            if (!ref_function) {
                return;
            }
            if (napi_get_reference_value(env, ref_function, &cb) != napi_ok) {
                return;
            }
            if (argc) {
                napi_call_function(env, undefined, cb, argc, argv, &call_result);
            } else {
                napi_call_function(env, undefined, cb, 0, nullptr, &call_result);
            }
        };

        if (resolved) {
            make_callback(callbacks[0], argc, argv);
        } else {
            make_callback(callbacks[1], argc, argv);
        }
        make_callback(callbacks[2], 0, nullptr);
    }

    void notify(napi_value param, bool resolved = true)
    {
        // notify promise
        if (deferred) {
            if (resolved) {
                napi_resolve_deferred(env, deferred, param);
            } else {
                napi_reject_deferred(env, deferred, param);
            }
        }
        // callback mode
        napi_value undefined;

        if (napi_get_undefined(env, &undefined) != napi_ok) {
            return;
        }

        auto make_callback = [this, undefined](napi_ref ref_function, napi_value param) {
            napi_value cb, call_result;
            if (!ref_function) {
                return;
            }
            if (napi_get_reference_value(env, ref_function, &cb) != napi_ok) {
                return;
            }
            if (param) {
                napi_call_function(env, undefined, cb, 1, &param, &call_result);
            } else {
                napi_call_function(env, undefined, cb, 0, nullptr, &call_result);
            }
        };
        // make success or fail callback
        if (resolved) {
            make_callback(callbacks[0], param);
        } else {
            make_callback(callbacks[1], param);
        }
        make_callback(callbacks[2], nullptr);
    }
    napi_async_work async_work = nullptr;
protected:
    napi_env env;
private:
    napi_ref callbacks[3] = {nullptr};
    napi_deferred deferred = nullptr;
};

#endif

