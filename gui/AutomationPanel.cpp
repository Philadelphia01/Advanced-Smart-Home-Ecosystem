#include "AutomationPanel.h"

AutomationPanel::AutomationPanel(AutomationManager* mgr, QWidget* parent)
    : QWidget(parent), manager(mgr)
{
    setStyleSheet("background:#0d1117;");
    auto* layout = new QVBoxLayout(this);
    layout->setSpacing(10);
    layout->setContentsMargins(16, 16, 16, 16);

    QLabel* title = new QLabel("⚡  Automation Rules");
    title->setStyleSheet("color:#cdd9e5; font-size:16px; font-weight:bold;");
    layout->addWidget(title);

    QLabel* hint = new QLabel("Select a rule and click Execute to run it, or Toggle to enable/disable it.");
    hint->setStyleSheet("color:#8b949e; font-size:12px;");
    hint->setWordWrap(true);
    layout->addWidget(hint);

    ruleList = new QListWidget;
    ruleList->setStyleSheet(
        "QListWidget { background:#161b22; border:1px solid #30363d; border-radius:6px; color:#cdd9e5; font-size:13px; }"
        "QListWidget::item { padding:10px 14px; border-bottom:1px solid #21262d; }"
        "QListWidget::item:selected { background:#1f6feb33; color:#58a6ff; }"
        "QListWidget::item:hover { background:#21262d; }"
    );
    ruleList->setMinimumHeight(200);
    layout->addWidget(ruleList);

    descLabel = new QLabel("Select a rule to see its description.");
    descLabel->setStyleSheet("color:#8b949e; font-size:12px; padding:8px; background:#161b22;"
                              " border:1px solid #30363d; border-radius:6px;");
    descLabel->setWordWrap(true);
    descLabel->setMinimumHeight(50);
    layout->addWidget(descLabel);

    auto* btnRow = new QHBoxLayout;
    executeBtn = new QPushButton("▶  Execute Rule");
    executeBtn->setEnabled(false);
    executeBtn->setStyleSheet("background:#238636; color:white; border-radius:6px; padding:8px 18px; font-size:13px;");
    toggleBtn = new QPushButton("⏸  Toggle Enable");
    toggleBtn->setEnabled(false);
    toggleBtn->setStyleSheet("background:#21262d; color:#cdd9e5; border:1px solid #30363d;"
                               " border-radius:6px; padding:8px 18px; font-size:13px;");
    btnRow->addWidget(executeBtn);
    btnRow->addWidget(toggleBtn);
    btnRow->addStretch();
    layout->addLayout(btnRow);
    layout->addStretch();

    connect(executeBtn, &QPushButton::clicked, this, &AutomationPanel::onExecute);
    connect(toggleBtn,  &QPushButton::clicked, this, &AutomationPanel::onToggle);
    connect(ruleList,   &QListWidget::itemSelectionChanged, this, &AutomationPanel::onSelectionChanged);

    rebuildList();
}

void AutomationPanel::rebuildList() {
    int prevRow = ruleList->currentRow();
    ruleList->clear();
    auto infos = manager->getRuleInfos();
    for (const auto& info : infos) {
        QString icon = info.isActive ? "✅" : "⬜";
        QString text = icon + "  " + QString::fromStdString(info.name)
                     + "  —  " + QString::fromStdString(info.description);
        auto* item = new QListWidgetItem(text);
        if (!info.isActive)
            item->setForeground(QColor("#484f58"));
        ruleList->addItem(item);
    }
    if (prevRow >= 0 && prevRow < ruleList->count())
        ruleList->setCurrentRow(prevRow);
}

void AutomationPanel::refresh() {
    rebuildList();
}

void AutomationPanel::onSelectionChanged() {
    bool hasSelection = !ruleList->selectedItems().isEmpty();
    executeBtn->setEnabled(hasSelection);
    toggleBtn->setEnabled(hasSelection);
}

void AutomationPanel::onExecute() {
    int idx = ruleList->currentRow();
    if (idx >= 0) manager->executeRule(idx);
    rebuildList();
}

void AutomationPanel::onToggle() {
    int idx = ruleList->currentRow();
    if (idx >= 0) manager->toggleRule(idx);
    rebuildList();
}
