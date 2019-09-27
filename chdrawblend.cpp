//---------------------------------------------------------------------------
#pragma inline
//---------------------------------------------------------------------------
#pragma hdrstop
#include <math.h>
#include "chdrawdirect.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
// #define COLORKEY
#define min(A, B) (A < B ? A : B)
//---------------------------------------------------------------------------
namespace ChimeraDrawSystem
{
    void TransBlend16MMX(const TChimeraDrawDirect &Param);
    void AddBlend16MMX(const TChimeraDrawDirect &Param);
    void ModBlend16MMX(const TChimeraDrawDirect &Param);
    void SubBlend16MMX(const TChimeraDrawDirect &Param);

    void TransBlend32MMX(const TChimeraDrawDirect &Param);
    void AddBlend32MMX(const TChimeraDrawDirect &Param);
    void ModBlend32MMX(const TChimeraDrawDirect &Param);
    void SubBlend32MMX(const TChimeraDrawDirect &Param);
};


//***************************************************************************
// 回転回転
// 16bit ＯＮＬＹ！　速度重視
//***************************************************************************
void ChimeraDrawSystem::RotDraw16(const TChimeraDrawDirect &Param)
{
    using TChimeraDrawDirect::DCos;
    using TChimeraDrawDirect::DSin;
    using TChimeraDrawDirect::ScreenWidth;
    using TChimeraDrawDirect::ScreenHeight;

    unsigned char *PtrDest, *PtrSrc;
    long PitchDest;
    long PitchSrc;
    long TSin, TCos, ASin, ACos;
    long TWidth, THeight;

    long CX, CY, SX, SY, SW, SH, FX, FY;

    // 速度向上のためにローカルにおく
    PtrDest   = Param.PtrDest;
    PtrSrc    = Param.PtrSrc;
    PitchDest = Param.PitchDest;
    PitchSrc  = Param.PitchSrc;

    TWidth    = Param.Width;
    THeight   = Param.Height;

    TCos      = DCos[Param.Param1 & 0xFFFF];
    TSin      = DSin[Param.Param1 & 0xFFFF];

    ACos = (TCos * Param.Param2) >> 8;
    ASin = (TSin * Param.Param2) >> 8;

    // 転送先での転送範囲
    CX = (abs(ACos * TWidth / 2) + abs(ASin * THeight / 2)) >> 16;
    CY = (abs(ASin * TWidth / 2) + abs(ACos * THeight / 2)) >> 16;

    // 拡大縮小処理
    ACos = (TCos << 8) / Param.Param2;
    ASin = (TSin << 8) / Param.Param2;

    SX = ((TWidth  / 2) << 16) - CX * ACos + CY * ASin;
    SY = ((THeight / 2) << 16) - CX * ASin - CY * ACos;

    FX = Param.X - CX;
    FY = Param.Y - CY;

    // クリッピング処理
    if(FX < 0) { SX -= ACos * FX; SY -= ASin * FX; FX = 0; }
    if(FY < 0) { SX += ASin * FY; SY -= ACos * FY; FY = 0; }

    PtrDest += FX * 2 + PitchDest * FY;

    // クリッピング処理
    SW = min(Param.X + CX, (long)ScreenWidth ) - FX;
    SH = min(Param.Y + CY, (long)ScreenHeight) - FY;

    if(SW <= 0 || SH <= 0) return;

    // 転送
    for(int DY = 0; DY < SH; ++DY)
    {
        asm
        {
            mov     edi, PtrDest
            mov     esi, PtrSrc
            mov     edx, PitchSrc
            xor     ecx, ecx
            mov     eax, SX
            mov     ebx, SY
        LOOP_ROT16:
            cmp     ecx, SW
            jz      LOOPEND_ROT16

            // 保存
            push    eax
            push    ebx

            // 下駄脱ぎ
            sar     eax, 16
            sar     ebx, 16
            test    eax, eax

            // クリッピング
            jl      LOOPCMP_ROT16
            test    ebx, ebx
            jl      LOOPCMP_ROT16
            cmp     eax, TWidth
            jge     LOOPCMP_ROT16
            cmp     ebx, THeight
            jge     LOOPCMP_ROT16

            // ポインタ計算＆転送
            imul    ebx, edx
            lea     ebx, [ebx+eax*2]
            mov     ax, [esi+ebx]
            mov     [edi+ecx*2], ax

        LOOPCMP_ROT16:
            pop     ebx
            pop     eax
            add     eax, ACos
            add     ebx, ASin
            inc     ecx
            jmp     LOOP_ROT16
        LOOPEND_ROT16:
        }
        SX -= ASin;
        SY += ACos;
        PtrDest += PitchDest;
    }
}


//***************************************************************************
// 回転回転
// 24bit ＯＮＬＹ！　速度重視
//***************************************************************************
void ChimeraDrawSystem::RotDraw24(const TChimeraDrawDirect &Param)
{
    using TChimeraDrawDirect::DCos;
    using TChimeraDrawDirect::DSin;
    using TChimeraDrawDirect::ScreenWidth;
    using TChimeraDrawDirect::ScreenHeight;

    unsigned char *PtrDest, *PtrSrc;
    long PitchDest;
    long PitchSrc;
    long TSin, TCos, ASin, ACos;
    long TWidth, THeight;

    long CX, CY, SX, SY, SW, SH, FX, FY;

    // 速度向上のためにローカルにおく
    PtrDest   = Param.PtrDest;
    PtrSrc    = Param.PtrSrc;
    PitchDest = Param.PitchDest;
    PitchSrc  = Param.PitchSrc;

    TWidth    = Param.Width;
    THeight   = Param.Height;

    TCos      = DCos[Param.Param1 & 0xFFFF];
    TSin      = DSin[Param.Param1 & 0xFFFF];

    ACos = (TCos * Param.Param2) >> 8;
    ASin = (TSin * Param.Param2) >> 8;

    // 転送先での転送範囲
    CX = (abs(ACos * TWidth / 2) + abs(ASin * THeight / 2)) >> 16;
    CY = (abs(ASin * TWidth / 2) + abs(ACos * THeight / 2)) >> 16;

    // 拡大縮小処理
    ACos = (TCos << 8) / Param.Param2;
    ASin = (TSin << 8) / Param.Param2;

    SX = ((TWidth  / 2) << 16) - CX * ACos + CY * ASin;
    SY = ((THeight / 2) << 16) - CX * ASin - CY * ACos;

    FX = Param.X - CX;
    FY = Param.Y - CY;

    // クリッピング処理
    if(FX < 0) { SX -= ACos * FX; SY -= ASin * FX; FX = 0; }
    if(FY < 0) { SX += ASin * FY; SY -= ACos * FY; FY = 0; }

    PtrDest += FX * 3 + PitchDest * FY;

    // クリッピング処理
    SW = min(Param.X + CX, (long)ScreenWidth ) - FX;
    SH = min(Param.Y + CY, (long)ScreenHeight) - FY;

    if(SW <= 0 || SH <= 0) return;

    // 転送
    for(int DY = 0; DY < SH; ++DY)
    {
        asm
        {
            mov     edi, PtrDest
            mov     esi, PtrSrc
            mov     edx, PitchSrc
            xor     ecx, ecx
            mov     eax, SX
            mov     ebx, SY
        LOOP_ROT24:
            cmp     ecx, SW
            jz      LOOPEND_ROT24

            // 保存
            push    eax
            push    ebx

            // 下駄脱ぎ
            sar     eax, 16
            sar     ebx, 16

            // クリッピング
            test    eax, eax
            jl      LOOPCMP_ROT24
            test    ebx, ebx
            jl      LOOPCMP_ROT24
            cmp     eax, TWidth
            jge     LOOPCMP_ROT24
            cmp     ebx, THeight
            jge     LOOPCMP_ROT24

            // ポインタ計算＆転送
            imul    ebx, edx
            lea     eax, [eax+eax*2]
            lea     ebx, [ebx+eax]
            mov     eax, [esi+ebx]
            and     eax, 0x00FFFFFF
            lea     ebx, [ecx+ecx*2]
            and     [edi+ebx], 0xFF000000
            or      [edi+ebx], eax

        LOOPCMP_ROT24:
            pop     ebx
            pop     eax
            add     eax, ACos
            add     ebx, ASin
            inc     ecx
            jmp     LOOP_ROT24
        LOOPEND_ROT24:
        }
        SX -= ASin;
        SY += ACos;
        PtrDest += PitchDest;
    }
}


//***************************************************************************
// 回転回転
// 32bit ＯＮＬＹ！　速度重視
//***************************************************************************
void ChimeraDrawSystem::RotDraw32(const TChimeraDrawDirect &Param)
{
    using TChimeraDrawDirect::DCos;
    using TChimeraDrawDirect::DSin;
    using TChimeraDrawDirect::ScreenWidth;
    using TChimeraDrawDirect::ScreenHeight;

    unsigned char *PtrDest, *PtrSrc;
    long PitchDest;
    long PitchSrc;
    long TSin, TCos, ASin, ACos;
    long TWidth, THeight;

    long CX, CY, SX, SY, SW, SH, FX, FY;

    // 速度向上のためにローカルにおく
    PtrDest   = Param.PtrDest;
    PtrSrc    = Param.PtrSrc;
    PitchDest = Param.PitchDest;
    PitchSrc  = Param.PitchSrc;

    TWidth    = Param.Width;
    THeight   = Param.Height;

    TCos      = DCos[Param.Param1 & 0xFFFF];
    TSin      = DSin[Param.Param1 & 0xFFFF];

    ACos = (TCos * Param.Param2) >> 8;
    ASin = (TSin * Param.Param2) >> 8;

    // 転送先での転送範囲
    CX = (abs(ACos * TWidth / 2) + abs(ASin * THeight / 2)) >> 16;
    CY = (abs(ASin * TWidth / 2) + abs(ACos * THeight / 2)) >> 16;

    // 拡大縮小処理
    ACos = (TCos << 8) / Param.Param2;
    ASin = (TSin << 8) / Param.Param2;

    SX = ((TWidth  / 2) << 16) - CX * ACos + CY * ASin;
    SY = ((THeight / 2) << 16) - CX * ASin - CY * ACos;

    FX = Param.X - CX;
    FY = Param.Y - CY;

    // クリッピング処理
    if(FX < 0) { SX -= ACos * FX; SY -= ASin * FX; FX = 0; }
    if(FY < 0) { SX += ASin * FY; SY -= ACos * FY; FY = 0; }

    PtrDest += FX * 4 + PitchDest * FY;

    // クリッピング処理
    SW = min(Param.X + CX, (long)ScreenWidth ) - FX;
    SH = min(Param.Y + CY, (long)ScreenHeight) - FY;

    if(SW <= 0 || SH <= 0) return;

    // 転送
    for(int DY = 0; DY < SH; ++DY)
    {
        asm
        {
            mov     edi, PtrDest
            mov     esi, PtrSrc
            mov     edx, PitchSrc
            xor     ecx, ecx
            mov     eax, SX
            mov     ebx, SY
        LOOP_ROT32:
            cmp     ecx, SW
            jz      LOOPEND_ROT32

            // 保存
            push    eax
            push    ebx

            // 下駄脱ぎ
            sar     eax, 16
            sar     ebx, 16

            // クリッピング
            test    eax, eax
            jl      LOOPCMP_ROT32
            test    ebx, ebx
            jl      LOOPCMP_ROT32
            cmp     eax, TWidth
            jge     LOOPCMP_ROT32
            cmp     ebx, THeight
            jge     LOOPCMP_ROT32

            // ポインタ計算＆転送
            imul    ebx, edx
            lea     ebx, [ebx+eax*4]
            mov     eax, [esi+ebx]
            mov     [edi+ecx*4], eax

        LOOPCMP_ROT32:
            pop     ebx
            pop     eax
            add     eax, ACos
            add     ebx, ASin
            inc     ecx
            jmp     LOOP_ROT32
        LOOPEND_ROT32:
        }
        SX -= ASin;
        SY += ACos;
        PtrDest += PitchDest;
    }
}






//***************************************************************************
// 回転回転MMX
// 16bit ＯＮＬＹ！　速度重視
//***************************************************************************
void ChimeraDrawSystem::RotDraw16MMX(const TChimeraDrawDirect &Param)
{
    using TChimeraDrawDirect::DCos;
    using TChimeraDrawDirect::DSin;
    using TChimeraDrawDirect::ScreenWidth;
    using TChimeraDrawDirect::ScreenHeight;

    unsigned char *PtrDest, *PtrSrc;
    long PitchDest;
    long PitchSrc;
    long TSin, TCos, ASin, ACos;
    long TWidth, THeight;

    long CX, CY, SX, SY, SW, SH, FX, FY;

    __int64 Pixel, Comp, Delta, Pitch;

    // 速度向上のためにローカルにおく
    PtrDest   = Param.PtrDest;
    PtrSrc    = Param.PtrSrc;
    PitchDest = Param.PitchDest;
    PitchSrc  = Param.PitchSrc;

    TWidth    = Param.Width;
    THeight   = Param.Height;

    TCos      = DCos[Param.Param1 & 0xFFFF];
    TSin      = DSin[Param.Param1 & 0xFFFF];

    ACos = (TCos * Param.Param2) >> 8;
    ASin = (TSin * Param.Param2) >> 8;

    // 転送先での転送範囲
    CX = (abs(ACos * TWidth / 2) + abs(ASin * THeight / 2)) >> 16;
    CY = (abs(ASin * TWidth / 2) + abs(ACos * THeight / 2)) >> 16;

    // 拡大縮小処理
    ACos = (TCos << 8) / Param.Param2;
    ASin = (TSin << 8) / Param.Param2;

    SX = ((TWidth  / 2) << 16) - CX * ACos + CY * ASin;
    SY = ((THeight / 2) << 16) - CX * ASin - CY * ACos;

    FX = Param.X - CX;
    FY = Param.Y - CY;

    // クリッピング処理
    if(FX < 0) { SX -= ACos * FX; SY -= ASin * FX; FX = 0; }
    if(FY < 0) { SX += ASin * FY; SY -= ACos * FY; FY = 0; }

    PtrDest += FX * 2 + PitchDest * FY;

    // クリッピング処理
    SW = min(Param.X + CX, (long)ScreenWidth ) - FX;
    SH = min(Param.Y + CY, (long)ScreenHeight) - FY;

    if(SW <= 0 || SH <= 0) return;

    ((long*)&Comp)[0]  = TWidth;
    ((long*)&Comp)[1]  = THeight;

    ((long*)&Delta)[0]  = ACos;
    ((long*)&Delta)[1]  = ASin;

    ((short*)&Pitch)[0] = 2;
    ((short*)&Pitch)[1] = PitchSrc;

    // 転送
    for(int DY = 0; DY < SH; ++DY)
    {
        ((long*)&Pixel)[0]  = SX;
        ((long*)&Pixel)[1]  = SY;

        asm
        {
            mov         edi, PtrDest
            mov         esi, PtrSrc
            mov         edx, PitchSrc
            xor         ecx, ecx

            movq        mm4, Pixel
            movq        mm5, Pitch
            movq        mm6, Comp
            movq        mm7, Delta

        LOOP_ROT16MMX:
            // ペアリング済みのためコメント不可能
            cmp         ecx, SW
            jz          LOOPEND_ROT16MMX
            movq        mm0, mm4
            pxor        mm1, mm1
            movq        mm2, mm6
            psrld       mm0, 16
            movq        mm3, mm0
            pcmpgtd     mm1, mm0
            pcmpgtd     mm2, mm3
            packssdw    mm0, mm3
            pandn       mm1, mm2
            packsswb    mm1, mm2
            pmaddwd     mm0, mm5
            movd        eax, mm1
            paddd       mm4, mm7
            cmp         eax, 0xFFFFFFFF
            jne         LOOPCMP_ROT16MMX
            movd        ebx, mm0
            mov         ax, [esi+ebx]
            mov         [edi+ecx*2], ax
        LOOPCMP_ROT16MMX:

            inc         ecx
            jmp         LOOP_ROT16MMX
        LOOPEND_ROT16MMX:
        }
        SX -= ASin;
        SY += ACos;
        PtrDest += PitchDest;
    }
    asm emms;
}



/////////////////////////////////////////////////////////////////////////////
//***************************************************************************
// Srcいじり〜
//***************************************************************************

//***************************************************************************
// 加算
//***************************************************************************
void ChimeraDrawSystem::AddDraw(const TChimeraDrawDirect &Param)
{
    using TChimeraDrawDirect::RMask;
    using TChimeraDrawDirect::GMask;
    using TChimeraDrawDirect::BMask;
    using TChimeraDrawDirect::WMask;
    using TChimeraDrawDirect::RGBMask;
    using TChimeraDrawDirect::ColorBitSize;
    using TChimeraDrawDirect::ColorKey;

    unsigned char *PtrDest, *PtrSrc;
    unsigned char *PixelDest, *PixelSrc;
    long PitchDest, PitchSrc;
    unsigned long D, S, RD, GD, BD, RS, GS, BS, RA, GA, BA;
    unsigned long Alpha, InvAlpha;

    PtrDest   = Param.PtrDest;
    PtrSrc    = Param.PtrSrc;
    PitchDest = Param.PitchDest;
    PitchSrc  = Param.PitchSrc;
    Alpha     = Param.Param1;

    RA = (((Alpha >> 16) & 0xFF) * RMask / 0x100) & RMask;
    GA = (((Alpha >>  8) & 0xFF) * GMask / 0x100) & GMask;
    BA = (((Alpha >>  0) & 0xFF) * BMask / 0x100) & BMask;

    for(unsigned long y = 0; y < Param.Height; ++y)
    {
        PixelDest = PtrDest;
        PixelSrc  = PtrSrc;

        for(unsigned long x = 0; x < Param.Width; ++x)
        {
            S  = *((unsigned long*)PixelSrc);
#ifdef COLORKEY
            if((S & RGBMask) == ColorKey)
            { PixelDest += ColorBitSize; PixelSrc += ColorBitSize; continue; }
#endif
            RS = S & RMask;
            GS = S & GMask;
            BS = S & BMask;

            RD = min(RS + RA, RMask);
            GD = min(GS + GA, GMask);
            BD = min(BS + BA, BMask);

            *((unsigned long*)PixelDest) &= WMask;
            *((unsigned long*)PixelDest) |= RD | GD | BD;
            PixelDest += ColorBitSize;
            PixelSrc  += ColorBitSize;
        }
        PtrDest += PitchDest;
        PtrSrc  += PitchSrc;
    }
}


//***************************************************************************
// 乗算
//***************************************************************************
void ChimeraDrawSystem::ModDraw(const TChimeraDrawDirect &Param)
{
    using TChimeraDrawDirect::RMask;
    using TChimeraDrawDirect::GMask;
    using TChimeraDrawDirect::BMask;
    using TChimeraDrawDirect::WMask;
    using TChimeraDrawDirect::RGBMask;
    using TChimeraDrawDirect::ColorBitSize;
    using TChimeraDrawDirect::ColorKey;

    unsigned char *PtrDest, *PtrSrc;
    unsigned char *PixelDest, *PixelSrc;
    long PitchDest, PitchSrc;
    unsigned long D, S, RD, GD, BD, RS, GS, BS, RA, GA, BA;
    unsigned long Alpha, InvAlpha;

    PtrDest   = Param.PtrDest;
    PtrSrc    = Param.PtrSrc;
    PitchDest = Param.PitchDest;
    PitchSrc  = Param.PitchSrc;
    Alpha     = Param.Param1;

    RA = (Alpha >> 16) & 0xFF;
    GA = (Alpha >>  8) & 0xFF;
    BA = (Alpha >>  0) & 0xFF;

    for(unsigned long y = 0; y < Param.Height; ++y)
    {
        PixelDest = PtrDest;
        PixelSrc  = PtrSrc;

        for(unsigned long x = 0; x < Param.Width; ++x)
        {
            S  = *((unsigned long*)PixelSrc);
#ifdef COLORKEY
            if((S & RGBMask) == ColorKey)
            { PixelDest += ColorBitSize; PixelSrc += ColorBitSize; continue; }
#endif
            RS = S & RMask;
            GS = S & GMask;
            BS = S & BMask;

            RD = (RS * RA / 0x100) & RMask;
            GD = (GS * GA / 0x100) & GMask;
            BD = (BS * BA / 0x100) & BMask;

            *((unsigned long*)PixelDest) &= WMask;
            *((unsigned long*)PixelDest) |= RD | GD | BD;
            PixelDest += ColorBitSize;
            PixelSrc  += ColorBitSize;
        }
        PtrDest += PitchDest;
        PtrSrc  += PitchSrc;
    }
}










/////////////////////////////////////////////////////////////////////////////
//***************************************************************************
// 汎用ブレンド
// 16bit、24bit、32bit　ＯＫ！　互換性重視
//***************************************************************************

//***************************************************************************
// 半透明合成
//***************************************************************************
void ChimeraDrawSystem::TransBlend(const TChimeraDrawDirect &Param)
{
    using TChimeraDrawDirect::RMask;
    using TChimeraDrawDirect::GMask;
    using TChimeraDrawDirect::BMask;
    using TChimeraDrawDirect::WMask;
    using TChimeraDrawDirect::RGBMask;
    using TChimeraDrawDirect::ColorBitSize;
    using TChimeraDrawDirect::ColorKey;

    unsigned char *PtrDest, *PtrSrc;
    unsigned char *PixelDest, *PixelSrc;
    long PitchDest, PitchSrc;
    unsigned long D, S, RD, GD, BD, RS, GS, BS;
    unsigned long Alpha, InvAlpha;

    PtrDest   = Param.PtrDest;
    PtrSrc    = Param.PtrSrc;
    PitchDest = Param.PitchDest;
    PitchSrc  = Param.PitchSrc;
    Alpha     = Param.Param1;
    InvAlpha  = 0xFF - Alpha;

    for(unsigned long y = 0; y < Param.Height; ++y)
    {
        PixelDest = PtrDest;
        PixelSrc  = PtrSrc;

        for(unsigned long x = 0; x < Param.Width; ++x)
        {
            S  = *((unsigned long*)PixelSrc);
#ifdef COLORKEY
            if((S & RGBMask) == ColorKey)
            { PixelDest += ColorBitSize; PixelSrc += ColorBitSize; continue; }
#endif
            RS = S & RMask;
            GS = S & GMask;
            BS = S & BMask;

            D  = *((unsigned long*)PixelDest);
            RD = D & RMask;
            GD = D & GMask;
            BD = D & BMask;

            RD = ((RS * Alpha + RD * InvAlpha) / 0x100) & RMask;
            GD = ((GS * Alpha + GD * InvAlpha) / 0x100) & GMask;
            BD = ((BS * Alpha + BD * InvAlpha) / 0x100) & BMask;

            *((unsigned long*)PixelDest) &= WMask;
            *((unsigned long*)PixelDest) |= RD | GD | BD;
            PixelDest += ColorBitSize;
            PixelSrc  += ColorBitSize;
        }
        PtrDest += PitchDest;
        PtrSrc  += PitchSrc;
    }
}


//***************************************************************************
// 加算合成
//***************************************************************************
void ChimeraDrawSystem::AddBlend(const TChimeraDrawDirect &Param)
{
    using TChimeraDrawDirect::RMask;
    using TChimeraDrawDirect::GMask;
    using TChimeraDrawDirect::BMask;
    using TChimeraDrawDirect::WMask;
    using TChimeraDrawDirect::RGBMask;
    using TChimeraDrawDirect::ColorBitSize;
    using TChimeraDrawDirect::ColorKey;

    unsigned char *PtrDest, *PtrSrc;
    unsigned char *PixelDest, *PixelSrc;
    long PitchDest, PitchSrc;
    unsigned long D, S, RD, GD, BD, RS, GS, BS;
    unsigned long Alpha, InvAlpha;

    PtrDest   = Param.PtrDest;
    PtrSrc    = Param.PtrSrc;
    PitchDest = Param.PitchDest;
    PitchSrc  = Param.PitchSrc;
    Alpha     = Param.Param1;

    for(unsigned long y = 0; y < Param.Height; ++y)
    {
        PixelDest = PtrDest;
        PixelSrc  = PtrSrc;

        for(unsigned long x = 0; x < Param.Width; ++x)
        {
            S  = *((unsigned long*)PixelSrc);
#ifdef COLORKEY
            if((S & RGBMask) == ColorKey)
            { PixelDest += ColorBitSize; PixelSrc += ColorBitSize; continue; }
#endif
            RS = S & RMask;
            GS = S & GMask;
            BS = S & BMask;

            D  = *((unsigned long*)PixelDest);
            RD = D & RMask;
            GD = D & GMask;
            BD = D & BMask;

            RD = min((RS * Alpha / 0x100) + RD, RMask) & RMask;
            GD = min((GS * Alpha / 0x100) + GD, GMask) & GMask;
            BD = min((BS * Alpha / 0x100) + BD, BMask) & BMask;

            *((unsigned long*)PixelDest) &= WMask;
            *((unsigned long*)PixelDest) |= RD | GD | BD;
            PixelDest += ColorBitSize;
            PixelSrc  += ColorBitSize;
        }
        PtrDest += PitchDest;
        PtrSrc  += PitchSrc;
    }
}


//***************************************************************************
// 乗算合成
//***************************************************************************
void ChimeraDrawSystem::ModBlend(const TChimeraDrawDirect &Param)
{
    using TChimeraDrawDirect::RMask;
    using TChimeraDrawDirect::GMask;
    using TChimeraDrawDirect::BMask;
    using TChimeraDrawDirect::WMask;
    using TChimeraDrawDirect::RGBMask;
    using TChimeraDrawDirect::ColorBitSize;
    using TChimeraDrawDirect::ColorKey;

    unsigned char *PtrDest, *PtrSrc;
    unsigned char *PixelDest, *PixelSrc;
    long PitchDest, PitchSrc;
    unsigned long D, S, RD, GD, BD, RS, GS, BS;
    unsigned long Alpha, InvAlpha;

    PtrDest   = Param.PtrDest;
    PtrSrc    = Param.PtrSrc;
    PitchDest = Param.PitchDest;
    PitchSrc  = Param.PitchSrc;
    Alpha     = Param.Param1;

    for(unsigned long y = 0; y < Param.Height; ++y)
    {
        PixelDest = PtrDest;
        PixelSrc  = PtrSrc;

        for(unsigned long x = 0; x < Param.Width; ++x)
        {
            S  = ~*((unsigned long*)PixelSrc);
#ifdef COLORKEY
            if((~S & RGBMask) == ColorKey)
            { PixelDest += ColorBitSize; PixelSrc += ColorBitSize; continue; }
#endif
            RS = S & RMask;
            GS = S & GMask;
            BS = S & BMask;

            D  = ~*((unsigned long*)PixelDest);
            RD = D & RMask;
            GD = D & GMask;
            BD = D & BMask;

            RD = min((RS * Alpha / 0x100) + RD, RMask) & RMask;
            GD = min((GS * Alpha / 0x100) + GD, GMask) & GMask;
            BD = min((BS * Alpha / 0x100) + BD, BMask) & BMask;

            *((unsigned long*)PixelDest) &= WMask;
            *((unsigned long*)PixelDest) |= ~(RD | GD | BD) & RGBMask;
            PixelDest += ColorBitSize;
            PixelSrc  += ColorBitSize;
        }
        PtrDest += PitchDest;
        PtrSrc  += PitchSrc;
    }
}


//***************************************************************************
// 減算合成
//***************************************************************************
void ChimeraDrawSystem::SubBlend(const TChimeraDrawDirect &Param)
{
    using TChimeraDrawDirect::RMask;
    using TChimeraDrawDirect::GMask;
    using TChimeraDrawDirect::BMask;
    using TChimeraDrawDirect::WMask;
    using TChimeraDrawDirect::RGBMask;
    using TChimeraDrawDirect::ColorBitSize;
    using TChimeraDrawDirect::ColorKey;

    unsigned char *PtrDest, *PtrSrc;
    unsigned char *PixelDest, *PixelSrc;
    long PitchDest, PitchSrc;
    unsigned long D, S, RD, GD, BD, RS, GS, BS;
    unsigned long Alpha, InvAlpha;

    PtrDest   = Param.PtrDest;
    PtrSrc    = Param.PtrSrc;
    PitchDest = Param.PitchDest;
    PitchSrc  = Param.PitchSrc;
    Alpha     = Param.Param1;

    for(unsigned long y = 0; y < Param.Height; ++y)
    {
        PixelDest = PtrDest;
        PixelSrc  = PtrSrc;

        for(unsigned long x = 0; x < Param.Width; ++x)
        {
            S  = *((unsigned long*)PixelSrc);
#ifdef COLORKEY
            if((S & RGBMask) == ColorKey)
            { PixelDest += ColorBitSize; PixelSrc += ColorBitSize; continue; }
#endif
            RS = S & RMask;
            GS = S & GMask;
            BS = S & BMask;
            D  = ~*((unsigned long*)PixelDest);
            RD = D & RMask;
            GD = D & GMask;
            BD = D & BMask;

            RD = min((RS * Alpha / 0x100) + RD, RMask) & RMask;
            GD = min((GS * Alpha / 0x100) + GD, GMask) & GMask;
            BD = min((BS * Alpha / 0x100) + BD, BMask) & BMask;

            *((unsigned long*)PixelDest) &= WMask;
            *((unsigned long*)PixelDest) |= ~(RD | GD | BD) & RGBMask;
            PixelDest += ColorBitSize;
            PixelSrc  += ColorBitSize;
        }
        PtrDest += PitchDest;
        PtrSrc  += PitchSrc;
    }
}


//***************************************************************************
// アルファ合成（半透明）
//***************************************************************************
void ChimeraDrawSystem::AlphaTransBlend(const TChimeraDrawDirect &Param)
{
    using TChimeraDrawDirect::RMask;
    using TChimeraDrawDirect::GMask;
    using TChimeraDrawDirect::BMask;
    using TChimeraDrawDirect::WMask;
    using TChimeraDrawDirect::RGBMask;
    using TChimeraDrawDirect::ColorBitSize;

    unsigned char *PtrDest, *PtrSrc, *PtrAlpha;
    unsigned char *PixelDest, *PixelSrc, *PixelAlpha;
    long PitchDest, PitchSrc, PitchAlpha;
    unsigned long D, S, A, B, RD, GD, BD, RS, GS, BS;

    PtrDest    = Param.PtrDest;
    PtrSrc     = Param.PtrSrc;
    PtrAlpha   = Param.PtrAlpha;
    PitchDest  = Param.PitchDest;
    PitchSrc   = Param.PitchSrc;
    PitchAlpha = Param.PitchAlpha;

    for(unsigned long y = 0; y < Param.Height; ++y)
    {
        PixelDest  = PtrDest;
        PixelSrc   = PtrSrc;
        PixelAlpha = PtrAlpha;

        for(unsigned long x = 0; x < Param.Width; ++x)
        {
            S  = *((unsigned long*)PixelSrc);
            RS = S & RMask;
            GS = S & GMask;
            BS = S & BMask;

            D  = *((unsigned long*)PixelDest);
            RD = D & RMask;
            GD = D & GMask;
            BD = D & BMask;

            A  = *PixelAlpha;
            B  = 0xFF - A;

            RD = ((RS * A + RD * B) / 0x100) & RMask;
            GD = ((GS * A + GD * B) / 0x100) & GMask;
            BD = ((BS * A + BD * B) / 0x100) & BMask;

            *((unsigned long*)PixelDest) &= WMask;
            *((unsigned long*)PixelDest) |= RD | GD | BD;
            PixelDest += ColorBitSize;
            PixelSrc  += ColorBitSize;
            PixelAlpha++;
        }
        PtrDest  += PitchDest;
        PtrSrc   += PitchSrc;
        PtrAlpha += PitchAlpha;
    }
}



/////////////////////////////////////////////////////////////////////////////
//***************************************************************************
// ChimeraMMXLib16
// 16bit　ＯＮＬＹ！
//***************************************************************************

/////////////////////////////////////////////////////////////////////////////
//***************************************************************************
// 16bit用αブレンド
//***************************************************************************

#define Word2QWord(WValue, QValue)                                          \
    asm                                                                     \
    {                                                                       \
        movd        mm0, WValue         ;                                   \
        punpcklwd   mm0, mm0            ;                                   \
        punpckldq   mm0, mm0            ;                                   \
        movq        QValue, mm0         ;                                   \
    }                                                                       \
//---------------------------------------------------------------------------

// 合成処理初期化＆変数定義
#define Blend16MMXInitialize                                                \
    unsigned __int64 Mask1, Mask2, Shift1, Shift2;                          \
    unsigned __int64 ColorKey64, RestMask64, XORMask64;                     \
                                                                            \
    unsigned char *dpix = Param.PtrDest;                                    \
    unsigned char *spix = Param.PtrSrc;                                     \
    unsigned long Width = Param.Width / 4;                                  \
    unsigned long Rest  = Param.Width % 4;                                  \
    unsigned long ColorKey = TChimeraDrawDirect::ColorKey;                  \
                                                                            \
    RestMask64 = 0xFFFFFFFFFFFFFFFFui64;                                    \
    for(unsigned long i = 0; i < Rest; ++i) RestMask64 <<= 16;              \
    Rest *= 2;                                                              \
                                                                            \
    XORMask64 = 0xFFFFFFFFFFFFFFFFui64;                                     \
                                                                            \
    if(TChimeraDrawDirect::GMask == 0x07E0)                                 \
    {                                                                       \
        Mask1  = 0x7BEF7BEF7BEF7BEFui64;                                    \
        Mask2  = 0x8410841084108410ui64;                                    \
        Shift1 = 1;                                                         \
        Shift2 = 4;                                                         \
    }                                                                       \
    else                                                                    \
    {                                                                       \
        Mask1  = 0x7BDF7BDF7BDF7BDFui64;                                    \
        Mask2  = 0x8420842084208420ui64;                                    \
        Shift1 = 0;                                                         \
        Shift2 = 5;                                                         \
    }                                                                       \
                                                                            \
    Word2QWord(ColorKey, ColorKey64);                                       \
//---------------------------------------------------------------------------


// アルファブレンド処理初期化＆変数定義
#define Blend16AlphaMMXInitialize                                           \
    unsigned __int64 RMask64, GMask64, BMask64;                             \
    unsigned __int64 Alpha64, InvAlpha64, ColorKey64, RestMask64;           \
    unsigned __int64 RShift64 = TChimeraDrawDirect::RShift;                 \
    unsigned __int64 GShift64 = TChimeraDrawDirect::GShift;                 \
    unsigned __int64 BShift64 = TChimeraDrawDirect::BShift;                 \
                                                                            \
    unsigned char *dpix = Param.PtrDest;                                    \
    unsigned char *spix = Param.PtrSrc;                                     \
    unsigned long Width = Param.Width / 4;                                  \
    unsigned long Rest  = Param.Width % 4;                                  \
    unsigned long Alpha = Param.Param1;                                     \
    unsigned long InvAlpha = 0xFF - Alpha;                                  \
    unsigned long RMask = TChimeraDrawDirect::RMask;                        \
    unsigned long GMask = TChimeraDrawDirect::GMask;                        \
    unsigned long BMask = TChimeraDrawDirect::BMask;                        \
    unsigned long ColorKey = TChimeraDrawDirect::ColorKey;                  \
                                                                            \
    RestMask64 = 0xFFFFFFFFFFFFFFFFui64;                                    \
    for(unsigned long i = 0; i < Rest; ++i) RestMask64 <<= 16;              \
    Rest *= 2;                                                              \
                                                                            \
    Word2QWord(Alpha, Alpha64);                                             \
    Word2QWord(InvAlpha, InvAlpha64);                                       \
    Word2QWord(RMask, RMask64);                                             \
    Word2QWord(GMask, GMask64);                                             \
    Word2QWord(BMask, BMask64);                                             \
    Word2QWord(ColorKey, ColorKey64);                                       \
//---------------------------------------------------------------------------


// カラーキー適用
#ifdef COLORKEY
#define Blend16MMXColorKeyFunc                                              \
        movq    mm2, ColorKey64         ;                                   \
        pcmpeqw mm2, mm4                ;                                   \
        movq    mm1, mm2                ;                                   \
        pand    mm1, mm5                ;                                   \
        pandn   mm2, mm3                ;                                   \
        por     mm2, mm1                ;                                   \
//---------------------------------------------------------------------------
#else
#define Blend16MMXColorKeyFunc                                              \
        movq    mm2, mm3                ;                                   \
//---------------------------------------------------------------------------
#endif


// 64bit一括処理で転送できなかった分を転送
#define Blend16MMXRestMaskFunc                                              \
        movq    mm3, RestMask64         ;                                   \
        movq    mm1, mm3                ;                                   \
        pand    mm1, mm5                ;                                   \
        pandn   mm3, mm2                ;                                   \
        por     mm3, mm1                ;                                   \
//---------------------------------------------------------------------------


// 加算合成
#define AddBlend16MMXFunc                                                   \
        movq    mm3, mm5                ;                                   \
        movq    mm2, mm4                ;                                   \
        psrlw   mm3, mm0                ;                                   \
        psrlw   mm2, mm0                ;                                   \
        pand    mm3, mm7                ;                                   \
        pand    mm2, mm7                ;                                   \
        paddusw mm3, mm2                ;                                   \
        movq    mm2, mm3                ;                                   \
        pand    mm3, mm6                ;                                   \
        movq    mm1, mm3                ;                                   \
        psrlw   mm1, Shift2             ;                                   \
        psubusw mm3, mm1                ;                                   \
        por     mm3, mm2                ;                                   \
        psllw   mm3, mm0                ;                                   \
//---------------------------------------------------------------------------


// 乗算合成
#define ModBlend16MMXFunc                                                   \
        movq    mm3, mm5                ;                                   \
        movq    mm2, mm4                ;                                   \
        pxor    mm3, XORMask64          ;                                   \
        pxor    mm2, XORMask64          ;                                   \
        psrlw   mm3, mm0                ;                                   \
        psrlw   mm2, mm0                ;                                   \
        pand    mm3, mm7                ;                                   \
        pand    mm2, mm7                ;                                   \
        paddusw mm3, mm2                ;                                   \
        movq    mm2, mm3                ;                                   \
        pand    mm3, mm6                ;                                   \
        movq    mm1, mm3                ;                                   \
        psrlw   mm1, Shift2             ;                                   \
        psubusw mm3, mm1                ;                                   \
        por     mm3, mm2                ;                                   \
        psllw   mm3, mm0                ;                                   \
        pxor    mm3, XORMask64          ;                                   \
//---------------------------------------------------------------------------


// 減算合成
#define SubBlend16MMXFunc                                                   \
        movq    mm3, mm5                ;                                   \
        movq    mm2, mm4                ;                                   \
        pxor    mm3, XORMask64          ;                                   \
        psrlw   mm3, mm0                ;                                   \
        psrlw   mm2, mm0                ;                                   \
        pand    mm3, mm7                ;                                   \
        pand    mm2, mm7                ;                                   \
        paddusw mm3, mm2                ;                                   \
        movq    mm2, mm3                ;                                   \
        pand    mm3, mm6                ;                                   \
        movq    mm1, mm3                ;                                   \
        psrlw   mm1, Shift2             ;                                   \
        psubusw mm3, mm1                ;                                   \
        por     mm3, mm2                ;                                   \
        psllw   mm3, mm0                ;                                   \
        pxor    mm3, XORMask64          ;                                   \
//---------------------------------------------------------------------------


// アルファ値付半透明合成
#define TransBlend16AlphaMMXFunc(Mask, Shift)                               \
        movq    mm0, mm4                ;                                   \
        pand    mm0, Mask               ;                                   \
        psrlw   mm0, Shift              ;                                   \
        movq    mm1, mm5                ;                                   \
        pand    mm1, Mask               ;                                   \
        psrlw   mm1, Shift              ;                                   \
        pmullw  mm0, mm7                ;                                   \
        pmullw  mm1, mm6                ;                                   \
        paddusw mm1, mm0                ;                                   \
        psrlw   mm1, 8                  ;                                   \
        psllw   mm1, Shift              ;                                   \
        por     mm3, mm1                ;                                   \
//---------------------------------------------------------------------------


// アルファ値付加算合成
#define AddBlend16AlphaMMXFunc(Mask, Shift)                                 \
        movq    mm0, mm4                ;                                   \
        pand    mm0, Mask               ;                                   \
        psrlw   mm0, Shift              ;                                   \
        pmullw  mm0, mm7                ;                                   \
        psrlw   mm0, 8                  ;                                   \
        movq    mm1, mm5                ;                                   \
        movq    mm2, Mask               ;                                   \
        pand    mm1, Mask               ;                                   \
        psubusw mm2, mm1                ;                                   \
        psrlw   mm2, Shift              ;                                   \
        psubusw mm2, mm0                ;                                   \
        psllw   mm2, Shift              ;                                   \
        movq    mm1, Mask               ;                                   \
        psubusw mm1, mm2                ;                                   \
        por     mm3, mm1                ;                                   \
//---------------------------------------------------------------------------


// アルファ値付乗算合成合成
#define ModBlend16AlphaMMXFunc(Mask, Shift)                                 \
        movq    mm1, mm4                ;                                   \
        movq    mm0, Mask               ;                                   \
        pand    mm1, Mask               ;                                   \
        psubusw mm0, mm1                ;                                   \
        psrlw   mm0, Shift              ;                                   \
        pmullw  mm0, mm7                ;                                   \
        psrlw   mm0, 8                  ;                                   \
        movq    mm1, mm5                ;                                   \
        pand    mm1, Mask               ;                                   \
        psrlw   mm1, Shift              ;                                   \
        psubusw mm1, mm0                ;                                   \
        psllw   mm1, Shift              ;                                   \
        por     mm3, mm1                ;                                   \
//---------------------------------------------------------------------------


// アルファ値付減算算合成合成
#define SubBlend16AlphaMMXFunc(Mask, Shift)                                 \
        movq    mm0, mm4                ;                                   \
        pand    mm0, Mask               ;                                   \
        psrlw   mm0, Shift              ;                                   \
        pmullw  mm0, mm7                ;                                   \
        psrlw   mm0, 8                  ;                                   \
        movq    mm1, mm5                ;                                   \
        pand    mm1, Mask               ;                                   \
        psrlw   mm1, Shift              ;                                   \
        psubusw mm1, mm0                ;                                   \
        psllw   mm1, Shift              ;                                   \
        por     mm3, mm1                ;                                   \
//---------------------------------------------------------------------------


//***************************************************************************
// 半透明合成
//***************************************************************************
void ChimeraDrawSystem::TransBlend16MMX(const TChimeraDrawDirect &Param)
{
    unsigned __int64 Mask;
    unsigned __int64 ColorKey64, RestMask64;

    unsigned char *dpix = Param.PtrDest;
    unsigned char *spix = Param.PtrSrc;
    unsigned long Width = Param.Width / 4;
    unsigned long Rest  = Param.Width % 4;
    unsigned long ColorKey = TChimeraDrawDirect::ColorKey;

    RestMask64 = 0xFFFFFFFFFFFFFFFFui64;
    for(unsigned long i = 0; i < Rest; ++i) RestMask64 <<= 16;
    Rest *= 2;

    if(TChimeraDrawDirect::GMask == 0x07E0)
        Mask = 0x7BEF7BEF7BEF7BEFui64;
    else
        Mask = 0x3DEF3DEF3DEF3DEFui64;

    Word2QWord(ColorKey, ColorKey64);

    for(unsigned long i = 0; i < Param.Height; ++i)
    {
        asm
        {
            mov     edi, dpix
            mov     esi, spix
            mov     ecx, Width

            movq    mm7, Mask
            movq    mm6, Mask

            cmp     Rest, 0
            je      LOOP_T16_MMX

            movq    mm5, [edi]
            movq    mm4, [esi]

            movq    mm3, mm5
            movq    mm2, mm4
            psrlw   mm3, 1
            psrlw   mm2, 1
            pand    mm3, mm7
            pand    mm2, mm6
            paddusw mm3, mm2

            Blend16MMXColorKeyFunc;
            Blend16MMXRestMaskFunc;

            movq    [edi], mm3
            add     esi, Rest
            add     edi, Rest

            test    ecx, ecx
            jz      LOOP_T16_END_MMX

        LOOP_T16_MMX:
            movq    mm5, [edi]
            movq    mm4, [esi]

            movq    mm3, mm5
            movq    mm2, mm4
            psrlw   mm3, 1
            psrlw   mm2, 1
            pand    mm3, mm7
            pand    mm2, mm6
            paddusw mm3, mm2
            Blend16MMXColorKeyFunc;

            movq    [edi], mm2
            add     esi, 8
            add     edi, 8

            dec     ecx
            jnz     LOOP_T16_MMX
        LOOP_T16_END_MMX:
        }
        dpix += Param.PitchDest;
        spix += Param.PitchSrc;
    }
    asm emms;
}


//***************************************************************************
// 加算合成
//***************************************************************************
void ChimeraDrawSystem::AddBlend16MMX(const TChimeraDrawDirect &Param)
{
    Blend16MMXInitialize;

    for(unsigned long i = 0; i < Param.Height; ++i)
    {
        asm
        {
            mov     edi, dpix
            mov     esi, spix
            mov     ecx, Width

            movq    mm7, Mask1
            movq    mm6, Mask2
            movq    mm0, Shift1

            cmp     Rest, 0
            je      LOOP_A16_MMX

            movq    mm5, [edi]
            movq    mm4, [esi]

            AddBlend16MMXFunc;
            Blend16MMXColorKeyFunc;
            Blend16MMXRestMaskFunc;

            movq    [edi], mm3
            add     esi, Rest
            add     edi, Rest

            test    ecx, ecx
            jz      LOOP_A16_END_MMX

        LOOP_A16_MMX:
            movq    mm5, [edi]
            movq    mm4, [esi]

            AddBlend16MMXFunc;
            Blend16MMXColorKeyFunc;

            movq    [edi], mm2
            add     esi, 8
            add     edi, 8

            dec     ecx
            jnz     LOOP_A16_MMX
        LOOP_A16_END_MMX:
        }
        dpix += Param.PitchDest;
        spix += Param.PitchSrc;
    }
    asm emms;
}


//***************************************************************************
// 乗算合成
//***************************************************************************
void ChimeraDrawSystem::ModBlend16MMX(const TChimeraDrawDirect &Param)
{
    Blend16MMXInitialize;

    for(unsigned long i = 0; i < Param.Height; ++i)
    {
        asm
        {
            mov     edi, dpix
            mov     esi, spix
            mov     ecx, Width

            movq    mm7, Mask1
            movq    mm6, Mask2
            movq    mm0, Shift1

            cmp     Rest, 0
            je      LOOP_M16_MMX

            movq    mm5, [edi]
            movq    mm4, [esi]

            ModBlend16MMXFunc;
            Blend16MMXColorKeyFunc;
            Blend16MMXRestMaskFunc;

            movq    [edi], mm3
            add     esi, Rest
            add     edi, Rest

            test    ecx, ecx
            jz      LOOP_M16_END_MMX

        LOOP_M16_MMX:
            movq    mm5, [edi]
            movq    mm4, [esi]

            ModBlend16MMXFunc;
            Blend16MMXColorKeyFunc;

            movq    [edi], mm2
            add     esi, 8
            add     edi, 8

            dec     ecx
            jnz     LOOP_M16_MMX
        LOOP_M16_END_MMX:
        }
        dpix += Param.PitchDest;
        spix += Param.PitchSrc;
    }
    asm emms;
}


//***************************************************************************
// 減算合成
//***************************************************************************
void ChimeraDrawSystem::SubBlend16MMX(const TChimeraDrawDirect &Param)
{
    Blend16MMXInitialize;

    for(unsigned long i = 0; i < Param.Height; ++i)
    {
        asm
        {
            mov     edi, dpix
            mov     esi, spix
            mov     ecx, Width

            movq    mm7, Mask1
            movq    mm6, Mask2
            movq    mm0, Shift1

            cmp     Rest, 0
            je      LOOP_S16_MMX

            movq    mm5, [edi]
            movq    mm4, [esi]

            SubBlend16MMXFunc;
            Blend16MMXColorKeyFunc;
            Blend16MMXRestMaskFunc;

            movq    [edi], mm3
            add     esi, Rest
            add     edi, Rest

            test    ecx, ecx
            jz      LOOP_S16_END_MMX

        LOOP_S16_MMX:
            movq    mm5, [edi]
            movq    mm4, [esi]

            SubBlend16MMXFunc;
            Blend16MMXColorKeyFunc;

            movq    [edi], mm2
            add     esi, 8
            add     edi, 8

            dec     ecx
            jnz     LOOP_S16_MMX
        LOOP_S16_END_MMX:
        }
        dpix += Param.PitchDest;
        spix += Param.PitchSrc;
    }
    asm emms;
}


//***************************************************************************
// 半透明合成
//***************************************************************************
void ChimeraDrawSystem::TransBlend16AlphaMMX(const TChimeraDrawDirect &Param)
{
    if(Param.Param1 == 0x80)
    { TransBlend16MMX(Param); return; }

    Blend16AlphaMMXInitialize;

    for(unsigned long i = 0; i < Param.Height; ++i)
    {
        asm
        {
            mov     edi, dpix
            mov     esi, spix
            mov     ecx, Width
            movq    mm7, Alpha64
            movq    mm6, InvAlpha64

            cmp     Rest, 0
            je      LOOP_T16_ALPHA_MMX

            movq    mm5, [edi]
            movq    mm4, [esi]
            pxor    mm3, mm3

            TransBlend16AlphaMMXFunc(RMask64, RShift64);
            TransBlend16AlphaMMXFunc(GMask64, GShift64);
            TransBlend16AlphaMMXFunc(BMask64, BShift64);
            Blend16MMXColorKeyFunc;
            Blend16MMXRestMaskFunc;

            movq    [edi], mm3
            add     esi, Rest
            add     edi, Rest

            test    ecx, ecx
            jz      LOOP_T16_END_ALPHA_MMX

        LOOP_T16_ALPHA_MMX:
            movq    mm5, [edi]
            movq    mm4, [esi]
            pxor    mm3, mm3

            TransBlend16AlphaMMXFunc(RMask64, RShift64);
            TransBlend16AlphaMMXFunc(GMask64, GShift64);
            TransBlend16AlphaMMXFunc(BMask64, BShift64);
            Blend16MMXColorKeyFunc;

            movq    [edi], mm2
            add     esi, 8
            add     edi, 8

            dec     ecx
            jnz     LOOP_T16_ALPHA_MMX
        LOOP_T16_END_ALPHA_MMX:
        }
        dpix += Param.PitchDest;
        spix += Param.PitchSrc;
    }
    asm emms;
}


//***************************************************************************
// 加算合成
//***************************************************************************
void ChimeraDrawSystem::AddBlend16AlphaMMX(const TChimeraDrawDirect &Param)
{
    if(Param.Param1 == 0xFF)
    { AddBlend16MMX(Param); return; }

    Blend16AlphaMMXInitialize;

    for(unsigned long i = 0; i < Param.Height; ++i)
    {
        asm
        {
            mov     edi, dpix
            mov     esi, spix
            mov     ecx, Width
            movq    mm7, Alpha64
            movq    mm6, InvAlpha64

            cmp     Rest, 0
            je      LOOP_A16_ALPHA_MMX

            movq    mm5, [edi]
            movq    mm4, [esi]
            pxor    mm3, mm3

            AddBlend16AlphaMMXFunc(RMask64, RShift64);
            AddBlend16AlphaMMXFunc(GMask64, GShift64);
            AddBlend16AlphaMMXFunc(BMask64, BShift64);
            Blend16MMXColorKeyFunc;
            Blend16MMXRestMaskFunc;

            movq    [edi], mm3
            add     esi, Rest
            add     edi, Rest

            test    ecx, ecx
            jz      LOOP_A16_END_ALPHA_MMX

        LOOP_A16_ALPHA_MMX:
            movq    mm5, [edi]
            movq    mm4, [esi]
            pxor    mm3, mm3

            AddBlend16AlphaMMXFunc(RMask64, RShift64);
            AddBlend16AlphaMMXFunc(GMask64, GShift64);
            AddBlend16AlphaMMXFunc(BMask64, BShift64);
            Blend16MMXColorKeyFunc;

            movq    [edi], mm2
            add     esi, 8
            add     edi, 8

            dec     ecx
            jnz     LOOP_A16_ALPHA_MMX
        LOOP_A16_END_ALPHA_MMX:
        }
        dpix += Param.PitchDest;
        spix += Param.PitchSrc;
    }
    asm emms;
}


//***************************************************************************
// 乗算合成
//***************************************************************************
void ChimeraDrawSystem::ModBlend16AlphaMMX(const TChimeraDrawDirect &Param)
{
    if(Param.Param1 == 0xFF)
    { ModBlend16MMX(Param); return; } 

    Blend16AlphaMMXInitialize;

    for(unsigned long i = 0; i < Param.Height; ++i)
    {
        asm
        {
            mov     edi, dpix
            mov     esi, spix
            mov     ecx, Width
            movq    mm7, Alpha64
            movq    mm6, InvAlpha64

            cmp     Rest, 0
            je      LOOP_M16_ALPHA_MMX

            movq    mm5, [edi]
            movq    mm4, [esi]
            pxor    mm3, mm3

            ModBlend16AlphaMMXFunc(RMask64, RShift64);
            ModBlend16AlphaMMXFunc(GMask64, GShift64);
            ModBlend16AlphaMMXFunc(BMask64, BShift64);
            Blend16MMXColorKeyFunc;
            Blend16MMXRestMaskFunc;

            movq    [edi], mm3
            add     esi, Rest
            add     edi, Rest

            test    ecx, ecx
            jz      LOOP_M16_END_ALPHA_MMX

        LOOP_M16_ALPHA_MMX:
            movq    mm5, [edi]
            movq    mm4, [esi]
            pxor    mm3, mm3

            ModBlend16AlphaMMXFunc(RMask64, RShift64);
            ModBlend16AlphaMMXFunc(GMask64, GShift64);
            ModBlend16AlphaMMXFunc(BMask64, BShift64);
            Blend16MMXColorKeyFunc;

            movq    [edi], mm2
            add     esi, 8
            add     edi, 8

            dec     ecx
            jnz     LOOP_M16_ALPHA_MMX
        LOOP_M16_END_ALPHA_MMX:
        }
        dpix += Param.PitchDest;
        spix += Param.PitchSrc;
    }
    asm emms;
}


//***************************************************************************
// 減算合成
//***************************************************************************
void ChimeraDrawSystem::SubBlend16AlphaMMX(const TChimeraDrawDirect &Param)
{
    if(Param.Param1 == 0xFF)
    { SubBlend16MMX(Param); return; } 

    Blend16AlphaMMXInitialize;

    for(unsigned long i = 0; i < Param.Height; ++i)
    {
        asm
        {
            mov     edi, dpix
            mov     esi, spix
            mov     ecx, Width
            movq    mm7, Alpha64
            movq    mm6, InvAlpha64

            cmp     Rest, 0
            je      LOOP_S16_ALPHA_MMX

            movq    mm5, [edi]
            movq    mm4, [esi]
            pxor    mm3, mm3

            SubBlend16AlphaMMXFunc(RMask64, RShift64);
            SubBlend16AlphaMMXFunc(GMask64, GShift64);
            SubBlend16AlphaMMXFunc(BMask64, BShift64);
            Blend16MMXColorKeyFunc;
            Blend16MMXRestMaskFunc;

            movq    [edi], mm3
            add     esi, Rest
            add     edi, Rest

            test    ecx, ecx
            jz      LOOP_S16_END_ALPHA_MMX

        LOOP_S16_ALPHA_MMX:
            movq    mm5, [edi]
            movq    mm4, [esi]
            pxor    mm3, mm3

            SubBlend16AlphaMMXFunc(RMask64, RShift64);
            SubBlend16AlphaMMXFunc(GMask64, GShift64);
            SubBlend16AlphaMMXFunc(BMask64, BShift64);
            Blend16MMXColorKeyFunc;

            movq    [edi], mm2
            add     esi, 8
            add     edi, 8

            dec     ecx
            jnz     LOOP_S16_ALPHA_MMX
        LOOP_S16_END_ALPHA_MMX:
        }
        dpix += Param.PitchDest;
        spix += Param.PitchSrc;
    }
    asm emms;
}










/////////////////////////////////////////////////////////////////////////////
//***************************************************************************
// ChimeraMMXLib32
// 32bit　ＯＮＬＹ！
//***************************************************************************

/////////////////////////////////////////////////////////////////////////////
//***************************************************************************
// 32bit用αブレンド
//***************************************************************************

#define DWord2QWord(DValue, QValue)                                         \
    asm                                                                     \
    {                                                                       \
        movd        mm0, DValue         ;                                   \
        punpckldq   mm0, mm0            ;                                   \
        movq        QValue, mm0         ;                                   \
    }                                                                       \
//---------------------------------------------------------------------------

// 合成処理初期化＆変数定義
#define Blend32MMXInitialize                                                \
    unsigned __int64 ColorKey64, RestMask64;                                \
                                                                            \
    unsigned char *dpix = Param.PtrDest;                                    \
    unsigned char *spix = Param.PtrSrc;                                     \
    unsigned long Width = Param.Width / 2;                                  \
    unsigned long Rest  = Param.Width % 2;                                  \
    unsigned long ColorKey = TChimeraDrawDirect::ColorKey;                  \
                                                                            \
    RestMask64 = 0xFFFFFFFF00000000ui64;                                    \
    Rest *= 4;                                                              \
                                                                            \
    Word2QWord(ColorKey, ColorKey64);                                       \
//---------------------------------------------------------------------------


// アルファブレンド処理初期化＆変数定義
#define Blend32AlphaMMXInitialize                                           \
    unsigned __int64 Alpha64, InvAlpha64, ColorKey64, RestMask64;           \
                                                                            \
    unsigned char *dpix = Param.PtrDest;                                    \
    unsigned char *spix = Param.PtrSrc;                                     \
    unsigned long Width = Param.Width / 2;                                  \
    unsigned long Rest  = Param.Width % 2;                                  \
    unsigned long Alpha = Param.Param1;                                     \
    unsigned long InvAlpha = 0xFF - Alpha;                                  \
    unsigned long ColorKey = TChimeraDrawDirect::ColorKey;                  \
                                                                            \
    RestMask64 = 0xFFFFFFFF00000000ui64;                                    \
    Rest *= 4;                                                              \
                                                                            \
    Word2QWord(Alpha, Alpha64);                                             \
    Word2QWord(InvAlpha, InvAlpha64);                                       \
    DWord2QWord(ColorKey, ColorKey64);                                      \
//---------------------------------------------------------------------------


// カラーキー適用
#ifdef COLORKEY
#define Blend32MMXColorKeyFunc                                              \
        movq    mm2, ColorKey64         ;                                   \
        pcmpeqd mm2, mm4                ;                                   \
        movq    mm1, mm2                ;                                   \
        pand    mm1, mm5                ;                                   \
        pandn   mm2, mm3                ;                                   \
        por     mm2, mm1                ;                                   \
//---------------------------------------------------------------------------
#else
#define Blend32MMXColorKeyFunc                                              \
        movq    mm2, mm3                ;                                   \
//---------------------------------------------------------------------------
#endif


// 64bit一括処理で転送できなかった分を転送
#define Blend32MMXRestMaskFunc                                              \
        movq    mm3, RestMask64         ;                                   \
        movq    mm1, mm3                ;                                   \
        pand    mm1, mm5                ;                                   \
        pandn   mm3, mm2                ;                                   \
        por     mm3, mm1                ;                                   \
//---------------------------------------------------------------------------


// 半透明合成
#define TransBlend32MMXFunc                                                 \
        movq    mm3, mm5                ;                                   \
        movq    mm2, mm4                ;                                   \
        psrlq   mm3, 1                  ;                                   \
        psrlq   mm2, 1                  ;                                   \
        pand    mm3, Mask               ;                                   \
        pand    mm2, Mask               ;                                   \
        paddusb mm3, mm2                ;                                   \
//---------------------------------------------------------------------------


// 加算合成
#define AddBlend32MMXFunc                                                   \
        movq    mm3, mm5                ;                                   \
        paddusb mm3, mm4                ;                                   \
//---------------------------------------------------------------------------


// 乗算合成
#define ModBlend32MMXFunc                                                   \
        movq    mm3, mm5                ;                                   \
        movq    mm2, mm4                ;                                   \
        pxor    mm2, XORMask64          ;                                   \
        psubusb mm3, mm2                                                    \
//---------------------------------------------------------------------------


// 減算合成
#define SubBlend32MMXFunc                                                   \
        movq    mm3, mm5                ;                                   \
        psubusb mm3, mm4                ;                                   \
//---------------------------------------------------------------------------


// アルファ値付半透明合成
#define TransBlend32AlphaMMXFunc                                            \
        movq    mm3, mm5                ;                                   \
        pxor    mm2, mm2                ;                                   \
        movq    mm1, mm5                ;                                   \
        punpcklbw   mm3, mm2            ;                                   \
        punpckhbw   mm1, mm2            ;                                   \
        pmullw  mm3, mm6                ;                                   \
        psrlw   mm3, 8                  ;                                   \
        pmullw  mm1, mm6                ;                                   \
        psrlw   mm1, 8                  ;                                   \
        packuswb    mm3, mm1            ;                                   \
        movq    mm0, mm4                ;                                   \
        movq    mm1, mm4                ;                                   \
        punpcklbw   mm0, mm2            ;                                   \
        punpckhbw   mm1, mm2            ;                                   \
        pmullw  mm0, mm7                ;                                   \
        psrlw   mm0, 8                  ;                                   \
        pmullw  mm1, mm7                ;                                   \
        psrlw   mm1, 8                  ;                                   \
        packuswb    mm0, mm1            ;                                   \
        paddusb mm3, mm0                ;                                   \
//---------------------------------------------------------------------------


// アルファ値付加算合成
#define AddBlend32AlphaMMXFunc                                              \
        movq    mm3, mm5                ;                                   \
        movq    mm0, mm4                ;                                   \
        pxor    mm2, mm2                ;                                   \
        movq    mm1, mm4                ;                                   \
        punpcklbw   mm0, mm2            ;                                   \
        punpckhbw   mm1, mm2            ;                                   \
        pmullw  mm0, mm7                ;                                   \
        psrlw   mm0, 8                  ;                                   \
        pmullw  mm1, mm7                ;                                   \
        psrlw   mm1, 8                  ;                                   \
        packuswb    mm0, mm1            ;                                   \
        paddusb mm3, mm0                ;                                   \
//---------------------------------------------------------------------------


// アルファ値付乗算合成合成
#define ModBlend32AlphaMMXFunc                                              \
        movq    mm3, mm5                ;                                   \
        movq    mm0, mm4                ;                                   \
        pxor    mm2, mm2                ;                                   \
        pxor    mm0, XORMask64          ;                                   \
        movq    mm1, mm0                ;                                   \
        punpcklbw   mm0, mm2            ;                                   \
        punpckhbw   mm1, mm2            ;                                   \
        pmullw  mm0, mm7                ;                                   \
        psrlw   mm0, 8                  ;                                   \
        pmullw  mm1, mm7                ;                                   \
        psrlw   mm1, 8                  ;                                   \
        packuswb    mm0, mm1            ;                                   \
        psubusb mm3, mm0                ;                                   \
//---------------------------------------------------------------------------


// アルファ値付減算算合成合成
#define SubBlend32AlphaMMXFunc                                              \
        movq    mm3, mm5                ;                                   \
        movq    mm0, mm4                ;                                   \
        pxor    mm2, mm2                ;                                   \
        movq    mm1, mm4                ;                                   \
        punpcklbw   mm0, mm2            ;                                   \
        punpckhbw   mm1, mm2            ;                                   \
        pmullw  mm0, mm7                ;                                   \
        psrlw   mm0, 8                  ;                                   \
        pmullw  mm1, mm7                ;                                   \
        psrlw   mm1, 8                  ;                                   \
        packuswb    mm0, mm1            ;                                   \
        psubusb mm3, mm0                ;                                   \
//---------------------------------------------------------------------------


//***************************************************************************
// 半透明合成
//***************************************************************************
void ChimeraDrawSystem::TransBlend32MMX(const TChimeraDrawDirect &Param)
{
    Blend32MMXInitialize;
    unsigned __int64 Mask = 0x7F7F7F7F7F7F7F7Fui64;

    for(unsigned long i = 0; i < Param.Height; ++i)
    {
        asm
        {
            mov     edi, dpix
            mov     esi, spix
            mov     ecx, Width

            cmp     Rest, 0
            je      LOOP_T32_MMX

            movq    mm5, [edi]
            movq    mm4, [esi]

            TransBlend32MMXFunc;
            Blend32MMXColorKeyFunc;
            Blend32MMXRestMaskFunc;

            movq    [edi], mm3
            add     esi, Rest
            add     edi, Rest

            test    ecx, ecx
            jz      LOOP_T32_END_MMX

        LOOP_T32_MMX:
            movq    mm5, [edi]
            movq    mm4, [esi]

            TransBlend32MMXFunc;
            Blend32MMXColorKeyFunc;

            movq    [edi], mm2
            add     esi, 8
            add     edi, 8

            dec     ecx
            jnz     LOOP_T32_MMX
        LOOP_T32_END_MMX:
        }
        dpix += Param.PitchDest;
        spix += Param.PitchSrc;
    }
    asm emms;
}


//***************************************************************************
// 加算合成
//***************************************************************************
void ChimeraDrawSystem::AddBlend32MMX(const TChimeraDrawDirect &Param)
{
    Blend32MMXInitialize;

    for(unsigned long i = 0; i < Param.Height; ++i)
    {
        asm
        {
            mov     edi, dpix
            mov     esi, spix
            mov     ecx, Width

            cmp     Rest, 0
            je      LOOP_A32_MMX

            movq    mm5, [edi]
            movq    mm4, [esi]

            AddBlend32MMXFunc;
            Blend32MMXColorKeyFunc;
            Blend32MMXRestMaskFunc;

            movq    [edi], mm3
            add     esi, Rest
            add     edi, Rest

            test    ecx, ecx
            jz      LOOP_A32_END_MMX

        LOOP_A32_MMX:
            movq    mm5, [edi]
            movq    mm4, [esi]

            AddBlend32MMXFunc;
            Blend32MMXColorKeyFunc;

            movq    [edi], mm2
            add     esi, 8
            add     edi, 8

            dec     ecx
            jnz     LOOP_A32_MMX
        LOOP_A32_END_MMX:
        }
        dpix += Param.PitchDest;
        spix += Param.PitchSrc;
    }
    asm emms;
}


//***************************************************************************
// 乗算合成
//***************************************************************************
void ChimeraDrawSystem::ModBlend32MMX(const TChimeraDrawDirect &Param)
{
    Blend32MMXInitialize;
    unsigned __int64 XORMask64 = 0xFFFFFFFFFFFFFFFFui64;

    for(unsigned long i = 0; i < Param.Height; ++i)
    {
        asm
        {
            mov     edi, dpix
            mov     esi, spix
            mov     ecx, Width

            cmp     Rest, 0
            je      LOOP_M32_MMX

            movq    mm5, [edi]
            movq    mm4, [esi]

            ModBlend32MMXFunc;
            Blend32MMXColorKeyFunc;
            Blend32MMXRestMaskFunc;

            movq    [edi], mm3
            add     esi, Rest
            add     edi, Rest

            test    ecx, ecx
            jz      LOOP_M32_END_MMX

        LOOP_M32_MMX:
            movq    mm5, [edi]
            movq    mm4, [esi]

            ModBlend32MMXFunc;
            Blend32MMXColorKeyFunc;

            movq    [edi], mm2
            add     esi, 8
            add     edi, 8

            dec     ecx
            jnz     LOOP_M32_MMX
        LOOP_M32_END_MMX:
        }
        dpix += Param.PitchDest;
        spix += Param.PitchSrc;
    }
    asm emms;
}


//***************************************************************************
// 減算合成
//***************************************************************************
void ChimeraDrawSystem::SubBlend32MMX(const TChimeraDrawDirect &Param)
{
    Blend32MMXInitialize;

    for(unsigned long i = 0; i < Param.Height; ++i)
    {
        asm
        {
            mov     edi, dpix
            mov     esi, spix
            mov     ecx, Width

            cmp     Rest, 0
            je      LOOP_S32_MMX

            movq    mm5, [edi]
            movq    mm4, [esi]

            SubBlend32MMXFunc;
            Blend32MMXColorKeyFunc;
            Blend32MMXRestMaskFunc;

            movq    [edi], mm3
            add     esi, Rest
            add     edi, Rest

            test    ecx, ecx
            jz      LOOP_S32_END_MMX

        LOOP_S32_MMX:
            movq    mm5, [edi]
            movq    mm4, [esi]

            SubBlend32MMXFunc;
            Blend32MMXColorKeyFunc;

            movq    [edi], mm2
            add     esi, 8
            add     edi, 8

            dec     ecx
            jnz     LOOP_S32_MMX
        LOOP_S32_END_MMX:
        }
        dpix += Param.PitchDest;
        spix += Param.PitchSrc;
    }
    asm emms;
}


//***************************************************************************
// 半透明合成
//***************************************************************************
void ChimeraDrawSystem::TransBlend32AlphaMMX(const TChimeraDrawDirect &Param)
{
    if(Param.Param1 == 0x80)
    { TransBlend32MMX(Param); return; }

    Blend32AlphaMMXInitialize;

    for(unsigned long i = 0; i < Param.Height; ++i)
    {
        asm
        {
            mov     edi, dpix
            mov     esi, spix
            mov     ecx, Width
            movq    mm7, Alpha64
            movq    mm6, InvAlpha64

            cmp     Rest, 0
            je      LOOP_T32_ALPHA_MMX

            movq    mm5, [edi]
            movq    mm4, [esi]

            TransBlend32AlphaMMXFunc;
            Blend32MMXColorKeyFunc;
            Blend32MMXRestMaskFunc;

            movq    [edi], mm3
            add     esi, Rest
            add     edi, Rest

            test    ecx, ecx
            jz      LOOP_T32_END_ALPHA_MMX

        LOOP_T32_ALPHA_MMX:
            movq    mm5, [edi]
            movq    mm4, [esi]

            TransBlend32AlphaMMXFunc;
            Blend32MMXColorKeyFunc;

            movq    [edi], mm2
            add     esi, 8
            add     edi, 8

            dec     ecx
            jnz     LOOP_T32_ALPHA_MMX
        LOOP_T32_END_ALPHA_MMX:
        }
        dpix += Param.PitchDest;
        spix += Param.PitchSrc;
    }
    asm emms;
}


//***************************************************************************
// 加算合成
//***************************************************************************
void ChimeraDrawSystem::AddBlend32AlphaMMX(const TChimeraDrawDirect &Param)
{
    if(Param.Param1 == 0xFF)
    { AddBlend32MMX(Param); return; }

    Blend32AlphaMMXInitialize;

    for(unsigned long i = 0; i < Param.Height; ++i)
    {
        asm
        {
            mov     edi, dpix
            mov     esi, spix
            mov     ecx, Width
            movq    mm7, Alpha64
            movq    mm6, InvAlpha64

            cmp     Rest, 0
            je      LOOP_A32_ALPHA_MMX

            movq    mm5, [edi]
            movq    mm4, [esi]

            AddBlend32AlphaMMXFunc;
            Blend32MMXColorKeyFunc;
            Blend32MMXRestMaskFunc;

            movq    [edi], mm3
            add     esi, Rest
            add     edi, Rest

            test    ecx, ecx
            jz      LOOP_A32_END_ALPHA_MMX

        LOOP_A32_ALPHA_MMX:
            movq    mm5, [edi]
            movq    mm4, [esi]

            AddBlend32AlphaMMXFunc;
            Blend32MMXColorKeyFunc;

            movq    [edi], mm2
            add     esi, 8
            add     edi, 8

            dec     ecx
            jnz     LOOP_A32_ALPHA_MMX
        LOOP_A32_END_ALPHA_MMX:
        }
        dpix += Param.PitchDest;
        spix += Param.PitchSrc;
    }
    asm emms;
}


//***************************************************************************
// 乗算合成
//***************************************************************************
void ChimeraDrawSystem::ModBlend32AlphaMMX(const TChimeraDrawDirect &Param)
{
    if(Param.Param1 == 0xFF)
    { ModBlend32MMX(Param); return; } 
    unsigned __int64 XORMask64 = 0xFFFFFFFFFFFFFFFFui64;  

    Blend32AlphaMMXInitialize;

    for(unsigned long i = 0; i < Param.Height; ++i)
    {
        asm
        {
            mov     edi, dpix
            mov     esi, spix
            mov     ecx, Width
            movq    mm7, Alpha64
            movq    mm6, InvAlpha64

            cmp     Rest, 0
            je      LOOP_M32_ALPHA_MMX

            movq    mm5, [edi]
            movq    mm4, [esi]

            ModBlend32AlphaMMXFunc;
            Blend32MMXColorKeyFunc;
            Blend32MMXRestMaskFunc;

            movq    [edi], mm3
            add     esi, Rest
            add     edi, Rest

            test    ecx, ecx
            jz      LOOP_M32_END_ALPHA_MMX

        LOOP_M32_ALPHA_MMX:
            movq    mm5, [edi]
            movq    mm4, [esi]

            ModBlend32AlphaMMXFunc;
            Blend32MMXColorKeyFunc;

            movq    [edi], mm2
            add     esi, 8
            add     edi, 8

            dec     ecx
            jnz     LOOP_M32_ALPHA_MMX
        LOOP_M32_END_ALPHA_MMX:
        }
        dpix += Param.PitchDest;
        spix += Param.PitchSrc;
    }
    asm emms;
}


//***************************************************************************
// 減算合成
//***************************************************************************
void ChimeraDrawSystem::SubBlend32AlphaMMX(const TChimeraDrawDirect &Param)
{
    if(Param.Param1 == 0xFF)
    { SubBlend32MMX(Param); return; } 

    Blend32AlphaMMXInitialize;

    for(unsigned long i = 0; i < Param.Height; ++i)
    {
        asm
        {
            mov     edi, dpix
            mov     esi, spix
            mov     ecx, Width
            movq    mm7, Alpha64
            movq    mm6, InvAlpha64

            cmp     Rest, 0
            je      LOOP_S32_ALPHA_MMX

            movq    mm5, [edi]
            movq    mm4, [esi]

            SubBlend32AlphaMMXFunc;
            Blend32MMXColorKeyFunc;
            Blend32MMXRestMaskFunc;

            movq    [edi], mm3
            add     esi, Rest
            add     edi, Rest

            test    ecx, ecx
            jz      LOOP_S32_END_ALPHA_MMX

        LOOP_S32_ALPHA_MMX:
            movq    mm5, [edi]
            movq    mm4, [esi]

            SubBlend32AlphaMMXFunc;
            Blend32MMXColorKeyFunc;

            movq    [edi], mm2
            add     esi, 8
            add     edi, 8

            dec     ecx
            jnz     LOOP_S32_ALPHA_MMX
        LOOP_S32_END_ALPHA_MMX:
        }
        dpix += Param.PitchDest;
        spix += Param.PitchSrc;
    }
    asm emms;
}
