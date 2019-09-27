//---------------------------------------------------------------------------
#pragma inline
//---------------------------------------------------------------------------
#pragma hdrstop
#include <math.h>
#include "ch3damdlib.h"
#include "chcpuid.h"
#include "ch3dtype.h"
asm INCLUDE TASMEX.INC
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------

//***************************************************************************
// ローカル変数
//***************************************************************************
bool Chimera3DSystem::Has3DNow = NULL;


//***************************************************************************
// 3DNow!有効可
//***************************************************************************
void Chimera3DSystem::Enable3DNow(void)
{
    TCPUID CPUID;
    CallCPUID(CPUID);
    Has3DNow = CPUID.AMD3DNOW;
}


//***************************************************************************
// 正規化
//***************************************************************************
void Chimera3DSystem::VectorNormalize(
    TChimera3DVector *Result,
    const TChimera3DVector *Dest)
{
    asm
    {
        femms
        mov         eax, Dest
        mov         edx, Result
        movq        mm0,[eax]
        movd        mm1,[eax+8]
        movq        mm4,mm0
        movq        mm3,mm1
        pfmul       mm0,mm0
        pfmul       mm1,mm1
        pfacc       mm0,mm0
        pfadd       mm0,mm1
        movd        eax,mm0
        pfrsqrt     mm1,mm0
        movq        mm2,mm1
        cmp         eax,0x38D1B717
        jl          short zero_mag

        pfmul       mm1,mm1
        pfrsqit1    mm1,mm0
        pfrcpit2    mm1,mm2
        punpckldq   mm1,mm1
        pfmul       mm3,mm1
        pfmul       mm4,mm1
        movd        [edx+8],mm3
        movq        [edx],mm4
        femms
        jmp         short zero_end

        ALIGN       32
    zero_mag:
        mov         eax,Dest
        movq        mm0,[eax]
        movd        mm1,[eax+8]
        movq        [edx],mm0
        movd        [edx+8],mm1
        femms
    zero_end:
    }
}


//***************************************************************************
// 外積演算
//***************************************************************************
void Chimera3DSystem::VectorCrossProduct(
    TChimera3DVector *Result,
    const TChimera3DVector *Dest,
    const TChimera3DVector *Src)
{
    asm
    {
        femms
        mov         edx, Src
        mov         eax, Dest
        movd        mm0,[edx+8]
        movd        mm1,[eax+8]
        punpckldq   mm0,mm0
        movd        mm3,[eax+4]
        punpckldq   mm1,mm1
        pfmul       mm0,[eax]
        movq        mm5,[edx]
        movd        mm4,[edx+4]
        pfmul       mm1,mm5
        movq        mm2,mm0
        pfmul       mm3,[edx]
        pfsub       mm0,mm1
        pfmul       mm4,[eax]
        mov         eax,Result
        pfsub       mm1,mm2
        punpckhdq   mm0,mm0
        pfsub       mm4,mm3
        punpckldq   mm0,mm1
        movd        [eax+8],mm4
        movd        [eax+4],mm1
        movd        [eax],mm0
        femms
    }
}


//***************************************************************************
// ４ｘ４行列乗算演算
//***************************************************************************
void Chimera3DSystem::MatrixMul(
    TChimera3DMatrix *Result,
    const TChimera3DMatrix *Dest,
    const TChimera3DMatrix *Src)
{
    asm
    {
        femms
        mov ecx, Dest
        mov eax, Result
        mov edx, Src

        movq        mm0,[ecx]       // a_21 | a_11
        movq        mm1,[ecx+8]     // a_41 | a_31
        movq        mm4,[edx]       // b_21 | b_11
        punpckhdq   mm2,mm0         // a_21 |
        movq        mm5,[edx+16]    // b_22 | b_12
        punpckhdq   mm3,mm1         // a_41 |
        movq        mm6,[edx+32]    // b_23 | b_13
        punpckldq   mm0,mm0         // a_11 | a_11
        punpckldq   mm1,mm1         // a_31 | a_31
        pfmul       mm4,mm0         // a_11 * b_21 | a_11 * b_11
        punpckhdq   mm2,mm2         // a_21 | a_21
        pfmul       mm0,[edx+8]     // a_11 * b_41 | a_11 * b_31
        movq        mm7,[edx+48]    // b_24 | b_14
        pfmul       mm5,mm2         // a_21 * b_22 | a_21 * b_12
        punpckhdq   mm3,mm3         // a_41 | a_41
        pfmul       mm2,[edx+24]    // a_21 * b_42 | a_21 * b_32
        pfmul       mm6,mm1         // a_31 * b_23 | a_31 * b_13
        pfadd       mm5,mm4         // a_21 * b_22 + a_11 * b_21 | a_21 * b_12 + a_11 * b_11
        pfmul       mm1,[edx+40]    // a_31 * b_43 | a_31 * b_33
        pfadd       mm2,mm0         // a_21 * b_42 + a_11 * b_41 | a_21 * b_32 + a_11 * b_31
        pfmul       mm7,mm3         // a_41 * b_24 | a_41 * b_14 
        pfadd       mm6,mm5         // a_21 * b_22 + a_11 * b_21 + a_31 * b_23 | a_21 * b_12 + a_11 * b_11 + a_31 * b_13
        pfmul       mm3,[edx+56]    // a_41 * b_44 | a_41 * b_34
        pfadd       mm2,mm1         // a_21 * b_42 + a_11 * b_41 + a_31 * b_43 | a_21 * b_32 + a_11 * b_31 + a_31 * b_33 
        pfadd       mm7,mm6         // a_41 * b_24 + a_21 * b_22 + a_11 * b_21 + a_31 * b_23 | a_41 * b_14 + a_21 * b_12 + a_11 * b_11 + a_31 * b_13
        movq        mm0,[ecx+16]    // a_22 | a_12
        pfadd       mm3,mm2         // a_41 * b_44 + a_21 * b_42 + a_11 * b_41 + a_31 * b_43 | a_41 * b_34 + a_21 * b_32 + a_11 * b_31 + a_31 * b_33
        movq        mm1,[ecx+24]    // a_42 | a_32
        movq        [eax],mm7       // r_21 | r_11
        movq        mm4,[edx]       // b_21 | b_11
        movq        [eax+8],mm3     // r_41 | r_31

        punpckhdq   mm2,mm0         // a_22 | XXX
        movq        mm5,[edx+16]    // b_22 | b_12
        punpckhdq   mm3,mm1         // a_42 | XXX
        movq        mm6,[edx+32]    // b_23 | b_13
        punpckldq   mm0,mm0         // a_12 | a_12
        punpckldq   mm1,mm1         // a_32 | a_32
        pfmul       mm4,mm0         // a_12 * b_21 | a_12 * b_11
        punpckhdq   mm2,mm2         // a_22 | a_22
        pfmul       mm0,[edx+8]     // a_12 * b_41 | a_12 * b_31
        movq        mm7,[edx+48]    // b_24 | b_14
        pfmul       mm5,mm2         // a_22 * b_22 | a_22 * b12
        punpckhdq   mm3,mm3         // a_42 | a_42
        pfmul       mm2,[edx+24]    // a_22 * b_42 | a_22 * b_32
        pfmul       mm6,mm1         // a_32 * b_23 | a_32 * b_13
        pfadd       mm5,mm4         // a_12 * b_21 + a_22 * b_22 | a_12 * b_11 + a_22 * b12
        pfmul       mm1,[edx+40]    // a_32 * b_43 | a_32 * b_33
        pfadd       mm2,mm0         // a_12 * b_41 + a_22 * b_42 | a_12 * b_11 + a_22 * b_32
        pfmul       mm7,mm3         // a_42 * b_24 | a_42 * b_14
        pfadd       mm6,mm5         // a_32 * b_23 + a_12 * b_21 + a_22 * b_22 | a_32 * b_13 + a_12 * b_11 + a_22 * b12
        pfmul       mm3,[edx+56]    // a_42 * b_44 | a_42 * b_34
        pfadd       mm2,mm1         // a_32 * b_43 + a_12 * b_41 + a_22 * b_42 | a_32 * b_33 + a_12 * b_11 + a_22 * b_32
        pfadd       mm7,mm6         // a_42 * b_24 + a_32 * b_23 + a_12 * b_21 + a_22 * b_22 | a_42 * b_14 + a_32 * b_13 + a_12 * b_11 + a_22 * b12
        movq        mm0,[ecx+32]    // a_23 | a_13
        pfadd       mm3,mm2         // a_42 * b_44 + a_32 * b_43 + a_12 * b_41 + a_22 * b_42 | a_42 * b_34 + a_32 * b_33 + a_12 * b_11 + a_22 * b_32
        movq        mm1,[ecx+40]    // a_43 | a_33
        movq        [eax+16],mm7    // r_22 | r_12
        movq        mm4,[edx]       // b_21 | b_11
        movq        [eax+24],mm3    // r_42 | r_32

        punpckhdq   mm2,mm0         // a_23 | XXX
        movq        mm5,[edx+16]    // b_22 | b_12
        punpckhdq   mm3,mm1         // a_43 | XXX
        movq        mm6,[edx+32]    // b_23 | b_13
        punpckldq   mm0,mm0         // a_13 | a_13
        punpckldq   mm1,mm1         // a_33 | a_33
        pfmul       mm4,mm0         // a_13 * b_21 | a_13 * b_11
        punpckhdq   mm2,mm2         // a_23 | a_23
        pfmul       mm0,[edx+8]     // a_13 * b_41 | a_13 * b_31
        movq        mm7,[edx+48]    // b_24 | b_14
        pfmul       mm5,mm2         // a_23 * b_22 | a_23 * b_12
        punpckhdq   mm3,mm3         // a_43 | a_43
        pfmul       mm2,[edx+24]    // a_23 * b_42 | a_23 * b_32
        pfmul       mm6,mm1         // a_33 * b_23 | a_33 * b_13
        pfadd       mm5,mm4         // a_23 * b_22 + a_13 * b_21 | a_23 * b_12 + a_13 * b_11
        pfmul       mm1,[edx+40]    // a_33 * b_43 | a_33 * b_33
        pfadd       mm2,mm0         // a_13 * b_41 + a_23 * b_42 | a_13 * b_31 + a_23 * b_32
        pfmul       mm7,mm3         // a_43 * b_24 | a_43 * b_14
        pfadd       mm6,mm5         // a_33 * b_23 + a_23 * b_22 + a_13 * b_21 | a_33 * b_13 + a_23 * b_12 + a_13 * b_11
        pfmul       mm3,[edx+56]    // a_43 * b_44 | a_43 * b_34
        pfadd       mm2,mm1         // a_33 * b_43 * a_13 * b_41 + a_23 * b_42 | a_33 * b_33 + a_13 * b_31 + a_23 * b_32
        pfadd       mm7,mm6         // a_43 * b_24 + a_33 * b_23 + a_23 * b_22 + a_13 * b_21 | a_43 * b_14 + a_33 * b_13 + a_23 * b_12 + a_13 * b_11
        movq        mm0,[ecx+48]    // a_24 | a_14
        pfadd       mm3,mm2         // a_43 * b_44 + a_33 * b_43 * a_13 * b_41 + a_23 * b_42 | a_43 * b_34 + a_33 * b_33 + a_13 * b_31 + a_23 * b_32
        movq        mm1,[ecx+56]    // a_44 | a_34
        movq        [eax+32],mm7    // r_23 | r_13
        movq        mm4,[edx]       // b_21 | b_11
        movq        [eax+40],mm3    // r_43 | r_33

        punpckhdq   mm2,mm0         // a_24 | XXX
        movq        mm5,[edx+16]    // b_22 | b_12
        punpckhdq   mm3,mm1         // a_44 | XXX
        movq        mm6,[edx+32]    // b_23 | b_13
        punpckldq   mm0,mm0         // a_14 | a_14
        punpckldq   mm1,mm1         // a_34 | a_34
        pfmul       mm4,mm0         // a_14 * b_21 | a_14 * b_11
        punpckhdq   mm2,mm2         // a_24 | a_24
        pfmul       mm0,[edx+8]     // a_14 * b_41 | a_14 * b_31
        movq        mm7,[edx+48]    // b_24 | b_14
        pfmul       mm5,mm2         // a_24 * b_22 | a_24 * b_12
        punpckhdq   mm3,mm3         // a_44 | a_44
        pfmul       mm2,[edx+24]    // a_24 * b_ 42 | a_24 * b_32
        pfmul       mm6,mm1         // a_34 * b_23 | a_34 * b_13
        pfadd       mm5,mm4         // a_14 * b_21 + a_24 * b_22 | a_14 * b_11 + a_24 * b_12
        pfmul       mm1,[edx+40]    // a_34 * b_43 | a_34 * b_33
        pfadd       mm2,mm0         // a_14 * b_41 + a_24 * b_42 | a_14 * b_31 + a_24 * b_32
        pfmul       mm7,mm3         // a_44 * b_24 | a_44 * b_14
        pfadd       mm6,mm5         // a_34 * b_23 + a_14 * b_21 + a_24 * b_22 | a_34 * b_13 + a_14 * b_11 + a_24 * b_12
        pfmul       mm3,[edx+56]    // a_44 * b_44 | a_44 * b_34
        pfadd       mm2,mm1         // a_34 * b_43 + a_14 * b_41 + a_24 * b_42 | a_34 * b_33 + a_14 * b_31 + a_24 * b_32
        pfadd       mm7,mm6         // a_44 * b_24 + a_14 * b_23 + a_24 * b_42 | a_44 * b_14 + a_14 * b_31 + a_24 * b_32
        pfadd       mm3,mm2         // a_44 * b_44 + a_34 * b_43 + a_14 * b_41 + a_24 * b_42 | a_44 * b_34 + a_34 * b_33 + a_14 * b_31 + a_24 * b_32
        movq        [eax+48],mm7    // r_24 | r_14
        movq        [eax+56],mm3    // r_44 | r_34
        femms
    }
}



void Chimera3DSystem::TransTLVertex(
    TChimera3DTLVertex *Result,
    const TChimera3DTLVertex *Dest,
    const TChimera3DMatrix *Mat, int Count)
{
    asm
    {
        mov         ecx,Count
        cmp         ecx,0
        jle         exit

        femms
        mov         eax,Result
        mov         ebx,Mat
        mov         edx,Dest

        // Unrolled loop does two passes - detect odd number of vectors
        shr         ecx,1
        prefetch    [edx]
        prefetchw   [eax]
        jnc         short do_next
        add         ecx,1
        jmp         do_next2

        ALIGN       32
do_next:
        prefetch    [edx+32]
        prefetchw   [eax+32]
        movq        mm0,[edx]
        movq        mm1,[edx+8]
        movq        mm4,[ebx]
        punpckhdq   mm2,mm0
        movq        mm5,[ebx+16]
        punpckldq   mm0,mm0
        movq        mm6,[ebx+32]
        pfmul       mm4,mm0
        movq        mm7,[ebx+48]
        punpckhdq   mm2,mm2
        punpckhdq   mm3,mm1
        pfmul       mm5,mm2
        punpckldq   mm1,mm1
        pfmul       mm0,[ebx+8]
        punpckhdq   mm3,mm3
        pfmul       mm2,[ebx+24]
        pfmul       mm6,mm1
        pfadd       mm5,mm4
        pfmul       mm1,[ebx+40]
        pfadd       mm2,mm0
        pfmul       mm7,mm3
        pfadd       mm6,mm5
        pfmul       mm3,[ebx+56]
        pfadd       mm2,mm1
        pfadd       mm7,mm6
        pfadd       mm3,mm2
        add         edx,32          //STRIDE FOR D3DTLVERTEX
        movq        [eax+0],mm7
        movq        [eax+8],mm3
        add         eax,32          //STRIDE FOR D3DTLVERTEX

        ALIGN       32
do_next2:
        prefetch    [edx+32]
        prefetchw   [eax+32]
        movq        mm0,[edx]
        movq        mm1,[edx+8]
        movq        mm4,[ebx]
        punpckhdq   mm2,mm0
        movq        mm5,[ebx+16]
        punpckldq   mm0,mm0
        movq        mm6,[ebx+32]
        pfmul       mm4,mm0
        movq        mm7,[ebx+48]
        punpckhdq   mm2,mm2
        punpckhdq   mm3,mm1
        pfmul       mm5,mm2
        punpckldq   mm1,mm1
        pfmul       mm0,[ebx+8]
        punpckhdq   mm3,mm3
        pfmul       mm2,[ebx+24]
        pfmul       mm6,mm1
        pfadd       mm5,mm4
        pfmul       mm1,[ebx+40]
        pfadd       mm2,mm0
        pfmul       mm7,mm3
        pfadd       mm6,mm5
        pfmul       mm3,[ebx+56]
        pfadd       mm2,mm1
        pfadd       mm7,mm6
        pfadd       mm3,mm2
        add         edx,32          //STRIDE FOR D3DTLVERTEX
        movq        [eax],mm7
        movq        [eax+8],mm3
        add         eax,32          //STRIDE FOR D3DTLVERTEX

        dec         ecx
        jne         do_next

exit:
        femms
    }
}
