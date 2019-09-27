//---------------------------------------------------------------------------
#pragma inline
//---------------------------------------------------------------------------
#pragma hdrstop
#include <math.h>
#include "chdrawdirect.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
bool TChimeraDrawDirect::HasMMX;
unsigned long TChimeraDrawDirect::ColorBitSize;
unsigned long TChimeraDrawDirect::RMask;
unsigned long TChimeraDrawDirect::GMask;
unsigned long TChimeraDrawDirect::BMask;
unsigned long TChimeraDrawDirect::WMask;
unsigned long TChimeraDrawDirect::RGBMask;
unsigned long TChimeraDrawDirect::RShift;
unsigned long TChimeraDrawDirect::GShift;
unsigned long TChimeraDrawDirect::BShift;
unsigned long TChimeraDrawDirect::RCount;
unsigned long TChimeraDrawDirect::GCount;
unsigned long TChimeraDrawDirect::BCount;
unsigned long TChimeraDrawDirect::ColorKey;
unsigned long TChimeraDrawDirect::ScreenWidth;
unsigned long TChimeraDrawDirect::ScreenHeight;
long TChimeraDrawDirect::DSin[65536];
long TChimeraDrawDirect::DCos[65536];


//***************************************************************************
// 初期化
//***************************************************************************
void TChimeraDrawDirect::Init(unsigned long ARMask, unsigned long AGMask,
    unsigned long ABMask, unsigned long AColorBitSize, unsigned long AColorKey,
    unsigned long AScreenWidth, unsigned long AScreenHeight)
{
    unsigned long Features     = 0;
    unsigned long MinCount;

    // マスク
    RMask        = ARMask;
    GMask        = AGMask;
    BMask        = ABMask;
    RGBMask      = RMask | GMask | BMask;
    WMask        = ~RGBMask;
    ColorBitSize = AColorBitSize;
    ColorKey     = AColorKey;
    ScreenWidth  = AScreenWidth;
    ScreenHeight = AScreenHeight;

    if(AColorBitSize != 1)
    {
        RShift = RCount = 0;
        while((ARMask & 0x01) == 0) { ARMask >>= 1; ++RShift; }
        while((ARMask & 0x01) == 1) { ARMask >>= 1; ++RCount; }

        GShift = GCount = 0;
        while((AGMask & 0x01) == 0) { AGMask >>= 1; ++GShift; }
        while((AGMask & 0x01) == 1) { AGMask >>= 1; ++GCount; }

        BShift = BCount = 0;
        while((ABMask & 0x01) == 0) { ABMask >>= 1; ++BShift; }
        while((ABMask & 0x01) == 1) { ABMask >>= 1; ++BCount; }

        MinCount = RCount   < GCount ? RCount   : GCount;
        MinCount = MinCount < BCount ? MinCount : BCount;

        RShift += RCount - MinCount;
        GShift += GCount - MinCount;
        BShift += BCount - MinCount;
    }
    else
    {
        RShift = RCount = 0;
        GShift = GCount = 0;
        BShift = BCount = 0;
    }

    // MMX能力判定
    asm
    {
        pushfd
        pop     eax
        mov     edx, eax
        xor     eax, 0x200000
        push    eax
        popfd
        pushfd
        pop     eax
        xor     eax, edx
        jz      end
        mov     eax,1
        cpuid
        mov     Features, edx
        mov     eax,1
    end:
    }
    if((Features >> 23) & 1) HasMMX = true;

    // 三角関数テーブル化
    for(int i = 0; i < 65536; ++i)
    {
        DSin[i] = sin(i * M_PI * 2.0f / 65536.0f) * 65536.0;
        DCos[i] = cos(i * M_PI * 2.0f / 65536.0f) * 65536.0;
    }
}
