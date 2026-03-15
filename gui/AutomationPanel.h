#ifndef AUTOMATIONPANEL_H
#define AUTOMATIONPANEL_H

#include <QWidget>
#include <QListWidget>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include "../AutomationRule.h"

class AutomationPanel : public QWidget {
    Q_OBJECT

public:
    explicit AutomationPanel(AutomationManager* manager, QWidget* parent = nullptr);
    void refresh();

private slots:
    void onExecute();
    void onToggle();
    void onSelectionChanged();

private:
    AutomationManager* manager;
    QListWidget*   ruleList;
    QPushButton*   executeBtn;
    QPushButton*   toggleBtn;
    QLabel*        descLabel;

    void rebuildList();
};

#endif
