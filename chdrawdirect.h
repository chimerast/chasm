//---------------------------------------------------------------------------
#ifndef chdrawdirectH
#define chdrawdirectH
//---------------------------------------------------------------------------
namespace ChimeraDrawSystem
{
    struct TChimeraDrawDirect {
        static bool HasMMX;                 // MMX使用可能
        static unsigned long ColorBitSize;          // ビットサイズ
        static unsigned long RMask, GMask, BMask;   // ビットマスク
        static unsigned long WMask, RGBMask;        // ビットマスク
        static unsigned long RShift, GShift, BShift;// ビットシフト
        static unsigned long RCount, GCount, BCount;// ビット数
        static unsigned long ColorKey;      // カラーキー
        static unsigned long ScreenWidth, ScreenHeight; // スクリーンサイズ

        static long DSin[65536], DCos[65536];

        unsigned char *PtrDest;             // 転送先ポインタ
        unsigned char *PtrSrc;              // 転送元ポインタ
        unsigned char *PtrAlpha;            // アルファポインタ
        long PitchDest;                     // 転送先ピッチ
        long PitchSrc;                      // 転送元ピッチ
        long PitchAlpha;                    // アルファピッチ
        unsigned long Width;                // 転送サイズ
        unsigned long Height;               // 転送サイズ
        long X;                             // 転送開始座標
        long Y;                             // 転送開始座標
        long Param1;                        // アルファ値
        long Param2;

        static void Init(unsigned long ARMask, unsigned long AGMask,
            unsigned long ABMask, unsigned long AColorBitSize,
            unsigned long AColorKey,
            unsigned long AScreenWidth, unsigned long AScreenHeight);
    };

    void RotDraw16(const TChimeraDrawDirect &Param);
    void RotDraw24(const TChimeraDrawDirect &Param);
    void RotDraw32(const TChimeraDrawDirect &Param);
    void RotDraw16MMX(const TChimeraDrawDirect &Param);

    void AddDraw(const TChimeraDrawDirect &Param);
    void ModDraw(const TChimeraDrawDirect &Param);

    void TransBlend(const TChimeraDrawDirect &Param);
    void AddBlend(const TChimeraDrawDirect &Param);
    void ModBlend(const TChimeraDrawDirect &Param);
    void SubBlend(const TChimeraDrawDirect &Param);

    void AlphaTransBlend(const TChimeraDrawDirect &Param);

    void TransBlend16AlphaMMX(const TChimeraDrawDirect &Param);
    void AddBlend16AlphaMMX(const TChimeraDrawDirect &Param);
    void ModBlend16AlphaMMX(const TChimeraDrawDirect &Param);
    void SubBlend16AlphaMMX(const TChimeraDrawDirect &Param);

    void TransBlend32AlphaMMX(const TChimeraDrawDirect &Param);
    void AddBlend32AlphaMMX(const TChimeraDrawDirect &Param);
    void ModBlend32AlphaMMX(const TChimeraDrawDirect &Param);
    void SubBlend32AlphaMMX(const TChimeraDrawDirect &Param);


    void ColorKeyRotDraw16(const TChimeraDrawDirect &Param);
    void ColorKeyRotDraw24(const TChimeraDrawDirect &Param);
    void ColorKeyRotDraw32(const TChimeraDrawDirect &Param);
    void ColorKeyRotDraw16MMX(const TChimeraDrawDirect &Param);
    void ColorKeyAddDraw(const TChimeraDrawDirect &Param);
    void ColorKeyModDraw(const TChimeraDrawDirect &Param);

    void ColorKeyTransBlend(const TChimeraDrawDirect &Param);
    void ColorKeyAddBlend(const TChimeraDrawDirect &Param);
    void ColorKeyModBlend(const TChimeraDrawDirect &Param);
    void ColorKeySubBlend(const TChimeraDrawDirect &Param);

    void ColorKeyTransBlend16AlphaMMX(const TChimeraDrawDirect &Param);
    void ColorKeyAddBlend16AlphaMMX(const TChimeraDrawDirect &Param);
    void ColorKeyModBlend16AlphaMMX(const TChimeraDrawDirect &Param);
    void ColorKeySubBlend16AlphaMMX(const TChimeraDrawDirect &Param);

    void ColorKeyTransBlend32AlphaMMX(const TChimeraDrawDirect &Param);
    void ColorKeyAddBlend32AlphaMMX(const TChimeraDrawDirect &Param);
    void ColorKeyModBlend32AlphaMMX(const TChimeraDrawDirect &Param);
    void ColorKeySubBlend32AlphaMMX(const TChimeraDrawDirect &Param);
};
//---------------------------------------------------------------------------
using namespace ChimeraDrawSystem;
//---------------------------------------------------------------------------
#endif
