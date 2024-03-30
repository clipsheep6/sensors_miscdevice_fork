/*
 * Copyright (c) 2021-2022 Huawei Device Co., Ltd.
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
 */

#include <cstdio>
#include <map>
#include <string>
#include <unistd.h>

#include "hilog/log.h"
#include "napi/native_api.h"
#include "napi/native_node_api.h"

#include "file_utils.h"
#include "miscdevice_log.h"
#include "vibrator_agent.h"
#include "vibrator_napi_error.h"
#include "vibrator_napi_utils.h"

#undef LOG_TAG
#define LOG_TAG "VibratorJs"

namespace OHOS {
namespace Sensors {
namespace {
constexpr int32_t VIBRATE_SHORT_DURATION = 35;
constexpr int32_t VIBRATE_LONG_DURATION = 1000;
}  // namespace

static std::map<std::string, int32_t> g_usageType = {
    {"unknown", USAGE_UNKNOWN},
    {"alarm", USAGE_ALARM},
    {"ring", USAGE_RING},
    {"notification", USAGE_NOTIFICATION},
    {"communication", USAGE_COMMUNICATION},
    {"touch", USAGE_TOUCH},
    {"media", USAGE_MEDIA},
    {"physicalFeedback", USAGE_PHYSICAL_FEEDBACK},
    {"simulateReality", USAGE_SIMULATE_REALITY},
};

static napi_value EmitAsyncWork(napi_value param, sptr<AsyncCallbackInfo> info)
{
    CHKPP(info);
    napi_status status = napi_generic_failure;
    if (param != nullptr) {
        status = napi_create_reference(info->env, param, 1, &info->callback[0]);
        if (status != napi_ok) {
            MISC_HILOGE("napi_create_reference fail");
            return nullptr;
        }
        EmitAsyncCallbackWork(info);
        return nullptr;
    }
    napi_deferred deferred = nullptr;
    napi_value promise = nullptr;
    status = napi_create_promise(info->env, &deferred, &promise);
    if (status != napi_ok) {
        MISC_HILOGE("napi_create_promise fail");
        return nullptr;
    }
    info->deferred = deferred;
    EmitPromiseWork(info);
    return promise;
}

static napi_value VibrateTime(napi_env env, napi_value args[], size_t argc)
{
    NAPI_ASSERT(env, (argc >= 1), "Wrong argument number");
    int32_t duration = 0;
    NAPI_ASSERT(env, GetInt32Value(env, args[0], duration), "Get int number fail");
    sptr<AsyncCallbackInfo> asyncCallbackInfo = new (std::nothrow) AsyncCallbackInfo(env);
    CHKPP(asyncCallbackInfo);
    asyncCallbackInfo->vibrateType = VibrateType::START_VIBRATE_TIME;
    asyncCallbackInfo->duration = duration;
    if (argc >= 2 && IsMatchType(env, args[1], napi_function)) {
        return EmitAsyncWork(args[1], asyncCallbackInfo);
    }
    return EmitAsyncWork(nullptr, asyncCallbackInfo);
}

static napi_value VibrateEffectId(napi_env env, napi_value args[], size_t argc)
{
    NAPI_ASSERT(env, (argc >= 1), "Wrong argument number");
    string effectId;
    NAPI_ASSERT(env, GetStringValue(env, args[0], effectId), "Wrong argument type. String or function expected");
    sptr<AsyncCallbackInfo> asyncCallbackInfo = new (std::nothrow) AsyncCallbackInfo(env);
    CHKPP(asyncCallbackInfo);
    asyncCallbackInfo->effectId = effectId;
    asyncCallbackInfo->vibrateType = VibrateType::VIBRATE_EFFECT_ID;
    if (argc >= 2 && IsMatchType(env, args[1], napi_function)) {
        return EmitAsyncWork(args[1], asyncCallbackInfo);
    }
    return EmitAsyncWork(nullptr, asyncCallbackInfo);
}

static bool GetCallbackInfo(const napi_env &env, napi_value args[],
    sptr<AsyncCallbackInfo> &asyncCallbackInfo, string &mode)
{
    CHKPF(asyncCallbackInfo);
    CHKPF(args);
    napi_value value = nullptr;

    CHKCF(napi_get_named_property(env, args[0], "success", &value) == napi_ok, "napi get sucess property fail");
    CHKCF(napi_create_reference(env, value, 1, &asyncCallbackInfo->callback[0]) == napi_ok,
        "napi_create_reference fail");

    if (napi_get_named_property(env, args[0], "fail", &value) == napi_ok) {
        if (IsMatchType(env, value, napi_function)) {
            CHKCF(napi_create_reference(env, value, 1, &asyncCallbackInfo->callback[1]) == napi_ok,
                "napi_create_reference fail");
        }
    }
    if (napi_get_named_property(env, args[0], "complete", &value) == napi_ok) {
        if (IsMatchType(env, value, napi_function)) {
            CHKCF(napi_create_reference(env, value, 1, &asyncCallbackInfo->callback[2]) == napi_ok,
                "napi_create_reference fail");
        }
    }
    if (napi_get_named_property(env, args[0], "mode", &value) == napi_ok) {
        bool result = GetStringValue(env, value, mode);
        if (!result || (mode != "long" && mode != "short")) {
            mode = "long";
        }
    }
    return true;
}

static napi_value VibrateMode(napi_env env, napi_value args[], size_t argc)
{
    if (argc == 0) {
        if (StartVibratorOnce(VIBRATE_LONG_DURATION) != 0) {
            MISC_HILOGE("Vibrate long mode fail");
        }
        return nullptr;
    }
    sptr<AsyncCallbackInfo> asyncCallbackInfo = new (std::nothrow) AsyncCallbackInfo(env);
    CHKPP(asyncCallbackInfo);
    asyncCallbackInfo->callbackType = SYSTEM_VIBRATE_CALLBACK;
    asyncCallbackInfo->vibrateType = VibrateType::SYSTEM_VIBRATE_CALLBACK;
    string mode = "long";
    if (!GetCallbackInfo(env, args, asyncCallbackInfo, mode)) {
        MISC_HILOGE("Get callback info fail");
        return nullptr;
    }
    int32_t duration = ((mode == "long") ? VIBRATE_LONG_DURATION : VIBRATE_SHORT_DURATION);
    asyncCallbackInfo->duration = duration;
    EmitAsyncCallbackWork(asyncCallbackInfo);
    return nullptr;
}

bool ParseParameter(napi_env env, napi_value args[], size_t argc, VibrateMsg &msg)
{
    CHKCF((argc >= 2), "Wrong argument number");
    CHKCF(GetPropertyString(env, args[0], "type", msg.type), "Get vibrate type fail");
    if (msg.type == "time") {
        CHKCF(GetPropertyInt32(env, args[0], "duration", msg.duration), "Get vibrate duration fail");
    } else if (msg.type == "preset") {
        CHKCF(GetPropertyInt32(env, args[0], "count", msg.count), "Get vibrate count fail");
        CHKCF(GetPropertyString(env, args[0], "effectId", msg.effectId), "Get vibrate effectId fail");
    } else if (msg.type == "file") {
        napi_value hapticFd = nullptr;
        CHKCF(GetPropertyItem(env, args[0], "hapticFd", hapticFd), "Get vibrate hapticFd fail");
        CHKCF(IsMatchType(env, hapticFd, napi_object), "Wrong argument type. Napi object expected");
        CHKCF(GetPropertyInt32(env, hapticFd, "fd", msg.fd), "Get vibrate fd fail");
        GetPropertyInt64(env, hapticFd, "offset", msg.offset);
        int64_t fdSize = GetFileSize(msg.fd);
        CHKCF((msg.offset >= 0) && (msg.offset <= fdSize), "The parameter of offset is invalid");
        msg.length = fdSize - msg.offset;
        GetPropertyInt64(env, hapticFd, "length", msg.length);
    }
    CHKCF(GetPropertyString(env, args[1], "usage", msg.usage), "Get vibrate usage fail");
    return true;
}

bool SetUsage(const std::string &usage)
{
    if (auto iter = g_usageType.find(usage); iter == g_usageType.end()) {
        MISC_HILOGE("Wrong usage type");
        return false;
    }
    return SetUsage(g_usageType[usage]);
}

int32_t CheckVibrateMsg(const VibrateMsg &msg)
{
    if (!SetUsage(msg.usage)) {
        MISC_HILOGE("SetUsage fail");
        return PARAMETER_ERROR;
    }
    if ((msg.type != "time") && (msg.type != "preset") && (msg.type != "file")) {
        MISC_HILOGE("Invalid vibrate type, type:%{public}s", msg.type.c_str());
        return PARAMETER_ERROR;
    }
    if (msg.type == "preset") {
        if (!SetLoopCount(msg.count)) {
            MISC_HILOGE("SetLoopCount fail");
            return PARAMETER_ERROR;
        }
        return SUCCESS;
    } else if (msg.type == "file") {
        #ifdef OHOS_BUILD_ENABLE_VIBRATOR_CUSTOM
            MISC_HILOGD("Time delay measurement:start time");
            if (msg.fd < 0 || msg.offset < 0 || msg.length <= 0) {
                MISC_HILOGE("Input parameter invalid, fd:%{public}d, offset:%{public}lld, length:%{public}lld",
                    fd, static_cast<long long>(offset), static_cast<long long>(length));
                return PARAMETER_ERROR;
            }
            return SUCCESS;
        #else
            MISC_HILOGE("The device does not support this operation");
            return IS_NOT_SUPPORTED;
        #endif // OHOS_BUILD_ENABLE_VIBRATOR_CUSTOM
    }
    if (msg.duration <= 0) {
        MISC_HILOGE("duration is invalid");
        return PARAMETER_ERROR;
    }
    return SUCCESS;
}

static napi_value VibrateEffect(napi_env env, napi_value args[], size_t argc)
{
    sptr<AsyncCallbackInfo> asyncCallbackInfo = new (std::nothrow) AsyncCallbackInfo(env);
    CHKPP(asyncCallbackInfo);
    asyncCallbackInfo->vibrateType = VibrateType::START_VIBRATE;
    if (!ParseParameter(env, args, argc, asyncCallbackInfo->msg)) {
        ThrowErr(env, PARAMETER_ERROR, "parameter fail");
        return nullptr;
    }
    if (CheckVibrateMsg(asyncCallbackInfo->msg) == PARAMETER_ERROR) {
        ThrowErr(env, PARAMETER_ERROR, "parameter fail");
        return nullptr;
    }
    if (argc >= 3 && IsMatchType(env, args[2], napi_function)) {
        return EmitAsyncWork(args[2], asyncCallbackInfo);
    }
    return EmitAsyncWork(nullptr, asyncCallbackInfo);
}

static napi_value StartVibrate(napi_env env, napi_callback_info info)
{
    CHKPP(env);
    CHKPP(info);
    size_t argc = 3;
    napi_value args[3] = {};
    napi_value thisArg = nullptr;
    napi_status status = napi_get_cb_info(env, info, &argc, args, &thisArg, nullptr);
    if (status != napi_ok || argc < 2) {
        ThrowErr(env, PARAMETER_ERROR, "napi_get_cb_info fail or number of parameter invalid");
        return nullptr;
    }
    if (!IsMatchType(env, args[0], napi_object) || !IsMatchType(env, args[1], napi_object)) {
        ThrowErr(env, PARAMETER_ERROR, "args[0] and args[1] should is napi_object");
        return nullptr;
    }
    return VibrateEffect(env, args, argc);
}

static napi_value Vibrate(napi_env env, napi_callback_info info)
{
    CHKPP(env);
    CHKPP(info);
    size_t argc = 2;
    napi_value args[2] = {};
    napi_value thisArg = nullptr;
    napi_status status = napi_get_cb_info(env, info, &argc, args, &thisArg, nullptr);
    if (status != napi_ok) {
        ThrowErr(env, PARAMETER_ERROR, "napi_get_cb_info fail");
        return nullptr;
    }
    if (argc >= 1 && IsMatchType(env, args[0], napi_number)) {
        return VibrateTime(env, args, argc);
    }
    if (argc >= 1 && IsMatchType(env, args[0], napi_string)) {
        return VibrateEffectId(env, args, argc);
    }
    return VibrateMode(env, args, argc);
}

static napi_value Cancel(napi_env env, napi_callback_info info)
{
    size_t argc = 1;
    napi_value args[1] = {};
    napi_value thisArg = nullptr;
    napi_status status = napi_get_cb_info(env, info, &argc, args, &thisArg, nullptr);
    if (status != napi_ok) {
        ThrowErr(env, PARAMETER_ERROR, "napi_get_cb_info fail");
        return nullptr;
    }
    sptr<AsyncCallbackInfo> asyncCallbackInfo = new (std::nothrow) AsyncCallbackInfo(env);
    CHKPP(asyncCallbackInfo);
    asyncCallbackInfo->vibrateType = VibrateType::CANCEL;
    if ((argc > 0) && (IsMatchType(env, args[0], napi_function))) {
        return EmitAsyncWork(args[0], asyncCallbackInfo);
    }
    return EmitAsyncWork(nullptr, asyncCallbackInfo);
}

static napi_value Stop(napi_env env, napi_callback_info info)
{
    size_t argc = 2;
    napi_value args[2] = {};
    napi_value thisArg = nullptr;
    napi_status status = napi_get_cb_info(env, info, &argc, args, &thisArg, nullptr);
    if (status != napi_ok) {
        ThrowErr(env, PARAMETER_ERROR, "napi_get_cb_info fail");
        return nullptr;
    }
    if (argc >= 1 && IsMatchType(env, args[0], napi_string)) {
        string mode;
        if (!GetStringValue(env, args[0], mode)) {
            ThrowErr(env, PARAMETER_ERROR, "Parameters invalid");
            return nullptr;
        }
        if (mode != "time" && mode != "preset") {
            MISC_HILOGE("Input parameter invalid, mode is %{public}s", mode.c_str());
            ThrowErr(env, PARAMETER_ERROR, "Parameters invalid");
            return nullptr;
        }
        sptr<AsyncCallbackInfo> asyncCallbackInfo = new (std::nothrow) AsyncCallbackInfo(env);
        CHKPP(asyncCallbackInfo);
        asyncCallbackInfo->mode = mode;
        asyncCallbackInfo->vibrateType = VibrateType::STOP_VIBRATOR;
        if (argc >= 2 && IsMatchType(env, args[1], napi_function)) {
            return EmitAsyncWork(args[1], asyncCallbackInfo);
        }
        return EmitAsyncWork(nullptr, asyncCallbackInfo);
    } else {
        return Cancel(env, info);
    }
}

static napi_value IsSupportEffect(napi_env env, napi_callback_info info)
{
    size_t argc = 2;
    napi_value args[2] = {};
    napi_value thisArg = nullptr;
    napi_status status = napi_get_cb_info(env, info, &argc, args, &thisArg, nullptr);
    if ((status != napi_ok) || (argc == 0)) {
        ThrowErr(env, PARAMETER_ERROR, "napi_get_cb_info fail or number of parameter invalid");
        return nullptr;
    }
    string effectId;
    if (!GetStringValue(env, args[0], effectId)) {
        ThrowErr(env, PARAMETER_ERROR, "GetStringValue fail");
        return nullptr;
    }
    sptr<AsyncCallbackInfo> asyncCallbackInfo = new (std::nothrow) AsyncCallbackInfo(env);
    CHKPP(asyncCallbackInfo);
    asyncCallbackInfo->callbackType = IS_SUPPORT_EFFECT_CALLBACK;
    asyncCallbackInfo->vibrateType = VibrateType::IS_SUPPORT_EFFECT_CALLBACK;
    asyncCallbackInfo->effectId = effectId;
    if ((argc > 1) && (IsMatchType(env, args[1], napi_function))) {
        return EmitAsyncWork(args[1], asyncCallbackInfo);
    }
    return EmitAsyncWork(nullptr, asyncCallbackInfo);
}

static napi_value EnumClassConstructor(const napi_env env, const napi_callback_info info)
{
    size_t argc = 0;
    napi_value args[1] = {0};
    napi_value res = nullptr;
    void *data = nullptr;
    NAPI_CALL(env, napi_get_cb_info(env, info, &argc, args, &res, &data));
    return res;
}

static napi_value CreateEnumStopMode(const napi_env env, napi_value exports)
{
    napi_value timeMode = nullptr;
    napi_value presetMode = nullptr;
    NAPI_CALL(env, napi_create_string_utf8(env, "time", NAPI_AUTO_LENGTH, &timeMode));
    NAPI_CALL(env, napi_create_string_utf8(env, "preset", NAPI_AUTO_LENGTH, &presetMode));

    napi_property_descriptor desc[] = {
        DECLARE_NAPI_STATIC_PROPERTY("VIBRATOR_STOP_MODE_TIME", timeMode),
        DECLARE_NAPI_STATIC_PROPERTY("VIBRATOR_STOP_MODE_PRESET", presetMode),
    };
    napi_value result = nullptr;
    NAPI_CALL(env, napi_define_class(env, "VibratorStopMode", NAPI_AUTO_LENGTH, EnumClassConstructor, nullptr,
        sizeof(desc) / sizeof(*desc), desc, &result));
    NAPI_CALL(env, napi_set_named_property(env, exports, "VibratorStopMode", result));
    return exports;
}

static napi_value CreateEnumEffectId(const napi_env env, const napi_value exports)
{
    napi_value clockTime = nullptr;
    napi_create_string_utf8(env, "haptic.clock.timer", NAPI_AUTO_LENGTH, &clockTime);
    napi_property_descriptor desc[] = {
        DECLARE_NAPI_STATIC_PROPERTY("EFFECT_CLOCK_TIMER", clockTime),
    };
    napi_value result = nullptr;
    NAPI_CALL(env, napi_define_class(env, "EffectId", NAPI_AUTO_LENGTH, EnumClassConstructor, nullptr,
        sizeof(desc) / sizeof(*desc), desc, &result));
    NAPI_CALL(env, napi_set_named_property(env, exports, "EffectId", result));
    return exports;
}

static napi_value Init(napi_env env, napi_value exports)
{
    napi_property_descriptor desc[] = {
        DECLARE_NAPI_FUNCTION("vibrate", Vibrate),
        DECLARE_NAPI_FUNCTION("stop", Stop),
        DECLARE_NAPI_FUNCTION("startVibration", StartVibrate),
        DECLARE_NAPI_FUNCTION("stopVibration", Stop),
        DECLARE_NAPI_FUNCTION("isSupportEffect", IsSupportEffect),
    };
    NAPI_CALL(env, napi_define_properties(env, exports, sizeof(desc) / sizeof(napi_property_descriptor), desc));
    NAPI_ASSERT_BASE(env, CreateEnumStopMode(env, exports) != nullptr, "Create enum stop mode fail", exports);
    NAPI_ASSERT_BASE(env, CreateEnumEffectId(env, exports) != nullptr, "Create enum effect id fail", exports);
    return exports;
}

static napi_module _module = {
    .nm_version = 1,
    .nm_flags = 0,
    .nm_filename = nullptr,
    .nm_register_func = Init,
    .nm_modname = "vibrator",
    .nm_priv = ((void *)0),
    .reserved = {0}
};

extern "C" __attribute__((constructor)) void RegisterModule(void)
{
    napi_module_register(&_module);
}
}  // namespace Sensors
}  // namespace OHOS
