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

#include "vibrator.h"
#include "util.h"
#include "AsyncWorkData.h"
#include "vibrator_if.h"
#include "vibrator_type.h"

struct VibratorData : public AsyncWorkData
{
    explicit VibratorData(napi_env env) : AsyncWorkData {env}
    {} 
    int code;
    uint32_t duration;
};

napi_value NAPIVibrator(napi_env env, napi_callback_info info)
{
    GET_PARAMS(env, info, 2);
    NAPI_ASSERT(env, argc >= 1, "requires 1 parameter");
    napi_value undefined, retval, resourceName;

    NAPI_CALL(env, napi_get_undefined(env, &undefined));
    NAPI_CALL(env, napi_get_cb_info(env, info, &argc, argv, NULL, NULL));
    struct VibratorData *callbackData = new VibratorData { env };
    std::unique_ptr<VibratorData> cleaner { callbackData };
    if (!callbackData->parse_callback_args(argc - 1, argv + 1, &retval)) {
        return nullptr;
    }
    NAPI_CALL(env, napi_get_value_uint32(env, argv[0], &callbackData->duration));
    NAPI_CALL(env, napi_create_string_latin1(env, __FUNCTION__, NAPI_AUTO_LENGTH, &resourceName));
    NAPI_CALL(env, napi_create_async_work(env,
        nullptr,
        resourceName,
        [](napi_env env, void *data) {
            struct VibratorData *callbackData = (struct VibratorData *)data;
            const VibratorInterface *vibratorDev = NewVibratorInterfaceInstance();
            if (!vibratorDev) {
                callbackData->code = 1;
                return;
            }
            if (vibratorDev->StartOnce(callbackData->duration) != 0) {
                callbackData->code = 2;
                return;
            }
            callbackData->code = 0;
        },
        [](napi_env env, napi_status status, void *data) {
            struct VibratorData *callbackData = (struct VibratorData *)data;
            std::unique_ptr<VibratorData> cleaner { callbackData };

            if (callbackData->code == 0) {
                callbackData->notify(nullptr, true);
            } else {
                callbackData->notify(nullptr, false);
            }
        },
        (void*)callbackData,
        &callbackData->async_work
    ));
    NAPI_CALL(env, napi_queue_async_work(env, callbackData->async_work));
    cleaner.release();
    return retval;
}
