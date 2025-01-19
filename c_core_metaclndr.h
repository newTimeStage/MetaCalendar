#ifndef C_CORE_METACLNDR_H
#define C_CORE_METACLNDR_H

#include <stdint.h>
#include <string>

typedef struct strct_Core_RefDateTime
{
    uint16_t Year;
    uint8_t  Month;
    uint8_t  Day;
    uint8_t  Hour;
    uint8_t  Minute;
    uint8_t  Second;
}T_Core_RefDateTime;

class C_Core_MetaClndr
{
public:
    C_Core_MetaClndr(const uint64_t& iBeginToRefMl,
                     const uint64_t& iAfterRefMl = 0U);
    static const T_Core_RefDateTime* RefDateTime(void);
    void Refersh(void);
    uint64_t Year(void);
    uint64_t Mont(void);
    uint64_t Day (void);
    uint64_t Hour(void);
    uint64_t Minu(void);
    uint64_t Seco(void);
    uint64_t Mili(void);
    ~C_Core_MetaClndr(void);
    std::string GetCalendar(void);
    static std::string TransFromMil(const uint64_t& iMilSt,
                                    const uint64_t& iMilAj = 0U);
};

#endif // C_CORE_METACLNDR_H
