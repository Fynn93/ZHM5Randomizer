#include "ZHM5Randomizer/src/Version.h"

#include <Windows.h>

#include "ZHM5Randomizer/src/Pe.h"

namespace hitman_randomizer {

GameVersion getVersion() {
    auto timestamp = PE::getTimestamp();
    if(timestamp == 0x6283A378)
        return GameVersion::H3DX12_EGS;
    if(timestamp == 0x6283A806)
        return GameVersion::H3DX12_STEAM;
    return GameVersion::UNK;
}

}
