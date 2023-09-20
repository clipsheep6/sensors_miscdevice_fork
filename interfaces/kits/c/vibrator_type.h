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

#ifndef VIBRATOR_TYPE_H
#define VIBRATOR_TYPE_H

#include <stdint.h>
#include "vibrator_agent_type.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief 振动事件类型。
 *
 * @since 11
 */
enum VibrateEventType {
    /**< 无效振动类型 */
    VIBRATE_TYPE_INVALID = 0,
    /**< 瞬时振动类型 */
    VIBRATE_TYPE_TRANSIENT = 1,
    /**< 连续振动类型 */
    VIBRATE_TYPE_CONTINUOUS = 2
};

/**
 * @brief 自定义振动效果。
 *
 * @since 11
 */
struct VibrateEvent {
    /**< 振动事件类型 */
    enum VibrateEventType type;
    /**< 振动开始时间 */
    int32_t startTime;
    /**< 振动持续时长 */
    int32_t duration;
    /**< 振动强度 */
    int32_t intensity;
    /**< 振动频率 */
    int32_t frequency;
};

/**
 * @brief 马达属性。
 *
 * @since 11
 */
struct VibrateAttribute {
    /**< 马达ID */
    int32_t id;
    /**< 振动场景 */
    enum VibratorUsage usage;
};

/**
 * @brief 振动效果。
 *
 * @since 11
 */
struct VibrateEffect {
    /**< 事件个数 */
    int32_t count;
    /**< 振动事件 */
    struct VibrateEvent* vibrateEvents;
};

/**
 * @brief 振动文件描述。
 *
 * @since 11
 */
struct VibrateFileDescription {
    /**< 自定义振动序列的文件句柄。 */
    int32_t fd;
    /**< 自定义振动序列的偏移地址。 */
    int64_t offset;
    /**< 自定义振动序列的总长度。 */
    int64_t length;
};
/** @} */
#ifdef __cplusplus
};
#endif

#endif  // endif VIBRATOR_TYPE_H