#pragma once
#ifndef HD_INC_OSLAYER_OS_WINDOWS_API_H
#define HD_INC_OSLAYER_OS_WINDOWS_API_H

#if !defined(HD_OS_WINDOWS)
#error This file must be included only when targetting Windows OS
#endif

#define WIN32_LEAN_AND_MEAN // Exclude APIs such as Cryptography, DDE, RPC, Shell, and Windows Sockets.
#define NOCOMM // Excludes the serial communication API

#include <Windows.h>

#undef assert

#endif // HD_INC_OSLAYER_OS_WINDOWS_API_H