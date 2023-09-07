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
#define USE_RESIZE_SIGNAL


#define USE_GUI_MODULE


// Singleton 기능 관련
#define USE_INTANCE_SINGLETON
#if defined(USE_INTANCE_SINGLETON)
#define USE_INTANCE_SINGLETON_GUI
#endif



// Excel 기능 관련
#define USE_EXCEL_FUNCTION_NORMAL                  // Excel Cell Merge/Split/Edit 기능 초기 버전 사용
#define USE_EXCEL_CELL_INFO_TEMP                   // Excel Cell 정보 구성서 임시 텍스트 사용
#define USE_AUTO_COMPLETE_NORMAL                   // Excel Text 자동완성 기능 초기 버전 사용









// #define USE_DEMO








#endif    // COMMON_DEFINE_H
