#ifndef COMMON_DEFINE_H
#define COMMON_DEFINE_H

#include <QDebug>

#if defined(__BUILD_TYPE_TARGET__)
#define PLATFORM_TARGET
#else
#define PLATFORM_X86
#endif

#if defined(__LINUX__)
#define OS_LINUX
#else
#define OS_X86
#endif



// ===================================================================================================
// Feature, Funtion Define
// ===================================================================================================
#define USE_KEY_EVENT
#define USE_SCREEN_CAPTURE





#define USE_INTANCE_SINGLETON
#define USE_GUI_MODULE






















#endif    // COMMON_DEFINE_H
