//---------------------------------------------------------------------------
#ifndef chamdlibH
#define chamdlibH
//---------------------------------------------------------------------------
namespace ChimeraSystem
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

    extern bool Has3DNow;

    void Enable3DNow(void);

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
using namespace ChimeraSystem;
//---------------------------------------------------------------------------
#endif
