//---------------------------------------------------------------------------
#ifndef chprofH
#define chprofH
//---------------------------------------------------------------------------
#include "cppfwd.h"
//---------------------------------------------------------------------------
namespace ChimeraSystem
{
    class TChimeraProfiler;
    class TChimeraFunctionProfile;
    struct TChimeraLargeInteger;
    struct TChimeraProfileResult;

    // TChimeraProfile =============================
    class __declspec(package) TChimeraProfiler
    {
        typedef std::map<const char*, TChimeraProfileResult,
                std::less<const char*>,
                std::allocator<std::pair<const char* const, TChimeraProfileResult> >
            > TChimeraProfileResultMap;
        typedef std::stack<TChimeraLargeInteger,
                std::deque<TChimeraLargeInteger, std::allocator<TChimeraLargeInteger> >
            > TChimeraProfileTimeStack;

    private:
        std::ostream &OutPutStrm;           // 出力先
        TChimeraProfileResultMap *ResultMap;// 関数結果マップ
        TChimeraProfileTimeStack *TimeStack;// タイム計測

    protected:
        friend class TChimeraFunctionProfile;

        void Start(void);
        void End(const char *AFunctionName);
        virtual void PrintOut(void);

    public:
        void Trace(const char *AMessage);

        TChimeraProfiler(std::ostream *os);
        ~TChimeraProfiler(void);
    };


    // TChimeraFunctionProfile =====================
    class __declspec(package) TChimeraFunctionProfile
    {
    private:
        const char *FunctionName;           // 関数名
        TChimeraProfiler *Profiler;         // プロファイラ

    public:
        TChimeraFunctionProfile(TChimeraProfiler *AProfiler,
            const char *AFunctionName)
            : Profiler(AProfiler), FunctionName(AFunctionName)
            { if(Profiler) Profiler->Start(); }
        ~TChimeraFunctionProfile(void)
            { if(Profiler) Profiler->End(FunctionName); }
    };
};
//---------------------------------------------------------------------------
using namespace ChimeraSystem;
//---------------------------------------------------------------------------
#endif
