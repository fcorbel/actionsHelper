#ifndef SHORTCUTSHELPER_H
#define SHORTCUTSHELPER_H

#include <string>
#include <utility>
#include <json/json.h>
#include <simstring/simstring.h>

class ShortcutsHelper
{
  private:
    const std::string AppListPath;

    std::string loadedShortcutsName;
    Json::Value loadedShortcutsEntries;
    // simstring::writer_base<std::string> contentDb;


  public:
    ShortcutsHelper(std::string appName="help");
    bool loadShortcuts(const std::string appName);
    bool findShortcutFile(std::string& result, const std::string appName);
    bool loadAppList();
    bool processCmd(std::string cmd);

    std::string getLoadedShortcutsName();
    Json::Value getLoadedShortcutsEntries();
    /*
    void makeSearch(std::string search);
    std::vector<std::string> retrieve(simstring::reader& dbr, const std::string& query, int measure, double threshold);
*/

};


#endif
