#ifndef SHORTCUTSHELPER_H
#define SHORTCUTSHELPER_H

#include <string>
#include <vector>
#include <utility>
#include <json/json.h>
#include <simstring/simstring.h>

typedef struct {
  std::string shortcut;
  std::string content;
} Entry;

// typedef simstring::cosine cosineSS;

class ShortcutsHelper
{
  private:
    const std::string AppListPath;

    std::string loadedAppName;
    std::vector<Entry> loadedShortcutsEntries;

  public:
    ShortcutsHelper(std::string appName="help");
    bool loadShortcuts(const std::string appName);
    bool findShortcutFile(std::string& result, const std::string appName);
    bool loadAppList();
    bool processCmd(std::string cmd);
    bool addEntry(Entry newEntry);
    bool hasDb();
    bool createDb();
    std::vector<std::string> makeSearch(std::string search,std::string measure="cosine", double threshold=0.1);

    std::string getLoadedAppName();
    std::vector<Entry> getLoadedShortcutsEntries();
    


};


#endif
