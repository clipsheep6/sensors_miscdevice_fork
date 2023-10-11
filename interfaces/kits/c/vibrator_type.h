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
 * @brief Defines the Error code for user.
 *
 * @since 11
 */
enum VibratorErrorCode : int32_t {
    /**< Use this error code when permission is denied. */
    PERMISSION_DENIED = 201,
    /**< Use this error code when the input parameter type or range does not match. */
    PARAMETER_ERROR = 401,
    /**< Use this error code when capability not supported. */
    IS_NOT_SUPPORTED = 801,
    /**< Use this error code when operating the device fail. */
    DEVICE_OPERATION_FAILED = 14600101,
};

/**
 * @brief Defines the vibrator attribute.
 *
 * @since 11
 */
struct VibrateAttribute {
    /**< Vibrator ID. */
    int32_t vibratorId;
    /**< Vibration scenario. */
    VibratorUsage usage;
};

/**
 * @brief Defines the vibration file description.
 *
 * @since 11
 */
struct VibrateFileDescription {
    /**< File handle of the custom vibration sequence. */
    int32_t fd;
    /**< Offset address of the custom vibration sequence. */
    int64_t offset;
    /**< Total length of the custom vibration sequence. */
    int64_t length;
};
/** @} */
#ifdef __cplusplus
};
#endif

#endif  // endif VIBRATOR_TYPE_H
