/*
 * Copyright (c) 2023 Huawei Device Co., Ltd.
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

#include "custom_vibration_default_matcher.h"

#include <cmath>
#include <map>

#include "sensors_errors.h"

namespace OHOS {
namespace Sensors {
namespace {
std::map<int32_t, std::vector<int32_t>> TRANSIENT_VIBRATION_INFOS = {
    {0x01, {0x4b, 0x35, 0x14}}, {0x02, {0x23, 0x32, 0x14}}, {0x03, {0x0d, 0x2e, 0x14}},
    {0x04, {0x2f, 0x36, 0x14}}, {0x05, {0x17, 0x2f, 0x14}}, {0x06, {0x09, 0x26, 0x14}},
    {0x11, {0x64, 0x32, 0x14}}, {0x12, {0x54, 0x3a, 0x14}}, {0x13, {0x2d, 0x33, 0x14}},
    {0x14, {0x11, 0x30, 0x14}}, {0x15, {0x2f, 0x30, 0x14}}, {0x16, {0x23, 0x25, 0x14}},
    {0x17, {0x14, 0x33, 0x14}}, {0x18, {0x1b, 0x2d, 0x14}}, {0x19, {0x11, 0x2f, 0x14}},
};  // {Id, {intensity, frequency, duration}}
constexpr int32_t F_RESONANCE = 245;
constexpr int32_t BAND_WIDTH = 100;
constexpr float INTENSITY_WEIGHT = 0.5;
constexpr float FREQUENCY_WEIGHT = 0.5;
constexpr float WEIGHT_SUM_INIT = 100;
constexpr OHOS::HiviewDFX::HiLogLabel LABEL = { LOG_CORE, MISC_LOG_DOMAIN, "CustomVibrationMatcher" };
}  // namespace

int32_t CustomVibrationDefaultMatcher::TransformEffect(const std::set<VibrateEvent> &vibrateSet,
    HdfCompositeEffect &hdfCompositeEffect)
{
    CALL_LOG_ENTER;
    hdfCompositeEffect.type = HDF_EFFECT_TYPE_PRIMITIVE;
    int32_t preStartTime = 0;
    int32_t preDuration = 0;
    for (const auto &event : vibrateSet) {
        if ((preDuration != 0) && (event.startTime < preStartTime + preDuration)) {
            MISC_HILOGE("Vibration events overlap");
            return ERROR;
        }
        if (event.tag == EVENT_TAG_CONTINUOUS) {
            ProcessContinuousEvent(event, preStartTime, preDuration, hdfCompositeEffect.compositeEffects);
        } else if (event.tag == EVENT_TAG_TRANSIENT) {
            ProcessTransientEvent(event, preStartTime, preDuration, hdfCompositeEffect.compositeEffects);
        } else {
            MISC_HILOGE("Unknown event tag, tag:%{public}d", event.tag);
            return ERROR;
        }
    }
    return SUCCESS;
}


void CustomVibrationDefaultMatcher::ProcessContinuousEvent(const VibrateEvent &event, int32_t &preStartTime,
    int32_t &preDuration, std::vector<CompositeEffect> &compositeEffects)
{
    PrimitiveEffect primitiveEffect;
    primitiveEffect.delay = event.startTime - preStartTime;
    primitiveEffect.effectId = (event.duration << 16) + (event.frequency + F_RESONANCE - BAND_WIDTH / 2.);
    primitiveEffect.intensity = event.intensity;
    CompositeEffect compositeEffect;
    compositeEffect.primitiveEffect = primitiveEffect;
    compositeEffects.push_back(compositeEffect);
    preStartTime = event.startTime;
    preDuration = event.duration;
}

void CustomVibrationDefaultMatcher::ProcessTransientEvent(const VibrateEvent &event, int32_t &preStartTime,
    int32_t &preDuration, std::vector<CompositeEffect> &compositeEffects)
{
    int32_t matchId = 0;
    float minWeightSum = WEIGHT_SUM_INIT;
    for (const auto &transientInfo : TRANSIENT_VIBRATION_INFOS) {
        int32_t id = transientInfo.first;
        const std::vector<int32_t> &info = transientInfo.second;
        float intensityDistance = std::abs(event.intensity - info[0]);
        float frequencyDistance = std::abs(event.frequency - info[1]);
        float weightSum = INTENSITY_WEIGHT * intensityDistance + FREQUENCY_WEIGHT * frequencyDistance;
        if (weightSum <= minWeightSum) {
            minWeightSum = weightSum;
            matchId = id;
        }
    }
    PrimitiveEffect primitiveEffect;
    primitiveEffect.delay = event.startTime - preStartTime;
    primitiveEffect.effectId = matchId;
    CompositeEffect compositeEffect;
    compositeEffect.primitiveEffect = primitiveEffect;
    compositeEffects.push_back(compositeEffect);
    preStartTime = event.startTime;
    preDuration = event.duration;
}
}  // namespace Sensors
}  // namespace OHOS