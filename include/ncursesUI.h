#ifndef NCURSESUI_H
#define NCURSESUI_H

#include <shortcutsUI.h>
#include <string>
#include <ncurses.h>
#include <json/json.h>

class NcursesUI: public ShortcutsUI
{
  private:
    bool stop;
    int inputHeight;
    WINDOW* inputWin;
    WINDOW* inputWinBorder;
    WINDOW* titleWin;
    WINDOW* listWin;
    WINDOW* listWinBorder;

  public:
    NcursesUI(ShortcutsHelper &logic);
    void startUI();
    void stopUI();
    void drawUI();
    void showTitle(std::string title);
    void showEntries(const Json::Value entries);
    void updateInput();

};

#endif
