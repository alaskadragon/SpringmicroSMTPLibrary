
#include "pch.h"
#include <cstring>
#include <string>
#include "spring_smtp_lib.h"


struct UploadStatus {
    size_t bytes_read = 0;
    std::string payload;
};

static size_t payload_source(char* ptr, size_t size, size_t nmemb, void* userp) {
    auto* upload = static_cast<UploadStatus*>(userp);
    const size_t buffer_size = size * nmemb;

    if (upload->bytes_read >= upload->payload.size()) return 0;

    const size_t remaining = upload->payload.size() - upload->bytes_read;
    const size_t to_copy = (remaining < buffer_size) ? remaining : buffer_size;

    std::memcpy(ptr, upload->payload.data() + upload->bytes_read, to_copy);
    upload->bytes_read += to_copy;
    return to_copy;
}

static void write_error(char* out, int out_bytes, const char* msg) {
    if (!out || out_bytes <= 0) return;
    //std::strncpy(out, msg ? msg : "Unknown error", static_cast<size_t>(out_bytes) - 1);
    strncpy_s(out, out_bytes, msg ? msg : "Unknown error", _TRUNCATE);
    out[out_bytes - 1] = '\0';
}

extern "C" SPRING_SMTP_API int smtp_send_text(
    const char* smtp_url,
    const char* username,
    const char* password,
    const char* mail_from,
    const char* rcpt_to,
    const char* subject,
    const char* body,
    char* error_out,
    int error_out_bytes
) {
    if (!smtp_url || !username || !password || !mail_from || !rcpt_to || !subject || !body) {
        write_error(error_out, error_out_bytes, "Missing required argument(s).");
        return 1;
    }

    // RFC 5322 requires CRLF line endings.
    std::string message;
    message.reserve(1024);
    message += "To: ";
    message += rcpt_to;
    message += "\r\nFrom: ";
    message += mail_from;
    message += "\r\nSubject: ";
    message += subject;
    message += "\r\n\r\n";
    message += body;
    message += "\r\n";

    CURLcode global_init_res = curl_global_init(CURL_GLOBAL_DEFAULT);
    if (global_init_res != CURLE_OK) {
        write_error(error_out, error_out_bytes, "curl_global_init failed.");
        return 2;
    }

    CURL* curl = curl_easy_init();
    if (!curl) {
        curl_global_cleanup();
        write_error(error_out, error_out_bytes, "curl_easy_init failed.");
        return 3;
    }

    struct curl_slist* recipients = nullptr;
    recipients = curl_slist_append(recipients, rcpt_to);

    UploadStatus upload;
    upload.payload = message;

    curl_easy_setopt(curl, CURLOPT_URL, smtp_url);
    curl_easy_setopt(curl, CURLOPT_USERNAME, username);
    curl_easy_setopt(curl, CURLOPT_PASSWORD, password);

    // STARTTLS or SMTPS, depending on URL/port. Safe default:
    curl_easy_setopt(curl, CURLOPT_USE_SSL, (long)CURLUSESSL_ALL);

    // Keep verification enabled.
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 1L);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 2L);

    curl_easy_setopt(curl, CURLOPT_MAIL_FROM, mail_from);
    curl_easy_setopt(curl, CURLOPT_MAIL_RCPT, recipients);

    curl_easy_setopt(curl, CURLOPT_READFUNCTION, payload_source);
    curl_easy_setopt(curl, CURLOPT_READDATA, &upload);
    curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);

    // Optional: timeouts
    curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 15L);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 60L);

    curl_easy_setopt(curl, CURLOPT_CAINFO, "cacert.pem");


    CURLcode res = curl_easy_perform(curl);

    int ret = 0;
    if (res != CURLE_OK) {
        write_error(error_out, error_out_bytes, curl_easy_strerror(res));
        ret = 4;
    }

    curl_slist_free_all(recipients);
    curl_easy_cleanup(curl);
    curl_global_cleanup();
    return ret;
}
