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

#include "vibrator_thread.h"

#include "custom_vibration_matcher.h"
#include "sensors_errors.h"

namespace OHOS {
namespace Sensors {
namespace {
constexpr OHOS::HiviewDFX::HiLogLabel LABEL = { LOG_CORE, MISC_LOG_DOMAIN, "VibratorThread" };
constexpr size_t COMPOSITE_EFFECT_PART = 128;
}  // namespace

bool VibratorThread::Run()
{
    VibrateInfo info = GetCurrentVibrateInfo();
    if (info.mode == "time") {
        int32_t ret = PlayOnce(info);
        if (ret != SUCCESS) {
            MISC_HILOGE("Play once vibration fail, package:%{public}s", info.packageName.c_str());
            return false;
        }
    } else if (info.mode == "preset") {
        int32_t ret = PlayEffect(info);
        if (ret != SUCCESS) {
            MISC_HILOGE("Play effect vibration fail, package:%{public}s", info.packageName.c_str());
            return false;
        }
    } else if (info.mode == "custom.hd") {
        int32_t ret = PlayCustomByHdHptic(info);
        if (ret != SUCCESS) {
            MISC_HILOGE("Play custom vibration by hd haptic fail, package:%{public}s", info.packageName.c_str());
            return false;
        }
    } else if (info.mode == "custom.predefined" || info.mode == "custom.time") {
        int32_t ret = PlayCustomByCompositeEffect(info);
        if (ret != SUCCESS) {
            MISC_HILOGE("Play custom vibration by composite effect fail, package:%{public}s", info.packageName.c_str());
            return false;
        }
    }
    return false;
}

int32_t VibratorThread::PlayOnce(const VibrateInfo &info)
{
    std::unique_lock<std::mutex> vibrateLck(vibrateMutex_);
    int32_t ret = VibratorDevice.StartOnce(static_cast<uint32_t>(info.duration));
    if (ret != SUCCESS) {
        MISC_HILOGE("StartOnce fail, duration:%{public}d", info.duration);
        return ERROR;
    }
    cv_.wait_for(vibrateLck, std::chrono::milliseconds(info.duration), [this] { return exitFlag_; });
    VibratorDevice.Stop(HDF_VIBRATOR_MODE_ONCE);
    if (exitFlag_) {
        MISC_HILOGI("Stop duration:%{public}d, package:%{public}s", info.duration, info.packageName.c_str());
        return SUCCESS;
    }
    return SUCCESS;
}

int32_t VibratorThread::PlayEffect(const VibrateInfo &info)
{
    std::unique_lock<std::mutex> vibrateLck(vibrateMutex_);
    for (int32_t i = 0; i < info.count; ++i) {
        std::string effect = info.effect;
        int32_t ret = VibratorDevice.Start(effect);
        if (ret != SUCCESS) {
            MISC_HILOGE("Vibrate effect %{public}s failed, ", effect.c_str());
            return ERROR;
        }
        cv_.wait_for(vibrateLck, std::chrono::milliseconds(info.duration), [this] { return exitFlag_; });
        VibratorDevice.Stop(HDF_VIBRATOR_MODE_PRESET);
        if (exitFlag_) {
            MISC_HILOGI("Stop effect:%{public}s, package:%{public}s", effect.c_str(), info.packageName.c_str());
            return SUCCESS;
        }
    }
    return SUCCESS;
}

int32_t VibratorThread::PlayCustomByHdHptic(const VibrateInfo &info)
{
    std::unique_lock<std::mutex> vibrateLck(vibrateMutex_);
    const std::vector<VibratePattern> &patterns = info.package.patterns;
    size_t patternSize = patterns.size();
    for (size_t i = 0; i < patternSize; ++i) {
        // int32_t ret = VibratorDevice.playPattern(patterns[i]);
        // if (ret != SUCCESS) {
        //     MISC_HILOGE("Vibrate custom vibration by hd haptic failed");
        //     return ERROR;
        // }
        int32_t delayTime;
        if (i < patternSize - 1) {
            delayTime = patterns[i + 1].startTime - patterns[i].startTime;
        } else {
            const VibrateEvent &lastEvent = patterns[i].events.back();
            delayTime = lastEvent.time + lastEvent.duration;
        }
        cv_.wait_for(vibrateLck, std::chrono::milliseconds(info.duration), [this] { return exitFlag_; });
        if (exitFlag_) {
            MISC_HILOGI("Stop hd haptic, package:%{public}s", info.packageName.c_str());
            return SUCCESS;
        }
    }
    return SUCCESS;
}

int32_t VibratorThread::PlayCustomByCompositeEffect(const VibrateInfo &info)
{
    const std::vector<VibratePattern> &patterns = info.package.patterns;
    size_t patternSize = patterns.size();
    for (size_t i = 0; i < patternSize; ++i) {
        CustomVibrationMatcher matcher;
        HdfCompositeEffect hdfCompositeEffect;
        if (info.mode == "custom.predefined") {
            hdfCompositeEffect.type = HDF_EFFECT_TYPE_PRIMITIVE;
            int32_t ret = matcher.TransformEffect(patterns[i], hdfCompositeEffect.compositeEffects);
            if (ret != SUCCESS) {
                MISC_HILOGE("Transform pattern to predefined wave fail");
                return ERROR;
            }
        } else if (info.mode == "custom.time") {
            hdfCompositeEffect.type = HDF_EFFECT_TYPE_TIME;
            int32_t ret = matcher.TransformTime(patterns[i], hdfCompositeEffect.compositeEffects);
            if (ret != SUCCESS) {
                MISC_HILOGE("Transform pattern to time series fail");
                return ERROR;
            }
        }
        int32_t ret = PlayCompositeEffect(hdfCompositeEffect);
        if (ret != SUCCESS) {
            MISC_HILOGE("Vibrate pattern by composite effect failed");
            return ERROR;
        }
        int32_t delayTime;
        const VibrateEvent &lastEvent = patterns[i].events.back();
        if (i < patternSize - 1) {
            delayTime = patterns[i + 1].startTime - (patterns[i].startTime + lastEvent.time + lastEvent.duration);
        } else {
            delayTime = lastEvent.time + lastEvent.duration;
        }
        std::unique_lock<std::mutex> vibrateLck(vibrateMutex_);
        cv_.wait_for(vibrateLck, std::chrono::milliseconds(info.duration), [this] { return exitFlag_; });
        if (exitFlag_) {
            MISC_HILOGI("Stop composite effect, package:%{public}s", info.packageName.c_str());
            return SUCCESS;
        }
    }
    return SUCCESS;
}

int32_t VibratorThread::PlayCompositeEffect(const HdfCompositeEffect &hdfCompositeEffect)
{
    std::unique_lock<std::mutex> vibrateLck(vibrateMutex_);
    HdfCompositeEffect effectsPart;
    effectsPart.type = hdfCompositeEffect.type;
    size_t effectSize = hdfCompositeEffect.compositeEffects.size();
    int32_t delayTime = 0;
    for (size_t i = 0; i <= effectSize; ++i) {
        if ((effectsPart.compositeEffects.size() >= COMPOSITE_EFFECT_PART) || (i == effectSize)) {
            int32_t ret = VibratorDevice.EnableCompositeEffect(effectsPart);
            if (ret != SUCCESS) {
                MISC_HILOGE("EnableCompositeEffect failed");
                return ERROR;
            }
            cv_.wait_for(vibrateLck, std::chrono::milliseconds(delayTime), [this] { return exitFlag_; });
            delayTime = 0;
            effectsPart.compositeEffects.clear();
            if (exitFlag_) {
                MISC_HILOGI("Stop composite effect part");
                return SUCCESS;
            }
        } else {
            effectsPart.compositeEffects.push_back(hdfCompositeEffect.compositeEffects[i]);
            if (effectsPart.type == HDF_EFFECT_TYPE_TIME) {
                delayTime += hdfCompositeEffect.compositeEffects[i].timeEffect.delay;
            } else if (effectsPart.type == HDF_EFFECT_TYPE_PRIMITIVE) {
                delayTime += hdfCompositeEffect.compositeEffects[i].primitiveEffect.delay;
            } else {
                MISC_HILOGE("Effect type is valid");
                return ERROR;
            }
        }
    }
    return SUCCESS;
}

void VibratorThread::UpdateVibratorEffect(VibrateInfo info)
{
    std::unique_lock<std::mutex> lck(currentVibrationMutex_);
    currentVibration_ = info;
}

VibrateInfo VibratorThread::GetCurrentVibrateInfo()
{
    std::unique_lock<std::mutex> lck(currentVibrationMutex_);
    return currentVibration_;
}

void VibratorThread::SetExitStatus(bool status)
{
    exitFlag_ = status;
}
}  // namespace Sensors
}  // namespace OHOS
