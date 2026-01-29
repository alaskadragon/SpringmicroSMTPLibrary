// SpringSmtpCli.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "spring_smtp_lib.h"

int main(int argc, char** argv) {
    if (argc < 8) {
        std::cerr
            << "Usage:\n"
            << "  SpringSmtpCli.exe <smtp_url> <username> <password> <from> <to> <subject> <body>\n"
            << "Example:\n"
            << "  SpringSmtpCli.exe smtp://smtp.gmail.com:587 you@gmail.com APPPASS "
            "\"<you@gmail.com>\" \"<friend@example.com>\" \"Test\" \"Hello\"\n";
        return 1;
    }

    char err[512] = { 0 };

    int rc = smtp_send_text(
        argv[1], argv[2], argv[3],
        argv[4], argv[5],
        argv[6], argv[7],
        err, (int)sizeof(err)
    );

    if (rc != 0) {
        std::cerr << "Send failed (" << rc << "): " << err << "\n";
        return rc;
    }

    std::cout << "Email sent.\n";
    return 0;
}



