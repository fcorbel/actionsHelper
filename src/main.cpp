#include <actionsHelper.h>
#include <ncursesUI.h>
#include <qtUI.h>


int main(int argc, char* argv[]) {

  // NcursesUI ui;
  // QtUI ui(argc, argv);
  // ShortcutsHelper helper(&ui);

  ActionsHelper helper;
  QtUI ui(helper, argc, argv);
  ui.startUI();

  return 0;
}
