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
#define dCycLenMont      (29.530588853f*dCycLenDay)   /* 月球周年 */
#define dOffstMont       (5.0f*dCycLenMont + 18.0f*dCycLenDay)
#define dCycLnMonDiffYr  (0.000000002162f*dCycLenDay)  /* from J2000 + */
#define dCycLenMercury   (87.9691f*dCycLenDay)    /* 水星周年 */
#define dCycLenVenus     (224.701f*dCycLenDay)    /* 金星周年 */
#define dCycLenYear      (365.242189670f*dCycLenDay)  /* 地球周年 */
#define dCycLenSlrTrm    (dCycLenYear/24.0f)
#define dOffstSlrTrm     (9.0f*dCycLenSlrTrm + 9.0f*dCycLenDay)
#define dCycLenSeason    (dCycLenYear/4.0f)
#define dOffstSeason     (1.0f*dCycLenSeason + 11.0f*dCycLenDay)
#define dCycLenYrDiffDy  (0.00000006162f*dCycLenDay)  /* from J2000 - */
#define dCycLenMars      (686.971f*dCycLenDay)    /* 火星周年 */
#define dCycLenCeres     (4.06f*dCycLenYear)      /* 谷神星周年 */
#define dCycLenJupiter   (11.862615f*dCycLenYear) /* 木星周年 */
#define dCycLenSaturn    (29.447498f*dCycLenYear) /* 土星周年 */
#define dCycLenUranus    (84.016846f*dCycLenYear) /* 天王星周年 */
#define dCycLenNeptune   (164.79132f*dCycLenYear) /* 海王星周年 */
#define dCycLenPluto     (247.92065f*dCycLenYear)   /* 冥王星周年 */
#define dCycLenHaumea    (283.28f*dCycLenYear)      /* 妊神星周年 */
#define dCycLenMakemake  (305.5f*dCycLenYear)       /* 鸟神星周年 */
#define dCycLenEris      (556.901f*dCycLenYear)     /* 阋神星周年 */
#define dCycLenAxial     (25771.5f*dCycLenYear)
#define dCycLenWall      (dCycLenAxial/10.0f)
#define dCycLenWork      (dCycLenAxial/5.0f)
#define dCycLenMeta      (129600.0f*dCycLenYear)
#define dCycLenMax       (3.1415926f*2.718281828f*dCycLenMeta)



#define dIndxMili        (0L)
#define dIndxSeco        (dIndxMili     + 1L)
#define dIndxMinu        (dIndxSeco     + 1L)
#define dIndxHour        (dIndxMinu     + 1L)
#define dIndxDay         (dIndxHour     + 1L)
#define dIndxSlrTrm      (dIndxDay      + 1L)
#define dIndxMont        (dIndxSlrTrm   + 1L)
#define dIndxMercury     (dIndxMont     + 1L)
#define dIndxSeason      (dIndxMercury  + 1L)
#define dIndxVenus       (dIndxSeason   + 1L)
#define dIndxYear        (dIndxVenus    + 1L)
#define dIndxMars        (dIndxYear     + 1L)
#define dIndxCeres       (dIndxMars     + 1L)
#define dIndxJupiter     (dIndxCeres    + 1L)
#define dIndxSaturn      (dIndxJupiter  + 1L)
#define dIndxUranus      (dIndxSaturn   + 1L)
#define dIndxNeptune     (dIndxUranus   + 1L)
#define dIndxPluto       (dIndxNeptune  + 1L)
#define dIndxHaumea      (dIndxPluto    + 1L)
#define dIndxMakemake    (dIndxHaumea   + 1L)
#define dIndxEris        (dIndxMakemake + 1L)
#define dIndxWall        (dIndxEris     + 1L)
#define dIndxWork        (dIndxWall     + 1L)
#define dIndxAxial       (dIndxWork     + 1L)
#define dIndxMeta        (dIndxAxial    + 1L)
#define dNumCycle        (dIndxMeta     + 1L)


typedef struct stctMetaCyc
{
    uint64_t CycNo;
    uint64_t LenMl;
    uint64_t DiffA;
    uint64_t CyLen;
    uint64_t ReLen;
    uint64_t DiffL;
}T_MetaCyc;

typedef struct stctCycCfg
{
    uint64_t Start;
    double   CyLen;
    double   ReLen;
}T_CycCfg;

class CMetaClndr
{
public:
    CMetaClndr(const uint64_t& iBeginToRefMil,
               const uint64_t& iAfterRefMil);
    void Refrash(void);
    uint64_t CycNo(const int iIdx);
    std::string StrClndr(void);
    ~CMetaClndr(void);
private:
    T_MetaCyc MtCycLst[dNumCycle];
    void InitCycLst(void);
    void SetBegin(const uint64_t& iBeginToRefMil);
    void UpTo(const uint64_t& iAfterRefMil);
    void Adjust(const uint64_t& iAfterRefMil);
    static uint64_t DoubleToUint(const double iDblVal);
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
    { 0U, dCycLenMili    , dCycLenSeco},
    { 0U, dCycLenSeco    , dCycLenMinu},
    { 0U, dCycLenMinu    , dCycLenHour},
    { 0U, dCycLenHour    , dCycLenDay },
    { 1U, dCycLenDay     , dCycLenMont},
    { 1U, dCycLenSlrTrm  , dCycLenYear},
    { 1U, dCycLenMont    , 12.0f*dCycLenMont},
    { 1U, dCycLenMercury , dCycLenWall},
    { 1U, dCycLenSeason  , dCycLenYear},
    { 1U, dCycLenVenus   , dCycLenWall},
    { 1U, dCycLenYear    , dCycLenWall},
    { 1U, dCycLenMars    , dCycLenWall},
    { 1U, dCycLenCeres   , dCycLenWall},
    { 1U, dCycLenJupiter , dCycLenWall},
    { 1U, dCycLenSaturn  , dCycLenWall},
    { 1U, dCycLenUranus  , dCycLenWall},
    { 1U, dCycLenNeptune , dCycLenWall},
    { 1U, dCycLenPluto   , dCycLenWall},
    { 1U, dCycLenHaumea  , dCycLenWork},
    { 1U, dCycLenMakemake, dCycLenWork},
    { 1U, dCycLenEris    , dCycLenWork},
    { 0U, dCycLenWall    , dCycLenAxial},
    { 1U, dCycLenWork    , dCycLenAxial},
    { 1U, dCycLenAxial   , dCycLenMeta },
    { 1U, dCycLenMeta    , dCycLenMax },
};
  
static const std::string scnstKeyTag[dNumCycle] = {
    "毫","秒","分","时","日","候","月","水","季","金","年","火","谷","木","土","天","海","冥","妊","鸟","阋","垣","行","纪","元",
};

static CMetaClndr* sclsMetaClndr = NULL;


CMetaClndr::CMetaClndr(const uint64_t& iBeginToRefMil,
                       const uint64_t& iAfterRefMil) {
    InitCycLst();
    SetBegin(iBeginToRefMil);
    Adjust(iAfterRefMil);
}

uint64_t CMetaClndr::DoubleToUint(const double iDblVal) {
    double IntPart, FracPart;
    FracPart = std::modf(iDblVal, &IntPart);
    return static_cast<uint64_t>(IntPart);
}

void CMetaClndr::InitCycLst(void) {
    for(int i=0L; dNumCycle > i; i++) {
        MtCycLst[i].CycNo = scnstCycCfgLst[i].Start;
        double IntPart, FracPart;
        FracPart = std::modf(scnstCycCfgLst[i].ReLen, &IntPart);
        MtCycLst[i].ReLen = static_cast<uint64_t>(IntPart);
        FracPart = std::modf(scnstCycCfgLst[i].CyLen, &IntPart);
        MtCycLst[i].CyLen = static_cast<uint64_t>(IntPart);
        MtCycLst[i].DiffL = dCycDifSgt*static_cast<uint64_t>(FracPart);
        MtCycLst[i].DiffA = 0U;
        MtCycLst[i].LenMl = 0U;
    }
    return;
}

void CMetaClndr::SetBegin(const uint64_t& iBeginToRefMil) {
    for(int i=dNumCycle-1L; 0L<=i; i--) {
        uint64_t DiffSum = MtCycLst[i].CycNo*MtCycLst[i].DiffL;
        MtCycLst[i].DiffA = DiffSum%dCycDifSgt;
        uint64_t LastMil = iBeginToRefMil + DiffSum/dCycDifSgt;
        (dIndxDay    == i)?(LastMil += DoubleToUint(dOffstMont  )):(0L);
        (dIndxSlrTrm == i)?(LastMil += DoubleToUint(dOffstSlrTrm)):(0L);
        (dIndxMont   == i)?(LastMil += DoubleToUint(dOffstMont  )):(0L);
        (dIndxSeason == i)?(LastMil += DoubleToUint(dOffstSeason)):(0L);
        LastMil %= MtCycLst[i].ReLen;
        MtCycLst[i].CycNo = scnstCycCfgLst[i].Start + LastMil/MtCycLst[i].CyLen;
        MtCycLst[i].LenMl += MtCycLst[i].CycNo*MtCycLst[i].CyLen + LastMil%MtCycLst[i].CyLen;
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
        if(MtCycLst[i].ReLen <= MtCycLst[i].LenMl) {
            MtCycLst[i].LenMl -= MtCycLst[i].ReLen;
            MtCycLst[i].CycNo = scnstCycCfgLst[i].Start;
        } else {
            MtCycLst[i].LenMl++;
            if(0U == MtCycLst[i].LenMl%MtCycLst[i].CyLen) {
                MtCycLst[i].DiffA += MtCycLst[i].DiffL;
                if(dCycDifSgt <= MtCycLst[i].DiffA) {
                    MtCycLst[i].DiffA -= dCycDifSgt;
                    MtCycLst[i].LenMl--;
                } else {
                    MtCycLst[i].CycNo++;
                }
                if(dIndxDay == i) {
                    MtCycLst[dIndxYear].CyLen -= DoubleToUint(dCycLenYrDiffDy);
                }
                if(dIndxYear == i) {
                    MtCycLst[dIndxMont].CyLen += DoubleToUint(dCycLnMonDiffYr);
                }
            }
        }
    }
    return;
}

uint64_t CMetaClndr::CycNo(const int iIdx) {
    return MtCycLst[iIdx].CycNo;
}

std::string CMetaClndr::StrClndr(void) {
    std::ostringstream Oss;
    for(int i=dNumCycle-1L; 0L<=i; i--) {
        Oss << CycNo(i) << scnstKeyTag[i];
    }
    return Oss.str();
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
    return sclsMetaClndr->StrClndr();
}

std::string C_Core_MetaClndr::TransFromMil(const uint64_t& iMilSt,
                                           const uint64_t& iMilAj) {
    CMetaClndr* pMtClndr = new CMetaClndr(iMilSt, iMilAj);
    std::string StrMetaClndr = pMtClndr->StrClndr();
    delete pMtClndr;
    return StrMetaClndr;
}

C_Core_MetaClndr::~C_Core_MetaClndr(void) {
    delete sclsMetaClndr;
}
