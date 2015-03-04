#ifndef SHORTCUTSUI_H
#define SHORTCUTSUI_H

#include <shortcutsHelper.h>
#include <json/json.h>
#include <string>
#include <iostream>

class ShortcutsUI
{
  private:

  protected:
    ShortcutsHelper& logic_;
    std::string currentCmd;
    bool cmdMode;

    ShortcutsUI(ShortcutsHelper& logic): logic_(logic), currentCmd(""), cmdMode(false) {}

  public:

    virtual ~ShortcutsUI() {};
    virtual void startUI() = 0;
    virtual void stopUI() = 0;
    // virtual void showEntries(const Json::Value entries) = 0;
    // virtual void updateInput() = 0;
    // virtual void showTitle(const std::string title) = 0;
    // void setLogic(ShortcutsHelper* logic) {
    //   logic_ = logic;
    // };
    void setCurrentCmd(std::string value) {
      currentCmd = value;
    };
};

#endif
