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
                     const uint64_t& iAfterRefMl);
    static const T_Core_RefDateTime* RefDateTime(void);
    void Refersh(void);
    uint64_t Prim(void);
    uint64_t Peri(void);
    uint64_t Asse(void);
    uint64_t Oper(void);
    uint64_t Era (void);
    uint64_t Year(void);
    uint64_t Mont(void);
    uint64_t Day (void);
    uint64_t Hour(void);
    uint64_t Minu(void);
    uint64_t Seco(void);
    uint64_t Mili(void);
    ~C_Core_MetaClndr(void);
    std::string GetCalendar(void);
};

#endif // C_CORE_METACLNDR_H
