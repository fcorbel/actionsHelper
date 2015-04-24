#ifndef ACTIONSHELPER_H
#define ACTIONSHELPER_H

#include <string>
#include <vector>
#include <utility>
#include <json/json.h>
#include <simstring/simstring.h>
#include <quark.h>

typedef struct {
  std::string action;
  std::string description;
} Entry;

// typedef simstring::cosine cosineSS;

class ActionsHelper
{
  private:
    const std::string AppListPath;

    std::string loadedAppPath;
    std::string loadedAppTitle;
    std::vector<Entry> loadedEntries;
    quark descIndex;

  public:
    ActionsHelper(std::string appName="help");
    bool loadEntries(const std::string appPath);
    bool findAppFolder(std::string& result, const std::string appName);
    bool loadAppList();
    bool processCmd(std::string cmd);
    bool addEntry(Entry newEntry);
    bool deleteEntry(std::string action);
    bool hasDb();
    bool createDb();
    std::vector<Entry> makeSearch(std::string search,std::string measure="cosine", double threshold=0.1);

    std::string getLoadedAppTitle();
    std::vector<Entry> getLoadedEntries();
    


};


#endif
