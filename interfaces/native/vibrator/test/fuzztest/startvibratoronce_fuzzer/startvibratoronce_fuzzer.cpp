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

#include "startvibratoronce_fuzzer.h"

#include <thread>
#include <unistd.h>

#include "vibrator_agent.h"

namespace {
constexpr int32_t MAX_VIBRATOR_TIME = 1800000;
}  // namespace

namespace OHOS {
bool StartVibratorOnceFuzzTest(const uint8_t* data, size_t size)
{
    uintptr_t duration = reinterpret_cast<uintptr_t>(data);
    int32_t ret = OHOS::Sensors::StartVibratorOnce(duration);
    if ((duration > MAX_VIBRATOR_TIME && ret == 0) || (duration <= MAX_VIBRATOR_TIME && ret != 0)) {
        return false;
    }
    return true;
}
}

extern "C" int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size)
{
    OHOS::StartVibratorOnceFuzzTest(data, size);
    return 0;
}