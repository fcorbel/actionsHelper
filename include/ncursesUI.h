#ifndef NCURSESUI_H
#define NCURSESUI_H

#include <actionsHelperUI.h>
#include <string>
#include <ncurses.h>
#include <json/json.h>

class NcursesUI: public ActionsHelperUI
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
    NcursesUI(ActionsHelper &logic);
    void startUI();
    void stopUI();
    void drawUI();
    void showTitle(std::string title);
    void showEntries(const Json::Value entries);
    void updateInput();

};

#endif
