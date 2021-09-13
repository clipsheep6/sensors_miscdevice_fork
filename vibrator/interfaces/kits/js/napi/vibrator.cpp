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
#include "asyncworkdata.h"
#include "vibrator_if.h"
#include "vibrator_type.h"

struct VibratorData : public AsyncWorkData
{
    VibratorData(napi_env env) : AsyncWorkData {env}
    {} 
    int code;
    uint32_t duration;
};

napi_value NAPI_Vibrator_vibrate(napi_env env, napi_callback_info info)
{
    GET_PARAMS(env, info, 2);
    NAPI_ASSERT(env, argc >= 1, "requires 1 parameter");
    napi_value undefined, retval, resourceName;

    NAPI_CALL(env, napi_get_undefined(env, &undefined));
    NAPI_CALL(env, napi_get_cb_info(env, info, &argc, argv, NULL, NULL));
    struct VibratorData *callback_data = new VibratorData { env };
    std::unique_ptr<VibratorData> cleaner { callback_data };
    if (!callback_data->parse_callback_args(argc - 1, argv + 1, &retval)) {
        return nullptr;
    }
    NAPI_CALL(env, napi_get_value_uint32(env, argv[0], &callback_data->duration));
    NAPI_CALL(env, napi_create_string_latin1(env, __FUNCTION__, NAPI_AUTO_LENGTH, &resourceName));
    NAPI_CALL(env, napi_create_async_work(env,
        nullptr,
        resourceName,
        [](napi_env env, void *data) {
            struct VibratorData *callback_data = (struct VibratorData *)data;
            const VibratorInterface *vibratorDev = NewVibratorInterfaceInstance();
            if (!vibratorDev) {
                callback_data->code = 1;
                return;
            }
            if (vibratorDev->StartOnce(callback_data->duration) != 0) {
                callback_data->code = 2;
                return;
            }
            callback_data->code = 0;
        },
        [](napi_env env, napi_status status, void *data) {
            struct VibratorData *callback_data = (struct VibratorData *)data;
            std::unique_ptr<VibratorData> cleaner { callback_data };

            if (callback_data->code == 0) {
                callback_data->notify(nullptr, true);
            } else {
                callback_data->notify(nullptr, false);
            }
        },
        (void*)callback_data,
        &callback_data->async_work
    ));
    NAPI_CALL(env, napi_queue_async_work(env, callback_data->async_work));
    cleaner.release();
    return retval;
}
