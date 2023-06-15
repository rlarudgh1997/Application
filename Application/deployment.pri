# ===========================================================================
# [컴파일 로그 정보 출력 관련]
# ===========================================================================

# [QVarinat 관련]
QMAKE_CXXFLAGS += -Wno-deprecated-copy

# [unused Variable, Parameter, Result]
QMAKE_CXXFLAGS += -Wno-unused-variable -Wno-unused-parameter -Wno-unused-result

# [unused Function, Switch, Return Type]
#QMAKE_CXXFLAGS += -Wno-unused-function -Wno-switch -Wno-return-type -Wno-sign-compare



# Compile Warning 삭제 : 추후 확인 필요
QMAKE_CXXFLAGS += -Wno-deprecated -Wno-missing-field-initializers -Wno-deprecated-declarations -Wno-implicit-fallthrough
#QMAKE_CXXFLAGS += -Wall-Wextra-Wpadded



# [Ignore warning: "note: parameter passing for argument of type '...' changed in GCC 7.1"]
#     [https://stackoverflow.com/questions/48149323/what-does-the-gcc-warning-project-parameter-passing-for-x-changed-in-gcc-7-1-m]
#     [https://gcc.gnu.org/legacy-ml/gcc/2017-05/msg00073.html]
#QMAKE_CXXFLAGS += -Wno-psabi








# ===========================================================================
win32{
    DEFINES += __WIN32__
}
else{
    DEFINES += __LINUX__
}

CONFIG(debug, debug|release) {
    DEFINES += __DEBUG__
    DESTDIR = $$PWD/debug/$$PLATFORM
}

CONFIG(release, debug|release) {
    QMAKE_CFLAGS += -g
    QMAKE_CXXFLAGS += -g
    QMAKE_LFLAGS += -rdynamic
    QMAKE_LFLAGS = $$replace(QMAKE_LFLAGS, "-rdynamic ", "")

    DEFINES += __RELEASE__
    DESTDIR = $$PWD/release/$$PLATFORM
}


OBJECTS_DIR = $$DESTDIR/.obj
MOC_DIR     = $$DESTDIR/.moc
RCC_DIR     = $$DESTDIR/.qrc
UI_DIR      = $$DESTDIR/.ui


