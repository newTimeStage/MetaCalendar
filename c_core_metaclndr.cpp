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

#define dTagDifSgt       (1000U)

#define dTagLenMili      (1.0f)
#define dTagLenSeco      (1000.0f*dTagLenMili)
#define dTagLenMinu      (60.0f*dTagLenSeco)
#define dTagLenHour      (60.0f*dTagLenMinu)
#define dTagLenDay       (24.0f*dTagLenHour)
#define dOffstDay        (-12.0f*dTagLenDay)
#define dTagLenMont      (29.530588853f*dTagLenDay)   /* 月球周年 */
#define dOffstMont       (3.0f*dTagLenMont + 17.0f*dTagLenDay)
#define dTagLnMonDiffYr  (0.000000002162f*dTagLenDay)  /* from J2000 + */
#define dTagLenMercury   (87.9691f*dTagLenDay)    /* 水星周年 */
#define dTagLenVenus     (224.701f*dTagLenDay)    /* 金星周年 */
#define dTagLenYear      (365.242189670f*dTagLenDay)  /* 地球周年 */
#define dTagLenSlrTrm    (15.0f*dTagLenDay)
#define dOffstSlrTrm     (9.0f*dTagLenSlrTrm + 10.0f*dTagLenDay - 49.0f*dTagLenMinu)
#define dTagLenSeason    (dTagLenYear/4.0f)
#define dOffstSeason     (1.0f*dTagLenSeason + 10.0f*dTagLenDay - 13.0f*dTagLenHour - 4.0f*dTagLenMinu)
#define dTagLenYrDiffDy  (0.00000006162f*dTagLenDay)  /* from J2000 - */
#define dTagLenMars      (686.971f*dTagLenDay)    /* 火星周年 */
#define dTagLenCeres     (4.06f*dTagLenYear)      /* 谷神星周年 */
#define dTagLenJupiter   (11.862615f*dTagLenYear) /* 木星周年 */
#define dTagLenSaturn    (29.447498f*dTagLenYear) /* 土星周年 */
#define dTagLenUranus    (84.016846f*dTagLenYear) /* 天王星周年 */
#define dTagLenNeptune   (164.79132f*dTagLenYear) /* 海王星周年 */
#define dTagLenPluto     (247.92065f*dTagLenYear)   /* 冥王星周年 */
#define dTagLenHaumea    (283.28f*dTagLenYear)      /* 妊神星周年 */
#define dTagLenMakemake  (305.5f*dTagLenYear)       /* 鸟神星周年 */
#define dTagLenEris      (556.901f*dTagLenYear)     /* 阋神星周年 */
#define dTagLenAxial     (25771.5f*dTagLenYear)
#define dTagLenWall      (dTagLenAxial/10.0f)
#define dTagLenWork      (dTagLenAxial/5.0f)
#define dTagLenMeta      (129600.0f*dTagLenYear)
#define dTagLenMax       (3.1415926f*2.718281828f*dTagLenMeta)



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
#define dNumTagle        (dIndxMeta     + 1L)


typedef struct typMetaTag
{
    uint64_t Num   ;
    uint64_t CurLen;
    uint64_t DiffA ;
    uint64_t Length;
    uint64_t DiffL ;
}T_MetaTag;

typedef struct typTagCfg
{
    uint64_t Start ;
    double   Length;
    int      RstIdx;
}T_TagCfg;

class CMetaClndr
{
public:
    CMetaClndr(const uint64_t& iBeginToRefMil,
               const uint64_t& iAfterRefMil);
    void Refrash(void);
    uint64_t Num(const int iIdx);
    std::string StrClndr(void);
    ~CMetaClndr(void);
private:
    T_MetaTag MtTagLst[dNumTagle];
    void InitTagLst(void);
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

static const T_TagCfg scTagCfgLst[dNumTagle] = {
    { 0U, dTagLenMili    ,dIndxSeco },
    { 0U, dTagLenSeco    ,dIndxMinu },
    { 0U, dTagLenMinu    ,dIndxHour },
    { 0U, dTagLenHour    ,dIndxDay  },
    { 1U, dTagLenDay     ,dIndxMont },
    { 1U, dTagLenSlrTrm  ,dIndxYear },
    { 1U, dTagLenMont    ,dIndxYear },
    { 1U, dTagLenMercury ,dIndxWall },
    { 1U, dTagLenSeason  ,dIndxYear },
    { 1U, dTagLenVenus   ,dIndxWall },
    { 1U, dTagLenYear    ,dIndxWall },
    { 1U, dTagLenMars    ,dIndxWall },
    { 1U, dTagLenCeres   ,dIndxWall },
    { 1U, dTagLenJupiter ,dIndxWall },
    { 1U, dTagLenSaturn  ,dIndxWall },
    { 1U, dTagLenUranus  ,dIndxWall },
    { 1U, dTagLenNeptune ,dIndxWall },
    { 1U, dTagLenPluto   ,dIndxWall },
    { 1U, dTagLenHaumea  ,dIndxWork },
    { 1U, dTagLenMakemake,dIndxWork },
    { 1U, dTagLenEris    ,dIndxWork },
    { 0U, dTagLenWall    ,dIndxAxial},
    { 1U, dTagLenWork    ,dIndxAxial},
    { 1U, dTagLenAxial   ,dIndxMeta },
    { 1U, dTagLenMeta    ,dIndxMeta },
};
  
static const std::string scnstKeyTag[dNumTagle] = {
    "毫","秒","分","时","日","气","月","水","季","金","年","火","谷","木","土","天","海","冥","妊","鸟","阋","垣","行","纪","元",
};

static CMetaClndr* sclsMetaClndr = NULL;


CMetaClndr::CMetaClndr(const uint64_t& iBeginToRefMil,
                       const uint64_t& iAfterRefMil) {
    InitTagLst();
    SetBegin(iBeginToRefMil);
    Adjust(iAfterRefMil);
}

uint64_t CMetaClndr::DoubleToUint(const double iDblVal) {
    double IntPart, FracPart;
    FracPart = std::modf(iDblVal, &IntPart);
    return static_cast<uint64_t>(IntPart);
}

void CMetaClndr::InitTagLst(void) {
    for(int i=0L; dNumTagle > i; i++) {
        MtTagLst[i].Num = scTagCfgLst[i].Start;
        double IntPart, FracPart;
        FracPart = std::modf(scTagCfgLst[i].Length, &IntPart);
        MtTagLst[i].Length = static_cast<uint64_t>(IntPart);
        MtTagLst[i].DiffL = dTagDifSgt*static_cast<uint64_t>(FracPart);
        MtTagLst[i].DiffA = 0U;
        MtTagLst[i].CurLen = 0U;
    }
    return;
}

void CMetaClndr::SetBegin(const uint64_t& iBeginToRefMil) {
    for(int i=dNumTagle-1L; 0L<=i; i--) {
        uint64_t BeginToRefMil = iBeginToRefMil;
        (dIndxSeason == i)?(BeginToRefMil += DoubleToUint(dOffstSeason)):(0L);
        (dIndxSlrTrm == i)?(BeginToRefMil += DoubleToUint(dOffstSlrTrm)):(0L);
        (dIndxMont   == i)?(BeginToRefMil += DoubleToUint(dOffstMont  )):(0L);
        (dIndxDay    == i)?(BeginToRefMil += DoubleToUint(dOffstDay   )):(0L);
        uint64_t CycCnt = BeginToRefMil/MtTagLst[scTagCfgLst[i].RstIdx].Length;
        uint64_t TagCnt = (CycCnt*MtTagLst[scTagCfgLst[i].RstIdx].Length + \
                          CycCnt*MtTagLst[scTagCfgLst[i].RstIdx].DiffL/dTagDifSgt)/MtTagLst[i].Length;
        uint64_t CurLen = BeginToRefMil - TagCnt*MtTagLst[i].Length;
        MtTagLst[i].Num = scTagCfgLst[i].Start + CurLen/MtTagLst[i].Length;
        MtTagLst[i].CurLen = CurLen%MtTagLst[i].Length;
        MtTagLst[i].DiffA = (BeginToRefMil/MtTagLst[i].Length*MtTagLst[i].DiffA)%dTagDifSgt;
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
    MtTagLst[dIndxMont].CurLen += static_cast<uint64_t>(dTagLnMonDiffYr*static_cast<double>(iAfterRefMil/dTagLenYear));
    MtTagLst[dIndxYear].CurLen -= static_cast<uint64_t>(dTagLenYrDiffDy*static_cast<double>(iAfterRefMil/dTagLenDay));
    return;
}

void CMetaClndr::Refrash(void) {
    for(int i=0L; dNumTagle>i; i++) {
        MtTagLst[i].CurLen++;
        MtTagLst[i].DiffA += MtTagLst[i].DiffL;
        if(dTagDifSgt <= MtTagLst[i].DiffA) {
            MtTagLst[i].CurLen--;
            MtTagLst[i].DiffA -= MtTagLst[i].DiffL;
        }
        if(0U == MtTagLst[i].CurLen%MtTagLst[i].Length) {
            MtTagLst[i].Num++;
            if(MtTagLst[scTagCfgLst[i].RstIdx].Length <= (MtTagLst[i].Num - scTagCfgLst[i].Start)*scTagCfgLst[i].Length) {
                MtTagLst[i].Num = scTagCfgLst[i].Start;
                MtTagLst[i].CurLen = 0U;
            }
            if(dIndxDay == i) {
                MtTagLst[dIndxYear].Length -= DoubleToUint(dTagLenYrDiffDy);
            }
            if(dIndxYear == i) {
                MtTagLst[dIndxMont].Length += DoubleToUint(dTagLnMonDiffYr);
            }
        }
    }
    return;
}

uint64_t CMetaClndr::Num(const int iIdx) {
    return MtTagLst[iIdx].Num;
}

std::string CMetaClndr::StrClndr(void) {
    std::ostringstream Oss;
    for(int i=dNumTagle-1L; 0L<=i; i--) {
        Oss << Num(i) << scnstKeyTag[i];
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
    return sclsMetaClndr->Num(dIndxYear);
}

uint64_t C_Core_MetaClndr::Mont(void) {
    return sclsMetaClndr->Num(dIndxMont);
}

uint64_t C_Core_MetaClndr::Day (void) {
    return sclsMetaClndr->Num(dIndxDay );
}

uint64_t C_Core_MetaClndr::Hour(void) {
    return sclsMetaClndr->Num(dIndxHour);
}

uint64_t C_Core_MetaClndr::Minu(void) {
    return sclsMetaClndr->Num(dIndxMinu);
}

uint64_t C_Core_MetaClndr::Seco(void) {
    return sclsMetaClndr->Num(dIndxSeco);
}

uint64_t C_Core_MetaClndr::Mili(void) {
    return sclsMetaClndr->Num(dIndxMili);
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
