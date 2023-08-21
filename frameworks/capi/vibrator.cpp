/*
 * Copyright (C) 2023 Huawei Device Co., Ltd.
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

#include "vibrator.h"
#include "vibrator_type.h"

#include "vibrator_agent.h"

#include "miscdevice_log.h"
#include "sensors_errors.h"

namespace OHOS {
namespace Sensors {
using OHOS::HiviewDFX::HiLog;
using namespace OHOS::HiviewDFX;

namespace {
constexpr HiLogLabel LABEL = { LOG_CORE, MISC_LOG_DOMAIN, "NATIVEVIBRATORIF" };
constexpr int32_t DEFAULT_VIBRATOR_ID = 123;
} // namespace

int32_t OH_Vibrator_PlayVibration(int32_t duration, struct VibrateAttribute attribute)
{
    if (duration <= 0) {
        MISC_HILOGE("duration is invalid, duration is %{public}d", duration);
        return PARAMETER_ERROR;
    }
    //CHKCR(attribute, PARAMETER_ERROR);
    if (attribute.id != DEFAULT_VIBRATOR_ID || attribute.usage < USAGE_UNKNOWN || attribute.usage > USAGE_MAX) {
        MISC_HILOGE("vibrate attribute value is invalid");
        return PARAMETER_ERROR;
    }
    if (!SetUsage(attribute.usage)) {
        MISC_HILOGE("SetUsage fail");
        return PARAMETER_ERROR;
    }
    int32_t ret = StartVibratorOnce(duration);
    if (ret != OHOS::ERR_OK) {
        MISC_HILOGE("start Vibrator failed, ret is %{public}d", ret);
    }
    return ret;
}

int32_t OH_Vibrator_PlayVibrationCustom(struct VibrateFileDescription fileDescription, struct VibrateAttribute attribute)
{
    if (attribute.id != DEFAULT_VIBRATOR_ID || attribute.usage < USAGE_UNKNOWN || attribute.usage > USAGE_MAX) {
        MISC_HILOGE("vibrate attribute value is invalid");
        return PARAMETER_ERROR;
    }
    //CHKCR(attribute, PARAMETER_ERROR);
    if (!SetUsage(attribute.usage)) {
        MISC_HILOGE("SetUsage fail");
        return PARAMETER_ERROR;
    }
    int32_t ret = PlayVibratorCustom(fileDescription.fd, fileDescription.offset, fileDescription.length);
    if (ret != OHOS::ERR_OK) {
        MISC_HILOGE("play vibrator custom failed, ret is %{public}d", ret);
    }
    return ret;
}

// int32_t OH_Vibrator_PlayVibrationEvent(struct VibrateEffect vibrateEffect, struct VibrateAttribute attribute)
// {
//     if (attribute.id != DEFAULT_VIBRATOR_ID || attribute.usage < USAGE_UNKNOWN || attribute.usage > USAGE_MAX) {
//         MISC_HILOGE("vibrate attribute value is invalid");
//         return PARAMETER_ERROR;
//     }
//     //CHKCR(attribute, PARAMETER_ERROR);
//     if (!SetUsage(attribute.usage)) {
//         MISC_HILOGE("SetUsage fail");
//         return PARAMETER_ERROR;
//     }
//     int32_t ret = PlayVibrationEvent(vibrateEffect);
//     if (ret != OHOS::ERR_OK) {
//         MISC_HILOGE("play vibration event, ret is %{public}d", ret);
//     }
//     return ret;
// }
}
}