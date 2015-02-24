#ifndef SHORTCUTSUI_H
#define SHORTCUTSUI_H

#include <string>
#include <json/json.h>
#include <iostream>

// #include <shortcutsHelper.h>
class ShortcutsHelper;

class ShortcutsUI
{
  private:

  protected:
    ShortcutsHelper* logic_;
    std::string currentCmd;
    bool cmdMode;

    ShortcutsUI(): currentCmd(""), cmdMode(false) {}

  public:

    virtual void startUI() = 0;
    virtual void stopUI() = 0;
    virtual void showEntries(const Json::Value entries) = 0;
    virtual void updateInput() = 0;
    virtual void showTitle(const std::string title) = 0;
    void setLogic(ShortcutsHelper* logic) {
      logic_ = logic;
    };
    void setCurrentCmd(std::string value) {
      currentCmd = value;
    };
};

#endif
