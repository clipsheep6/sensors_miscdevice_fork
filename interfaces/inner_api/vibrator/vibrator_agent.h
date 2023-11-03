/*
 * Copyright (c) 2021-2023 Huawei Device Co., Ltd.
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

/**
 * @addtogroup vibrator
 * @{
 *
 * @brief Provides functions for managing vibrators.
 * @since 6
 */

/**
 * @file vibrator_agent.h
 *
 * @brief Declares the functions for starting or stopping a vibrator.
 * @since 6
 */
#ifndef VIBRATOR_AGENT_H
#include <stdint.h>
#include "vibrator_agent_type.h"
#define VIBRATOR_AGENT_H

#ifdef __cplusplus
extern "C" {
#endif

namespace OHOS {
namespace Sensors {
/**
 * @brief Indicates the mode of stopping a one-shot vibration effect.
 *
 * @since 6
 */
const char *VIBRATOR_STOP_MODE_TIME = "time";

/**
 * @brief Indicates the mode of stopping a preset vibration effect.
 *
 * @since 6
 */
const char *VIBRATOR_STOP_MODE_PRESET = "preset";

/**
 * @brief Controls this vibrator to perform a vibration with a preset vibration effect.
 *
 * @param effectId Indicates the preset vibration effect, which is described in {@link vibrator_agent_type.h}, for
 * example:
 * {@link VIBRATOR_TYPE_CLOCK_TIMER}: Describes the vibration effect of the vibrator when a user adjusts the timer.
 * @return Returns <b>0</b> if the vibrator vibrates as expected; returns <b>-1</b> otherwise, for example, the preset
 * vibration effect is not supported.
 *
 * @since 6
 */
int32_t StartVibrator(const char *effectId);

/**
 * @brief Controls this vibrator to perform a one-shot vibration at a given duration.
 *
 * @param duration Indicates the duration that the one-shot vibration lasts, in milliseconds.
 * @return Returns <b>0</b> if the vibrator vibrates as expected; returns <b>-1</b> otherwise, for example, the given
 * duration for the one-shot vibration is invalid.
 *
 * @since 6
 */
int32_t StartVibratorOnce(int32_t duration);

/**
 * @brief Query whether the device supports custom vibration.
 *
 * @return Returning true indicates support; otherwise, it indicates no support.
 *
 * @since 10
 */
bool IsSupportVibratorCustom();

/**
 * @brief Play a custom vibration sequence.
 *
 * @param fd Indicates the file handle for custom vibration sequence.
 * @param offset Indicates the starting address (in bytes) of the custom vibration sequence.
 * @param length Indicates the total length (in bytes) of the custom vibration sequence.
 * @return Returning 0 indicates success; otherwise, it indicates failure.
 *
 * @since 10
 */
int32_t PlayVibratorCustom(int32_t fd, int64_t offset, int64_t length);

/**
 * @brief Sets the number of cycles for vibration.
 * @param count Indicates the number of cycles for vibration.
 * @since 9
 */
bool SetLoopCount(int32_t count);

/**
 * @brief Stop the motor vibration according to the input mode.
 *
 * @param mode Indicates the mode of the vibration to stop. The values can be <b>time</b> and <b>preset</b>,
 * respectively representing a one-shot vibration mode and a preset vibration mode.
 * @return Returns <b>0</b> if the vibration is stopped as expected; returns <b>-1</b> otherwise.
 * @since 6
 */
int32_t StopVibrator(const char *mode);

/**
 * @brief Cancel the current motor vibration.
 * @return Returning 0 indicates success; otherwise, it indicates failure.
 * @since 10
 */
int32_t Cancel();

/**
 * @brief Set the usage of vibration.
 *
 * @param usage Indicates the vibration usage, which is described in {@link vibrator_agent_type.h},for
 * example:
 * {@link USAGE_ALARM}: Describes the vibration is used for alarm.
 *
 * @since 9
 */
bool SetUsage(int32_t usage);

/**
 * @brief Query whether a vibration effect is supported.
 *
 * @param effectId Indicates the preset vibration effect, which is described in {@link vibrator_agent_type.h}, for
 * example:
 * {@link VIBRATOR_TYPE_CLOCK_TIMER}: Describes the vibration effect of the vibrator when a user adjusts the timer.
 * @param state Indicates a pointer to the query result.
 * @return Returning 0 indicates success; otherwise, it indicates failure.
 *
 * @since 10
 */
int32_t IsSupportEffect(const char *effectId, bool *state);

/**
 * @brief 获取振动时延，从下发振动到启动振动的时间间隔，可用于音振协调场景。
 * @param delayTime：出参，返回振动时间延迟，从下发振动到启动振动的时间间隔，单位：毫秒.
 * @return 返回0表示成功，否则表示失败。
 * @since 11
 */
int32_t GetDelayTime(int32_t &delayTime);

/**
 * @brief 解码振动媒体文件，生成振动波形。
 * @param fileDescription: 振动效果描述文件，支持he、json文件格式，如{@link VibrationFileDescription}。
 * @param package: 出参，生成的振动序列包，如 {@link VibrationPackage}。
 * @return 返回0表示成功，否则表示失败。
 * @since 11
 */
int32_t PreProcess(const VibratorFileDescription &fd, VibratorPackage &package);

/**
 * @brief 释放振动序列包内存。
 * @param package: 生成的振动序列包，如 {@link VibrationPackage}。
 * @return 返回0表示成功，否则表示失败。
 * @since 11
 */
int32_t FreeVibratorPackage(VibratorPackage &package);

/**
 * @brief 播放振动序列。
 * @param channelId: 播放振动的通道ID。
 * @param pattern: 振动序列，如{@link VibratorPattern}。
 * @return 返回0表示成功，否则表示失败。
 * @since 11
 */
int32_t PlayPattern(const VibratorPattern &pattern);
} // namespace Sensors
} // namespace OHOS
#ifdef __cplusplus
};
#endif
/** @} */
#endif // endif VIBRATOR_AGENT_H