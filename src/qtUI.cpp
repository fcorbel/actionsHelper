#include <qtUI.h>
#include <actionsHelper.h>
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

QtUI::QtUI(ActionsHelper &logic, int &argc,char* argv[]):
  ActionsHelperUI(logic),
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

  loadEntries(logic_.getLoadedEntries());
  view->setSource(QUrl::fromLocalFile("../src/actionsHelperQT.qml"));

  titleQML = view->rootObject()->findChild<QObject*>("titleEl");
  if (titleQML == NULL) {
      QMessageBox::warning(NULL, "Warning", "Failed to resolve title QML element");
  }
  inputQML = view->rootObject()->findChild<QObject*>("inputEl");
  if (inputQML == NULL) {
      QMessageBox::warning(NULL, "Warning", "Failed to resolve title QML element");
  }

  showTitle(logic_.getLoadedAppName());

  view->show();
  app.exec();
}

void QtUI::stopUI() {
  app.quit();
}

void QtUI::showTitle(std::string title) {
  titleQML->setProperty("text", title.c_str());
}

void QtUI::loadEntries(const std::vector<Entry> entries) {
  QList<QObject*> dataList;
  for (auto it=entries.begin(); it != entries.end(); ++it) {
    Entry entry = *it;
    dataList.append(new DataObject(QString::fromStdString(entry.action), QString::fromStdString(entry.description)));
  }
  QQmlContext* ctxt = view->rootContext();
  ctxt->setContextProperty("entriesModel", QVariant::fromValue(dataList));
}

bool QtUI::processCmd(const QString &cmd) {
  if (cmd == "/quit") {
    stopUI();
    return false;
  }
  if (logic_.processCmd(cmd.toStdString())) {
    loadEntries(logic_.getLoadedEntries());
    showTitle(logic_.getLoadedAppName());
    inputQML->setProperty("text", "");
    return true;
  }
  return false;
}

void QtUI::makeSearch(const QString &search, const QString &measure, const double &threshold) {
  auto result = logic_.makeSearch(search.toStdString(), measure.toStdString(), threshold);
  if (result.size() > 0) {
    std::cout << "[" << std::endl;
    for (int i = 0;i < (int)result.size();++i) {
        std::cout << result[i] << std::endl;
    }
    std::cout << "]" << std::endl;
  }
}
