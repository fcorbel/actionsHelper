#ifndef QTUI_H
#define QTUI_H

#include <shortcutsUI.h>
#include <string>
#include <json/json.h>
#include <QApplication>
#include <QObject>

class QQuickView; //Because weird error when putting include here

class QtUI:  public QObject, public ShortcutsUI
{
  Q_OBJECT
  private:
    QApplication app;
    QQuickView* view;
    QObject* titleQML;
    QObject* inputQML;

  public:
    QtUI(int &argc,char* argv[]);
    ~QtUI();
    void startUI();
    void startUI(int &argc,char* argv[]);
    void stopUI();
    void showTitle(std::string title);
    void showEntries(const Json::Value entries);
    void updateInput();
    Q_INVOKABLE void processCmd(const QString &cmd);

};


#endif

