#ifndef ACTIONSHELPERUI_H
#define ACTIONSHELPERUI_H

#include <actionsHelper.h>
#include <string>
#include <iostream>

class ActionsHelperUI
{
  private:

  protected:
    ActionsHelper& logic_;
    std::string currentCmd;
    bool cmdMode;

    ActionsHelperUI(ActionsHelper& logic): logic_(logic), currentCmd(""), cmdMode(false) {}

  public:

    virtual ~ActionsHelperUI() {};
    virtual void startUI() = 0;
    virtual void stopUI() = 0;
    // virtual void showEntries(const Json::Value entries) = 0;
    // virtual void updateInput() = 0;
    // virtual void showTitle(const std::string title) = 0;
    void setCurrentCmd(std::string value) {
      currentCmd = value;
    };
};

#endif
