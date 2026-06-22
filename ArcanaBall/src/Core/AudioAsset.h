#pragma once
#include <cstdint>

enum class AudioAsset : uint8_t {
//--------------------------------------------------------------------//
//                           UI Audio                                 //
//--------------------------------------------------------------------//
    UI_ButtonClick      = 0x10,
    UI_TestAudio2,
//--------------------------------------------------------------------//
//                           Mechanics Effects                        //
//--------------------------------------------------------------------//
    FX_Slowmo           = 0x20,
    FX_StoneBreak,
    FX_BrickHit,
    FX_BrickBreak,
    FX_WoodHit,
    FX_WoodBreak,
    FX_SteelHit,
    FX_SteelBreak,

//--------------------------------------------------------------------//
//                           Elemental Effect                         //
//--------------------------------------------------------------------//
    Elem_InfFire           = 0x50,
    Elem_InfIce,
    Elem_InfLightning,
    Elem_IceShatter,
    Elem_Overload,
    Elem_LightningCross,
//--------------------------------------------------------------------//
//                           Ambience & Music                         //
//--------------------------------------------------------------------//
    Music_Test             = 0x70
};
