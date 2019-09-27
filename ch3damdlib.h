//---------------------------------------------------------------------------
#ifndef ch3damdlibH
#define ch3damdlibH
//---------------------------------------------------------------------------
namespace Chimera3DSystem
{
    struct TChimera2DVector;
    struct TChimera3DVector;
    struct TChimera3DMatrix;
    struct TChimera3DVertex;
    struct TChimera3DLVertex;
    struct TChimera3DTLVertex;
    struct TChimera3DVertexTex2;
    struct TChimera3DLVertexTex2;
    struct TChimera3DTLVertexTex2;

    enum CPU_MFGS
    {
        MFG_UNKNOWN,
        MFG_AMD,
        MFG_INTEL,
        MFG_CYRIX,
        MFG_CENTAUR
    };

    enum CPU_TYPES
    {
        UNKNOWN,
        AMD_Am486,
        AMD_K5,
        AMD_K6_MMX,
        AMD_K6_2,
        AMD_K6_3,
        AMD_K7,

        INTEL_486DX,
        INTEL_486SX,
        INTEL_486DX2,
        INTEL_486SL,
        INTEL_486SX2,
        INTEL_486DX2E,
        INTEL_486DX4,
        INTEL_Pentium,
        INTEL_Pentium_MMX,
        INTEL_Pentium_Pro,
        INTEL_Pentium_II,
        INTEL_Celeron,
        INTEL_Pentium_III,
    };

    enum CPUCAPS
    {
        CPU_MFG,        // Manufacturer (returns enum CPU_MFGS)
        CPU_TYPE,       // CPU type (return enum CPU_TYPES)

        HAS_CPUID,      // Supports CPUID instruction
        HAS_FPU,        // FPU present
        HAS_VME,        // Virtual Mode Extensions
        HAS_DEBUG,      // Debug extensions
        HAS_PSE,        // Page Size Extensions
        HAS_TSC,        // Time Stamp Counter
        HAS_MSR,        // Model Specific Registers
        HAS_MCE,        // Machine Check Extensions
        HAS_CMPXCHG8,   // CMPXCHG8 instruction
        HAS_MMX,        // MMX support
        HAS_3DNOW,      // 3DNow! support
        HAS_SSE_MMX,    // SSE MMX support
        HAS_SSE_FP,     // SSE FP support
    };

    extern bool Has3DNow;

    void Enable3DNow(void);
    unsigned int GetCPUCaps(CPUCAPS cap);

    void VectorNormalize(
        TChimera3DVector *Result,
        const TChimera3DVector *Dest);

    void VectorCrossProduct(
        TChimera3DVector *Result,
        const TChimera3DVector *Dest,
        const TChimera3DVector *Src);

    void MatrixMul(
        TChimera3DMatrix *Result,
        const TChimera3DMatrix *Dest,
        const TChimera3DMatrix *Src);

    void TransTLVertex(
        TChimera3DTLVertex *Result,
        const TChimera3DTLVertex *Dest,
        const TChimera3DMatrix *Mat, int Count);
};
//---------------------------------------------------------------------------
using namespace Chimera3DSystem;
//---------------------------------------------------------------------------
#endif
