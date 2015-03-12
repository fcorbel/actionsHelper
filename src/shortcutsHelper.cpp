#include <shortcutsHelper.h>
#include <iostream>
#include <fstream>
#include <memory>
#include <dirent.h>
#include <cstring>
#include <algorithm>
#include <iterator>


ShortcutsHelper::ShortcutsHelper(std::string appName): 
  AppListPath("../ressources/appList"),
  loadedAppName(""),
  loadedShortcutsEntries()
  // contentDb(nullptr)
{
  loadShortcuts(appName);
}

bool ShortcutsHelper::loadShortcuts(const std::string appName) {
  std::string fileName;
  if (!findShortcutFile(fileName, appName)) {
    return false;
  }
  // std::cout << "Try to open file at: " << fileName << std::endl;
  Json::Value root;
  Json::Reader reader;
  std::ifstream stream(fileName.c_str(), std::ifstream::binary);
  if (!reader.parse(stream, root, false)) {
    std::cout << reader.getFormatedErrorMessages() << std::endl;
    return false;
  }
  loadedAppName = appName;
  std::vector<Entry> newEntries;
  Json::Value entriesJson = root["entries"];
  for (unsigned i=0; i<entriesJson.size(); ++i) {
    Json::Value entryJson = entriesJson[i];
    Entry entry;
    entry.shortcut = entryJson.get("shortcut", "").asCString();
    entry.content = entryJson.get("content", "").asCString();
    newEntries.push_back(entry);
  }
  loadedShortcutsEntries = newEntries;
  if (!hasDb()) {
    createDb();
  }
  return true;
}

bool ShortcutsHelper::findShortcutFile(std::string& result, const std::string appName) {
  // std::cout << "Search application list in: " << AppListPath << std::endl;
  //TODO search in all folders for a file "appName.txt" -> check if appName is in it
  std::string fileName;
  fileName = AppListPath +"/"+appName+"/actions.json";
  if (std::ifstream(fileName.c_str())) {
    result = fileName;
    return true;
  } else {
    std::cerr << "Could not find any file for that name" << std::endl;
    return false;
  }
}

bool ShortcutsHelper::loadAppList() {
  // Open and list files
  DIR* dir;
  if ((dir = opendir(AppListPath.c_str())) == NULL) {
    return false;
  }
  std::string appName = "Applications list";
  std::vector<Entry> entries;
  // Json::Value entries(Json::arrayValue);
  dirent* ent;
  while ((ent = readdir(dir)) != NULL) {
    char* name = ent->d_name;
    if ((strcmp(name, ".") != 0) && (strcmp(name, "..") != 0) && (name[0] != '.')) {
      Entry entry;
      entry.shortcut = name;
      entry.content = "";
      entries.push_back(entry);
    }
  }
  closedir(dir);
  loadedAppName = appName;
  loadedShortcutsEntries = entries;
  return true;
}

//return true if GUI needs to refresh
bool ShortcutsHelper::processCmd(std::string cmd) {
  cmd.erase(0, 1); // erase "/"
  if (cmd == "list") {
    return loadAppList();
  }
  if (cmd.substr(0, 4) == "add ") {
    auto first = cmd.find_first_of("\"\'"); 
    auto second = cmd.find_first_of("\"\'", first+1); 
    Entry newEntry;
    newEntry.shortcut = cmd.substr(first+1, second-first-1);
    first = cmd.find_first_of("\"\'", second+1); 
    second = cmd.find_first_of("\"\'", first+1); 
    newEntry.content = cmd.substr(first+1, second-first-1);
    return addEntry(newEntry);
  }
  return loadShortcuts(cmd);
}

bool ShortcutsHelper::addEntry(Entry newEntry) {
  std::cout << "Try to add a new entry: " << newEntry.shortcut << " => " << newEntry.content << std::endl;
  if (newEntry.shortcut.size() == 0) {
    return false;
  }
  // Add to file
  std::string fileName;
  if (!findShortcutFile(fileName, loadedAppName)) {
    return false;
  }
  // Read previous content
  Json::Value root;
  Json::Reader reader;
  std::ifstream stream(fileName.c_str(), std::ifstream::binary);
  if (!reader.parse(stream, root, false)) {
    std::cout << reader.getFormatedErrorMessages() << std::endl;
    return false;
  }
  Json::Value jsonEntry(Json::objectValue);
  jsonEntry["shortcut"] = newEntry.shortcut;
  jsonEntry["content"] = newEntry.content;
  root["entries"].append(jsonEntry);
  // Rewrite file
  std::ofstream file(fileName);
  if (file.is_open()) {
    file << root;
    file.close();
  }

  loadShortcuts(loadedAppName);
  return true;
}

bool ShortcutsHelper::hasDb() {
  std::string fileName(AppListPath +"/"+loadedAppName+"/database.db");
  if (std::ifstream(fileName.c_str())) {
    return true;
  } else {
    return false;
  }
}

bool ShortcutsHelper::createDb() {
  //Fill Database
  simstring::ngram_generator gen(3, false);
  std::string fileName(AppListPath +"/"+loadedAppName+"/database.db");
  std::cout << "Create database at: " << fileName << std::endl;
  auto db = new simstring::writer_base<std::string>(gen, fileName);
  for (auto it=loadedShortcutsEntries.begin(); it != loadedShortcutsEntries.end(); ++it) {
    Entry entry = *it;
    std::string content = entry.content;
    if (content.size() > 0) {
      db->insert(content);
    }
  }
  db->close();
  return true;
}

std::vector<std::string> ShortcutsHelper::makeSearch(std::string search, std::string measure, double threshold) {
  std::cout << "Make search with mesure=" << measure << " and threshold=" <<threshold << std::endl;
  // Open the database for reading.
  simstring::reader dbr;
  std::string fileName(AppListPath +"/"+loadedAppName+"/database.db");
  if (!dbr.open(fileName)) {
    std::cerr << "Could not open file";
    return std::vector<std::string>();
  }
  int measureStruct;
  if (measure == "cosine") {
    measureStruct = simstring::cosine;
  } else if (measure == "dice") {
    measureStruct = simstring::dice;
  } else if (measure == "exact") {
    measureStruct = simstring::exact;
  } else if (measure == "jaccard") {
    measureStruct = simstring::jaccard;
  } else if (measure == "overlap") {
    measureStruct = simstring::overlap;
  } else {
    std::cerr << "Unknown measure";
    return std::vector<std::string>();
  }
  std::vector<std::string> result;
  dbr.retrieve(search, measureStruct, threshold, std::back_inserter(result));
  // dbr.retrieve(search, simstring::cosine, 1, std::back_inserter(result));
  return result;
}

std::string ShortcutsHelper::getLoadedAppName() {
  return loadedAppName;
}

std::vector<Entry> ShortcutsHelper::getLoadedShortcutsEntries() {
  return loadedShortcutsEntries;
}

