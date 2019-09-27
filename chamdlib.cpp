//---------------------------------------------------------------------------
#pragma inline
//---------------------------------------------------------------------------
#pragma hdrstop
#include <math.h>
#include "chamdlib.h"
#include "chcpuid.h"
#include "chtype.h"
asm INCLUDE TASMEX.INC
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------

//***************************************************************************
// ローカル変数
//***************************************************************************
bool ChimeraSystem::Has3DNow = NULL;


//***************************************************************************
// 3DNow!有効可
//***************************************************************************
void ChimeraSystem::Enable3DNow(void)
{
    TCPUID CPUID;
    CallCPUID(CPUID);
    Has3DNow = CPUID.AMD3DNOW;
}


//***************************************************************************
// 正規化
//***************************************************************************
void ChimeraSystem::VectorNormalize(
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
void ChimeraSystem::VectorCrossProduct(
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
void ChimeraSystem::MatrixMul(
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

        movq        mm0,[ecx]       
        movq        mm1,[ecx+8]     
        movq        mm4,[edx]       
        punpckhdq   mm2,mm0         
        movq        mm5,[edx+16]    
        punpckhdq   mm3,mm1         
        movq        mm6,[edx+32]    
        punpckldq   mm0,mm0         
        punpckldq   mm1,mm1         
        pfmul       mm4,mm0         
        punpckhdq   mm2,mm2         
        pfmul       mm0,[edx+8]     
        movq        mm7,[edx+48]    
        pfmul       mm5,mm2         
        punpckhdq   mm3,mm3         
        pfmul       mm2,[edx+24]    
        pfmul       mm6,mm1         
        pfadd       mm5,mm4         
        pfmul       mm1,[edx+40]    
        pfadd       mm2,mm0         
        pfmul       mm7,mm3         
        pfadd       mm6,mm5         
        pfmul       mm3,[edx+56]    
        pfadd       mm2,mm1         
        pfadd       mm7,mm6         
        movq        mm0,[ecx+16]    
        pfadd       mm3,mm2         
        movq        mm1,[ecx+24]    
        movq        [eax],mm7       
        movq        mm4,[edx]       
        movq        [eax+8],mm3     

        punpckhdq   mm2,mm0         
        movq        mm5,[edx+16]    
        punpckhdq   mm3,mm1         
        movq        mm6,[edx+32]    
        punpckldq   mm0,mm0         
        punpckldq   mm1,mm1         
        pfmul       mm4,mm0         
        punpckhdq   mm2,mm2         
        pfmul       mm0,[edx+8]     
        movq        mm7,[edx+48]    
        pfmul       mm5,mm2         
        punpckhdq   mm3,mm3         
        pfmul       mm2,[edx+24]    
        pfmul       mm6,mm1         
        pfadd       mm5,mm4         
        pfmul       mm1,[edx+40]    
        pfadd       mm2,mm0         
        pfmul       mm7,mm3         
        pfadd       mm6,mm5         
        pfmul       mm3,[edx+56]    
        pfadd       mm2,mm1         
        pfadd       mm7,mm6         
        movq        mm0,[ecx+32]    
        pfadd       mm3,mm2         
        movq        mm1,[ecx+40]    
        movq        [eax+16],mm7    
        movq        mm4,[edx]       
        movq        [eax+24],mm3    

        punpckhdq   mm2,mm0         
        movq        mm5,[edx+16]    
        punpckhdq   mm3,mm1         
        movq        mm6,[edx+32]    
        punpckldq   mm0,mm0         
        punpckldq   mm1,mm1         
        pfmul       mm4,mm0         
        punpckhdq   mm2,mm2         
        pfmul       mm0,[edx+8]     
        movq        mm7,[edx+48]    
        pfmul       mm5,mm2         
        punpckhdq   mm3,mm3         
        pfmul       mm2,[edx+24]    
        pfmul       mm6,mm1         
        pfadd       mm5,mm4         
        pfmul       mm1,[edx+40]    
        pfadd       mm2,mm0         
        pfmul       mm7,mm3         
        pfadd       mm6,mm5         
        pfmul       mm3,[edx+56]    
        pfadd       mm2,mm1         
        pfadd       mm7,mm6         
        movq        mm0,[ecx+48]    
        pfadd       mm3,mm2         
        movq        mm1,[ecx+56]    
        movq        [eax+32],mm7    
        movq        mm4,[edx]       
        movq        [eax+40],mm3    

        punpckhdq   mm2,mm0         
        movq        mm5,[edx+16]    
        punpckhdq   mm3,mm1         
        movq        mm6,[edx+32]    
        punpckldq   mm0,mm0         
        punpckldq   mm1,mm1         
        pfmul       mm4,mm0         
        punpckhdq   mm2,mm2         
        pfmul       mm0,[edx+8]     
        movq        mm7,[edx+48]    
        pfmul       mm5,mm2         
        punpckhdq   mm3,mm3         
        pfmul       mm2,[edx+24]    
        pfmul       mm6,mm1         
        pfadd       mm5,mm4         
        pfmul       mm1,[edx+40]    
        pfadd       mm2,mm0         
        pfmul       mm7,mm3         
        pfadd       mm6,mm5         
        pfmul       mm3,[edx+56]    
        pfadd       mm2,mm1         
        pfadd       mm7,mm6         
        pfadd       mm3,mm2
        movq        [eax+48],mm7    
        movq        [eax+56],mm3    
        femms
    }
}



void ChimeraSystem::TransTLVertex(
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
        add         edx,32
        movq        [eax+0],mm7
        movq        [eax+8],mm3
        add         eax,32

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
        add         edx,32
        movq        [eax],mm7
        movq        [eax+8],mm3
        add         eax,32

        dec         ecx
        jne         do_next

exit:
        femms
    }
}
