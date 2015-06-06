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
  // Manage options
  bool useQt = false;
  if (argc > 1) {
    if (strcmp(argv[1], "qt") == 0) {
      useQt = true;
    }
  }

  ActionsHelper helper;
  ActionsHelperUI* ui = nullptr;
  if (isatty(fileno(stdin))) {
    puts("Launched from a terminal.");
    if (useQt) {
      ui = new QtUI(helper, argc, argv);
    } else {
      ui = new NcursesUI(helper, argc, argv);
    }
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
