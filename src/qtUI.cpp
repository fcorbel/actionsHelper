#include <qtUI.h>
#include <shortcutsHelper.h>
#include <QtQml/QQmlEngine>
#include <iostream>
#include <QQuickView>
#include <QQuickItem> 
#include <qqmlcontext.h>
#include <QMessageBox>
#include "dataObject.h"

/////////////////////////
// QtUI
/////////////////////////

QtUI::QtUI(ShortcutsHelper &logic, int &argc,char* argv[]):
  ShortcutsUI(logic),
  app(argc, argv),
  view(new QQuickView)
{
  view->rootContext()->setContextProperty( "qtCpp", this );
}

QtUI::~QtUI() {
  delete view;
}

void QtUI::startUI() {

  view->connect(view->engine(), SIGNAL(quit()), SLOT(close()));
  view->setResizeMode(QQuickView::SizeRootObjectToView);

  loadEntries(logic_.getLoadedShortcutsEntries());
  view->setSource(QUrl::fromLocalFile("../src/shortcutHelperQT.qml"));

  titleQML = view->rootObject()->findChild<QObject*>("titleEl");
  if (titleQML == NULL) {
      QMessageBox::warning(NULL, "Warning", "Failed to resolve title QML element");
  }
  inputQML = view->rootObject()->findChild<QObject*>("inputEl");
  if (inputQML == NULL) {
      QMessageBox::warning(NULL, "Warning", "Failed to resolve title QML element");
  }

  showTitle(logic_.getLoadedShortcutsName());

  view->show();
  app.exec();
}

void QtUI::stopUI() {
  app.quit();
}

void QtUI::showEntries() {
  // Title

  // Shortcuts

}

void QtUI::showTitle(std::string title) {
  titleQML->setProperty("text", title.c_str());
}

void QtUI::loadEntries(const Json::Value entries) {
  QList<QObject*> dataList;
  for (unsigned i=0; i<entries.size(); ++i) {
    Json::Value entry = entries[i];
    dataList.append(new DataObject(entry.get("shortcut", "").asCString(), entry.get("content", "").asCString()));
  }
  QQmlContext* ctxt = view->rootContext();
  ctxt->setContextProperty("shortcutsModel", QVariant::fromValue(dataList));
}

// void QtUI::updateInput() {
//   inputQML->setProperty("text", currentCmd.c_str());
// }

bool QtUI::processCmd(const QString &cmd) {
  if (cmd == "/quit") {
    stopUI();
    return false;
  }
  if (logic_.processCmd(cmd.toStdString())) {
    loadEntries(logic_.getLoadedShortcutsEntries());
    showTitle(logic_.getLoadedShortcutsName());
    inputQML->setProperty("text", "");
    return true;
  }
  return false;
}

void QtUI::makeSearch(const QString &cmd) {
  // logic_.makeSearch(cmd.toStdString());
}
