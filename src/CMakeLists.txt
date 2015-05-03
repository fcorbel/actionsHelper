# ncurses
FIND_PACKAGE(Curses)
IF (${CURSES_FOUND})
  MESSAGE(STATUS "Build with ncurses.")
  SET(withNCURSES true)
  SET(guiSRC ${guiSRC} ncursesUI.cpp)
  SET(guiLIB ${guiLIB} ${CURSES_LIBRARIES})
  INCLUDE_DIRECTORIES(${CURSES_INCLUDE_DIR})
ELSE (${CURSES_FOUND})
  MESSAGE(STATUS "Build without ncurses.")
ENDIF (${CURSES_FOUND})

# Qt
FIND_PACKAGE(Qt5 COMPONENTS Core Quick Widgets Qml)
IF (${Qt5_FOUND})
  MESSAGE(STATUS "Build with Qt.")
  SET(withQT true)
  QT5_WRAP_CPP(mocfile ../include/dataObject.h ../include/qtUI.h)
  SET(guiSRC ${guiSRC} qtUI.cpp dataObject.cpp ${mocfile})
  SET(guiLIB ${guiLIB} ${Qt5Core_LIBRARIES} ${Qt5Quick_LIBRARIES} ${Qt5Widgets_LIBRARIES} ${Qt5Qml_LIBRARIES})
  INCLUDE_DIRECTORIES(${Qt5Core_INCLUDE_DIR} ${Qt5Quick_INCLUDE_DIR} ${Qt5Widgets_INCLUDE_DIR} ${Qt5Qml_INCLUDE_DIR})
ELSE (${Qt5_FOUND})
  MESSAGE(STATUS "Build without Qt.")
ENDIF (${Qt5_FOUND})

IF (NOT withQt AND NOT withNCURSES)
  MESSAGE(FATAL_ERROR "Can not build without any gui library.")
ENDIF (NOT withQt AND NOT withNCURSES)




SET(progSRCS main.cpp jsoncpp.cpp actionsHelper.cpp)
ADD_EXECUTABLE(ah ${progSRCS} ${guiSRC})

TARGET_LINK_LIBRARIES(ah ${guiLIB})
