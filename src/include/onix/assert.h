#ifndef ONIX_ASSERT_H
#define ONIX_ASSERT_H

#define MAG_CH_PANIC '\002'
#define MAG_CH_ASSERT '\003'

void assertion_failure(char *exp, char *file, char *base_file, int line);
#define assert(exp) \
    if (exp)        \
        ;           \
    else            \
        assertion_failure(#exp, __FILE__, __BASE_FILE__, __LINE__)

void panic(const char *format, ...);

#endif