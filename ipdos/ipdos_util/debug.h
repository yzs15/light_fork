
#ifndef __TASKSWITCHING__DEBUG__DEBUG__
#define __TASKSWITCHING__DEBUG__DEBUG__
// #define LOG
#ifdef LOG
#define LG(format, args...) fprintf(stdout,\
"[LOG](File: %s:%d, Func: %s()): "   \
, __FILE__,__LINE__, __FUNCTION__);       \
fprintf(stdout, format"\r\n", ##args)
#else
#define LG(format, args...)  do {} while (0)
#endif

// #define DEBUG
#ifdef DEBUG
#define DBG(format, ...) fprintf(stdout, \
"[DBG](File: %s, Func: %s(), Line: %d): " \
, __FILE__, __FUNCTION__, __LINE__);     \
fprintf(stdout, format"\r\n", ##__VA_ARGS__)
#else
#define DBG(format, ...)  do {} while (0)
#endif

// #include <execinfo.h>
// #include <stdio.h>
// #include <stdlib.h>

// inline void print_trace(void) {
//     void *buffer[10];
//     int nptrs = backtrace(buffer, 10);
//     char **strings = backtrace_symbols(buffer, nptrs);
//     if (strings == NULL) {
//         perror("backtrace_symbols");
//         exit(EXIT_FAILURE);
//     }
//     for (int j = 0; j < nptrs; j++) {
//         printf("%s\n", strings[j]);
//     }
//     free(strings);
// }

#define LG_ENV(expr) {const char* env_var_name = "PYTHONLOGENV";\
    char* env_var_value = NULL; \
    env_var_value = getenv(env_var_name);\
    if (env_var_value != NULL){ \
        if (strcmp(env_var_value, "print")==0){\
        expr; \
        } \
    }}\


#endif
