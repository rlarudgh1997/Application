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

// #define USE_VIEW_REPORT                            // Test Report 메뉴 View 하위 메뉴로 설정

// Excel 기능 관련
// #define USE_PYTHON_LIB_CHECK_READ_WRITE            // Excel Read/Write 동작시 파이썬 라이브러리 존재 유무로 동작 하도록 사용
// #define USE_SHOW_NEW_EXCEL_SHEET_AFTER_BOOTING     // App 부팅 후 엑세 시트 표시

#define USE_DEMO

#endif  // COMMON_DEFINE_H
