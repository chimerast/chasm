//---------------------------------------------------------------------------
#ifndef chtypeH
#define chtypeH
//---------------------------------------------------------------------------
#include <d3dtypes.h>
#include "cppfwd.h"
//---------------------------------------------------------------------------
namespace ChimeraSystem
{
    struct TChimeraVector;
    struct TChimeraRect;

    // TChimeraVector ==============================
    struct TChimeraVector
    {
        long X;
        long Y;

        TChimeraVector operator +(const TChimeraVector& rhs) const;
        TChimeraVector operator -(const TChimeraVector& rhs) const;
        TChimeraVector operator *(const TChimeraVector& rhs) const;
        TChimeraVector& operator +=(const TChimeraVector& rhs);
        TChimeraVector& operator -=(const TChimeraVector& rhs);
        TChimeraVector& operator *=(const TChimeraVector& rhs);

        TChimeraVector(void);
        TChimeraVector(long AX, long AY);
    };


    // TChimeraRect ================================
    struct TChimeraRect
    {
        long X1;
        long Y1;
        long X2;
        long Y2;

        TChimeraRect operator +(const TChimeraRect& rhs) const;
        TChimeraRect operator -(const TChimeraRect& rhs) const;
        TChimeraRect operator *(const TChimeraRect& rhs) const;
        TChimeraRect& operator +=(const TChimeraRect& rhs);
        TChimeraRect& operator -=(const TChimeraRect& rhs);
        TChimeraRect& operator *=(const TChimeraRect& rhs);
        TChimeraVector operator [](const int n) const;

        long Width(void) const;
        long Height(void) const;

        TChimeraRect(void);
        TChimeraRect(long AX1, long AY1, long AX2, long AY2);
    };




    struct TChimera2DVector;
    struct TChimera3DVector;
    struct TChimera3DMatrix;
    struct TChimera3DVertex;
    struct TChimera3DLVertex;
    struct TChimera3DTLVertex;
    struct TChimera3DVertexTex2;
    struct TChimera3DLVertexTex2;
    struct TChimera3DTLVertexTex2;

    typedef unsigned long TChimera3DColor;
    typedef unsigned long TChimera3DVertexFormat;
    typedef float TChimera3DValue;

    // TChimera2DVector ============================
    struct TChimera2DVector
    {
        union { float X; float U; };
        union { float Y; float V; };

        TChimera2DVector operator +(const TChimera2DVector& rhs) const;
        TChimera2DVector operator -(const TChimera2DVector& rhs) const;
        TChimera2DVector operator *(const float& rhs) const;
        TChimera2DVector& operator +=(const TChimera2DVector &rhs);
        TChimera2DVector& operator -=(const TChimera2DVector &rhs);
        TChimera2DVector& operator *=(const float& rhs);

        TChimera2DVector(void);
        TChimera2DVector(float AU, float AV);
    };


    // TChimera3DVector ============================
    struct TChimera3DVector
    {
        float X;
        float Y;
        float Z;

        TChimera3DVector operator +(const TChimera3DVector &rhs) const;
        TChimera3DVector operator -(const TChimera3DVector &rhs) const;
        TChimera3DVector operator *(const TChimera3DVector &rhs) const;
        TChimera3DVector operator *(const TChimera3DMatrix &rhs) const;
        TChimera3DVector operator *(const float &rhs) const;
        TChimera3DVector& operator +=(const TChimera3DVector &rhs);
        TChimera3DVector& operator -=(const TChimera3DVector &rhs);
        TChimera3DVector& operator *=(const TChimera3DVector &rhs);
        TChimera3DVector& operator *=(const float &rhs);

        float DotProduct(const TChimera3DVector &rhs) const;
        TChimera3DVector CrossProduct(const TChimera3DVector &rhs) const;
        TChimera3DVector& Normalize(void);
        float Magnitude(void);

        void RotTheta(const float Sin, const float Cos);
        void RotPhi(const float Sin, const float Cos);

        TChimera3DVector(void);
        TChimera3DVector(float AX, float AY, float AZ);
    };


    // TChimera3DMatrix ============================
    struct TChimera3DMatrix
    {
        union
        {
            float Matrix[4][4];
            struct
            {
                float _11, _12, _13, _14;
                float _21, _22, _23, _24;
                float _31, _32, _33, _34;
                float _41, _42, _43, _44;
            };
        };

        TChimera3DMatrix operator *(const TChimera3DMatrix &rhs);
        float& operator ()(const int Row, const int Col);
        const float operator ()(const int Row, const int Col) const;

        TChimera3DMatrix& Init(void);
        TChimera3DMatrix& RotX(float Rad);
        TChimera3DMatrix& RotY(float Rad);
        TChimera3DMatrix& RotZ(float Rad);
        TChimera3DMatrix& Scale(float S);
        TChimera3DMatrix& Scale(float SX, float SY, float SZ);
        TChimera3DMatrix& Scale(const TChimera3DVector &Vec);
        TChimera3DMatrix& Move(float DX, float DY, float DZ);
        TChimera3DMatrix& Move(const TChimera3DVector &Vec);
    };


    // TChimera3DVertex ============================
    struct TChimera3DVertex
    {
        static const TChimera3DVertexFormat Format
            = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1;

        TChimera3DVector Pos;
        TChimera3DVector Nor;
        TChimera2DVector Tex;

        TChimera3DVertex(void) { }
        TChimera3DVertex(
            const TChimera3DVector &APos,
            const TChimera3DVector &ANor,
            const TChimera2DVector &ATex)
            : Pos(APos), Nor(ANor), Tex(ATex) { }

        TChimera3DVertex(
            float X, float Y, float Z,
            float NX, float NY, float NZ,
            float U, float V)
            : Pos(X, Y, Z), Nor(NX, NY, NZ), Tex(U, V) { }
    };


    // TChimera3DLVertex ===========================
    struct TChimera3DLVertex
    {
        static const TChimera3DVertexFormat Format
            = D3DFVF_XYZ | D3DFVF_RESERVED1 | D3DFVF_DIFFUSE
            | D3DFVF_SPECULAR | D3DFVF_TEX1;

        TChimera3DVector Pos;
        TChimera3DValue Reserved;
        TChimera3DColor Diffuse;
        TChimera3DColor Specular;
        TChimera2DVector Tex;

        TChimera3DLVertex(void) { }

        TChimera3DLVertex(
            const TChimera3DVector &APos,
            const TChimera3DColor ADiffuse,
            const TChimera3DColor ASpecular,
            const TChimera2DVector &ATex)
            : Pos(APos), Reserved(1.0f),
              Diffuse(ADiffuse), Specular(ASpecular),
              Tex(ATex) { }

        TChimera3DLVertex(
            float X, float Y, float Z,
            const TChimera3DColor ADiffuse,
            const TChimera3DColor ASpecular,
            float U, float V)
            : Pos(X, Y, Z), Reserved(1.0f),
              Diffuse(ADiffuse), Specular(ASpecular),
              Tex(U, V) { }
    };


    // TChimera3DTLVertex ==========================
    struct TChimera3DTLVertex
    {
        static const TChimera3DVertexFormat Format
            = D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_SPECULAR | D3DFVF_TEX1;

        TChimera3DVector Pos;
        TChimera3DValue W;
        TChimera3DColor Diffuse;
        TChimera3DColor Specular;
        TChimera2DVector Tex;

        TChimera3DTLVertex(void) { }

        TChimera3DTLVertex(
            const TChimera3DVector &APos,
            const TChimera3DValue AW,
            const TChimera3DColor ADiffuse,
            const TChimera3DColor ASpecular,
            const TChimera2DVector &ATex)
            : Pos(APos), W(AW), Diffuse(ADiffuse), Specular(ASpecular),
              Tex(ATex) { }

        TChimera3DTLVertex(
            float X, float Y, float Z,
            const TChimera3DValue AW,
            const TChimera3DColor ADiffuse,
            const TChimera3DColor ASpecular,
            float U, float V)
            : Pos(X, Y, Z), W(AW), Diffuse(ADiffuse), Specular(ASpecular),
              Tex(U, V) { }
    };


    // TChimera3DVertexTex2 ========================
    struct TChimera3DVertexTex2
    {
        static const TChimera3DVertexFormat Format
            = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX2;

        TChimera3DVector Pos;
        TChimera3DVector Nor;
        TChimera2DVector Tex1;
        TChimera2DVector Tex2;

        TChimera3DVertexTex2(void) { }

        TChimera3DVertexTex2(
            const TChimera3DVector &APos,
            const TChimera3DVector &ANor,
            const TChimera2DVector &ATex1,
            const TChimera2DVector &ATex2)
            : Pos(APos), Nor(ANor), Tex1(ATex1), Tex2(ATex2) { }

        TChimera3DVertexTex2(
            float X, float Y, float Z,
            float NX, float NY, float NZ,
            float U1, float V1,
            float U2, float V2)
            : Pos(X, Y, Z), Nor(NX, NY, NZ), Tex1(U1, V1), Tex2(U2, V2){ }
    };


    // TChimera3DLVertexTex2 =======================
    struct TChimera3DLVertexTex2
    {
        static const TChimera3DVertexFormat Format
            = D3DFVF_XYZ | D3DFVF_RESERVED1 | D3DFVF_DIFFUSE | D3DFVF_SPECULAR | D3DFVF_TEX2;

        TChimera3DVector Pos;
        TChimera3DValue Reserved;
        TChimera3DColor Diffuse;
        TChimera3DColor Specular;
        TChimera2DVector Tex1;
        TChimera2DVector Tex2;

        TChimera3DLVertexTex2(void) { }

        TChimera3DLVertexTex2(
            const TChimera3DVector &APos,
            const TChimera3DColor ADiffuse,
            const TChimera3DColor ASpecular,
            const TChimera2DVector &ATex1,
            const TChimera2DVector &ATex2)
            : Pos(APos), Reserved(1.0f), Diffuse(ADiffuse), Specular(ASpecular),
              Tex1(ATex1), Tex2(ATex2) { }

        TChimera3DLVertexTex2(
            float X, float Y, float Z,
            const TChimera3DColor ADiffuse,
            const TChimera3DColor ASpecular,
            float U1, float V1,
            float U2, float V2)
            : Pos(X, Y, Z), Reserved(1.0f), Diffuse(ADiffuse), Specular(ASpecular),
              Tex1(U1, V1), Tex2(U2, V2) { }
    };


    // TChimera3DTLVertexTex2 ======================
    struct TChimera3DTLVertexTex2
    {
        static const TChimera3DVertexFormat Format
            = D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_SPECULAR | D3DFVF_TEX2;

        TChimera3DVector Pos;
        TChimera3DValue W;
        TChimera3DColor Diffuse;
        TChimera3DColor Specular;
        TChimera2DVector Tex1;
        TChimera2DVector Tex2;

        TChimera3DTLVertexTex2(void) { }

        TChimera3DTLVertexTex2(
            const TChimera3DVector &APos,
            const TChimera3DValue AW,
            const TChimera3DColor ADiffuse,
            const TChimera3DColor ASpecular,
            const TChimera2DVector &ATex1,
            const TChimera2DVector &ATex2)
            : Pos(APos), W(AW), Diffuse(ADiffuse), Specular(ASpecular),
              Tex1(ATex1), Tex2(ATex2) { }

        TChimera3DTLVertexTex2(
            float X, float Y, float Z,
            const TChimera3DValue AW,
            const TChimera3DColor ADiffuse,
            const TChimera3DColor ASpecular,
            float U1, float V1,
            float U2, float V2)
            : Pos(X, Y, Z), W(AW), Diffuse(ADiffuse), Specular(ASpecular),
              Tex1(U1, V1), Tex2(U2, V2) { }
    };
};
//---------------------------------------------------------------------------
using namespace ChimeraSystem;
#include "chtype.inl"
//---------------------------------------------------------------------------
#endif
