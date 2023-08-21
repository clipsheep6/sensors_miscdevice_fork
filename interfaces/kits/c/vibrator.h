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

/**
 * @addtogroup 马达
 * @{
 *
 * @brief 提为马达服务提供统一的API以访问马达驱动程序。
 * @since 11
 */

/**
 * @file vibrator.h
 *
 * @brief 声明用于启动或停止马达的函数。
 * @since 11
 */

#ifndef VIBRATOR_H
#include <stdint.h>
#include "vibrator_type.h"
#include "vibrator_agent.h"
#define VIBRATOR_H

#ifdef __cplusplus
extern "C" {
#endif

namespace OHOS {
namespace Sensors {
/**
 * @brief 控制马达执行给定持续时间的单次振动。
 *
 * @param duration 表示单次振动持续的持续时间，以毫秒为单位。
 * @param attribute 表示马达的属性。关于细节，请参阅｛@link VibrateAttribute｝。
 * @return 如果操作成功，则返回0；否则返回负值。
 *
 * @since 11
 */
int32_t OH_Vibrator_PlayVibration(int32_t duration, struct VibrateAttribute attribute);
/**
 * @brief 根据传入的振动效果启动马达。
 *
 * @param effect 表示振动效果。关于细节，请参阅｛@link VibrateEffect｝。
 * @param attribute 表示马达的属性。关于细节，请参阅｛@link VibrateAttribute｝。
 * @return 如果操作成功，则返回0；否则返回负值。
 *
 * @since 11
 */
int32_t OH_Vibrator_PlayVibrationCustom(struct VibrateFileDescription fileDescription,
                                        struct VibrateAttribute attribute);
/**
 * @brief 播放自定义振动序列。
 *
 * @param fd 表示自定义振动序列的文件描述。关于细节，请参阅｛@link HapticFileDiscriptor｝。
 * @param attribute 表示马达的属性。关于细节，请参阅｛@link VibrateAttribute｝。
 * @return 如果操作成功，则返回0；否则返回负值。
 *
 * @since 11
 */
int32_t OH_Vibrator_PlayVibrationEvent(struct VibrateEffect vibrateEffect, struct VibrateAttribute attribute);

#ifdef __cplusplus
};
}
}
#endif
/** @} */
#endif // endif VIBRATOR_H