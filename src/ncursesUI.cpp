#include "ncursesUI.h"
#include <shortcutsHelper.h>

#include <iostream>
NcursesUI::NcursesUI():
  stop(false),
  inputHeight(3)
{
}

void NcursesUI::startUI() {
  int ch;

  initscr();  // Start ncurses mode
  raw();  // React as soon as a key is pressed
  noecho(); //Don't show key pressed
  keypad(stdscr, true); // Enable arrows, F1, etc.

  drawUI();
  showTitle(logic_->currentShortcuts.name);
  showEntries(logic_->currentShortcuts.entries);

  move(1, 11);
  while (!stop) {
    ch = getch();
    switch(ch) {
      case '/':
        if (currentCmd.empty()) {
          cmdMode = true;
        }
        currentCmd += ch;
        break;
      case '\n':
        if (cmdMode) {
          logic_->processCmd(currentCmd);
        }
        break;
      case KEY_BACKSPACE:
        if (!currentCmd.empty()) {
          currentCmd.pop_back();
          if (currentCmd.empty() && cmdMode) {
            cmdMode = false;
          }
        }
        break;
      case 27: // Escape
        stopUI();
        break;
      case KEY_DOWN:
        break;
      case KEY_UP:
        break;
      case KEY_LEFT:
        break;
      case KEY_RIGHT:
        break;
      case KEY_RESIZE:
        clear();
        drawUI();
        showTitle(logic_->currentShortcuts.name);
        showEntries(logic_->currentShortcuts.entries);
        break;
      default:
        currentCmd += ch;
        break;
    }
    updateInput();
  }

	endwin();			/* End curses mode		  */
}

void NcursesUI::stopUI() {
  stop = true;
}

void NcursesUI::drawUI() {
  ///////////////////////
  // Input Win
  ///////////////////////
  refresh();			/* Print it on to the real screen */
  inputWinBorder = newwin(inputHeight, COLS, 0, 0);
  box(inputWinBorder, 0, 0);
  // wborder(inputWin, '|', '|', '-', '-', '+', '+', '+', '+');
  inputWin = derwin(inputWinBorder, inputHeight-2, COLS-2, 1, 1);
  wattron(inputWin, A_BOLD);
  mvwprintw(inputWin, 0, 1, "Command: ");
  wattroff(inputWin, A_BOLD);
  wrefresh(inputWinBorder);
  wrefresh(inputWin);

  ///////////////////////
  //  Title Win
  ///////////////////////
  titleWin = newwin(1,1,3, COLS/2);
  wrefresh(titleWin);

  ///////////////////////
  //  List Win
  ///////////////////////
  listWinBorder = newwin(LINES - inputHeight, COLS, 0+inputHeight, 0);
  box(listWinBorder, 0, 0);
  // wborder(inputWin, '|', '|', '-', '-', '+', '+', '+', '+');
  listWin = derwin(listWinBorder, LINES - inputHeight-2, COLS-2, 1, 1);
  wrefresh(listWinBorder);
  wrefresh(listWin);
}

void NcursesUI::updateInput() {
  wmove(inputWin, 0, 10);
  wclrtoeol(inputWin);
  mvwprintw(inputWin, 0, 10, "%s", currentCmd.c_str());
  wrefresh(inputWin);
}

void NcursesUI::showTitle(std::string title) {
  wclear(titleWin);
  wresize(titleWin, 1, title.size());
  redrawwin(listWinBorder);
  wrefresh(listWinBorder);
  mvwin(titleWin, 3, (COLS/2) - (title.size()/2));
  wprintw(titleWin, "%s", title.c_str());
  wrefresh(titleWin);
}

void NcursesUI::showEntries(Json::Value entries)
{
  //clean window
  wclear(listWin);
  if (entries.empty()) {
    return;
  }
  for (unsigned i=0; i<entries.size(); ++i) {
    Json::Value entry = entries[i];
    wattron(listWin, A_BOLD);
    mvwprintw(listWin, 0+i*3, 1, "%s", entry.get("shortcut", "").asCString());
    wattroff(listWin, A_BOLD);
    mvwprintw(listWin, 1+i*3, 10, "%s", entry.get("content", "").asCString());
  }
  wrefresh(listWin);
}
