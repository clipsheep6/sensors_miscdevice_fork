/*
 *    Copyright 2021 Institute of Software Chinese Academy of Sciences, ISRC

 *    Licensed under the Apache License, Version 2.0 (the "License");
 *    you may not use this file except in compliance with the License.
 *    You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *    Unless required by applicable law or agreed to in writing, software
 *    distributed under the License is distributed on an "AS IS" BASIS,
 *    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *    See the License for the specific language governing permissions and
 *    limitations under the License.
 */

#include "vibrator.h"
#include "util.h"
#include "asyncworkdata.h"
#include "vibrator_if.h"
#include "vibrator_type.h"
#define DURATION_SHORT 1
#define DURATION_LONG 2

struct VibratorData : public AsyncWorkData
{
    VibratorData(napi_env env) : AsyncWorkData {env}
    {} 
    unsigned int duration;
    int code;
    std::string message;
};

napi_value NAPI_Vibrator_vibrate(napi_env env, napi_callback_info info)
{
    size_t argc = 1;
    napi_value argv[argc];
    napi_value undefined, retval, resourceName;
    std::string mode;

    NAPI_CALL(env, napi_get_undefined(env, &undefined));
    NAPI_CALL(env, napi_get_cb_info(env, info, &argc, argv, NULL, NULL));
    argc = std::min(argc, sizeof(argv)/sizeof(napi_value));
    NAPI_ASSERT(env, argc == 1, "Wrong number of arguments");
    struct VibratorData *callback_data = new VibratorData { env };
    std::unique_ptr<VibratorData> cleaner { callback_data };
    if (!callback_data->parse_callback_object(argv[0], &retval))
    {
        return nullptr;
    }
    NAPI_CALL(env, util_get_property_stdstring(env, argv[0], "mode", &mode));
    if (mode == "short") {
        callback_data->duration = DURATION_SHORT;
    } else if (mode == "long") {
        callback_data->duration = DURATION_LONG;
    } else {
        napi_throw_error(env, NULL, "mode must be long or short");
        return nullptr;
    }
    NAPI_CALL(env, napi_create_string_latin1(env, __FUNCTION__, NAPI_AUTO_LENGTH, &resourceName));
    NAPI_CALL(env, napi_create_async_work(env,
    	nullptr,
        resourceName,
        [](napi_env env, void *data) {
            struct VibratorData *callback_data = (struct VibratorData *)data;
            const VibratorInterface *vibratorDev = NewVibratorInterfaceInstance();
            if (!vibratorDev) {
                callback_data->code = 1;
                callback_data->message = "no vibrator device";
                return;
            }
            if (vibratorDev->StartOnce(callback_data->duration) != 0) {
                callback_data->code = 2;
                callback_data->message = "vibrate error";
                return;
            }
            callback_data->code = 0;
    	},
        [](napi_env env, napi_status status, void *data){
            struct VibratorData *callback_data = (struct VibratorData *)data;
            std::unique_ptr<VibratorData> cleaner { callback_data };

            if (callback_data->code == 0)
            {
                callback_data->notify(nullptr);
            }
            else
            {
                constexpr size_t argc = 2;
                napi_value argv[argc];
                static const char *names[argc] = { "data", "code" };
                const std::string &data = callback_data->message;
                (void)(
                    (napi_create_string_utf8(env, data.c_str(), data.size(), &argv[0]) == napi_ok) &&
                    (napi_create_int32(env, callback_data->code, &argv[1]) == napi_ok) &&
                    (callback_data->notify_multiple_argument(argc, argv, names, false), true)
                );
            }
    	},
		(void*)callback_data,
        &callback_data->async_work
    ));
    NAPI_CALL(env, napi_queue_async_work(env, callback_data->async_work));
    cleaner.release();
    return retval;
}
