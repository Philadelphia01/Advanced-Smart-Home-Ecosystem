#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTabWidget>
#include <QScrollArea>
#include <QTextEdit>
#include <QStatusBar>
#include <QToolBar>
#include <QLabel>
#include <QAction>
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QTimer>
#include <vector>

#include "../SmartHome.h"
#include "../Light.h"
#include "../Thermostat.h"
#include "../Speaker.h"
#include "../SecurityCamera.h"
#include "../TV.h"
#include "../DoorLock.h"
#include "../AlarmSystem.h"
#include "../FileManager.h"
#include "../AutomationRule.h"
#include "DeviceWidget.h"
#include "AutomationPanel.h"
#include "LogRedirect.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow() override;

private slots:
    void onSaveState();
    void onLoadState();
    void onTurnAllOn();
    void onTurnAllOff();
    void appendLog(const QString& msg);
    void onRefreshStatus();

private:
    void buildHome();
    void buildUI();
    void buildToolbar();
    void buildRoomTabs();
    void buildAutomationTab();
    void buildLogTab();
    void applyDarkTheme();
    void updateStatusBar();

    // ── Home objects ───────────────────────────────────────────
    SmartHome*      home          = nullptr;
    FileManager*    fileManager   = nullptr;
    AutomationManager* autoMgr   = nullptr;

    // Rooms
    Room* livingRoom  = nullptr;
    Room* bedroom     = nullptr;
    Room* kitchen     = nullptr;
    Room* garage      = nullptr;
    Room* entryHall   = nullptr;

    // Devices (living room)
    Light*          ceilingLight  = nullptr;
    Light*          floorLamp     = nullptr;
    Thermostat*     livingThermo  = nullptr;
    Speaker*        homeTheatre   = nullptr;
    TV*             livingTV      = nullptr;

    // Devices (bedroom)
    Light*          bedsideLamp   = nullptr;
    Thermostat*     bedroomThermo = nullptr;
    Speaker*        btSpeaker     = nullptr;
    TV*             bedroomTV     = nullptr;

    // Devices (kitchen)
    Light*          kitchenLight  = nullptr;
    Speaker*        kitchenRadio  = nullptr;

    // Devices (garage)
    SecurityCamera* frontCam      = nullptr;
    SecurityCamera* garageCam     = nullptr;
    Light*          garageLight   = nullptr;
    DoorLock*       garageDoor    = nullptr;

    // Devices (entry hall)
    DoorLock*       frontDoorLock = nullptr;
    AlarmSystem*    homeAlarm     = nullptr;
    SecurityCamera* hallCam       = nullptr;

    // ── UI ─────────────────────────────────────────────────────
    QTabWidget*     tabWidget     = nullptr;
    QTextEdit*      logView       = nullptr;
    AutomationPanel* autoPanel    = nullptr;
    QLabel*         statusLabel   = nullptr;
    QtLogRedirect*  logRedirect   = nullptr;

    std::vector<DeviceWidget*> allDeviceWidgets;
};

#endif
