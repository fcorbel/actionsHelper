# ncurses
FIND_PACKAGE(Curses REQUIRED)
INCLUDE_DIRECTORIES(${CURSES_INCLUDE_DIR})


SET(shortcutSRCS main.cpp jsoncpp.cpp shortcutsHelper.cpp ncursesUI.cpp)
ADD_EXECUTABLE(shortcut ${shortcutSRCS})

TARGET_LINK_LIBRARIES(shortcut ${CURSES_LIBRARIES})

