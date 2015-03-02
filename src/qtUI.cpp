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

QtUI::QtUI(int &argc,char* argv[]):
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

  showEntries(logic_->currentShortcuts.entries);
  view->setSource(QUrl::fromLocalFile("../src/shortcutHelperQT.qml"));

  titleQML = view->rootObject()->findChild<QObject*>("titleEl");
  if(titleQML == NULL) {
      QMessageBox::warning(NULL, "Warning", "Failed to resolve title QML element");
  }
  inputQML = view->rootObject()->findChild<QObject*>("inputEl");
if(inputQML == NULL) {
      QMessageBox::warning(NULL, "Warning", "Failed to resolve title QML element");
  }

  showTitle(logic_->currentShortcuts.name);

  view->show();
  app.exec();
}

void QtUI::stopUI() {
  app.quit();
}

void QtUI::showTitle(std::string title) {
  titleQML->setProperty("text", title.c_str());
}

void QtUI::showEntries(const Json::Value entries) {
  QList<QObject*> dataList;
  for (unsigned i=0; i<entries.size(); ++i) {
    Json::Value entry = entries[i];
    dataList.append(new DataObject(entry.get("shortcut", "").asCString(), entry.get("content", "").asCString()));
  }
  QQmlContext* ctxt = view->rootContext();
  ctxt->setContextProperty("shortcutsModel", QVariant::fromValue(dataList));
}

void QtUI::updateInput() {
  inputQML->setProperty("text", currentCmd.c_str());
}

void QtUI::processCmd(const QString &cmd) {
  logic_->processCmd(cmd.toStdString());
}
