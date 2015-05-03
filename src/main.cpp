#include <actionsHelper.h>

#if defined(USE_NCURSES)
  #include <ncursesUI.h>
#endif
#if defined(USE_QT)
  #include <qtUI.h>
#endif

#include <unistd.h>
#include <stdio.h>

int main(int argc, char* argv[]) {

  ActionsHelper helper;
  ActionsHelperUI* ui = nullptr;
  if (isatty(fileno(stdin))) {
    puts("Launched from a terminal.");
    // ui = new NcursesUI(helper, argc, argv);
  } else {
    puts("Launched from GUI.");
    ui = new QtUI(helper, argc, argv);
  }

  if (ui) {
    ui->startUI();
    delete ui;
  }

  return 0;
}
