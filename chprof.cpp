//---------------------------------------------------------------------------
#pragma inline
#pragma hdrstop
#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <map>
#include <stack>
#include "chprof.h"
//---------------------------------------------------------------------------
using namespace std;
#pragma package(smart_init)
//---------------------------------------------------------------------------
namespace ChimeraSystem
{
    struct TChimeraLargeInteger
    {
        union {
            unsigned __int64 QWord;
            unsigned __int32 DWord[2];
        };

        TChimeraLargeInteger(void)
            { ; }
        TChimeraLargeInteger(unsigned __int32 Low, unsigned __int32 High)
            { DWord[0] = Low; DWord[1] = High; }
    };

    struct TChimeraProfileResult
    {
        unsigned int Count;
        unsigned __int64 Time;
        TChimeraProfileResult(void)
            : Count(0), Time(0)
            { ; }
    };
};
//---------------------------------------------------------------------------

/////////////////////////////////////////////////////////////////////////////
//***************************************************************************
// クラス名  : TChimeraProfiler
// 内容      : プロファイリングクラス
// Version   : 0.5 (2000/11/07)
//***************************************************************************

//***************************************************************************
// コンストラクタ
//***************************************************************************
TChimeraProfiler::TChimeraProfiler(ostream *os)
    : OutPutStrm(os ? *os : cout)
{
    ResultMap = new TChimeraProfileResultMap();
    TimeStack = new TChimeraProfileTimeStack();
}


//***************************************************************************
// デストラクタ
//***************************************************************************
TChimeraProfiler::~TChimeraProfiler(void)
{
    PrintOut();
    delete ResultMap;
    delete TimeStack;
}





/////////////////////////////////////////////////////////////////////////////
//***************************************************************************
// 実行時間計測関係
//***************************************************************************

//***************************************************************************
// 実行時間取得開始
//***************************************************************************
void TChimeraProfiler::Start(void)
{
    unsigned __int32 STLow, STHigh;

    // CPUクロックカウンタの取得
    asm
    {
        db      0x0F, 0x31
        mov     STHigh, edx
        mov     STLow, eax
    }

    // 開始時刻をスタックにプッシュ
    (*TimeStack).push(TChimeraLargeInteger(STLow, STHigh));
}


//***************************************************************************
// 実行時間取得終了
//***************************************************************************
void TChimeraProfiler::End(const char *AFunctionName)
{
    unsigned __int32 ETLow, ETHigh;
    TChimeraLargeInteger EndTime;
    TChimeraProfileResult *Result;

    // CPUクロックカウンタの取得
    asm
    {
        db      0x0F, 0x31
        mov     ETHigh, edx
        mov     ETLow, eax
    }
    EndTime.DWord[0] = ETLow;
    EndTime.DWord[1] = ETHigh;

    Result = &(*ResultMap)[AFunctionName];
    Result->Count++;
    Result->Time += EndTime.QWord - (*TimeStack).top().QWord;
    (*TimeStack).pop();
}


//***************************************************************************
// トレース
//***************************************************************************
void TChimeraProfiler::Trace(const char *AMessage)
{
    OutPutStrm << AMessage << endl;
    OutPutStrm.flush();
}






/////////////////////////////////////////////////////////////////////////////
//***************************************************************************
// 結果出力関係
//***************************************************************************

//***************************************************************************
// プロファイル情報の出力
//***************************************************************************
void TChimeraProfiler::PrintOut(void)
{
    TChimeraProfileResultMap::iterator pitr;    // マップアクセス反復子

    OutPutStrm << "// ==========================================================================" << endl;
    OutPutStrm << "// プロファイリング結果出力                                                  " << endl;
    OutPutStrm << "//                                                Created by TChimeraProfiler" << endl;
    OutPutStrm << "//                                                      時間単位はCPUクロック" << endl;
    OutPutStrm << "// ==========================================================================" << endl;
    OutPutStrm << endl;

    OutPutStrm << "回数       全実行時間          平均           関数名                         " << endl;

    // プロファイル情報の出力
    pitr = (*ResultMap).begin();
    while(pitr != (*ResultMap).end())
    {
        const char* const FunctionName = (*pitr).first;
        const TChimeraProfileResult &Result = (*pitr).second;

        OutPutStrm << right
            << setw(10) << Result.Count
            << setw(20) << Result.Time
            << setw(15) << Result.Time / Result.Count
            << left << " " << FunctionName << endl;

        pitr++;
    }
}
