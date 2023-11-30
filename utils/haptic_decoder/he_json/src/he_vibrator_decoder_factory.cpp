/*
 * Copyright (c) 2023 Shanghai Ruisheng Kaitai Acoustic Science Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "he_vibrator_decoder_factory.h"
#include "he_vibrator_decoder.h"
#include "sensors_errors.h"

namespace OHOS {
namespace Sensors {
using namespace OHOS::HiviewDFX;
namespace {
constexpr HiLogLabel LABEL = { LOG_CORE, MISC_LOG_DOMAIN, "HEVibratorDecoderFactory" };
} // namespace
IVibratorDecoder *HEVibratorDecoderFactory::CreateDecoder()
{
    CALL_LOG_ENTER;
    return new HEVibratorDecoder();
}
} // namespace Sensors
} // namespace OHOS