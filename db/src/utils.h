
#ifndef UTILS_H
#define UTILS_H

inline int atoi_safe(const char* str)
{
    if (str == NULL) {
        return 0; 
    } else {
        return atoll(str); 
    }
}

inline bool is_same_day(int32_t time1, int32_t time2)
{
    if (time1 - time2 > 86400 || time2 - time1 > 86400) {
        return false;
    }
    struct tm date1, date2;
    time_t tm1 = time1;
    time_t tm2 = time2;
    localtime_r(&tm1, &date1);
    localtime_r(&tm2, &date2);
    return (date1.tm_mday== date2.tm_mday); 
}

#endif
