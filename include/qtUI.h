#ifndef QTUI_H
#define QTUI_H

#include <actionsHelperUI.h>
#include <string>
#include <json/json.h>
#include <QApplication>
#include <QObject>

class QQuickView; //Because weird error when putting include here

class QtUI:  public QObject, public ActionsHelperUI
{
  Q_OBJECT
  private:
    QApplication app;
    QQuickView* view;
    QObject* titleQML;
    QObject* inputQML;

  public:
    QtUI(ActionsHelper &logic, int &argc,char* argv[]);
    ~QtUI();
    void startUI();
    void startUI(int &argc,char* argv[]);
    void stopUI();
    void showTitle(std::string title);
    void loadEntries(const std::vector<Entry> entries);
    Q_INVOKABLE bool processCmd(const QString &cmd);
    Q_INVOKABLE void makeSearch(const QString &src, const QString &measure="cosine", const double &threshold=0);

};


#endif

