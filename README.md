# Springmicro SMTP Library & CLI

This repository contains a C++ SMTP email sender built with Visual Studio on Windows.

It consists of:
- **SpringmicroSMTPlibrary**: a DLL that implements SMTP email sending using libcurl
- **SpringSmtpCli**: a command-line application that calls the DLL to send emails

The design separates reusable email logic (DLL) from the executable interface (CLI).

---

## Requirements

- Windows 10 or later
- Visual Studio 2022 (x64)
- Git
- vcpkg (for dependency management)
- libcurl (installed via vcpkg)

---

## Dependencies

This project uses **libcurl** for SMTP and TLS.

### Install libcurl with vcpkg

```powershell
git clone https://github.com/microsoft/vcpkg
cd vcpkg
.\bootstrap-vcpkg.bat
.\vcpkg install curl:x64-windows
.\vcpkg integrate install
Make sure you build the solution as x64, not Win32.

Build Instructions
Clone this repository:

git clone https://github.com/<your-org-or-username>/<repo-name>.git
Open the solution file:

SpringmicroSMTPlibrary.sln
In Visual Studio:

Configuration: Debug or Release

Platform: x64

Build the solution:

Build → Build Solution
The output directory will contain:

SpringmicroSMTPlibrary.dll

SpringSmtpCli.exe

TLS Certificate Requirement
To verify SSL certificates, libcurl needs a CA bundle.

Setup
Download cacert.pem (Mozilla CA bundle)

Place it next to SpringSmtpCli.exe

Example:

x64\Debug\
  SpringSmtpCli.exe
  SpringmicroSMTPlibrary.dll
  cacert.pem
The DLL is configured to use this file via CURLOPT_CAINFO.

Running the CLI
Syntax
SpringSmtpCli.exe <smtp_url> <username> <password> <from> <to> <subject> <body>
Example (Gmail)
SpringSmtpCli.exe smtp://smtp.gmail.com:587 `
  your_email@gmail.com `
  YOUR_APP_PASSWORD `
  "<your_email@gmail.com>" `
  "<recipient@example.com>" `
  "Test from C++ SMTP DLL" `
  "Hello, this email was sent using the C++ SMTP DLL."
Notes:

Use app passwords, not your normal email password

Email addresses must be wrapped in angle brackets

Quote arguments that contain spaces

Supported SMTP Providers
Common examples:

Gmail: smtp://smtp.gmail.com:587

Outlook / Office365: smtp://smtp.office365.com:587

Yahoo: smtp://smtp.mail.yahoo.com:587

Security Notes
Do not commit app passwords

Do not hardcode credentials

Use environment variables or command-line arguments

Do not disable TLS verification in production

Repository Structure
SpringmicroSMTPlibrary/
├── SpringmicroSMTPlibrary/   # DLL project
├── SpringSmtpCli/            # CLI project
├── .gitignore
├── README.md
└── SpringmicroSMTPlibrary.sln
Troubleshooting
SSL peer certificate error
→ Ensure cacert.pem exists and is accessible

Authentication failed
→ Use an app password, not your account password

DLL not found at runtime
→ Ensure DLL and EXE are in the same folder

License
Internal / educational use unless otherwise specified.