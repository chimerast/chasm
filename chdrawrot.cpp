//---------------------------------------------------------------------------
#pragma inline
//---------------------------------------------------------------------------
#pragma hdrstop
#include <math.h>
#include "chdrawdirect.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
#define min(A, B) (A < B ? A : B)
//---------------------------------------------------------------------------

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
            test    ax, ax
            jz      LOOPCMP_ROT16
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
            test    eax, eax
            jz      LOOPCMP_ROT24
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
            test    eax, eax
            jz      LOOPCMP_ROT32
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
            test        ax, ax
            jz          LOOPCMP_ROT16MMX
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
