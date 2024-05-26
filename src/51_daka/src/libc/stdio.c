#include <libc/stddef.h>
#include <libc/stdarg.h>
#include <libc/limits.h>
#include <libc/stdbool.h>
#include <libc/string.h>
#include <libc/system.h>

extern void screen_put(char c);

int putchar(int ic) {
    char c = (char) ic;
    screen_put(c);
    return ic;
}

bool print(const char* data, size_t length) {
    const unsigned char* bytes = (const unsigned char*) data;
    for (size_t i = 0; i < length; i++) {
        if (putchar(bytes[i]) == EOF) {
            return false;
        }
    }
    return true;
}

int printf(const char* __restrict__ format, ...) {
    va_list parameters;
    va_start(parameters, format);

    int written = 0;

    while (*format != '\0') {
        size_t maxrem = INT_MAX - written;

        if (format[0] != '%' || format[1] == '%') {
            if (format[0] == '%') {
                format++;
            }
            size_t amount = 1;
            while (format[amount] && format[amount] != '%') {
                amount++;
            }
            if (maxrem < amount) {
                // TODO: Set errno to EOVERFLOW.
                return -1;
            }
            if (!print(format, amount)) {
                return -1;
            }
            format += amount;
            written += amount;
            continue;
        }

        const char* format_begun_at = format++;

        if (*format == 'c') {
            format++;
            char c = (char) va_arg(parameters, int /* char promotes to int */);
            if (maxrem < 1) {
                // TODO: Set errno to EOVERFLOW.
                return -1;
            }
            if (!print(&c, sizeof(c))) {
                return -1;
            }
            written++;
        } else if (*format == 's') {
            format++;
            const char* str = va_arg(parameters, const char*);
            size_t len = strlen(str);
            if (maxrem < len) {
                // TODO: Set errno to EOVERFLOW.
                return -1;
            }
            if (!print(str, len)) {
                return -1;
            }
            written += len;
        } else if (*format == 'd') {
            format++;
            int num = va_arg(parameters, int);
            char buffer[20];
            int i = 0;
            bool isNegative = num < 0;
            if (num == 0) {
                buffer[i++] = '0';
            } else {
                if (isNegative) {
                    buffer[i++] = '-';
                    num = -num;
                }
                int start = i;
                while (num != 0) {
                    buffer[i++] = (num % 10) + '0';
                    num /= 10;
                }
                int end = i - 1;
                while (start < end) {
                    char temp = buffer[start];
                    buffer[start] = buffer[end];
                    buffer[end] = temp;
                    start++;
                    end--;
                }
            }
            if (!print(buffer, i)) {
                return -1;
            }
            written += i;
        } else if (*format == 'x') {
            format++;
            unsigned int num = va_arg(parameters, unsigned int);
            char buffer[20];
            int i = 0;
            if (num == 0) {
                buffer[i++] = '0';
            } else {
                while (num != 0) {
                    int rem = num % 16;
                    if (rem < 10) {
                        buffer[i++] = rem + '0';
                    } else {
                        buffer[i++] = rem - 10 + 'a';
                    }
                    num /= 16;
                }
                int start = 0;
                int end = i - 1;
                while (start < end) {
                    char temp = buffer[start];
                    buffer[start] = buffer[end];
                    buffer[end] = temp;
                    start++;
                    end--;
                }
            }
            if (!print(buffer, i)) {
                return -1;
            }
            written += i;
        } else {
            format = format_begun_at;
            size_t len = strlen(format);
            if (maxrem < len) {
                // TODO: Set errno to EOVERFLOW.
                return -1;
            }
            if (!print(format, len)) {
                return -1;
            }
            written += len;
            format += len;
        }
    }

    va_end(parameters);
    return written;
}