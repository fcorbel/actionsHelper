#include <shortcutsHelper.h>
#include <iostream>
#include <fstream>
#include <memory>
#include <dirent.h>
#include <cstring>


ShortcutsHelper::ShortcutsHelper(ShortcutsUI* ui, std::string appName): 
  AppListPath("../ressources/appList"),
  ui_(ui)
{
  ui_->setLogic(this);

  std::string fileName;
  auto result = findShortcutFile(fileName, appName);
  if (result) {
    auto couldRead = getShortcutsForApp(currentShortcuts, fileName);
    if (!couldRead) {
      std::cout << "Could not read file" << std::endl;
    }
  }
  
  ui_->startUI();
}

bool ShortcutsHelper::getShortcutsForApp(ShortcutsInfos& result, const std::string fileName) {
  // std::cout << "Try to open file at: " << fileName << std::endl;
  Json::Value root;
  Json::Reader reader;
  std::ifstream stream(fileName.c_str(), std::ifstream::binary);
  if (!reader.parse(stream, root, false)) {
    std::cout << reader.getFormatedErrorMessages() << std::endl;
    return false;
  }
  result.name = root.get("name", "None").asString();
  result.entries = root["entries"];

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
    // return fileName;
  } else {
    // std::cerr << "Could not find any file for that name" << std::endl;
    // return std::string("");
    return false;
  }
}

bool ShortcutsHelper::getAppList(ShortcutsInfos& result) {
  std::string name = "Applications list";
  Json::Value entries(Json::arrayValue);
  // Open and list files
  DIR* dir;
  if ((dir = opendir(AppListPath.c_str())) == NULL) {
    return false;
  } else {
    dirent* ent;
    while ((ent = readdir(dir)) != NULL) {
      char* name = ent->d_name;
      if ((strcmp(name, ".") != 0) && (strcmp(name, "..") != 0) && (name[0] != '.')) {
        Json::Value fileInfos(Json::objectValue);
        fileInfos["shortcut"] = ent->d_name;
        fileInfos["content"] = "lalala";
        entries.append(fileInfos);
      }
    }
    closedir(dir);
    result.name = name;
    result.entries = entries;
    return true;
  }
}

void ShortcutsHelper::processCmd(std::string cmd) {
  if (cmd == "/quit") {
    ui_->stopUI();
  } else if (cmd == "/list") {
    bool couldRead = getAppList(currentShortcuts);
    if (couldRead) {
      ui_->setCurrentCmd("");
      ui_->updateInput();
      ui_->showTitle(currentShortcuts.name);
      ui_->showEntries(currentShortcuts.entries);
    }
  } else {
    // Search for a file with the corresponding name
    cmd.erase(0, 1); // erase "/"
    std::string fileName;
    bool result = findShortcutFile(fileName, cmd);
    if (result) {
      bool couldRead = getShortcutsForApp(currentShortcuts, fileName);
      if (couldRead) {
        ui_->setCurrentCmd("");
        ui_->updateInput();
        ui_->showTitle(currentShortcuts.name);
        ui_->showEntries(currentShortcuts.entries);
      } else {
        std::cerr << "Could not read file" << std::endl;
      }
    }
  }
}

