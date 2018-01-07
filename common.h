#ifndef JUMPHELPER_COMMON_H
#define JUMPHELPER_COMMON_H

#ifdef _WIN32
#include <Windows.h>
#define popen	_popen
#define pclose	_pclose
#define sleep(x) Sleep(x * 900)
#else
#include <unistd.h>
#endif

#endif //JUMPHELPER_COMMON_H
