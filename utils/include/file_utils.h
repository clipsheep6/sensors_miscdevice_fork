/*
 * Copyright (c) 2022-2023 Huawei Device Co., Ltd.
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

#ifndef MISCDEVICE_FILE_UTILS_H
#define MISCDEVICE_FILE_UTILS_H

#include <cstdint>
#include <cstdio>
#include <string>

#include <unistd.h>

<<<<<<< HEAD
=======
#include "raw_file_descriptor.h"

>>>>>>> bef164e846fb873c6f4a9bf7719c42a0d57d19dc
namespace OHOS {
namespace Sensors {
std::string ReadJsonFile(const std::string &filePath);
std::string ReadFile(const std::string &filePath);
bool CheckFileDir(const std::string& filePath, const std::string& dir);
bool CheckFileExtendName(const std::string& filePath, const std::string& checkExtension);
bool CheckFileSize(const std::string& filePath);
bool IsFileExists(const std::string& fileName);
int32_t GetFileSize(const std::string& filePath);
<<<<<<< HEAD
int32_t GetFileSize(int32_t fd);
std::string ReadFd(int32_t fd);
=======
int64_t GetFileSize(int32_t fd);
std::string ReadFd(const RawFileDescriptor &rawFd);
>>>>>>> bef164e846fb873c6f4a9bf7719c42a0d57d19dc
std::string GetFileSuffix(int32_t fd);
}  // namespace Sensors
}  // namespace OHOS
#endif  // MISCDEVICE_FILE_UTILS_H
