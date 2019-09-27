//---------------------------------------------------------------------------
#ifndef ch3dtypeINL
#define ch3dtypeINL
//---------------------------------------------------------------------------
#include <fastmath.h>
#include "ch3damdlib.h"
//---------------------------------------------------------------------------

/////////////////////////////////////////////////////////////////////////////
//***************************************************************************
// クラス名  : TChimera2DVector
// Version   : 0.7 (2000/10/29)
//***************************************************************************
inline TChimera2DVector::TChimera2DVector(void)
{
}
//---------------------------------------------------------------------------
inline TChimera2DVector::TChimera2DVector(float AU, float AV)
    : U(AU), V(AV)
{
}
//---------------------------------------------------------------------------
inline TChimera2DVector TChimera2DVector::operator +(const TChimera2DVector& rhs) const
{
    return TChimera2DVector(U+rhs.U, V+rhs.V);
}
//---------------------------------------------------------------------------
inline TChimera2DVector TChimera2DVector::operator -(const TChimera2DVector& rhs) const
{
    return TChimera2DVector(U-rhs.U, V-rhs.V);
}
//---------------------------------------------------------------------------
inline TChimera2DVector TChimera2DVector::operator *(const float& rhs) const
{
    return TChimera2DVector(U*rhs, V*rhs);
}
//---------------------------------------------------------------------------
inline TChimera2DVector& TChimera2DVector::operator +=(const TChimera2DVector &rhs)
{
    U += rhs.U; V += rhs.V; return *this;
}
//---------------------------------------------------------------------------
inline TChimera2DVector& TChimera2DVector::operator -=(const TChimera2DVector &rhs)
{
    U -= rhs.U; V -= rhs.V; return *this;
}
//---------------------------------------------------------------------------
inline TChimera2DVector& TChimera2DVector::operator *=(const float &rhs)
{
    U *= rhs; V *= rhs; return *this;
}
//---------------------------------------------------------------------------










/////////////////////////////////////////////////////////////////////////////
//***************************************************************************
// クラス名  : TChimera3DVector
// Version   : 0.7 (2000/10/29)
//***************************************************************************
inline TChimera3DVector::TChimera3DVector(void)
{
}
//---------------------------------------------------------------------------
inline TChimera3DVector::TChimera3DVector(float AX, float AY, float AZ)
    : X(AX), Y(AY), Z(AZ)
{
}
//---------------------------------------------------------------------------
inline TChimera3DVector TChimera3DVector::operator +(const TChimera3DVector& rhs) const
{
    return TChimera3DVector(X+rhs.X, Y+rhs.Y, Z+rhs.Z);
}
//---------------------------------------------------------------------------
inline TChimera3DVector TChimera3DVector::operator -(const TChimera3DVector& rhs) const
{
    return TChimera3DVector(X-rhs.X, Y-rhs.Y, Z-rhs.Z);
}
//---------------------------------------------------------------------------
inline TChimera3DVector TChimera3DVector::operator *(const TChimera3DVector& rhs) const
{
    return TChimera3DVector(X*rhs.X, Y*rhs.Y, Z*rhs.Z);
}
//---------------------------------------------------------------------------
inline TChimera3DVector TChimera3DVector::operator *(const TChimera3DMatrix& rhs) const
{
    float AX = X*rhs._11 + Y*rhs._21 + Z* rhs._31 + rhs._41;
    float AY = X*rhs._12 + Y*rhs._22 + Z* rhs._32 + rhs._42;
    float AZ = X*rhs._13 + Y*rhs._23 + Z* rhs._33 + rhs._43;
    float AW = X*rhs._14 + Y*rhs._24 + Z* rhs._34 + rhs._44;

    return TChimera3DVector(AX / AW, AY / AW, AZ / AW);
}
//---------------------------------------------------------------------------
inline TChimera3DVector TChimera3DVector::operator *(const float& rhs) const
{
    return TChimera3DVector(X*rhs, Y*rhs, Z*rhs);
}
//---------------------------------------------------------------------------
inline TChimera3DVector& TChimera3DVector::operator +=(const TChimera3DVector &rhs)
{
    X += rhs.X; Y += rhs.Y; Z += rhs.Z; return *this;
}
//---------------------------------------------------------------------------
inline TChimera3DVector& TChimera3DVector::operator -=(const TChimera3DVector &rhs)
{
    X -= rhs.X; Y -= rhs.Y; Z -= rhs.Z; return *this;
}
//---------------------------------------------------------------------------
inline TChimera3DVector& TChimera3DVector::operator *=(const TChimera3DVector &rhs)
{
    X *= rhs.X; Y *= rhs.Y; Z *= rhs.Z; return *this;
}
//---------------------------------------------------------------------------
inline TChimera3DVector& TChimera3DVector::operator *=(const float &rhs)
{
    X *= rhs; Y *= rhs; Z *= rhs; return *this;
}
//---------------------------------------------------------------------------
inline float TChimera3DVector::DotProduct(const TChimera3DVector &rhs) const
{
    return X * rhs.X + Y * rhs.Y + Z * rhs.Z;
}
//---------------------------------------------------------------------------
inline TChimera3DVector TChimera3DVector::CrossProduct(const TChimera3DVector &rhs) const
{
    if(Chimera3DSystem::Has3DNow)
    {
        TChimera3DVector Ret;
        Chimera3DSystem::VectorCrossProduct(&Ret, this, &rhs);
        return Ret;
    }
    else
    {
        return TChimera3DVector(Y*rhs.Z - Z*rhs.Y, Z*rhs.X - X*rhs.Z, X*rhs.Y - Y*rhs.X);
    }
}
//---------------------------------------------------------------------------
inline TChimera3DVector& TChimera3DVector::Normalize(void)
{
    if(Chimera3DSystem::Has3DNow)
    {
        TChimera3DVector Ret;
        Chimera3DSystem::VectorNormalize(&Ret, this);
        return *this = Ret;
    }
    else
    {
        float Size = 1.0f / Magnitude();
        X *= Size; Y *= Size; Z *= Size; return *this;
    }
}
//---------------------------------------------------------------------------
inline float TChimera3DVector::Magnitude(void)
{
    return sqrt(X*X + Y*Y + Z*Z);
}
//---------------------------------------------------------------------------
inline void TChimera3DVector::RotTheta(const float Sin, const float Cos)
{
    float PX = X, PY = Y;
    X = PX*Cos - PY*Sin;
    Y = PX*Sin + PY*Cos;
}
//---------------------------------------------------------------------------
inline void TChimera3DVector::RotPhi(const float Sin, const float Cos)
{
    float PX = X, PZ = Z;
    X = PX*Cos - PZ*Sin;
    Z = PX*Sin + PZ*Cos;
}










/////////////////////////////////////////////////////////////////////////////
//***************************************************************************
// クラス名  : TChimera3DMatrix
// Version   : 0.7 (2000/10/29)
//***************************************************************************
inline TChimera3DMatrix TChimera3DMatrix::operator *(const TChimera3DMatrix &rhs)
{

    if(Chimera3DSystem::Has3DNow)
    {
        TChimera3DMatrix Ret;
        MatrixMul(&Ret, this, &rhs);
        return Ret;
    }
    else
    {
        TChimera3DMatrix Ret;
        Ret._11 = _11 * rhs._11 + _12 * rhs._21 + _13 * rhs._31 + _14 * rhs._41;
        Ret._12 = _11 * rhs._12 + _12 * rhs._22 + _13 * rhs._32 + _14 * rhs._42;
        Ret._13 = _11 * rhs._13 + _12 * rhs._23 + _13 * rhs._33 + _14 * rhs._43;
        Ret._14 = _11 * rhs._14 + _12 * rhs._24 + _13 * rhs._34 + _14 * rhs._44;

        Ret._21 = _21 * rhs._11 + _22 * rhs._21 + _23 * rhs._31 + _24 * rhs._41;
        Ret._22 = _21 * rhs._12 + _22 * rhs._22 + _23 * rhs._32 + _24 * rhs._42;
        Ret._23 = _21 * rhs._13 + _22 * rhs._23 + _23 * rhs._33 + _24 * rhs._43;
        Ret._24 = _21 * rhs._14 + _22 * rhs._24 + _23 * rhs._34 + _24 * rhs._44;

        Ret._31 = _31 * rhs._11 + _32 * rhs._21 + _33 * rhs._31 + _34 * rhs._41;
        Ret._32 = _31 * rhs._12 + _32 * rhs._22 + _33 * rhs._32 + _34 * rhs._42;
        Ret._33 = _31 * rhs._13 + _32 * rhs._23 + _33 * rhs._33 + _34 * rhs._43;
        Ret._34 = _31 * rhs._14 + _32 * rhs._24 + _33 * rhs._34 + _34 * rhs._44;

        Ret._41 = _41 * rhs._11 + _42 * rhs._21 + _43 * rhs._31 + _44 * rhs._41;
        Ret._42 = _41 * rhs._12 + _42 * rhs._22 + _43 * rhs._32 + _44 * rhs._42;
        Ret._43 = _41 * rhs._13 + _42 * rhs._23 + _43 * rhs._33 + _44 * rhs._43;
        Ret._44 = _41 * rhs._14 + _42 * rhs._24 + _43 * rhs._34 + _44 * rhs._44;
        return Ret;
    }
}
//---------------------------------------------------------------------------
inline float& TChimera3DMatrix::operator ()(const int Col, const int Row)
{
    return Matrix[Col][Row];
}
//---------------------------------------------------------------------------
inline const float TChimera3DMatrix::operator ()(const int Col, const int Row) const
{
    return Matrix[Col][Row];
}
//---------------------------------------------------------------------------
inline TChimera3DMatrix& TChimera3DMatrix::Init(void)
{
    Matrix[0][1] = Matrix[0][2] = Matrix[0][3] = 0.0f;
    Matrix[1][0] = Matrix[1][2] = Matrix[1][3] = 0.0f;
    Matrix[2][0] = Matrix[2][1] = Matrix[2][3] = 0.0f;
    Matrix[3][0] = Matrix[3][1] = Matrix[3][2] = 0.0f;
    Matrix[0][0] = Matrix[1][1] = Matrix[2][2] = Matrix[3][3] = 1.0f;
    return *this;
}
//---------------------------------------------------------------------------
inline TChimera3DMatrix& TChimera3DMatrix::RotX(float Rad)
{
    double Sin, Cos;
    TChimera3DMatrix Temp;
    sincos(Rad, &Sin, &Cos);
    Temp.Init();
    Temp.Matrix[1][1] = Cos;
    Temp.Matrix[1][2] = Sin;
    Temp.Matrix[2][1] = -Sin;
    Temp.Matrix[2][2] = Cos;
    return (*this) = (*this) * Temp;
}
//---------------------------------------------------------------------------
inline TChimera3DMatrix& TChimera3DMatrix::RotY(float Rad)
{
    double Sin, Cos;
    TChimera3DMatrix Temp;
    sincos(Rad, &Sin, &Cos);
    Temp.Init();
    Temp.Matrix[0][0] = Cos;
    Temp.Matrix[0][2] = -Sin;
    Temp.Matrix[2][0] = Sin;
    Temp.Matrix[2][2] = Cos;
    return (*this) = (*this) * Temp;
}
//---------------------------------------------------------------------------
inline TChimera3DMatrix& TChimera3DMatrix::RotZ(float Rad)
{
    double Sin, Cos;
    TChimera3DMatrix Temp;
    sincos(Rad, &Sin, &Cos);
    Temp.Init();
    Temp.Matrix[0][0] = Cos;
    Temp.Matrix[0][1] = Sin;
    Temp.Matrix[1][0] = -Sin;
    Temp.Matrix[1][1] = Cos;
    return (*this) = (*this) * Temp;
}
//---------------------------------------------------------------------------
inline TChimera3DMatrix& TChimera3DMatrix::Move(float DX, float DY, float DZ)
{
    TChimera3DMatrix Temp;
    Temp.Init();
    Temp.Matrix[3][0] = DX;
    Temp.Matrix[3][1] = DY;
    Temp.Matrix[3][2] = DZ;
    return (*this) = (*this) * Temp;
}
//---------------------------------------------------------------------------
inline TChimera3DMatrix& TChimera3DMatrix::Move(const TChimera3DVector &Vec)
{
    return Move(Vec.X, Vec.Y, Vec.Z);
}
//---------------------------------------------------------------------------
inline TChimera3DMatrix& TChimera3DMatrix::Scale(float SX, float SY, float SZ)
{
    TChimera3DMatrix Temp;
    Temp.Init();
    Temp.Matrix[0][0] = SX;
    Temp.Matrix[1][1] = SY;
    Temp.Matrix[2][2] = SZ;
    return (*this) = (*this) * Temp;
}
//---------------------------------------------------------------------------
inline TChimera3DMatrix& TChimera3DMatrix::Scale(float S)
{
    return Scale(S, S, S);
}
//---------------------------------------------------------------------------
inline TChimera3DMatrix& TChimera3DMatrix::Scale(const TChimera3DVector &Vec)
{
    return Scale(Vec.X, Vec.Y, Vec.Z);
}
//---------------------------------------------------------------------------
#endif
