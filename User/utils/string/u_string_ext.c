#include "u_string_ext.h"


int32_t shortest_substring(const char *str, const char *start_sub, const char *end_sub, char *result, int32_t max_len) {
    const char *start = strstr(str, start_sub);
    if (start == NULL) {
        return -1;
    }
    const char *end = strstr(start, end_sub);
    if (end == NULL) {
        return -2;
    }

    int32_t len = end + strlen(end_sub) - start;

    if (len > max_len) {
        return -99;
    }
    memcpy(result, start, len);
    result[len+1] = '\0';
    return len;
}
