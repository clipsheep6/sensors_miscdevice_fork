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

/**
 * @file light_agent.h
 *
 * @brief Declares the functions for starting or stopping a light.
 * @since 10
 */
#ifndef LIGHT_AGENT_H
#define LIGHT_AGENT_H
#include "light_agent_type.h"
#ifdef __cplusplus
extern "C" {
#endif

namespace OHOS {
namespace Sensors {
/**
 * @brief 获取系统支持的灯列表.
 *
 * @param lightInfo：出参，存储灯光信息的数组
 * @param count：出参，数组大小
 * @return Returns <b>0</b> if the usage is set successfully; returns <b>-1</b> otherwise.
 *
 * @since 10
 */
int32_t GetLightList(LightInfo **lightInfo, int32_t &count);

/**
 * @brief 控制灯亮
 *
 * @param lightId: 逻辑灯id，值唯一
 * @param color：灯光的颜色信息
 * @param animation：灯光动效
 * @return Returns <b>0</b> if the usage is set successfully; returns <b>-1</b> otherwise.
 *
 * @since 10
 */
int32_t TurnOn(int32_t lightId, const LightColor color, const LightAnimation animation);

/**
 * @brief 控制多灯亮
 *
 * @param lightId: 逻辑灯id，值唯一
 * @param colors：一组物理灯颜色信息
 * @param size：物理灯个数
 * @return Returns <b>0</b> if the usage is set successfully; returns <b>-1</b> otherwise.
 *
 * @since 10
 */
int32_t TurnOnMulti(uint32_t lightId, const LightColor *colors, const uint32_t size);

/**
 * @brief 控制灯灭
 *
 * @param lightId：逻辑灯id，值唯一
 * @return Returns <b>0</b> if the usage is set successfully; returns <b>-1</b> otherwise.
 * @since 10
 */
int32_t TurnOff(int32_t lightId);
} // namespace Sensors
} // namespace OHOS
#ifdef __cplusplus
};
#endif
/** @} */
#endif // endif LIGHT_AGENT_H
