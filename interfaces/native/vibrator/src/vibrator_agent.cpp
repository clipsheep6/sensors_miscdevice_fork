/*
 * Copyright (c) 2021 Huawei Device Co., Ltd.
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
#include "vibrator_agent.h"
#include "sensors_errors.h"
#include "vibrator_service_client.h"

namespace OHOS {
namespace Sensors {
using OHOS::HiviewDFX::HiLog;
using OHOS::HiviewDFX::HiLogLabel;
using OHOS::Sensors::VibratorServiceClient;

static const HiLogLabel LABEL = { LOG_CORE, MISC_LOG_DOMAIN, "VibratorNDK" };
static const int32_t DEFAULT_VIBRATOR_ID = 123;
static int32_t g_loopCount = 1;
static int32_t g_usage = USAGE_UNKNOWN;

bool SetLoopCount(int32_t count)
{
    if (count <= 0) {
        MISC_HILOGE("input invalid, count is %{public}d", count);
        return false;
    }
    g_loopCount = count;
    return true;
}

int32_t StartVibrator(const char *effectId)
{
    CHKPR(effectId, ERROR);
    auto &client = VibratorServiceClient::GetInstance();
    int32_t ret = client.Vibrate(DEFAULT_VIBRATOR_ID, effectId, g_loopCount, g_usage);
    if (ret != ERR_OK) {
        MISC_HILOGE("vibrator effectId failed, ret: %{public}d", ret);
        return ERROR;
    }
    g_loopCount = 1;
    g_usage = USAGE_UNKNOWN;
    return SUCCESS;
}

int32_t StartVibratorOnce(int32_t duration)
{
    if (duration <= 0) {
        MISC_HILOGE("duration is invalid");
        return ERROR;
    }
    auto &client = VibratorServiceClient::GetInstance();
    int32_t ret = client.Vibrate(DEFAULT_VIBRATOR_ID, duration, g_usage);
    if (ret != ERR_OK) {
        MISC_HILOGE("vibrator duration failed, ret: %{public}d", ret);
        return ERROR;
    }
    g_usage = USAGE_UNKNOWN;
    return SUCCESS;
}

int32_t StopVibrator(const char *mode)
{
    CHKPR(mode, ERROR);
    if (strcmp(mode, "time") != 0 && strcmp(mode, "preset") != 0) {
        MISC_HILOGE("mode is invalid, mode is %{public}s", mode);
        return ERROR;
    }
    auto &client = VibratorServiceClient::GetInstance();
    int32_t ret = client.Stop(DEFAULT_VIBRATOR_ID, mode);
    if (ret != ERR_OK) {
        MISC_HILOGE("client is failed, ret: %{public}d", ret);
        return ERROR;
    }
    return SUCCESS;
}

bool SetUsage(int32_t usage)
{
    if ((usage < 0) || (usage >= USAGE_MAX)) {
        MISC_HILOGE("input invalid, usage is %{public}d", usage);
        return false;
    }
    g_usage = usage;
    return true;
}
}  // namespace Sensors
}  // namespace OHOS