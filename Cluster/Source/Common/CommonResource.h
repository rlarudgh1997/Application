#ifndef COMMON_RESOURCE_H
#define COMMON_RESOURCE_H

#include "CommonDefine.h"
#include <QObject>




// =============================================================================================================
// String Define
// =============================================================================================================
#define STRING_INVALID                             QString("invalid")
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
#define STRING_NODE_ADDRESS                        QString("Node")
#define STRING_NODE_ADDRESS_TIP                    QString("View node address infomation")
#define STRING_PYTHON                              QString("Python")
#define STRING_PYTHON_TIP                          QString("View python lib install infomation")
#define STRING_SETTING                             QString("Setting")
#define STRING_DEFAULT_PATH                        QString("Default Path")
#define STRING_DEFAULT_PATH_TIP                    QString("Setting default SFC test output foler")
#define STRING_NODE_PATH                           QString("Node Path")
#define STRING_NODE_PATH_TIP                       QString("Setting node address file path")
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







#define STRING_POPUP_SAVE                          QString("Save")
#define STRING_POPUP_DISCARD                       QString("Discard")
#define STRING_POPUP_CANCEL                        QString("Cancel")
#define STRING_POPUP_SAVE_FILE                     QString("File save")
#define STRING_POPUP_SAVE_FILE_TIP                 QString("Do you want to save your changes?")

#define STRING_POPUP_INSTALL                       QString("Install")
#define STRING_POPUP_CONFIRM                       QString("OK")
#define STRING_POPUP_LIB                           QString("Library")
#define STRING_POPUP_CELL_COLUMN                   QString("Select merged/split cells")
#define STRING_POPUP_NO_INSTALL_LIB_TIP            QString("Python-related libraries are not installed.")
#define STRING_POPUP_NOW_INSTALLING_TIP            QString("Python-related libraries are being installed.")
#define STRING_POPUP_INSTALL_COMPLETE_TIP          QString("Python-related libraries have been installed.")
#define STRING_POPUP_CELL_COLUMN_TIP               QString("Cell selection for merge/split is incorrect. \
                                                            Please select again.")
















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
