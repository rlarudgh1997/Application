#ifndef COMMON_RESOURCE_H
#define COMMON_RESOURCE_H

#include "CommonDefine.h"
// #include <QObject>

// =============================================================================================================
// String Define
// =============================================================================================================
#define STRING_INVALID QString("invalid")

#define STRING_FILE QString("File")
#define STRING_NEW QString("New")
#define STRING_NEW_TIP QString("Create a new file")
#define STRING_OPEN QString("Open")
#define STRING_OPEN_TIP QString("Open an existing file")
#define STRING_SAVE QString("Save")
#define STRING_SAVE_TIP QString("Save the document to disk")
#define STRING_SAVE_AS QString("Save As")
#define STRING_SAVE_AS_TIP QString("Save the document under a new name")
#define STRING_SAVE_EXIT QString("Exit")
#define STRING_SAVE_EXIT_TIP QString("Exit the application")
#define STRING_EDIT QString("Edit")
#define STRING_CUT QString("Cut")
#define STRING_CUT_TIP QString("Cut the current selection's contents to the clipboard")
#define STRING_COPY QString("Copy")
#define STRING_COPY_TIP QString("Copy the current selection's contents to the clipboard")
#define STRING_PASTE QString("Paste")
#define STRING_PASTE_TIP QString("Paste the clipboard's contents into the current selection")
#define STRING_CELL_INSERT QString("Insert")
#define STRING_CELL_INSERT_TIP QString("")
#define STRING_CELL_DELETE QString("Delete")
#define STRING_CELL_DELETE_TIP QString("")
#define STRING_CELL_MERGE_SPLIT QString("Merge/Split")
#define STRING_CELL_MERGE_SPLIT_TIP QString("")
#define STRING_VIEW QString("View")
#define STRING_CONFIG QString("Config")
#define STRING_CONFIG_TIP QString("View config infomation")
#define STRING_NODE_ADDRESS QString("Node View")
#define STRING_NODE_ADDRESS_TIP QString("View node address infomation")
#define STRING_PYTHON QString("Python")
#define STRING_PYTHON_TIP QString("View python lib install infomation")
#define STRING_SETTING QString("Setting")
#define STRING_MODEL_PATH QString("SFC Model Path")
#define STRING_MODEL_PATH_TIP QString("Setting SFC model foler")
#define STRING_DEFAULT_PATH QString("Default Path")
#define STRING_DEFAULT_PATH_TIP QString("Setting default SFC test output foler")
#define STRING_NODE_PATH QString("Node Path")
#define STRING_NODE_PATH_TIP QString("Setting node address file path")
#define STRING_VSM_PATH QString("VSM Path")
#define STRING_VSM_PATH_TIP QString("Setting VSM file path")
#define STRING_APP_MODE QString("App Mode")
#define STRING_APP_MODE_TIP QString("Setting application mode")
#define STRING_TEST_REPORT QString("Tes Report")
#define STRING_TEST_REPORT_TIP QString("Test report setting")
#define STRING_REPORT QString("Report")
#define STRING_REPORT_TIP QString("Setting SFC test result info")
#define STRING_RESULT QString("Result")
#define STRING_RESULT_TIP QString("Setting SFC test result info")
#define STRING_COVERAGE QString("Coverage")
#define STRING_COVERAGE_TIP QString("Setting SFC test result info")
#define STRING_RUN QString("Run")
#define STRING_RUN_TIP QString("Run SFC test case file")
#define STRING_RUN_TC QString("Run TC")
#define STRING_RUN_TC_TIP QString("Run TC file")
#define STRING_GENERATE_TC QString("Generate TC")
#define STRING_GENERATE_TC_TIP QString("Generate TC file")
#define STRING_TC_REPORT QString("TC Report")
#define STRING_TC_REPORT_TIP QString("Create tc report file")
#define STRING_GCOV_REPORT QString("Gcov Report")
#define STRING_GCOV_REPORT_TIP QString("Create gcov report file")
#define STRING_GENERATE_REPORT QString("Generate Report")
#define STRING_GENERATE_REPORT_TIP QString("Generate Report file")
#define STRING_ENTER_SCRIPT_TEXT QString("Enter Script Text")
#define STRING_ENTER_SCRIPT_TEXT_TIP QString("Manually entering script execution text")
#define STRING_VIEW_RUN_SCRIPT_LOG QString("View Run Script Log")
#define STRING_VIEW_RUN_SCRIPT_LOG_TIP QString("View of run script log file")
#define STRING_TEST_RESULT QString("Test Result")
#define STRING_TEST_RESULT_TIP QString("<b>Edit --></b> Test Result Tip")
#define STRING_TEST_RESULT_COVERAGE QString("Test Coverage")
#define STRING_TEST_RESULT_COVERAGE_TIP QString("<b>Edit --></b> Test Coverage Tip")
#define STRING_DOCKER QString("Docker")
#define STRING_MULTI_DOCKER QString("Multi Docker")
#define STRING_MULTI_DOCKER_TIP QString("Multi docker start")
#define STRING_HELP QString("Help")
#define STRING_ABOUT QString("About")
#define STRING_ABOUT_TIP QString("Show the application's About box")
#define STRING_POPUP_ABOUT QString("About Application")
#define STRING_POPUP_ABOUT_TIP \
    QString("This <b>Application</b> is a program for <b>SFC Test</b> (Excel conversion, parsing, sfc test, etc.)")
#define STRING_ABOUT_QT QString("About Qt")
#define STRING_ABOUT_QT_TIP QString("Show the Qt library's About box")
#define STRING_FILE_OPEN QString("File Open")
#define STRING_FILE_OPEN_FAIL QString("Fail to file open")
#define STRING_BINARY_NOT_EXISTS_ERROR QString("Can't find binary file : ")
#define STRING_SELECT_KEYWORD QString("Select Keyword")

// Popup
#define STRING_POPUP_SAVE QString("Save")
#define STRING_POPUP_DISCARD QString("Discard")
#define STRING_POPUP_CANCEL QString("Cancel")
#define STRING_POPUP_SAVE_FILE QString("There is something being edited.")
#define STRING_POPUP_SAVE_FILE_TIP QString("Do you want to save your changes?")

#define STRING_POPUP_INSTALL QString("Install")
#define STRING_POPUP_CONFIRM QString("OK")
#define STRING_POPUP_LIB QString("Library")
#define STRING_POPUP_LIB_INSTALL_TIP QString("Would you like to install Python-related libraries?")
#define STRING_POPUP_NO_INSTALL_LIB_TIP QString("Python-related libraries are not installed.")
#define STRING_POPUP_NOW_INSTALLING_TIP QString("Python-related libraries are being installed.")
#define STRING_POPUP_INSTALL_COMPLETE_TIP QString("Python-related libraries have been installed.")

#define STRING_POPUP_CONFIG_RESET QString("Reset config value")
#define STRING_POPUP_CONFIG_RESET_TIP QString("Do you want to reset the config value?")
#define STRING_POPUP_FILE_NOT_EXIST QString("File doesn't exist")
#define STRING_POPUP_FILE_NOT_EXIST_TIP QString("VSM file not found.\nDo you want to set SFC model path?")
#define STRING_POPUP_DEFAULT_PATH_ERROR QString("Default path error")
#define STRING_POPUP_DEFAULT_PATH_ERROR_TIP QString("Please reset the default path.")
#define STRING_POPUP_INPUT_TEXT_ERROR QString("Input text error")
#define STRING_POPUP_INPUT_TEXT_ERROR_TIP QString("Input text does not contain script commands.")
#define STRING_SCRIPT_RUNNIG_COMPLETED QString("Script runnig completed")
#define STRING_SCRIPT_RUNNIG_COMPLETED_TIP QString("The script execution request has been completed.")
#define STRING_POPUP_TC_REPORT QString("TC report error")
#define STRING_POPUP_TC_REPORT_TIP QString("TC report view value is not set.")
#define STRING_POPUP_GCOV_REPORT QString("Gcov report error")
#define STRING_POPUP_GCOV_REPORT_TIP QString("Gcov report view value is not set.")
#define STRING_POPUP_EXCEL_EDIT QString("Editing Excel file")
#define STRING_POPUP_EXCEL_EDIT_TIP QString("Would you like to create a new Excel sheet?")
#define STRING_POPUP_ANOTHER_FILE_OPEN_TIP \
    QString("Would you like  to open another file without saving the Excel sheet you're editing?")
#define STRING_POPUP_MODULE_SELECT_ERROR QString("Module select error")
#define STRING_POPUP_MODULE_SELECT_ERROR_TIP QString("Please select one or more modules.")
#define STRING_POPUP_BINARY_NOT_EXISTS QString("Binary not exists")
#define STRING_POPUP_BINARY_NOT_EXISTS_TIP QString("Can't find binary(altonservice) file.")

#define STRING_POPUP_DATA_VALIDATION QString("Data Validation")
#define STRING_POPUP_DATA_VALIDATION_TIP QString("The input data contains %1.\nPlease enter the enumeration string.")

#define STRING_POPUP_CELL_SELECTION_ERROR QString("Cell selection error")
#define STRING_POPUP_CELL_SELECTION_ERROR_TIP_0 QString("Multiple selection feature is not supported.\nPlease select again.")
#define STRING_POPUP_CELL_SELECTION_ERROR_TIP_1 QString("The cell selection for merging is incorrect.\nPlease select again.")
#define STRING_POPUP_CELL_SELECTION_ERROR_TIP_2 \
    QString("The cell selection for merging is incorrect.\nPlease select again based on the TCName for merging.")
#define STRING_POPUP_CELL_SELECTION_ERROR_TIP_3 \
    QString("The cell selection for merging is incorrect.\nPlease select again based on the Result for merging.")

// =============================================================================================================
// Image Define
// =============================================================================================================
#define IAMGE_NEW ":/Image/Menu/New.png"
#define IAMGE_OPEN ":/Image/Menu/Open.png"
#define IAMGE_SAVE ":/Image/Menu/Save.png"
#define IAMGE_CUT ":/Image/Menu/Cut.png"
#define IAMGE_COPY ":/Image/Menu/Copy.png"
#define IAMGE_PASTE ":/Image/Menu/Paste.png"
#define IAMGE_MERGE_SPLIT ":/Image/Menu/MergeSplit.png"
#define IAMGE_LOADING ":/Image/Popup/Loading.png"

#endif  // COMMON_RESOURCE_H
