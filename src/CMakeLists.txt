# ncurses
FIND_PACKAGE(Curses REQUIRED)
INCLUDE_DIRECTORIES(${CURSES_INCLUDE_DIR})

# Qt
find_package(Qt5 COMPONENTS Core Quick Widgets Qml)
INCLUDE_DIRECTORIES(${Qt5Core_INCLUDE_DIR} ${Qt5Quick_INCLUDE_DIR} ${Qt5Widgets_INCLUDE_DIR} ${Qt5Qml_INCLUDE_DIR})
QT5_WRAP_CPP(mocfile ../include/dataObject.h ../include/qtUI.h)

SET(progSRCS main.cpp jsoncpp.cpp actionsHelper.cpp ncursesUI.cpp qtUI.cpp dataObject.cpp)
ADD_EXECUTABLE(ah ${progSRCS} ${mocfile})

TARGET_LINK_LIBRARIES(ah ${CURSES_LIBRARIES} ${Qt5Core_LIBRARIES} ${Qt5Quick_LIBRARIES} ${Qt5Widgets_LIBRARIES} ${Qt5Qml_LIBRARIES})

