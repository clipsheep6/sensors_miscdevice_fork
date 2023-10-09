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

#ifndef VIBRATOR_AGENT_TYPE_H
#define VIBRATOR_AGENT_TYPE_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Describes the vibration effect of the vibrator when a user adjusts the timer.
 *
 * @since 1
 */
const char *VIBRATOR_TYPE_CLOCK_TIMER = "haptic.clock.timer";

/**
 * @brief Describes the vibration effect of the vibrator when authentication fails.
 *
 * @since 11
 */
const char *VIBRATOR_TYPE_FAIL = "haptic.fail";

/**
 * @brief Describes the vibration effect of the vibrator when charging.
 *
 * @since 11
 */
const char *VIBRATOR_TYPE_CHARGING = "haptic.charging";

/**
 * @brief Describes the vibration effect of the vibrator when long pressed.
 *
 * @since 11
 */
const char *VIBRATOR_TYPE_LONG_PRESS_HEAVY = "haptic.long_press.heavy";

////////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * @brief Describes the vibration effect of the vibrator when long pressed the search box.
 *
 * @since 11
 */
const char *VIBRATOR_TYPE_LONG_PRESS_THE_SEARCH_BOX = "haptic.long_press.light";

/**
 * @brief Describes the vibration effect of the vibrator when long pressed the text box.
 *
 * @since 11
 */
const char *VIBRATOR_TYPE_LONG_PRESS_THE_TEXT_BOX = "haptic.long_press.light";

/**
 * @brief Describes the vibration effect of the vibrator when text selection cursor movement.
 *
 * @since 11
 */
const char *VIBRATOR_TYPE_TEXT_SELECTION_CURSOR_MOVEMENT = "haptic.slide.light";

/**
 * @brief Describes the vibration effect of the vibrator when time adjustment.
 *
 * @since 11
 */
const char *VIBRATOR_TYPE_TIME_ADJUSTMENT = "TimePicker";

/**
 * @brief Describes the vibration effect of the vibrator when description file letter index.
 *
 * @since 11
 */
const char *VIBRATOR_TYPE_DESCRIPTION_FILE_LETTER_INDEX = "haptic.slide.light";

/**
 * @brief Describes the vibration effect of the vibrator when pull-to-refresh.
 *
 * @since 11
 */
const char *VIBRATOR_TYPE_PULL_TO_REFRESH = "haptic.threshold";

/**
 * @brief Describes the vibration effect of the vibrator when scroll bar.
 *
 * @since 11
 */
const char *VIBRATOR_TYPE_SCROLL_BAR = "haptic.long_press.light";

/**
 * @brief Describes the vibration effect of the vibrator when left slide delete.
 *
 * @since 11
 */
const char *VIBRATOR_TYPE_LEFT_SLIDE_DELETE = "haptic.threshold";

/**
 * @brief Describes the vibration effect of the vibrator when drag.
 *
 * @since 11
 */
const char *VIBRATOR_TYPE_DRAG = "haptic.long_press.medium";
////////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * @brief Enumerates vibration usages.
 *
 * @since 9
 */
typedef enum VibratorUsage {
    USAGE_UNKNOWN = 0,            /**< Vibration is used for unknown, lowest priority */
    USAGE_ALARM = 1,              /**< Vibration is used for alarm */
    USAGE_RING = 2,               /**< Vibration is used for ring */
    USAGE_NOTIFICATION = 3,       /**< Vibration is used for notification */
    USAGE_COMMUNICATION = 4,      /**< Vibration is used for communication */
    USAGE_TOUCH = 5,              /**< Vibration is used for touch */
    USAGE_MEDIA = 6,              /**< Vibration is used for media */
    USAGE_PHYSICAL_FEEDBACK = 7,  /**< Vibration is used for physical feedback */
    USAGE_SIMULATE_REALITY = 8,   /**< Vibration is used for simulate reality */
    USAGE_MAX = 9
} VibratorUsage;

/** @} */
#ifdef __cplusplus
};
#endif

#endif  // endif VIBRATOR_AGENT_TYPE_H