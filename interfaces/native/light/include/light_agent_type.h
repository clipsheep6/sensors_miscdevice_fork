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

#ifndef LIGHT_AGENT_TYPE_H
#define LIGHT_AGENT_TYPE_H
#include <stdint.h>
#ifdef __cplusplus
extern "C" {
#endif

/** Maximum length of the light name */
#ifndef NAME_MAX_LEN
#define NAME_MAX_LEN 32
#endif /* NAME_MAX_LEN */
/**
 * @brief 灯的信息和能力.
 *
 * @since 10
 */
typedef struct {
    char lightName[NAME_MAX_LEN]; /**< 逻辑灯名称 */
    int32_t lightId; /**< 逻辑灯ID，值唯一 */
    int32_t lightNumber; /**< 逻辑控制器中的物理灯数量 */
} LightInfo;

/**
 * @brief 灯光的闪烁模式.
 *
 * @since 10
 */
typedef enum {
    LIGHT_MODE_DEFAULT = 0, /**< 不闪烁，只简单的开关 */
    LIGHT_MODE_BLINK, /**< 灯光将由亮度和/或颜色控制，并会以 onTime/offTime 闪烁 */
    LIGHT_MODE_HARDWARE, /**< 灯光将由亮度和/或颜色控制，以硬件支持的方式呼吸，如果硬件不支持会以 onTime/offTime 闪烁 */
} LightMode;

/**
 * @brief 灯光动态效果。
 *
 * @since 10
 */
typedef struct {
    LightMode mode; /**< 灯光的闪烁模式，默认LIGHT_MODE_DEFAULT */
    uint32_t onTime; /**< 灯亮的时间，单位ms */
    uint32_t offTime; /**< 灯灭的时间，单位ms */
} LightAnimation;

/**
 * @brief 灯光颜色效果。
 *
 * @since 10
 */
typedef struct {
    union light_agent_type
    {
        struct {
            uint8_t brightness;
            uint8_t r;
            uint8_t g;
            uint8_t b;
        } rgbColor;
        struct {
            uint8_t w;
            uint8_t r;
            uint8_t g;
            uint8_t b;
        } wrgbColor;
    } lightColor;
} LightColor;

/** @} */
#ifdef __cplusplus
};
#endif
#endif  // endif LIGHT_AGENT_TYPE_H
