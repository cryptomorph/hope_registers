ACS CT-API Windows Library
Advanced Card Systems Ltd.



Introduction
------------

This library provides CT-API functions for communicating with ACS smart card
readers on Windows.



Release Notes
-------------

Version:      1.0.1.1
Release Date: 23/1/2017

System Requirements

- Windows XP SP3 or above

Development Environment

- Visual Studio 2010 or above

Supported Readers

- ACS smart card readers

Supported CT-BCS Commands

- RESET
- RESET CT
- REQUEST ICC
- GET STATUS
- DEACTIVATE ICC
- EJECT ICC



Installation
------------

1. Copy a DLL file "ctacs.dll" to the working directory of your application.

2. Create a INI file "ctacs.ini" in the working directory.

3. Edit the INI file. To define a card terminal, you need to add a entry with
   PC/SC reader name.

; Sample ctacs.ini (Windows)

[CardTerminal]
CTN1=ACR38U-CCID
CTN2=ACR128U

[ACR38U-CCID]
ICC1=ACS CCID USB Reader 0

[ACR128U]
ICC1=ACS ACR128U ICC Interface 0
ICC2=ACS ACR128U PICC Interface 0
ICC3=ACS ACR128U SAM Interface 0



History
-------

v1.0.1.1 (23/1/2017)
- Fix typos in ct_api.h.
- Fix a bug that the key value will be ignored in the INI file "ctacs.ini" on
  Linux/Mac OS X if it contains '[' and ']'.
- Update MANUFACTURER_DATA to "HKACS  ACR1.0.1".

v1.0.0.1 (12/9/2011)
- New release.



File Contents
-------------

API Documentation:      ctacs.chm
Demo Application (x86): Demo\i386
Demo Application (x64): Demo\AMD64
Header Files:           include
Library Files (x86):    lib\i386
Library Files (x64):    lib\AMD64
Script Files:           scripts



Support
-------

In case of problem, please contact ACS through:

Web Site: http://www.acs.com.hk/
E-mail: info@acs.com.hk
Tel: +852 2796 7873
Fax: +852 2796 1286



-------------------------------------------------------------------------------
Copyright (C) 2011-2017 Advanced Card Systems Ltd. All Rights Reserved.

Copyright
Copyright by Advanced Card Systems Ltd. (ACS) No part of this reference manual
may be reproduced or transmitted in any from without the expressed, written
permission of ACS.

Notice
Due to rapid change in technology, some of specifications mentioned in this
publication are subject to change without notice. Information furnished is
believed to be accurate and reliable. ACS assumes no responsibility for any
errors or omissions, which may appear in this document.
