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

#ifndef HDI_CONNECTION_H
#define HDI_CONNECTION_H

#include "v1_2/vibrator_interface_proxy.h"

#include "death_recipient_template.h"
#include "i_vibrator_hdi_connection.h"

using OHOS::HDI::Vibrator::V1_2::IVibratorInterface;
namespace OHOS {
namespace Sensors {
class HdiConnection : public IVibratorHdiConnection {
public:
    HdiConnection() = default;
    virtual ~HdiConnection() = default;
    virtual int32_t ConnectHdi() override;
    virtual int32_t StartOnce(uint32_t duration) override;
    virtual int32_t Start(const std::string &effectType) override;
#ifdef OHOS_BUILD_ENABLE_VIBRATOR_CUSTOM
    virtual int32_t EnableCompositeEffect(const HdfCompositeEffect &hdfCompositeEffect) override;
    virtual bool IsVibratorRunning() override;
#endif // OHOS_BUILD_ENABLE_VIBRATOR_CUSTOM
    virtual std::optional<HdfEffectInfo> GetEffectInfo(const std::string &effect) override;
    virtual int32_t Stop(HdfVibratorMode mode) override;
    virtual int32_t GetDelayTime(int32_t mode, int32_t &delayTime) override;
    virtual int32_t GetVibratorCapacity(VibratorCapacity &capacity) override;
    virtual int32_t PlayPattern(const VibratePattern &pattern) override;
    virtual int32_t DestroyHdiConnection() override;
    void ProcessDeathObserver(const wptr<IRemoteObject> &object);

private:
    DISALLOW_COPY_AND_MOVE(HdiConnection);
    sptr<IVibratorInterface> vibratorInterface_ = nullptr;
    sptr<IRemoteObject::DeathRecipient> hdiDeathObserver_ = nullptr;
    void RegisterHdiDeathRecipient();
    void UnregisterHdiDeathRecipient();
    void Reconnect();
};
}  // namespace Sensors
}  // namespace OHOS
#endif  // HDI_CONNECTION_H