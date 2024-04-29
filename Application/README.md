# Application : TC_Creator
[Description]
    - tc 엑셀 파일 작성 및 tc run 과 관련한 동작 수행을 위한 어플리케이션


[Build]
    - Release Build
        ./bulid.sh h r
    - Debug Build
        ./bulid.sh h d


[Clean]
    - build clean
        ./bulid.sh h c
    - build + install clean(deploy_x86 폴더)
        ./bulid.sh c


[Run]
    ./run.sh h


[Menu]
    - File : New, Open, Save, Save As, Exit
    - Edit : Copy, Cut, Paste
    - View : Config, Node View
    - Setting : Default Path, VSM Path
    - Run : Generate TC, Run TC, TC Report, Gcov Report, Enter Script Text, View Run Script Log
    - Help : About, About QT


[Etc]
    - DefaultModule.info : VSM Module 리스트 정보
    - NodeAddressSFC.info : SFC 리스트 정보
    - NodeAddressVSM.info : VSM 리스트 정보
    - Application.ini : Config 설정 파일 (앱 실행시 자동 생성)


[Folder]
    .
    ├── TC_Generator
    ├── build.sh
    ├── common
    │   ├── __init__.py
    │   ├── helper.py
    │   ├── message.py
    │   └── yalm.py
    ├── gen_tc.sh
    ├── run.sh
    ├── snippet
    │   ├── __init__.py
    │   ├── parser.py
    │   └── x2unit.py
    ├── tc_creator
    │   ├── Alton
    │   │   ├── alton_client
    │   │   └── altonservice
    │   ├── Application
    │   │   ├── Application.pro
    │   │   ├── README.md
    │   │   ├── ReleaseNote.info
    │   │   ├── Resource
    │   │   │   ├── Image
    │   │   │   │   ├── Menu
    │   │   │   │   │   ├── Copy.png
    │   │   │   │   │   ├── Cut.png
    │   │   │   │   │   ├── MergeSplit.png
    │   │   │   │   │   ├── New.png
    │   │   │   │   │   ├── Open.png
    │   │   │   │   │   ├── Paste.png
    │   │   │   │   │   └── Save.png
    │   │   │   │   └── Popup
    │   │   │   │       └── Loading.png
    │   │   │   └── Image.qrc
    │   │   ├── Source
    │   │   │   ├── Abstract
    │   │   │   │   ├── Abstract.pri
    │   │   │   │   ├── AbstractControl.cpp
    │   │   │   │   ├── AbstractControl.h
    │   │   │   │   ├── AbstractGui.h
    │   │   │   │   ├── AbstractHandler.cpp
    │   │   │   │   ├── AbstractHandler.h
    │   │   │   │   └── AbstractThread.cpp
    │   │   │   ├── Center
    │   │   │   │   ├── Center.pri
    │   │   │   │   ├── ControlCenter.cpp
    │   │   │   │   ├── ControlCenter.h
    │   │   │   │   ├── GuiCenter.cpp
    │   │   │   │   ├── GuiCenter.h
    │   │   │   │   ├── GuiCenter.ui
    │   │   │   │   ├── HandlerCenter.cpp
    │   │   │   │   └── HandlerCenter.h
    │   │   │   ├── Common
    │   │   │   │   ├── Common.pri
    │   │   │   │   ├── CommonDefine.h
    │   │   │   │   ├── CommonEnum.h
    │   │   │   │   ├── CommonFunction.h
    │   │   │   │   ├── CommonPopup.h
    │   │   │   │   ├── CommonResource.h
    │   │   │   │   ├── CommonUtil.h
    │   │   │   │   ├── LogUtil.cpp
    │   │   │   │   └── LogUtil.h
    │   │   │   ├── Config
    │   │   │   │   ├── Config.pri
    │   │   │   │   ├── ConfigInfo.h
    │   │   │   │   ├── ConfigSetting.cpp
    │   │   │   │   └── ConfigSetting.h
    │   │   │   ├── Dialog
    │   │   │   │   ├── Dialog.cpp
    │   │   │   │   ├── Dialog.h
    │   │   │   │   ├── Dialog.pri
    │   │   │   │   └── Dialog.ui
    │   │   │   ├── Excel
    │   │   │   │   ├── ControlExcel.cpp
    │   │   │   │   ├── ControlExcel.h
    │   │   │   │   ├── Excel.pri
    │   │   │   │   ├── GuiExcel.cpp
    │   │   │   │   ├── GuiExcel.h
    │   │   │   │   ├── GuiExcel.ui
    │   │   │   │   ├── HandlerExcel.cpp
    │   │   │   │   └── HandlerExcel.h
    │   │   │   ├── MainWindow.cpp
    │   │   │   ├── MainWindow.h
    │   │   │   ├── Manager
    │   │   │   │   ├── ControlManager.cpp
    │   │   │   │   ├── ControlManager.h
    │   │   │   │   └── Manager.pri
    │   │   │   ├── Menu
    │   │   │   │   ├── ControlMenu.cpp
    │   │   │   │   ├── ControlMenu.h
    │   │   │   │   ├── GuiMenu.cpp
    │   │   │   │   ├── GuiMenu.h
    │   │   │   │   ├── GuiMenu.ui
    │   │   │   │   ├── HandlerMenu.cpp
    │   │   │   │   ├── HandlerMenu.h
    │   │   │   │   └── Menu.pri
    │   │   │   ├── Screen
    │   │   │   │   ├── Screen.pri
    │   │   │   │   ├── ScreenInfo.cpp
    │   │   │   │   └── ScreenInfo.h
    │   │   │   ├── Source.pri
    │   │   │   └── main.cpp
    │   │   └── deployment.pri
    │   ├── Document
    │   │   └── TC Creator Guide.pptx
    │   └── Python
    │       ├── ExcelParser.py
    │       └── NodeAddress
    │           ├── CV
    │           │   ├── DefaultModule.info
    │           │   ├── NodeAddressSFC.info
    │           │   ├── NodeAddressVSM.info
    │           │   ├── NodeAddressVSM_Duplicate.info
    │           │   └── NodeAddressVSM_Title.info
    │           └── PV
    │               ├── DefaultModule.info
    │               ├── NodeAddressSFC.info
    │               ├── NodeAddressVSM.info
    │               ├── NodeAddressVSM_Duplicate.info
    │               └── NodeAddressVSM_Title.info
    ├── tcgen
    │   ├── __init__.py
    │   └── generator.py
    └── template
        └── tc_template.tc