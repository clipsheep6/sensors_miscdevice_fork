/*
 * Copyright (c) 2021 Huawei Device Co., Ltd.
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

#include <gtest/gtest.h>
#include <thread>

#include "accesstoken_kit.h"
#include "nativetoken_kit.h"
#include "token_setproc.h"

#include "light_agent.h"
#include "sensors_errors.h"

namespace OHOS {
namespace Sensors {
using namespace testing::ext;
using namespace OHOS::HiviewDFX;
using namespace Security::AccessToken;
using Security::AccessToken::AccessTokenID;

namespace {
    constexpr HiLogLabel LABEL = { LOG_CORE, MISC_LOG_DOMAIN, "LightAgentTest" };
}  // namespace

class LightAgentTest : public testing::Test {
public:
    static void SetUpTestCase() {}
    static void TearDownTestCase() {}
    void SetUp() {}
    void TearDown() {}

};

// void LightAgentTest::SetUp() {}

// void LightAgentTest::TearDown() {}

HWTEST_F(LightAgentTest, StartLightTest_001, TestSize.Level1)
{
    CALL_LOG_ENTER;
    LightInfo *lightInfo[100];
    int32_t count;
    int32_t ret = GetLightList(lightInfo, count);
    ASSERT_EQ(ret, 0);
}

HWTEST_F(LightAgentTest, StartLightTest_002, TestSize.Level1)
{
    CALL_LOG_ENTER;
    int32_t count;
    int32_t ret = GetLightList(nullptr, count);
    ASSERT_EQ(ret, -1);
}

HWTEST_F(LightAgentTest, StartLightTest_003, TestSize.Level1)
{
    CALL_LOG_ENTER;
    int32_t lightId = 1;
    LightColor color;
    color.lightColor.rgbColor.brightness = 99;
    color.lightColor.rgbColor.r = 10;
    color.lightColor.rgbColor.g = 20;
    color.lightColor.rgbColor.b = 30;

    LightAnimation animation;
    animation.mode = LIGHT_MODE_DEFAULT;
    animation.onTime = 2;
    animation.offTime = 2;
    int32_t ret = TurnOn(lightId, color, animation);
    ASSERT_EQ(ret, 0);
}

HWTEST_F(LightAgentTest, StartLightTest_004, TestSize.Level1)
{
    CALL_LOG_ENTER;
    int32_t lightId = -1;
    LightColor color;
    color.lightColor.rgbColor.brightness = 99;
    color.lightColor.rgbColor.r = 10;
    color.lightColor.rgbColor.g = 20;
    color.lightColor.rgbColor.b = 30;

    LightAnimation animation;
    animation.mode = LIGHT_MODE_DEFAULT;
    animation.onTime = 2;
    animation.offTime = 2;
    int32_t ret = TurnOn(lightId, color, animation);
    ASSERT_EQ(ret, -1);
}

HWTEST_F(LightAgentTest, StartLightTest_005, TestSize.Level1)
{
    CALL_LOG_ENTER;
    int32_t lightId = 1;
    int32_t ret = TurnOff(lightId);
    ASSERT_EQ(ret, 0);
}

HWTEST_F(LightAgentTest, StartLightTest_006, TestSize.Level1)
{
    CALL_LOG_ENTER;
    int32_t lightId = -1;
    int32_t ret = TurnOff(lightId);
    ASSERT_EQ(ret, -1);
}
}  // namespace Sensors
}  // namespace OHOS
