#include <actionsHelper.h>
#include <iostream>
#include <fstream>
#include <memory>
#include <dirent.h>
#include <cstring>
#include <algorithm>
#include <iterator>


ActionsHelper::ActionsHelper(std::string appName): 
  AppListPath("../ressources/appList"),
  loadedAppPath(""),
  loadedAppTitle(""),
  loadedEntries()
{
  if (findAppFolder(loadedAppPath, appName)) {
    loadEntries(loadedAppPath);
  }
}

bool ActionsHelper::loadEntries(const std::string appPath) {
  std::string fileName = appPath+"/actions.json";
  // std::cout << "Try to open file at: " << fileName << std::endl;
  Json::Value root;
  Json::Reader reader;
  std::ifstream stream(fileName.c_str(), std::ifstream::binary);
  if (!reader.parse(stream, root, false)) {
    std::cout << reader.getFormatedErrorMessages() << std::endl;
    return false;
  }
  loadedAppTitle = root["name"].asCString();
  std::vector<Entry> newEntries;
  Json::Value entriesJson = root["entries"];
  for (unsigned i=0; i<entriesJson.size(); ++i) {
    Json::Value entryJson = entriesJson[i];
    Entry entry;
    entry.action = entryJson.get("action", "").asCString();
    entry.description = entryJson.get("description", "").asCString();
    newEntries.push_back(entry);
  }
  loadedEntries = newEntries;
  if (!hasDb()) {
    createDb();
  }
  return true;
}

bool ActionsHelper::findAppFolder(std::string& result, const std::string appName) {
  // std::cout << "Search application list in: " << AppListPath << std::endl;
  //TODO search in all folders for a file "appName.txt" -> check if appName is in it
  std::string fileName;
  fileName = AppListPath +"/"+appName;
  if (std::ifstream(fileName.c_str())) {
    result = fileName;
    return true;
  } else {
    std::cerr << "Could not find any file for that name" << std::endl;
    return false;
  }
}

bool ActionsHelper::loadAppList() {
  // Open and list files
  DIR* dir;
  if ((dir = opendir(AppListPath.c_str())) == NULL) {
    return false;
  }
  std::vector<Entry> entries;
  dirent* ent;
  while ((ent = readdir(dir)) != NULL) {
    char* name = ent->d_name;
    std::cout << name << std::endl;
    if ((strcmp(name, ".") != 0) && (strcmp(name, "..") != 0) && (name[0] != '.')) {
      Entry entry;
      entry.action = name;
      entry.description = "";
      entries.push_back(entry);
    }
  }
  closedir(dir);
  loadedAppPath = AppListPath;
  loadedAppTitle = "Applications list";
  loadedEntries = entries;
  createDb();
  return true;
}

//return true if GUI needs to refresh
bool ActionsHelper::processCmd(std::string cmd) {
  cmd.erase(0, 1); // erase "/"
  if (cmd == "list") {
    return loadAppList();
  }
  if (cmd.substr(0, 4) == "add ") {
    auto first = cmd.find_first_of("\"\'"); 
    auto second = cmd.find_first_of("\"\'", first+1); 
    Entry newEntry;
    newEntry.action = cmd.substr(first+1, second-first-1);
    first = cmd.find_first_of("\"\'", second+1); 
    second = cmd.find_first_of("\"\'", first+1); 
    newEntry.description = cmd.substr(first+1, second-first-1);
    return addEntry(newEntry);
  }
  if (cmd.substr(0, 7) == "delete ") {
    auto first = cmd.find_first_of("\"\'"); 
    auto second = cmd.find_first_of("\"\'", first+1); 
    return deleteEntry(cmd.substr(first+1, second-first-1));
  }
  if (findAppFolder(loadedAppPath, cmd)) {
    return loadEntries(loadedAppPath);
  } else {
    return false;
  }
}

bool ActionsHelper::addEntry(Entry newEntry) {
  std::cout << "Try to add a new entry: " << newEntry.action << " => " << newEntry.description << std::endl;
  if (newEntry.action.size() == 0) {
    return false;
  }
  // Add to file
  std::string fileName = loadedAppPath + "/actions.json";
  // Read previous content
  Json::Value root;
  Json::Reader reader;
  std::ifstream stream(fileName.c_str(), std::ifstream::binary);
  if (!reader.parse(stream, root, false)) {
    std::cout << reader.getFormatedErrorMessages() << std::endl;
    return false;
  }
  Json::Value jsonEntry(Json::objectValue);
  jsonEntry["action"] = newEntry.action;
  jsonEntry["description"] = newEntry.description;
  root["entries"].append(jsonEntry);
  // Rewrite file
  std::ofstream file(fileName);
  if (file.is_open()) {
    file << root;
    file.close();
  }

  loadEntries(loadedAppPath);
  createDb();
  return true;
}

bool ActionsHelper::deleteEntry(std::string action) {
  if (action.size() < 0) {
    return false;
  }
  std::cout << "Try to delete an entry: " << action << std::endl;
  std::string fileName = loadedAppPath + "/actions.json";
  // Read previous content
  Json::Value root;
  Json::Reader reader;
  std::ifstream stream(fileName.c_str(), std::ifstream::binary);
  if (!reader.parse(stream, root, false)) {
    std::cout << reader.getFormatedErrorMessages() << std::endl;
    return false;
  }
  // Search for the action string
  Json::Value::ArrayIndex index;
  for (index=0; index<root["entries"].size(); ++index) {
    Json::Value entry = root["entries"][index];
    if (entry["action"].asString() == action) {
      //found the correct entry
      break;
    }
  }
  Json::Value removedEntry;
  if (root["entries"].removeIndex(index, &removedEntry)) {
    std::cout << "Removed entry: " << action << std::endl;
  }
  // Rewrite file
  std::ofstream file(fileName);
  if (file.is_open()) {
    file << root;
    file.close();
  }

  loadEntries(loadedAppPath);
  createDb();
  return true;
  
}

bool ActionsHelper::hasDb() {
  std::string fileName(loadedAppPath+"/database.db");
  if (std::ifstream(fileName.c_str())) {
    return true;
  } else {
    return false;
  }
}

bool ActionsHelper::createDb() {
  //Fill Database
  simstring::ngram_generator gen(3, false);
  std::string fileName(loadedAppPath+"/database.db");
  std::cout << "Create database at: " << fileName << std::endl;
  auto db = new simstring::writer_base<std::string>(gen, fileName);
  for (auto it=loadedEntries.begin(); it != loadedEntries.end(); ++it) {
    Entry entry = *it;
    std::string content = entry.description;
    if (content.size() > 0) {
      db->insert(content);
    }
  }
  db->close();
  return true;
}

std::vector<std::string> ActionsHelper::makeSearch(std::string search, std::string measure, double threshold) {
  // std::cout << "Make search with mesure=" << measure << " and threshold=" <<threshold << std::endl;
  // Open the database for reading.
  simstring::reader dbr;
  std::string fileName(loadedAppPath+"/database.db");
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

std::string ActionsHelper::getLoadedAppTitle() {
  return loadedAppTitle;
}

std::vector<Entry> ActionsHelper::getLoadedEntries() {
  return loadedEntries;
}

