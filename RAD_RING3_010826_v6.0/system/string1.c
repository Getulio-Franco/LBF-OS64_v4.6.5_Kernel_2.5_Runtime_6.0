/**
 * ============================================================================
 * STRING & MEMORY UTILS - CABEÇALHO (V1.2) data 22/06/26
 * ============================================================================
 */

#include "string.h"

// =========================================================================
// 🧠 Implementações de Gerenciamento de Memória (Otimizadas para 64 bits)
// =========================================================================

void* memset(void* s, int c, size_t n) {
    unsigned char* p = (unsigned char*)s;
    while (n--) *p++ = (unsigned char)c;
    return s;
}

void* memcpy(void* dest, const void* src, size_t n) {
    uint8_t* d8 = (uint8_t*)dest;
    const uint8_t* s8 = (const uint8_t*)src;

    // OTIMIZAÇÃO EM 64-BITS: Se os endereços e o tamanho forem múltiplos de 8 bytes,
    // copia usando registradores de 64 bits nativos da CPU do LBF OS.
    if (((uintptr_t)dest % 8 == 0) && ((uintptr_t)src % 8 == 0) && (n % 8 == 0)) {
        uint64_t* d64 = (uint64_t*)dest;
        const uint64_t* s64 = (const uint64_t*)src;
        size_t n64 = n / 8;
        while (n64--) *d64++ = *s64++;
    } 
    // Sub-otimização se for múltiplo de 4 bytes
    else if (((uintptr_t)dest % 4 == 0) && ((uintptr_t)src % 4 == 0) && (n % 4 == 0)) {
        uint32_t* d32 = (uint32_t*)dest;
        const uint32_t* s32 = (const uint32_t*)src;
        size_t n32 = n / 4;
        while (n32--) *d32++ = *s32++;
    } 
    else {
        // Fallback seguro byte a byte para ponteiros desalinhados
        while (n--) *d8++ = *s8++;
    }
    return dest;
}

void* memmove(void* dest, const void* src, size_t n) {
    unsigned char* d = (unsigned char*)dest;
    const unsigned char* s = (const unsigned char*)src;
    if (d < s) {
        while (n--) *d++ = *s++;
    } else {
        d += n;
        s += n;
        while (n--) *--d = *--s;
    }
    return dest;
}

int memcmp(const void *s1, const void *s2, size_t n) {
    const unsigned char *p1 = (const unsigned char *)s1;
    const unsigned char *p2 = (const unsigned char *)s2;
    for (size_t i = 0; i < n; i++) {
        if (p1[i] != p2[i]) return (int)p1[i] - (int)p2[i];
    }
    return 0;
}

// =========================================================================
// 🔤 Funções de String Estáveis e Seguras
// =========================================================================

size_t strlen(const char *str) {
    size_t len = 0;
    while (str && str[len]) len++;
    return len;
}

char* strcpy(char* dest, const char* src) {
    char* d = dest;
    while ((*d++ = *src++));
    return dest;
}

char* strncpy(char* dest, const char* src, size_t n) {
    size_t i;
    for (i = 0; i < n && src[i] != '\0'; i++)
        dest[i] = src[i];
    for ( ; i < n; i++)
        dest[i] = '\0';
    return dest;
}

char* strcat(char* dest, const char* src) {
    char* d = dest;
    while (*d) d++;
    while ((*d++ = *src++));
    return dest;
}

char* strncat(char* dest, const char* src, size_t n) {
    size_t dest_len = strlen(dest);
    size_t i;
    for (i = 0 ; i < n && src[i] != '\0' ; i++)
        dest[dest_len + i] = src[i];
    dest[dest_len + i] = '\0';
    return dest;
}

int strcmp(const char *s1, const char *s2) {
    while (*s1 && (*s1 == *s2)) {
        s1++; s2++;
    }
    return *(unsigned char *)s1 - *(unsigned char *)s2;
}

int strncmp(const char *s1, const char *s2, size_t n) {
    if (n == 0) return 0;
    while (n > 1 && *s1 && (*s1 == *s2)) {
        s1++; s2++; n--;
    }
    return (*(unsigned char *)s1 - *(unsigned char *)s2);
}

char* strchr(const char* s, int c) {
    while (*s != (char)c) {
        if (!*s++) return 0;
    }
    return (char*)s;
}

/**
 * @brief Localiza a primeira ocorrência da substring 'needle' dentro da string 'haystack'.
 * Crucial para o Shell identificar as extensões ".elf" no sistema de arquivos FAT32.
 */
char* strstr(const char* haystack, const char* needle) {
    if (!*needle) return (char*)haystack;
    
    while (*haystack) {
        const char* h = haystack;
        const char* n = needle;
        
        while (*h && *n && *h == *n) {
            h++;
            n++;
        }
        if (!*n) return (char*)haystack;
        haystack++;
    }
    return 0;
}

// =========================================================================
// ⚙️ Conversões de Dados e Formatação Numérica
// =========================================================================

int atoi(const char* str) {
    int res = 0;
    int sign = 1;
    if (!str) return 0;

    while (*str == ' ' || *str == '\t' || *str == '\n' || *str == '\r') str++;

    if (*str == '-') {
        sign = -1;
        str++;
    } else if (*str == '+') {
        str++;
    }

    while (*str >= '0' && *str <= '9') {
        res = res * 10 + (*str - '0');
        str++;
    }
    return sign * res;
}

void itoa(uint64_t n, char* str, int base) {
    int i = 0;
    if (n == 0) {
        str[i++] = '0';
        str[i] = '\0';
        return;
    }

    char temp[65]; 
    int j = 0;
    while (n > 0) {
        uint64_t rem = n % base;
        temp[j++] = (rem < 10) ? (rem + '0') : (rem - 10 + 'A');
        n /= base;
    }

    while (j > 0) str[i++] = temp[--j];
    str[i] = '\0';
}

void int_to_string(int n, char* buffer) {
    if (n == 0) {
        strcpy(buffer, "0");
        return;
    }
    if (n < 0) {
        *buffer++ = '-';
        n = -n;
    }
    itoa((uint64_t)n, buffer, 10);
}

void hex_to_string(uintptr_t n, char* buffer) {
    buffer[0] = '0';
    buffer[1] = 'x';
    itoa((uint64_t)n, buffer + 2, 16);
}

// =========================================================================
// ✂️ Tokenizador de Strings (Nativo / Thread-Safe Simples)
// =========================================================================

static int is_delimiter(char c, const char* delimiters) {
    while (*delimiters) {
        if (c == *delimiters) return 1;
        delimiters++;
    }
    return 0;
}

char* strtok(char* str, const char* delimiters) {
    static char* token_last_position = 0;

    if (str != 0) {
        token_last_position = str;
    }

    if (token_last_position == 0 || *token_last_position == '\0') {
        return 0;
    }

    while (*token_last_position && is_delimiter(*token_last_position, delimiters)) {
        token_last_position++;
    }

    if (*token_last_position == '\0') {
        token_last_position = 0;
        return 0;
    }

    char* token_start = token_last_position;

    while (*token_last_position && !is_delimiter(*token_last_position, delimiters)) {
        token_last_position++;
    }

    if (*token_last_position != '\0') {
        *token_last_position = '\0';
        token_last_position++;
    } else {
        token_last_position = 0;
    }

    return token_start;
}
