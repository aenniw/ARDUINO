#include "Service.h"

#define LOG_MAX_LEN 64

static Print *stream = nullptr;

void __log_init__(Print *printer) { stream = printer; }

void __log__(const __FlashStringHelper *fmt, ...) {
    if (!stream) return;
    char buf[LOG_MAX_LEN];
    sprintf_P(buf, (PGM_P) F("%010ld | "), millis());
    stream->print(buf);
    va_list args;
    va_start(args, fmt);
    vsnprintf_P(buf, LOG_MAX_LEN, (PGM_P) fmt, args);
    va_end(args);
    stream->println(buf);
}