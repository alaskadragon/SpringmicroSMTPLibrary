#pragma once

#pragma once

#ifdef _WIN32
#ifdef SPRINGMICROSMTPLIBRARY_EXPORTS
#define SPRING_SMTP_API __declspec(dllexport)
#else
#define SPRING_SMTP_API __declspec(dllimport)
#endif
#else
#define SPRING_SMTP_API
#endif

// Return 0 on success. Non-zero means failure.
// error_out: optional buffer to receive a short message (UTF-8).
extern "C" SPRING_SMTP_API int smtp_send_text(
    const char* smtp_url,          // e.g. "smtp://smtp.gmail.com:587"
    const char* username,          // e.g. "you@gmail.com"
    const char* password,          // app password recommended
    const char* mail_from,         // e.g. "<you@gmail.com>"
    const char* rcpt_to,           // e.g. "<friend@example.com>"
    const char* subject,           // e.g. "Hello"
    const char* body,              // plain text body
    char* error_out,               // optional
    int error_out_bytes            // size of error_out buffer
);
