/*
 * Copyright (c) 2021-2022 Huawei Device Co., Ltd.
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

#include "miscdevice_service_stub.h"

#include <string>
#include <memory.h>
#include <unistd.h>

#include "hisysevent.h"
#include "ipc_skeleton.h"
#include "securec.h"
#include "message_parcel.h"

#include "permission_util.h"
#include "sensors_errors.h"

namespace OHOS {
namespace Sensors {
using namespace OHOS::HiviewDFX;

namespace {
constexpr HiLogLabel LABEL = { LOG_CORE, MISC_LOG_DOMAIN, "MiscdeviceServiceStub" };
const std::string VIBRATE_PERMISSION = "ohos.permission.VIBRATE";
}  // namespace

MiscdeviceServiceStub::MiscdeviceServiceStub()
{
    MISC_HILOGI("cff MiscdeviceServiceStub begin,  %{public}p", this);
    baseFuncs_[IS_ABILITY_AVAILABLE] = &MiscdeviceServiceStub::IsAbilityAvailablePb;
    baseFuncs_[IS_VIBRATOR_EFFECT_AVAILABLE] = &MiscdeviceServiceStub::IsVibratorEffectAvailablePb;
    baseFuncs_[GET_VIBRATOR_ID_LIST] = &MiscdeviceServiceStub::GetVibratorIdListPb;
    baseFuncs_[VIBRATE] = &MiscdeviceServiceStub::VibratePb;
    baseFuncs_[CANCEL_VIBRATOR] = &MiscdeviceServiceStub::CancelVibratorPb;
    baseFuncs_[PLAY_VIBRATOR_EFFECT] = &MiscdeviceServiceStub::PlayVibratorEffectPb;
    baseFuncs_[PLAY_CUSTOM_VIBRATOR_EFFECT] = &MiscdeviceServiceStub::PlayCustomVibratorEffectPb;
    baseFuncs_[STOP_VIBRATOR_EFFECT] = &MiscdeviceServiceStub::StopVibratorEffectPb;
    baseFuncs_[SET_VIBRATOR_PARA] = &MiscdeviceServiceStub::SetVibratorParameterPb;
    baseFuncs_[GET_VIBRATOR_PARA] = &MiscdeviceServiceStub::GetVibratorParameterPb;
    baseFuncs_[GET_LIGHT_LIST] = &MiscdeviceServiceStub::GetLightListPb;
    baseFuncs_[TURN_ON] = &MiscdeviceServiceStub::TurnOnPb;
    baseFuncs_[TURN_OFF] = &MiscdeviceServiceStub::TurnOffPb;
}

MiscdeviceServiceStub::~MiscdeviceServiceStub()
{
    MISC_HILOGI("~MiscdeviceServiceStub begin, xigou %{public}p", this);
    baseFuncs_.clear();
}

int32_t MiscdeviceServiceStub::IsAbilityAvailablePb(MessageParcel &data, MessageParcel &reply)
{
    uint32_t groupID;
    if (!data.ReadUint32(groupID)) {
        MISC_HILOGE("Parcel read failed");
        return ERROR;
    }
    MiscdeviceDeviceId groupId = static_cast<MiscdeviceDeviceId>(groupID);
    reply.WriteBool(IsAbilityAvailable(groupId));
    return NO_ERROR;
}

int32_t MiscdeviceServiceStub::IsVibratorEffectAvailablePb(MessageParcel &data, MessageParcel &reply)
{
    int32_t vibratorId;
    std::string effectType;
    if ((!data.ReadInt32(vibratorId)) || (!data.ReadString(effectType))) {
        MISC_HILOGE("Parcel read failed");
        return ERROR;
    }
    reply.WriteBool(IsVibratorEffectAvailable(vibratorId, effectType));
    return NO_ERROR;
}

int32_t MiscdeviceServiceStub::VibratePb(MessageParcel &data, MessageParcel &reply)
{
    PermissionUtil &permissionUtil = PermissionUtil::GetInstance();
    int32_t ret = permissionUtil.CheckVibratePermission(this->GetCallingTokenID(), VIBRATE_PERMISSION);
    if (ret != PERMISSION_GRANTED) {
        HiSysEvent::Write(HiSysEvent::Domain::MISCDEVICE, "VIBRATOR_PERMISSIONS_EXCEPTION",
            HiSysEvent::EventType::SECURITY, "PKG_NAME", "VibratePb", "ERROR_CODE", ret);
        MISC_HILOGE("result: %{public}d", ret);
        return PERMISSION_DENIED;
    }
    int32_t vibratorId;
    int32_t duration;
    int32_t usage;
    if ((!data.ReadInt32(vibratorId)) || (!data.ReadInt32(duration)) || (!data.ReadInt32(usage))) {
        MISC_HILOGE("Parcel read failed");
        return ERROR;
    }
    return Vibrate(vibratorId, duration, usage);
}

int32_t MiscdeviceServiceStub::GetVibratorIdListPb(MessageParcel &data, MessageParcel &reply)
{
    std::vector<int32_t> idSet = GetVibratorIdList();
    reply.WriteUint32(static_cast<uint32_t>(idSet.size()));
    reply.WriteInt32Vector(idSet);
    return NO_ERROR;
}

int32_t MiscdeviceServiceStub::CancelVibratorPb(MessageParcel &data, MessageParcel &reply)
{
    PermissionUtil &permissionUtil = PermissionUtil::GetInstance();
    int32_t ret = permissionUtil.CheckVibratePermission(this->GetCallingTokenID(), VIBRATE_PERMISSION);
    if (ret != PERMISSION_GRANTED) {
        HiSysEvent::Write(HiSysEvent::Domain::MISCDEVICE, "VIBRATOR_PERMISSIONS_EXCEPTION",
            HiSysEvent::EventType::SECURITY, "PKG_NAME", "CancelVibratorPb", "ERROR_CODE", ret);
        MISC_HILOGE("result: %{public}d", ret);
        return PERMISSION_DENIED;
    }
    int32_t vibratorId;
    if (!data.ReadInt32(vibratorId)) {
        MISC_HILOGE("Parcel read failed");
        return ERROR;
    }
    return CancelVibrator(vibratorId);
}

int32_t MiscdeviceServiceStub::PlayVibratorEffectPb(MessageParcel &data, MessageParcel &reply)
{
    PermissionUtil &permissionUtil = PermissionUtil::GetInstance();
    int32_t ret = permissionUtil.CheckVibratePermission(this->GetCallingTokenID(), VIBRATE_PERMISSION);
    if (ret != PERMISSION_GRANTED) {
        HiSysEvent::Write(HiSysEvent::Domain::MISCDEVICE, "VIBRATOR_PERMISSIONS_EXCEPTION",
            HiSysEvent::EventType::SECURITY, "PKG_NAME", "PlayVibratorEffectPb", "ERROR_CODE", ret);
        MISC_HILOGE("result: %{public}d", ret);
        return PERMISSION_DENIED;
    }
    int32_t vibratorId;
    std::string effect;
    int32_t count;
    int32_t usage;
    if ((!data.ReadInt32(vibratorId)) || (!data.ReadString(effect)) ||
        (!data.ReadInt32(count)) || (!data.ReadInt32(usage))) {
        MISC_HILOGE("Parcel read failed");
        return ERROR;
    }
    return PlayVibratorEffect(vibratorId, effect, count, usage);
}

int32_t MiscdeviceServiceStub::PlayCustomVibratorEffectPb(MessageParcel &data, MessageParcel &reply)
{
    PermissionUtil &permissionUtil = PermissionUtil::GetInstance();
    int32_t ret = permissionUtil.CheckVibratePermission(this->GetCallingTokenID(), VIBRATE_PERMISSION);
    if (ret != PERMISSION_GRANTED) {
        HiSysEvent::Write(HiSysEvent::Domain::MISCDEVICE, "VIBRATOR_PERMISSIONS_EXCEPTION",
            HiSysEvent::EventType::SECURITY, "PKG_NAME", "PlayCustomVibratorEffectPb", "ERROR_CODE", ret);
        MISC_HILOGE("result: %{public}d", ret);
        return PERMISSION_DENIED;
    }
    std::vector<int32_t> timing;
    std::vector<int32_t> intensity;
    int32_t periodCount;
    if ((!data.ReadInt32(vibratorId)) || (!data.ReadInt32Vector(&timing)) ||
        (!data.ReadInt32Vector(&intensity)) || (!data.ReadInt32(periodCount))) {
        MISC_HILOGE("Parcel read failed");
        return ERROR;
    }
    return PlayCustomVibratorEffect(vibratorId, timing, intensity, periodCount);
}

int32_t MiscdeviceServiceStub::StopVibratorEffectPb(MessageParcel &data, MessageParcel &reply)
{
    PermissionUtil &permissionUtil = PermissionUtil::GetInstance();
    int32_t ret = permissionUtil.CheckVibratePermission(this->GetCallingTokenID(), VIBRATE_PERMISSION);
    if (ret != PERMISSION_GRANTED) {
        HiSysEvent::Write(HiSysEvent::Domain::MISCDEVICE, "VIBRATOR_PERMISSIONS_EXCEPTION",
            HiSysEvent::EventType::SECURITY, "PKG_NAME", "StopVibratorEffectPb", "ERROR_CODE", ret);
        MISC_HILOGE("result: %{public}d", ret);
        return PERMISSION_DENIED;
    }
    int32_t vibratorId;
    std::string effectType;
    if ((!data.ReadInt32(vibratorId)) || (!data.ReadString(effectType))) {
        MISC_HILOGE("Parcel read failed");
        return ERROR;
    }
    return StopVibratorEffect(vibratorId, effectType);
}

int32_t MiscdeviceServiceStub::SetVibratorParameterPb(MessageParcel &data, MessageParcel &reply)
{
    PermissionUtil &permissionUtil = PermissionUtil::GetInstance();
    int32_t ret = permissionUtil.CheckVibratePermission(this->GetCallingTokenID(), VIBRATE_PERMISSION);
    if (ret != PERMISSION_GRANTED) {
        HiSysEvent::Write(HiSysEvent::Domain::MISCDEVICE, "VIBRATOR_PERMISSIONS_EXCEPTION",
            HiSysEvent::EventType::SECURITY, "PKG_NAME", "SetVibratorParameterPb", "ERROR_CODE", ret);
        MISC_HILOGE("result: %{public}d", ret);
        return PERMISSION_DENIED;
    }
    int32_t vibratorId;
    std::string cmd;
    if ((!data.ReadInt32(vibratorId)) || (!data.ReadString(cmd))) {
        MISC_HILOGE("Parcel read failed");
        return ERROR;
    }
    return SetVibratorParameter(vibratorId, cmd);
}

int32_t MiscdeviceServiceStub::GetVibratorParameterPb(MessageParcel &data, MessageParcel &reply)
{
    int32_t vibratorId;
    std::string cmd;
    if ((!data.ReadInt32(vibratorId)) || (!data.ReadString(cmd))) {
        MISC_HILOGE("Parcel read failed");
        return ERROR;
    }
    std::string ret = GetVibratorParameter(vibratorId, cmd);
    reply.WriteString(ret);
    return NO_ERROR;
}

int32_t MiscdeviceServiceStub::GetLightListPb(MessageParcel &data, MessageParcel &reply)
{
    (void)data;
    std::vector<LightInfo> lightInfos(GetLightList());
    int32_t lightCount = int32_t { lightInfos.size() };
    reply.WriteInt32(lightCount);
    for (int32_t i = 0; i < lightCount; i++) {
        if (!reply.WriteBuffer(&lightInfos[i], sizeof(LightInfo))) {
            MISC_HILOGE("cff WriteBuffer failed");
            return WRITE_MSG_ERR;
        }
    }
    return NO_ERROR;
}

int32_t MiscdeviceServiceStub::TurnOnPb(MessageParcel &data, MessageParcel &reply)
{
    int32_t lightId = data.ReadInt32();
    const unsigned char *info = data.ReadBuffer(sizeof(LightColor));
    CHKPR(info, ERROR);
    LightColor lightColor;
    if (memcpy_s(&lightColor, sizeof(LightColor), info, sizeof(LightColor)) != SUCCESS) {
        return ERROR;
    }

    const unsigned char *buf = data.ReadBuffer(sizeof(LightColor));
    CHKPR(buf, ERROR);
    LightAnimation lightAnimation;
    if (memcpy_s(&lightAnimation, sizeof(LightAnimation), buf, sizeof(LightAnimation)) != SUCCESS) {
        return ERROR;
    }
    return TurnOn(lightId, lightColor, lightAnimation);
}

int32_t MiscdeviceServiceStub::TurnOffPb(MessageParcel &data, MessageParcel &reply)
{
    int32_t lightId = data.ReadInt32();
    return TurnOff(lightId);
}

int32_t MiscdeviceServiceStub::OnRemoteRequest(uint32_t code, MessageParcel &data, MessageParcel &reply,
                                               MessageOption &option)
{
    MISC_HILOGD("remoterequest begin, cmd : %{public}u", code);
    std::u16string descriptor = MiscdeviceServiceStub::GetDescriptor();
    std::u16string remoteDescriptor = data.ReadInterfaceToken();
    if (descriptor != remoteDescriptor) {
        MISC_HILOGE("client and service descriptors are inconsistent");
        return OBJECT_NULL;
    }
    auto itFunc = baseFuncs_.find(code);
    if (itFunc != baseFuncs_.end()) {
        auto memberFunc = itFunc->second;
        if (memberFunc != nullptr) {
            return (this->*memberFunc)(data, reply);
        }
    }
    MISC_HILOGD("remoterequest no member function default process");
    return IPCObjectStub::OnRemoteRequest(code, data, reply, option);
}
}  // namespace Sensors
}  // namespace OHOS
