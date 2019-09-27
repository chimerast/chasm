//---------------------------------------------------------------------------
#ifndef chcpuidH
#define chcpuidH
//---------------------------------------------------------------------------
#include "cppfwd.h"
//---------------------------------------------------------------------------
namespace ChimeraSystem
{
    struct TCPUID {
        enum TCPUProduct { cpUnknown=0, cpIntel, cpAMD, cpCyrix, cpCentaur };

        unsigned long HasCPUID;

        unsigned long ProcessorType;
        unsigned long Features;
        unsigned long ExtFeatures;

        unsigned long MaxCPUIDFunction;
        unsigned long MaxCPUIDExtFunction;

        char ProductString[16];
        char AMDCPUString[48];

        unsigned long CPUSpeed;
    
        unsigned long CPUFamily;
        unsigned long CPUModel;
        unsigned long CPUSteppingID;

        bool FPU, VME, DE, PSE, TSC, MSR, PAE, MCE, CX8, APIC, SEP, MTRR, PGE,
            MCA, CMOV, PAT, PSE36, PSN, CLFLUSH, DTS, ACPI, MMX, FXSR, SSE, SSE2,
            SS, TM, AMDEMMX, AMD3DNOW, AMDE3DNOW;

        TCPUProduct CPUProduct;
    };

    std::ostream& operator <<(std::ostream &os, TCPUID &CPUFeature);

    void CallCPUID(TCPUID &CPUFeature);
    void TestCPUSpeed(TCPUID &CPUFeature);
};
//---------------------------------------------------------------------------
using namespace ChimeraSystem;
//---------------------------------------------------------------------------
#endif
