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
  loadedShortcutsName(""),
  loadedShortcutsEntries(Json::arrayValue)
  // contentDb(simstring::ngram_generator(3, false), "content.db")
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
  loadedShortcutsName = root.get("name", "None").asString();
  loadedShortcutsEntries = root["entries"];
//   //Fill Database TODO
//   for (unsigned i=0; i<result.entries.size(); ++i) {
//     Json::Value entry = result.entries[i];
//     std::string content = entry.get("content", "").asCString();
//     if (content.size() > 0) {
//       contentDb.insert(content);
//     }
//   }
//   contentDb.close();
  return true;
}

bool ShortcutsHelper::findShortcutFile(std::string& result, const std::string appName) {
  // std::cout << "Search application list in: " << AppListPath << std::endl;
  //TODO search in file with name like "firefox,mozilla firefox,ff.json"
  std::string fileName;
  fileName = AppListPath +"/"+appName+".json";
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
  std::string name = "Applications list";
  Json::Value entries(Json::arrayValue);
  dirent* ent;
  while ((ent = readdir(dir)) != NULL) {
    char* name = ent->d_name;
    if ((strcmp(name, ".") != 0) && (strcmp(name, "..") != 0) && (name[0] != '.')) {
      Json::Value fileInfos(Json::objectValue);
      fileInfos["shortcut"] = name;
      fileInfos["content"] = "";
      entries.append(fileInfos);
    }
  }
  closedir(dir);
  loadedShortcutsName = name;
  loadedShortcutsEntries = entries;
  return true;
}

//return true if GUI needs to refresh
bool ShortcutsHelper::processCmd(std::string cmd) {
  cmd.erase(0, 1); // erase "/"
  if (cmd == "list") {
    return loadAppList();
  } else {
    return loadShortcuts(cmd);
  }
}

std::string ShortcutsHelper::getLoadedShortcutsName() {
  return loadedShortcutsName;
}

Json::Value ShortcutsHelper::getLoadedShortcutsEntries() {
  return loadedShortcutsEntries;
}

/*
void ShortcutsHelper::makeSearch(std::string search) {
  // Json::Value result;
  //Make fuzzy search to sort result
  //split string in words
  // std::istringstream iss(search);
  // std::vector<std::string> tokens{std::istream_iterator<std::string>{iss},
  //                     std::istream_iterator<std::string>{}};
  // std::cout << "[" << std::endl;
  // for( std::vector<std::string>::const_iterator i = tokens.begin(); i != tokens.end(); ++i) {
  //   std::cout << *i << '\n';
  // }
  // std::cout << "]" << std::endl;

    // Create a SimString database with the content.
    simstring::ngram_generator gen(3, false);
    simstring::writer_base<std::string> dbw(gen, "content.db");

    Json::Value entries = currentShortcuts.entries;
    for (unsigned i=0; i<entries.size(); ++i) {
      Json::Value entry = entries[i];
      std::string content = entry.get("content", "").asCString();
      if (content.size() > 0) {
        dbw.insert(content);
      }
    }
    dbw.close();

   // Open the database for reading.
    simstring::reader dbr;
    
    dbr.open("content.db");
    // retrieve(dbr, "Barack Obama", simstring::cosine, 0.6);
    // retrieve(dbr, "Gordon Brown", simstring::cosine, 0.6);
    // retrieve(dbr, "Obama", simstring::cosine, 0.6);
    // retrieve(dbr, "Obama", simstring::overlap, 1.0);

    auto result = retrieve(dbr, search, simstring::cosine, 0.2);

    // Output the retrieved strings separated by ", ".
    for (int i = 0;i < (int)result.size();++i) {
        std::cout << (i != 0 ? "\n" : "") << result[i];
    }

  // Json::Value entries = currentShortcuts.entries;
  // for (unsigned i=0; i<entries.size(); ++i) {
  //   Json::Value entry = entries[i];
  //   std::string content = entry.get("content", "").asCString();
  //   for( std::vector<std::string>::const_iterator i = tokens.begin(); i != tokens.end(); ++i) {

  //     std::cout << *i << '\n';
  //   }
  // }

  //for every word in search -> make fuzzy search
  // result = currentShortcuts.entries;
  // ui_->showEntries(result);

}

std::vector<std::string> ShortcutsHelper::retrieve(simstring::reader& dbr, const std::string& query, int measure, double threshold) {
    // Retrieve similar strings into a string vector.
    std::vector<std::string> xstrs;
    dbr.retrieve(query, measure, threshold, std::back_inserter(xstrs));

    std::cout << std::endl;
    return xstrs;
}
*/
