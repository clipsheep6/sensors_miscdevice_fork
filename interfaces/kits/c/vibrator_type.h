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
 * @brief 为用户定义错误码。
 *
 * @since 11
 */
typedef enum VibratorErrorCode : int32_t {
    /**< 权限校验失败。 */
    PERMISSION_DENIED = 201,
    /**< 参数检查失败，包括必选参数没有传入，参数类型错误等。 */
    PARAMETER_ERROR = 401,
    /**< 设备不支持该API，通常用于在设备已支持该针对其少量的API支持处理。 */
    IS_NOT_SUPPORTED = 801,
    /**< 设备操作失败。 */
    DEVICE_OPERATION_FAILED = 14600101,
} VibratorErrorCode;

/**
 * @brief Defines the vibrator attribute.
 *
 * @since 11
 */
typedef struct VibrateAttribute {
    /**< Vibrator ID. */
    int32_t vibratorId;
    /**< Vibration scenario. */
    VibratorUsage usage;
} VibrateAttribute;

/**
 * @brief Defines the vibration file description.
 *
 * @since 11
 */
typedef struct VibrateFileDescription {
    /**< File handle of the custom vibration sequence. */
    int32_t fd;
    /**< Offset address of the custom vibration sequence. */
    int64_t offset;
    /**< Total length of the custom vibration sequence. */
    int64_t length;
} VibrateFileDescription;
/** @} */
#ifdef __cplusplus
};
#endif

#endif  // endif VIBRATOR_TYPE_H
