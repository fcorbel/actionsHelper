#include <shortcutsHelper.h>
#include <ncursesUI.h>
#include <qtUI.h>


int main(int argc, char* argv[]) {

  // NcursesUI ui;
  QtUI ui(argc, argv);
  ShortcutsHelper helper(&ui);

  return 0;
}
