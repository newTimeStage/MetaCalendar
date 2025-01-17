/* -- coding utf-8 -- */
#include "c_core_metaclndr.h"

#include <cmath>
#include <sstream>

#define dClndrRefYear    (2000L)
#define dClndrRefMonth   (1L)
#define dClndrRefDay     (1L)
#define dClndrRefHour    (12L)
#define dClndrRefMinute  (0L)
#define dClndrRefSecond  (0L)

#define dCycDifSgt       (1000U)

#define dCycLenMili      (1.0f)
#define dCycLenSeco      (1000.0f*dCycLenMili)
#define dCycLenMinu      (60.0f*dCycLenSeco)
#define dCycLenHour      (60.0f*dCycLenMinu)
#define dCycLenDay       (24.0f*dCycLenHour)
#define dCycLenMont      (29.530588853f*dCycLenDay)
#define dCycLnMonDiffYr  (0.000000002162f*dCycLenDay)  /* from J2000 + */
#define dCycLenYear      (365.242189670f*dCycLenDay)
#define dCycLenYrDiffDy  (0.00000006162f*dCycLenDay)  /* from J2000 - */
#define dCycLenEra       (30.0f*dCycLenYear)
#define dCycLenOper      (12.0f*dCycLenEra)
#define dCycLenAsse      (30.0f*dCycLenOper)
#define dCycLenAxial     (25771.5f*dCycLenYear)
#define dCycLenPrim      (30.0f*dCycLenAsse)
#define dCycLenPrimMax   (60.0f*dCycLenPrim)

#define dNumCycle        (12L)
#define dIndxPrim        (dNumCycle - 1L)
#define dIndxPeri        (dIndxPrim - 1L)
#define dIndxAsse        (dIndxPeri - 1L)
#define dIndxOper        (dIndxAsse - 1L)
#define dIndxEra         (dIndxOper - 1L)
#define dIndxYear        (dIndxEra  - 1L)
#define dIndxMont        (dIndxYear - 1L)
#define dIndxDay         (dIndxMont - 1L)
#define dIndxHour        (dIndxDay  - 1L)
#define dIndxMinu        (dIndxHour - 1L)
#define dIndxSeco        (dIndxMinu - 1L)
#define dIndxMili        (dIndxSeco - 1L)


typedef struct stctMetaCyc
{
    uint64_t Start;
    uint64_t CycNo;
    uint64_t LenMl;
    uint64_t MilAl;
    uint64_t DiffL;
    uint64_t DiffA;
}T_MetaCyc;

typedef struct stctCycCfg
{
    uint64_t Start;
    double   LenMl;
}T_CycCfg;

class CMetaClndr
{
public:
    CMetaClndr(const uint64_t& iBeginToRefMil,
               const uint64_t& iAfterRefMil);
    void Refrash(void);
    uint64_t CycNo(const int iIdx);
    ~CMetaClndr(void);
private:
    T_MetaCyc MtCycLst[dNumCycle];
    void InitCycLst(void);
    void SetBegin(const uint64_t& iBeginToRefMil);
    void UpTo(const uint64_t& iAfterRefMil);
    void Adjust(const uint64_t& iAfterRefMil);
};

static const T_Core_RefDateTime scnstRefDateTime = {
    dClndrRefYear  ,
    dClndrRefMonth ,
    dClndrRefDay   ,
    dClndrRefHour  ,
    dClndrRefMinute,
    dClndrRefSecond,
};

static const T_CycCfg scnstCycCfgLst[dNumCycle] = {
    { 0U, dCycLenSeco},
    { 0U, dCycLenMinu},
    { 0U, dCycLenHour},
    { 0U, dCycLenDay },
    { 1U, dCycLenMont},
    { 1U, dCycLenYear},
    { 1U, dCycLenEra },
    { 1U, dCycLenOper},
    { 1U, dCycLenAsse},
    { 1U, dCycLenAxial},
    { 1U, dCycLenPrim},
    { 1U, dCycLenPrimMax},
};

static const std::string scnstKeyTag[dNumCycle] = {
    "毫","秒","分","时","日","月","年","世","运","会","纪","元",
};

static CMetaClndr* sclsMetaClndr = NULL;


CMetaClndr::CMetaClndr(const uint64_t& iBeginToRefMil,
                       const uint64_t& iAfterRefMil) {
    InitCycLst();
    SetBegin(iBeginToRefMil);
    Adjust(iAfterRefMil);
}

void CMetaClndr::InitCycLst(void) {
    for(int i=0L; dNumCycle > i; i++) {
        MtCycLst[i].Start = scnstCycCfgLst[i].Start;
        MtCycLst[i].CycNo = scnstCycCfgLst[i].Start;
        double IntPart, FracPart;
        FracPart = std::modf(scnstCycCfgLst[i].LenMl, &IntPart);
        MtCycLst[i].LenMl = static_cast<uint64_t>(IntPart);
        MtCycLst[i].DiffL = dCycDifSgt*static_cast<uint64_t>(FracPart);
        MtCycLst[i].MilAl = 0U;
        MtCycLst[i].DiffA = 0U;
    }
    return;
}

void CMetaClndr::SetBegin(const uint64_t& iBeginToRefMil) {
    uint64_t LastMil = iBeginToRefMil;
    for(int i=dNumCycle-1L; 0L<=i; i--) {
        if(0L < i) {
            uint64_t DiffSum = MtCycLst[i].CycNo*MtCycLst[i-1L].DiffL;
            MtCycLst[i].DiffA = DiffSum%dCycDifSgt;
            LastMil = (iBeginToRefMil + DiffSum/dCycDifSgt)%MtCycLst[i].LenMl;
            MtCycLst[i].CycNo = MtCycLst[i].Start + LastMil/MtCycLst[i-1L].LenMl;
        } else {
            MtCycLst[i].CycNo = LastMil%MtCycLst[i].LenMl;
            MtCycLst[i].DiffA = 0U;
        }
        MtCycLst[i].MilAl = LastMil%MtCycLst[i].LenMl;
        LastMil = MtCycLst[i].MilAl;
    }
    return;
}

void CMetaClndr::UpTo(const uint64_t& iAfterRefMil) {
    for(uint64_t i=0U; iAfterRefMil>i; i++) {
        Refrash();
    }
    return;
}

void CMetaClndr::Adjust(const uint64_t& iAfterRefMil) {
    MtCycLst[dIndxMont].LenMl += static_cast<uint64_t>(dCycLnMonDiffYr*static_cast<double>(iAfterRefMil/dCycLenYear));
    MtCycLst[dIndxYear].LenMl -= static_cast<uint64_t>(dCycLenYrDiffDy*static_cast<double>(iAfterRefMil/dCycLenDay));
    return;
}

void CMetaClndr::Refrash(void) {
    for(int i=0L; dNumCycle>i; i++) {
        MtCycLst[i].MilAl++;
        if(dIndxMili == i) {
            MtCycLst[i].CycNo++;
        }
        if(MtCycLst[i].MilAl >= MtCycLst[i].LenMl) {
            MtCycLst[i].MilAl -= MtCycLst[i].LenMl;
            MtCycLst[i].CycNo = MtCycLst[i].Start;
            MtCycLst[i].DiffA += MtCycLst[i].DiffL;
            if(dCycDifSgt <= MtCycLst[i].DiffA) {
                MtCycLst[i].MilAl--;
                MtCycLst[i].DiffA -= dCycDifSgt;
            } else if(dNumCycle-1L > i) {
                MtCycLst[i+1].CycNo++;
            }
            if(dIndxYear == i) {
                MtCycLst[dIndxMont].LenMl += static_cast<uint64_t>(dCycLnMonDiffYr);
            }
            if(dIndxDay == i) {
                MtCycLst[dIndxYear].LenMl -= static_cast<uint64_t>(dCycLenYrDiffDy);
            }
        }
    }
    return;
}

uint64_t CMetaClndr::CycNo(const int iIdx) {
    return MtCycLst[iIdx].CycNo;
}

CMetaClndr::~CMetaClndr(void) {

}

C_Core_MetaClndr::C_Core_MetaClndr(const uint64_t& iBeginToRefMl,
                                   const uint64_t& iAfterRefMl) {
    sclsMetaClndr = new CMetaClndr(iBeginToRefMl,
                                   iAfterRefMl);
}

const T_Core_RefDateTime* C_Core_MetaClndr::RefDateTime(void) {
    return &scnstRefDateTime;
}

void C_Core_MetaClndr::Refersh(void) {
    sclsMetaClndr->Refrash();
}

uint64_t C_Core_MetaClndr::Prim(void) {
    return sclsMetaClndr->CycNo(dIndxPrim);
}

uint64_t C_Core_MetaClndr::Peri(void) {
    return sclsMetaClndr->CycNo(dIndxPeri);
}

uint64_t C_Core_MetaClndr::Asse(void) {
    return sclsMetaClndr->CycNo(dIndxAsse);
}

uint64_t C_Core_MetaClndr::Oper(void) {
    return sclsMetaClndr->CycNo(dIndxOper);
}

uint64_t C_Core_MetaClndr::Era (void) {
    return sclsMetaClndr->CycNo(dIndxEra );
}

uint64_t C_Core_MetaClndr::Year(void) {
    return sclsMetaClndr->CycNo(dIndxYear);
}

uint64_t C_Core_MetaClndr::Mont(void) {
    return sclsMetaClndr->CycNo(dIndxMont);
}

uint64_t C_Core_MetaClndr::Day (void) {
    return sclsMetaClndr->CycNo(dIndxDay );
}

uint64_t C_Core_MetaClndr::Hour(void) {
    return sclsMetaClndr->CycNo(dIndxHour);
}

uint64_t C_Core_MetaClndr::Minu(void) {
    return sclsMetaClndr->CycNo(dIndxMinu);
}

uint64_t C_Core_MetaClndr::Seco(void) {
    return sclsMetaClndr->CycNo(dIndxSeco);
}

uint64_t C_Core_MetaClndr::Mili(void) {
    return sclsMetaClndr->CycNo(dIndxMili);
}

std::string C_Core_MetaClndr::GetCalendar(void) {
    std::ostringstream Oss;
    for(int i=dNumCycle-1L; 0L<=i; i--) {
        Oss << sclsMetaClndr->CycNo(i) << scnstKeyTag[i];
    }
    return Oss.str();
}

C_Core_MetaClndr::~C_Core_MetaClndr(void) {
    delete sclsMetaClndr;
}
