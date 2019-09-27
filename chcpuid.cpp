//---------------------------------------------------------------------------
#pragma option -r-
#pragma inline
#pragma hdrstop
#include <memory>
#include <string>
#include <iostream>
#include <mmsystem.h>
#include "chcpuid.h"
//---------------------------------------------------------------------------
using namespace std;
#pragma package(smart_init)
//---------------------------------------------------------------------------

//***************************************************************************
// 無名ネームスペース
//***************************************************************************
namespace
{
    void CheckCPUIDFunc(TCPUID &CPUFeature);
    void CheckCPUIDExtFunc(TCPUID &CPUFeature);
    void CPUIDFunction(TCPUID &CPUFeature);
    void CPUIDExtFunction(TCPUID &CPUFeature);
    void TestCPUAbility(TCPUID &CPUFeature);
    void TestCPUProduct(TCPUID &CPUFeature);
};




/////////////////////////////////////////////////////////////////////////////
//***************************************************************************
// 内容      : CPU識別
//***************************************************************************

//***************************************************************************
// CPU情報文字列化
//***************************************************************************
ostream& ChimeraSystem::operator <<(ostream &os, TCPUID &CPUFeature)
{
    os << "// ==========================================================================" << endl;
    os << "// ＣＰＵ情報出力                                                            " << endl;
    os << "//                                                   Created by TChimeraCPUID" << endl;
    os << "// ==========================================================================" << endl;
    os << endl;

    char *Maker;
    switch(CPUFeature.CPUProduct)
    {
        case TCPUID::cpIntel:   Maker = "Intel"; break;
        case TCPUID::cpAMD:     Maker = "AMD";   break;
        case TCPUID::cpCyrix:   Maker = "Cyrix"; break;
        case TCPUID::cpCentaur: Maker = "IDT";   break;
        case TCPUID::cpUnknown: Maker = "不明";  break;
    }

    os.precision(5);

    os << "CPUメーカー                    ：" << Maker << endl;
    os << "CPU周波数                      ：" << CPUFeature.CPUSpeed / 1000000.0f << "MHz" << endl;
    os << endl;

    os << "メーカー識別文字列             ：" << CPUFeature.ProductString     << endl;
    if(CPUFeature.AMDCPUString[0])
    os << "プロセッサ名文字列             ：" << CPUFeature.AMDCPUString      << endl;
    os << endl;

    os << "CPU命令ファミリ                ：" << CPUFeature.CPUFamily << "86" << endl;
    os << "CPUモデル                      ：" << CPUFeature.CPUModel          << endl;
    os << "CPUステッピングID              ：" << CPUFeature.CPUSteppingID     << endl;
    os << endl;

    os << hex;
    os << "最大CPUID関数                  ：" << CPUFeature.MaxCPUIDFunction        << endl;
    os << "最大CPUID拡張関数              ：" << CPUFeature.MaxCPUIDExtFunction     << endl;
    os << dec;
    os << endl;

    os << "CPUID命令サポート              ：" << (CPUFeature.HasCPUID?"ＹＥＳ":"ＮＯ") << endl;
    os << "浮動小数点ユニット             ：" << (CPUFeature.FPU  ?"ＹＥＳ":"ＮＯ") << endl;
    os << "仮想8086モード                 ：" << (CPUFeature.VME  ?"ＹＥＳ":"ＮＯ") << endl;
    os << "デバッグ拡張                   ：" << (CPUFeature.DE   ?"ＹＥＳ":"ＮＯ") << endl;
    os << "ページサイズ拡張               ：" << (CPUFeature.PSE  ?"ＹＥＳ":"ＮＯ") << endl;
    os << "タイムスタンプカウンタ         ：" << (CPUFeature.TSC  ?"ＹＥＳ":"ＮＯ") << endl;
    os << "モデル固有レジスタ             ：" << (CPUFeature.MSR  ?"ＹＥＳ":"ＮＯ") << endl;
    os << "物理アドレス拡張               ：" << (CPUFeature.PAE  ?"ＹＥＳ":"ＮＯ") << endl;
    os << "マシンチェック例外             ：" << (CPUFeature.MCE  ?"ＹＥＳ":"ＮＯ") << endl;
    os << "CMPXCHG8B命令                  ：" << (CPUFeature.CX8  ?"ＹＥＳ":"ＮＯ") << endl;
    os << "オンチップAPIC                 ：" << (CPUFeature.APIC ?"ＹＥＳ":"ＮＯ") << endl;
    os << "高速システムコール             ：" << (CPUFeature.SEP  ?"ＹＥＳ":"ＮＯ") << endl;
    os << "メモリタイプ範囲レジスタ       ：" << (CPUFeature.MTRR ?"ＹＥＳ":"ＮＯ") << endl;
    os << "PTEグローバルフラグ            ：" << (CPUFeature.PGE  ?"ＹＥＳ":"ＮＯ") << endl;
    os << "マシンチェックアーキテクチャ   ：" << (CPUFeature.MCA  ?"ＹＥＳ":"ＮＯ") << endl;
    os << "条件付転送および比較命令       ：" << (CPUFeature.CMOV ?"ＹＥＳ":"ＮＯ") << endl;
    os << "ページ属性テーブル             ：" << (CPUFeature.PAT  ?"ＹＥＳ":"ＮＯ") << endl;
    os << "36ビットページサイズ拡張       ：" << (CPUFeature.PSE36?"ＹＥＳ":"ＮＯ") << endl;
    os << "プロセッサ番号                 ：" << (CPUFeature.PSN  ?"ＹＥＳ":"ＮＯ") << endl;
    os << "CLFLUSH命令                    ：" << (CPUFeature.CLFLUSH?"ＹＥＳ":"ＮＯ") << endl;
    os << "デバッグトレースストア         ：" << (CPUFeature.DTS  ?"ＹＥＳ":"ＮＯ") << endl;
    os << "CPUACPIサポート                ：" << (CPUFeature.ACPI ?"ＹＥＳ":"ＮＯ") << endl;
    os << "MMXテクノロジ                  ：" << (CPUFeature.MMX  ?"ＹＥＳ":"ＮＯ") << endl;
    os << "FPU/MMX/SSE命令の高速保存&復元 ：" << (CPUFeature.FXSR ?"ＹＥＳ":"ＮＯ") << endl;
    os << "SSEテクノロジ                  ：" << (CPUFeature.SSE  ?"ＹＥＳ":"ＮＯ") << endl;
    os << "SSE2テクノロジ                 ：" << (CPUFeature.SSE2 ?"ＹＥＳ":"ＮＯ") << endl;
    os << "Self-Snoop                     ：" << (CPUFeature.SS   ?"ＹＥＳ":"ＮＯ") << endl;
    os << "温度モニタサポート             ：" << (CPUFeature.TM   ?"ＹＥＳ":"ＮＯ") << endl;

    os << "拡張MMXテクノロジ              ：" << (CPUFeature.AMDEMMX?"ＹＥＳ":"ＮＯ") <<endl;
    os << "3DNow!テクノロジ               ：" << (CPUFeature.AMD3DNOW?"ＹＥＳ":"ＮＯ") <<endl;
    os << "拡張3DNow!テクノロジ           ：" << (CPUFeature.AMDE3DNOW?"ＹＥＳ":"ＮＯ") <<endl;
    return os;
}


//***************************************************************************
// CPU能力チェック
//***************************************************************************
void ChimeraSystem::CallCPUID(TCPUID &CPUFeature)
{
    memset(&CPUFeature, 0, sizeof(TCPUID));

    CheckCPUIDFunc(CPUFeature);

    // CPUID命令が使えれば呼び出す
    if(CPUFeature.HasCPUID)
    {
        CPUIDFunction(CPUFeature);
        CheckCPUIDExtFunc(CPUFeature);

        // CPUID拡張関数を使えれば呼び出す
        if(CPUFeature.MaxCPUIDExtFunction >= 0x80000001)
            CPUIDExtFunction(CPUFeature);
        else
            CPUFeature.MaxCPUIDExtFunction = 0;

        TestCPUAbility(CPUFeature);
        TestCPUProduct(CPUFeature);
    }
}


//***************************************************************************
// CPU速度チェック
//***************************************************************************
void ChimeraSystem::TestCPUSpeed(TCPUID &CPUFeature)
{
    if(!CPUFeature.TSC) return;

    unsigned long Counter;
    unsigned long STHigh, STLow, ETHigh, ETLow;
    unsigned __int64 StartTime, EndTime;

    SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_TIME_CRITICAL);

    Counter = timeGetTime();

    asm rdtsc
    asm mov STHigh, edx
    asm mov STLow, eax

    while(timeGetTime() < Counter + 200);

    asm rdtsc
    asm mov ETHigh, edx
    asm mov ETLow, eax
    StartTime = ((unsigned __int64)STHigh << 32) | STLow;
    EndTime   = ((unsigned __int64)ETHigh << 32) | ETLow;

    SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_NORMAL);

    CPUFeature.CPUSpeed = (unsigned long)(EndTime - StartTime) * 5;
}






// 無名ネームスペース
namespace
{

//***************************************************************************
// CPUID命令存在チェック
//***************************************************************************
static void CheckCPUIDFunc(TCPUID &CPUFeature)
{
    unsigned long *Dest = &CPUFeature.HasCPUID;
    asm
    {
        mov     edi, Dest
        pushfd
        pop     eax
        mov     edx, eax
        xor     eax, 0x200000
        push    eax
        popfd
        pushfd
        pop     eax
        xor     eax, edx
        jz      CPUID_NG

    CPUID_OK:   // CPUID命令使用可
        mov     [edi], 1
        jmp     CPUID_END

    CPUID_NG:   // CPUID命令使用不可
        mov     [edi], 0

    CPUID_END:
    }
}


//***************************************************************************
// CPUID拡張関数存在チェック
//***************************************************************************
static void CheckCPUIDExtFunc(TCPUID &CPUFeature)
{
    // CPUID拡張関数チェック
    unsigned long *Dest = &CPUFeature.MaxCPUIDExtFunction;
    asm
    {
        push ebx
        push edi

        mov     edi, Dest
        mov     eax, 0x80000000
        cpuid
        mov     [edi], eax

        pop edi
        pop ebx
    }
}


//***************************************************************************
// CPUID関数呼び出し
//***************************************************************************
static void CPUIDFunction(TCPUID &CPUFeature)
{
    // CPUID関数０呼び出し
    unsigned long *Dest = &CPUFeature.MaxCPUIDFunction;
    unsigned long *Src  = (unsigned long*)&CPUFeature.ProductString;
    asm
    {
        push ebx
        push edi
        push esi

        mov     edi, Dest
        mov     esi, Src
        mov     eax, 0
        cpuid
        mov     [edi], eax
        mov     [esi + 0], ebx
        mov     [esi + 4], edx
        mov     [esi + 8], ecx

        pop esi
        pop edi
        pop ebx
    }

    // CPUID関数１呼び出し
    Dest = &CPUFeature.ProcessorType;
    Src  = &CPUFeature.Features;
    asm
    {
        push ebx
        push edi
        push esi

        mov     edi, Dest
        mov     esi, Src
        mov     eax, 1
        cpuid
        mov     [edi], eax
        mov     [esi], edx

        pop esi
        pop edi
        pop ebx
    }
}


//***************************************************************************
// CPUID拡張関数呼び出し
//***************************************************************************
static void CPUIDExtFunction(TCPUID &CPUFeature)
{
    // CPUID拡張関数１呼び出し
    unsigned long *Dest = &CPUFeature.ExtFeatures;
    asm
    {
        push ebx
        push edi

        mov     edi, Dest
        mov     eax, 0x80000001
        cpuid
        mov     [edi], edx

        pop edi
        pop ebx
    }
    if(CPUFeature.MaxCPUIDExtFunction == 0x80000001) return;

    // CPUID拡張関数２呼び出し
    Dest = (unsigned long*)&CPUFeature.AMDCPUString;
    asm
    {
        push ebx
        push edi

        mov     edi, Dest
        mov eax, 0x80000002
        cpuid
        mov [edi + 0], eax
        mov [edi + 4], ebx
        mov [edi + 8], ecx
        mov [edi + 12], edx

        mov eax, 0x80000003
        cpuid
        mov [edi + 16], eax
        mov [edi + 20], ebx
        mov [edi + 24], ecx
        mov [edi + 28], edx

        mov eax, 0x80000004
        cpuid
        mov [edi + 32], eax
        mov [edi + 36], ebx
        mov [edi + 40], ecx
        mov [edi + 44], edx

        pop edi
        pop ebx
    }
}


//***************************************************************************
// CPU能力チェック
//***************************************************************************
static void TestCPUAbility(TCPUID &CPUFeature)
{
    CPUFeature.CPUFamily     = (CPUFeature.ProcessorType >> 8) & 0x0F;
    CPUFeature.CPUModel      = (CPUFeature.ProcessorType >> 4) & 0x0F;
    CPUFeature.CPUSteppingID = (CPUFeature.ProcessorType >> 0) & 0x0F;


    CPUFeature.FPU      = CPUFeature.Features & (1 <<  0) ? true : false;
    CPUFeature.VME      = CPUFeature.Features & (1 <<  1) ? true : false;
    CPUFeature.DE       = CPUFeature.Features & (1 <<  2) ? true : false;
    CPUFeature.PSE      = CPUFeature.Features & (1 <<  3) ? true : false;
    CPUFeature.TSC      = CPUFeature.Features & (1 <<  4) ? true : false;
    CPUFeature.MSR      = CPUFeature.Features & (1 <<  5) ? true : false;
    CPUFeature.PAE      = CPUFeature.Features & (1 <<  6) ? true : false;
    CPUFeature.MCE      = CPUFeature.Features & (1 <<  7) ? true : false;
    CPUFeature.CX8      = CPUFeature.Features & (1 <<  8) ? true : false;
    CPUFeature.APIC     = CPUFeature.Features & (1 <<  9) ? true : false;
    CPUFeature.SEP      = CPUFeature.Features & (1 << 11) ? true : false;
    CPUFeature.MTRR     = CPUFeature.Features & (1 << 12) ? true : false;
    CPUFeature.PGE      = CPUFeature.Features & (1 << 13) ? true : false;
    CPUFeature.MCA      = CPUFeature.Features & (1 << 14) ? true : false;
    CPUFeature.CMOV     = CPUFeature.Features & (1 << 15) ? true : false;
    CPUFeature.PAT      = CPUFeature.Features & (1 << 16) ? true : false;
    CPUFeature.PSE36    = CPUFeature.Features & (1 << 17) ? true : false;
    CPUFeature.PSN      = CPUFeature.Features & (1 << 18) ? true : false;
    CPUFeature.CLFLUSH  = CPUFeature.Features & (1 << 19) ? true : false;
    CPUFeature.DTS      = CPUFeature.Features & (1 << 21) ? true : false;
    CPUFeature.ACPI     = CPUFeature.Features & (1 << 22) ? true : false;
    CPUFeature.MMX      = CPUFeature.Features & (1 << 23) ? true : false;
    CPUFeature.FXSR     = CPUFeature.Features & (1 << 24) ? true : false;
    CPUFeature.SSE      = CPUFeature.Features & (1 << 25) ? true : false;
    CPUFeature.SSE2     = CPUFeature.Features & (1 << 26) ? true : false;
    CPUFeature.SS       = CPUFeature.Features & (1 << 27) ? true : false;
    CPUFeature.TM       = CPUFeature.Features & (1 << 29) ? true : false;

    CPUFeature.AMDEMMX  = CPUFeature.ExtFeatures & (1 << 22) ? true : false;
    CPUFeature.AMD3DNOW = CPUFeature.ExtFeatures & (1 << 31) ? true : false;
    CPUFeature.AMDE3DNOW= CPUFeature.ExtFeatures & (1 << 30) ? true : false;
}


//***************************************************************************
// 生産メーカーチェック
//***************************************************************************
static void TestCPUProduct(TCPUID &CPUFeature)
{
         if(strncmp(CPUFeature.ProductString, "GenuineIntel", 12) == 0)
        CPUFeature.CPUProduct = TCPUID::cpIntel;
    else if(strncmp(CPUFeature.ProductString, "AuthenticAMD", 12) == 0)
        CPUFeature.CPUProduct = TCPUID::cpAMD;
    else if(strncmp(CPUFeature.ProductString, "CyrixInstead", 12) == 0)
        CPUFeature.CPUProduct = TCPUID::cpCyrix;
    else if(strncmp(CPUFeature.ProductString, "CentaurHauls", 12) == 0)
        CPUFeature.CPUProduct = TCPUID::cpCentaur;
    else
        CPUFeature.CPUProduct = TCPUID::cpUnknown;
}

};
