#ifndef COMMON_RESOURCE_H
#define COMMON_RESOURCE_H

#include "CommonDefine.h"
#include <QObject>




// =============================================================================================================
// String Define
// =============================================================================================================
#define STRING_INVALID                             QString("invalid")
#if 1    // defined(USE_MULTI_LANGUAGE_ENG)
#define STRING_FILE                                QString("File")
#define STRING_NEW                                 QString("New")
#define STRING_NEW_TIP                             QString("Create a new file")
#define STRING_OPEN                                QString("Open")
#define STRING_OPEN_TIP                            QString("Open an existing file")
#define STRING_SAVE                                QString("Save")
#define STRING_SAVE_TIP                            QString("Save the document to disk")
#define STRING_SAVE_AS                             QString("Save As")
#define STRING_SAVE_AS_TIP                         QString("Save the document under a new name")
#define STRING_SAVE_EXIT                           QString("Exit")
#define STRING_SAVE_EXIT_TIP                       QString("Exit the application")
#define STRING_EDIT                                QString("Edit")
#define STRING_CUT                                 QString("Cut")
#define STRING_CUT_TIP                             QString("Cut the current selection's contents to the clipboard")
#define STRING_COPY                                QString("Copy")
#define STRING_COPY_TIP                            QString("Copy the current selection's contents to the clipboard")
#define STRING_PASTE                               QString("Paste")
#define STRING_PASTE_TIP                           QString("Paste the clipboard's contents into the current selection")
#define STRING_VIEW                                QString("View")
#define STRING_CONFIG                              QString("Config")
#define STRING_CONFIG_TIP                          QString("View config infomation")
#define STRING_NODE_ADDRESS                        QString("Node Address")
#define STRING_NODE_ADDRESS_TIP                    QString("View node address infomation")
#define STRING_PYTHON                              QString("Python")
#define STRING_PYTHON_TIP                          QString("View python lib install infomation")
#define STRING_SETTING                             QString("Setting")
#define STRING_DEFAULT_PATH                        QString("Default Path")
#define STRING_DEFAULT_PATH_TIP                    QString("Setting default SFC test output foler")
#define STRING_NODE_PATH                           QString("Node Path")
#define STRING_NODE_PATH_TIP                       QString("Setting node address file path")
#define STRING_VSM_PATH                            QString("VSM Path")
#define STRING_VSM_PATH_TIP                        QString("Setting VSM file path")
#define STRING_TEST_REPORT                         QString("Tes Report")
#define STRING_TEST_REPORT_TIP                     QString("Test report setting")
#define STRING_REPORT                              QString("Report")
#define STRING_REPORT_TIP                          QString("Setting SFC test result info")
#define STRING_RESULT                              QString("Result")
#define STRING_RESULT_TIP                          QString("Setting SFC test result info")
#define STRING_COVERAGE                            QString("Coverage")
#define STRING_COVERAGE_TIP                        QString("Setting SFC test result info")
#define STRING_RUN                                 QString("Run")
#define STRING_RUN_TIP                             QString("Run SFC test case file")
#define STRING_RUN_TC                              QString("Run TC")
#define STRING_RUN_TC_TIP                          QString("Run TC file")
#define STRING_GENERATE_TC                         QString("Generate TC")
#define STRING_GENERATE_TC_TIP                     QString("Generate TC file")
#define STRING_GENERATE_REPORT                     QString("Generate Report")
#define STRING_GENERATE_REPORT_TIP                 QString("Generate Report file")
#define STRING_TEST_RESULT                         QString("Test Result")
#define STRING_TEST_RESULT_TIP                     QString("<b>Edit --></b> Test Result Tip")
#define STRING_TEST_RESULT_COVERAGE                QString("Test Coverage")
#define STRING_TEST_RESULT_COVERAGE_TIP            QString("<b>Edit --></b> Test Coverage Tip")
#define STRING_HELP                                QString("Help")
#define STRING_ABOUT                               QString("About")
#define STRING_ABOUT_TIP                           QString("Show the application's About box")
#define STRING_POPUP_ABOUT                         QString("About Application")
#define STRING_POPUP_ABOUT_TIP                     QString("This <b>Application</b> is a program for <b>SFC Test</b> \
                                                            (Excel conversion, parsing, etc.)")
#define STRING_ABOUT_QT                            QString("About Qt")
#define STRING_ABOUT_QT_TIP                        QString("Show the Qt library's About box")
#define STRING_FILE_OPEN                           QString("File Open")
#define STRING_FILE_OPEN_FAIL                      QString("Fail to file open")
#else
#define STRING_FILE                                QString("파일")
#define STRING_NEW                                 QString("새로 만들기")
#define STRING_NEW_TIP                             QString("새로운 파일 만들기")
#define STRING_OPEN                                QString("열기")
#define STRING_OPEN_TIP                            QString("이전 파일에서 열기")
#define STRING_SAVE                                QString("저장")
#define STRING_SAVE_TIP                            QString("새로운 문서 파일로 저장하기")
#define STRING_SAVE_AS                             QString("다른 이름으로 저장")
#define STRING_SAVE_AS_TIP                         QString("다른 이름으로 저장하기")
#define STRING_SAVE_EXIT                           QString("종료")
#define STRING_SAVE_EXIT_TIP                       QString("어플리케이션 종료하기")
#define STRING_EDIT                                QString("편집")
#define STRING_CUT                                 QString("잘라내기")
#define STRING_CUT_TIP                             QString("선택한 영역을 잘라내기 하여 클리보드에 복사하기")
#define STRING_COPY                                QString("복사")
#define STRING_COPY_TIP                            QString("선택한 영역을 복사하기 하여 클리보드에 복사하기")
#define STRING_PASTE                               QString("붙여넣기")
#define STRING_PASTE_TIP                           QString("클리보드에 복사되어 있는 내용을 붙여넣기")
#define STRING_VIEW                                QString("보기")
#define STRING_CONFIG                              QString("설정")
#define STRING_CONFIG_TIP                          QString("설정 정보를 표시 하기")
#define STRING_NODE_ADDRESS                        QString("노드 주소")
#define STRING_NODE_ADDRESS_TIP                    QString("노드 주소 정보를 표시 하기")
#define STRING_PYTHON                              QString("파이썬")
#define STRING_PYTHON_TIP                          QString("파이썬 라이브러리 설치 정보를 표시 하기")
#define STRING_SETTING                             QString("설정")
#define STRING_DEFAULT_PATH                        QString("기본 동작 위치")
#define STRING_DEFAULT_PATH_TIP                    QString("기본 동작 폴더를 설정하기")
#define STRING_NODE_PATH                           QString("노드 파일 위치")
#define STRING_NODE_PATH_TIP                       QString("노드 파일 위치 설정하기")
#define STRING_REPORT                              QString("설정")
#define STRING_REPORT_TIP                          QString("SFC 테스트 결과 정보 설정하기")
#define STRING_TEST_REPORT                         QString("테스트 리포트")
#define STRING_TEST_RESULT                         QString("테스트 결과")
#define STRING_TEST_RESULT_TIP                     QString("<b>Edit --></b> Test Result Tip")
#define STRING_TEST_RESULT_COVERAGE                QString("테스트 범위")
#define STRING_TEST_RESULT_COVERAGE_TIP            QString("<b>Edit --></b> Test Coverage Tip")
#define STRING_HELP                                QString("도움")
#define STRING_ABOUT                               QString("정보")
#define STRING_ABOUT_TIP                           QString("프로그램 정보를 표시하기")
#define STRING_POPUP_ABOUT                         QString("프로그램 정보")
#define STRING_POPUP_ABOUT_TIP                     QString("This <b>Application</b> is a program for <b>SFC Test</b> \
                                                            (Excel conversion, parsing, etc.)")
#define STRING_ABOUT_QT                            QString("Qt 정보")
#define STRING_ABOUT_QT_TIP                        QString("Qt 라이브러리 정보 화면 표시 하기")
#define STRING_FILE_OPEN                           QString("파일 열기")
#define STRING_FILE_OPEN_FAIL                      QString("파일 열기 실패")

#endif
#define STRING_POPUP_SAVE                          QString("저장")
#define STRING_POPUP_DISCARD                       QString("저장 안함")
#define STRING_POPUP_CANCEL                        QString("취소")
#define STRING_POPUP_SAVE_FILE                     QString("파일 저장")
#define STRING_POPUP_SAVE_FILE_TIP                 QString("변경 내용을 저장하시겠습니까?")

#define STRING_POPUP_INSTALL                       QString("설치")
#define STRING_POPUP_CONFIRM                       QString("확인")
#define STRING_POPUP_LIB                           QString("라이브러리")
#define STRING_POPUP_CELL_COLUMN                   QString("병합/해제 셀 선택")
#define STRING_POPUP_NO_INSTALL_LIB_TIP            QString("파이썬 관련 라이브러리가 설치 되지 않았습니다.")
#define STRING_POPUP_NOW_INSTALLING_TIP            QString("파이썬 관련 라이브러리가 설치 하는 중입니다.")
#define STRING_POPUP_INSTALL_COMPLETE_TIP          QString("파이썬 관련 라이브러리가 설치가 완료 되었습니다.")
#define STRING_POPUP_CELL_COLUMN_TIP               QString("병합/해제 하기 위한 셀의 선택 범위가 잘못 되었습니다. \
                                                            병합/해제 가능한 영역으로 다시 선택 하세요.")
















// =============================================================================================================
// Image Define
// =============================================================================================================
#define IAMGE_NEW                                  ":/Image/Menu/New.png"
#define IAMGE_OPEN                                 ":/Image/Menu/Open.png"
#define IAMGE_SAVE                                 ":/Image/Menu/Save.png"
#define IAMGE_CUT                                  ":/Image/Menu/Cut.png"
#define IAMGE_COPY                                 ":/Image/Menu/Copy.png"
#define IAMGE_PASTE                                ":/Image/Menu/Paste.png"
#define IAMGE_LOADING                              ":/Image/Popup/Loading.png"
#define IAMGE_LOADING_GIF                          ":/Image/Popup/Loading.gif"








































#endif  // COMMON_RESOURCE_H
