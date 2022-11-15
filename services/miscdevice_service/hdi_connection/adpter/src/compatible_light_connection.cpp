/*
 * Copyright (c) 2022 Huawei Device Co., Ltd.
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
#include "compatible_light_connection.h"

#include <ctime>
#include <string>
#include <vector>

#include "sensors_errors.h"

namespace OHOS {
namespace Sensors {
using namespace OHOS::HiviewDFX;
namespace {
constexpr HiLogLabel LABEL = { LOG_CORE, MISC_LOG_DOMAIN, "CompatibleLightConnection" };
}
std::vector<LightInfo> lightInfo = {
    {"light_test", 1, 3}
};
std::vector<int32_t> supportLights = {0};
std::atomic_bool CompatibleLightConnection::isStop_ = false;

int32_t CompatibleLightConnection::ConnectHdi()
{
    CALL_LOG_ENTER;
    return ERR_OK;
}

int32_t CompatibleLightConnection::GetLightList(std::vector<LightInfo>& lightList) const
{
    CALL_LOG_ENTER;
    lightList.assign(lightInfo.begin(), lightInfo.end());
    return ERR_OK;
}

int32_t CompatibleLightConnection::TurnOn(int32_t lightId, const HDI::Light::V1_0::HdfLightEffect& effect)
{
    CALL_LOG_ENTER;
    if (std::find(supportLights.begin(), supportLights.end(), lightId) == supportLights.end()) {
        MISC_HILOGE("not support TurnOn lightId:%{public}d", lightId);
        return LIGHT_ID_NOT_SUPPORT;
    }
    if (std::find(turnOnLights_.begin(), turnOnLights_.end(), lightId) != turnOnLights_.end()) {
        MISC_HILOGI("lightId:%{public}d has been turnOn", lightId);
        return ERR_OK;
    }
    turnOnLights_.push_back(lightId);
    return ERR_OK;
}

int32_t CompatibleLightConnection::TurnOff(int32_t lightId)
{
    CALL_LOG_ENTER;
    if (std::find(supportLights.begin(), supportLights.end(), lightId) == supportLights.end()) {
        MISC_HILOGE("not support TurnOff lightId:%{public}d", lightId);
        return LIGHT_ID_NOT_SUPPORT;
    }
    if (std::find(turnOnLights_.begin(), turnOnLights_.end(), lightId) != turnOnLights_.end()) {
        MISC_HILOGI("lightId:%{public}d has been TurnOff", lightId);
        return ERR_OK;
    }
    std::vector<int32_t>::iterator iter;
    for (iter = turnOnLights_.begin(); iter != turnOnLights_.end();) {
        if (*iter == lightId) {
            turnOnLights_.erase(iter++);
            break;
        }
        ++iter;
    }
    if (turnOnLights_.empty()) {
        isStop_ = true;
    }
    return ERR_OK;
}

int32_t CompatibleLightConnection::DestroyHdiConnection()
{
    CALL_LOG_ENTER;
    return ERR_OK;
}
}  // namespace Sensors
}  // namespace OHOS
