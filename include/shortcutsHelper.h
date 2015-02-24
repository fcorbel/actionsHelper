#ifndef SHORTCUTSHELPER_H
#define SHORTCUTSHELPER_H

#include <string>
#include <shortcutsUI.h>
#include <utility>
#include <json/json.h>

typedef struct {
  std::string name;
  Json::Value entries;
} ShortcutsInfos;


class ShortcutsHelper
{
  private:
    const std::string AppListPath;
    ShortcutsUI* ui_;


  public:
    ShortcutsInfos currentShortcuts;

    ShortcutsHelper(ShortcutsUI* ui, std::string appName="help");
    bool getShortcutsForApp(ShortcutsInfos& result, const std::string fileName);
    bool findShortcutFile(std::string& result, const std::string appName);
    bool getAppList(ShortcutsInfos& result);
    void processCmd(std::string cmd);


};


#endif
