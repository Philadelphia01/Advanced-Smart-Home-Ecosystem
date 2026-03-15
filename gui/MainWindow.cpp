#include "MainWindow.h"

#include <QScrollArea>
#include <QGridLayout>
#include <QMessageBox>
#include <QFileDialog>
#include <QToolBar>
#include <QAction>
#include <QFont>
#include <QDateTime>
#include <QSplitter>

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {
    setWindowTitle("🏠  Advanced Smart Home Ecosystem");
    setMinimumSize(1100, 720);
    resize(1280, 800);

    applyDarkTheme();
    buildHome();
    buildUI();
    buildToolbar();

    logRedirect = new QtLogRedirect(this);
    connect(logRedirect, &QtLogRedirect::newMessage, this, &MainWindow::appendLog);

    auto* timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::onRefreshStatus);
    timer->start(2000);
}

MainWindow::~MainWindow() {
    delete home;
    delete fileManager;
    delete autoMgr;

    delete ceilingLight;  delete floorLamp;    delete livingThermo;
    delete homeTheatre;   delete livingTV;
    delete bedsideLamp;   delete bedroomThermo; delete btSpeaker;     delete bedroomTV;
    delete kitchenLight;  delete kitchenRadio;
    delete frontCam;      delete garageCam;     delete garageLight;   delete garageDoor;
    delete frontDoorLock; delete homeAlarm;     delete hallCam;
    delete livingRoom;    delete bedroom;       delete kitchen;
    delete garage;        delete entryHall;
}

void MainWindow::buildHome() {
    home = new SmartHome("My Smart Home");

    livingRoom  = new Room("Living Room");
    bedroom     = new Room("Bedroom");
    kitchen     = new Room("Kitchen");
    garage      = new Room("Garage");
    entryHall   = new Room("Entry Hall");

    ceilingLight  = new Light("Ceiling Light",          80);
    floorLamp     = new Light("Floor Lamp",             50);
    livingThermo  = new Thermostat("Living Room Thermostat", 22.0);
    homeTheatre   = new Speaker("Home Theatre Speaker", 40);
    livingTV      = new TV("Living Room TV",             5, 30);

    bedsideLamp   = new Light("Bedside Lamp",           30);
    bedroomThermo = new Thermostat("Bedroom Thermostat",20.0);
    btSpeaker     = new Speaker("Bluetooth Speaker",    25);
    bedroomTV     = new TV("Bedroom TV",                 3, 15);

    kitchenLight  = new Light("Kitchen Light",         100);
    kitchenRadio  = new Speaker("Kitchen Radio",        35);

    frontCam      = new SecurityCamera("Front Door Camera");
    garageCam     = new SecurityCamera("Garage Camera");
    garageLight   = new Light("Garage Light",           70);
    garageDoor    = new DoorLock("Garage Door Lock",   "4321");

    frontDoorLock = new DoorLock("Front Door Lock",    "1234");
    homeAlarm     = new AlarmSystem("Home Alarm",      "9999");
    hallCam       = new SecurityCamera("Entry Hall Camera");

    livingRoom->addDevice(ceilingLight);
    livingRoom->addDevice(floorLamp);
    livingRoom->addDevice(livingThermo);
    livingRoom->addDevice(homeTheatre);
    livingRoom->addDevice(livingTV);

    bedroom->addDevice(bedsideLamp);
    bedroom->addDevice(bedroomThermo);
    bedroom->addDevice(btSpeaker);
    bedroom->addDevice(bedroomTV);

    kitchen->addDevice(kitchenLight);
    kitchen->addDevice(kitchenRadio);

    garage->addDevice(frontCam);
    garage->addDevice(garageCam);
    garage->addDevice(garageLight);
    garage->addDevice(garageDoor);

    entryHall->addDevice(frontDoorLock);
    entryHall->addDevice(homeAlarm);
    entryHall->addDevice(hallCam);

    home->addRoom(livingRoom);
    home->addRoom(bedroom);
    home->addRoom(kitchen);
    home->addRoom(garage);
    home->addRoom(entryHall);

    fileManager = new FileManager("smarthome_state.dat");
    autoMgr     = new AutomationManager;

    autoMgr->addRule(AutomationRule("Good Morning",
        "Lights on, thermostats to 22°C", [this]() {
            ceilingLight->setBrightness(100); ceilingLight->turnOn();
            bedsideLamp->setBrightness(60);   bedsideLamp->turnOn();
            kitchenLight->turnOn();
            livingThermo->setTemperature(22.0); livingThermo->turnOn();
            bedroomThermo->setTemperature(22.0); bedroomThermo->turnOn();
        }));

    autoMgr->addRule(AutomationRule("Good Night",
        "Dim lights, lock doors, silence speakers", [this]() {
            ceilingLight->setBrightness(10); ceilingLight->turnOn();
            floorLamp->turnOff(); kitchenLight->turnOff();
            homeTheatre->setVolume(0); homeTheatre->turnOff();
            btSpeaker->setVolume(0);   btSpeaker->turnOff();
            livingThermo->setTemperature(19.0);
            frontDoorLock->turnOn(); garageDoor->turnOn();
        }));

    autoMgr->addRule(AutomationRule("Movie Night",
        "Dim living room, TV + theatre on", [this]() {
            ceilingLight->setBrightness(20); ceilingLight->turnOn();
            floorLamp->setBrightness(10);    floorLamp->turnOn();
            livingTV->turnOn();  livingTV->setVolume(70);
            homeTheatre->setVolume(60); homeTheatre->turnOn();
        }));

    autoMgr->addRule(AutomationRule("Security Mode",
        "Arm alarm, cameras on, lock all doors", [this]() {
            homeAlarm->turnOn();
            frontCam->turnOn(); garageCam->turnOn(); hallCam->turnOn();
            frontDoorLock->turnOn(); garageDoor->turnOn();
        }));

    autoMgr->addRule(AutomationRule("Eco Mode",
        "Turn off non-essential devices", [this]() {
            floorLamp->turnOff(); homeTheatre->turnOff();
            livingTV->turnOff();  bedroomTV->turnOff();
            livingThermo->setTemperature(18.0);
            bedroomThermo->setTemperature(18.0);
        }));
}

void MainWindow::buildToolbar() {
    auto* tb = addToolBar("Main");
    tb->setMovable(false);
    tb->setStyleSheet("QToolBar { background:#161b22; border-bottom:1px solid #30363d; spacing:6px; padding:4px 8px; }"
                      "QToolButton { color:#cdd9e5; padding:5px 12px; border-radius:5px; font-size:13px; }"
                      "QToolButton:hover { background:#21262d; }");

    QLabel* homeLabel = new QLabel("  🏠  " + QString::fromStdString(home->getName()) + "  ");
    homeLabel->setStyleSheet("color:#58a6ff; font-size:14px; font-weight:bold;");
    tb->addWidget(homeLabel);
    tb->addSeparator();

    auto* allOnAct  = new QAction("✅  All ON",  this);
    auto* allOffAct = new QAction("❌  All OFF", this);
    auto* saveAct   = new QAction("💾  Save",    this);
    auto* loadAct   = new QAction("📂  Load",    this);

    connect(allOnAct,  &QAction::triggered, this, &MainWindow::onTurnAllOn);
    connect(allOffAct, &QAction::triggered, this, &MainWindow::onTurnAllOff);
    connect(saveAct,   &QAction::triggered, this, &MainWindow::onSaveState);
    connect(loadAct,   &QAction::triggered, this, &MainWindow::onLoadState);

    tb->addAction(allOnAct);
    tb->addAction(allOffAct);
    tb->addSeparator();
    tb->addAction(saveAct);
    tb->addAction(loadAct);

    // Status label on the right
    tb->addSeparator();
    statusLabel = new QLabel("  Devices: 0 ON / 0 OFF  ");
    statusLabel->setStyleSheet("color:#8b949e; font-size:12px;");
    tb->addWidget(statusLabel);
}

static QWidget* makeRoomPage(Room* room, std::vector<DeviceWidget*>& out) {
    auto* page = new QWidget;
    page->setStyleSheet("background:#0d1117;");
    auto* grid = new QGridLayout(page);
    grid->setSpacing(12);
    grid->setContentsMargins(16, 16, 16, 16);

    int col = 0, row = 0;
    const int COLS = 2;
    for (SmartDevice* dev : room->getDevices()) {
        auto* w = new DeviceWidget(dev);
        out.push_back(w);
        grid->addWidget(w, row, col);
        col++;
        if (col >= COLS) { col = 0; row++; }
    }
    if (col != 0) {
        // Fill empty columns with stretching space
        for (int c = col; c < COLS; c++)
            grid->addWidget(new QWidget, row, c);
    }
    grid->setRowStretch(row + 1, 1);

    auto* scroll = new QScrollArea;
    scroll->setWidget(page);
    scroll->setWidgetResizable(true);
    scroll->setStyleSheet("QScrollArea { border:none; background:#0d1117; }");

    auto* wrapper = new QWidget;
    auto* wl = new QVBoxLayout(wrapper);
    wl->setContentsMargins(0, 0, 0, 0);
    wl->addWidget(scroll);
    return wrapper;
}

void MainWindow::buildUI() {
    tabWidget = new QTabWidget;
    tabWidget->setStyleSheet(
        "QTabWidget::pane { border:none; background:#0d1117; }"
        "QTabBar::tab { background:#161b22; color:#8b949e; padding:8px 18px;"
        "  border:1px solid #30363d; border-bottom:none; border-radius:4px 4px 0 0; margin-right:2px; }"
        "QTabBar::tab:selected { background:#0d1117; color:#cdd9e5; font-weight:bold; }"
        "QTabBar::tab:hover { color:#cdd9e5; background:#21262d; }"
    );

    tabWidget->addTab(makeRoomPage(livingRoom, allDeviceWidgets), "🛋  Living Room");
    tabWidget->addTab(makeRoomPage(bedroom,    allDeviceWidgets), "🛏  Bedroom");
    tabWidget->addTab(makeRoomPage(kitchen,    allDeviceWidgets), "🍳  Kitchen");
    tabWidget->addTab(makeRoomPage(garage,     allDeviceWidgets), "🚗  Garage");
    tabWidget->addTab(makeRoomPage(entryHall,  allDeviceWidgets), "🚪  Entry Hall");

    autoPanel = new AutomationPanel(autoMgr);
    tabWidget->addTab(autoPanel, "⚡  Automation");

    // Log tab
    logView = new QTextEdit;
    logView->setReadOnly(true);
    logView->setStyleSheet("background:#0d1117; color:#8b949e; font-family:'Courier New',monospace; font-size:12px; border:none;");
    logView->setPlaceholderText("System log will appear here...");
    tabWidget->addTab(logView, "📋  Log");

    setCentralWidget(tabWidget);

    statusBar()->setStyleSheet("background:#161b22; color:#8b949e; font-size:12px;");
    statusBar()->showMessage("Smart Home System Ready");
}

void MainWindow::applyDarkTheme() {
    setStyleSheet(
        "QMainWindow { background:#0d1117; }"
        "QWidget { background:#0d1117; color:#cdd9e5; }"
        "QPushButton { background:#21262d; color:#cdd9e5; border:1px solid #30363d;"
        "  border-radius:5px; padding:5px 12px; font-size:13px; }"
        "QPushButton:hover { background:#30363d; }"
        "QSlider::groove:horizontal { background:#21262d; height:4px; border-radius:2px; }"
        "QSlider::handle:horizontal { background:#58a6ff; width:14px; height:14px;"
        "  margin:-5px 0; border-radius:7px; }"
        "QSlider::sub-page:horizontal { background:#1f6feb; border-radius:2px; }"
        "QSpinBox, QDoubleSpinBox, QLineEdit, QComboBox { background:#161b22; color:#cdd9e5;"
        "  border:1px solid #30363d; border-radius:4px; padding:3px 6px; }"
        "QScrollBar:vertical { background:#0d1117; width:8px; }"
        "QScrollBar::handle:vertical { background:#30363d; border-radius:4px; }"
        "QStatusBar { background:#161b22; }"
    );
}

void MainWindow::updateStatusBar() {
    int total = 0, on = 0;
    for (const Room* r : home->getRooms())
        for (const SmartDevice* d : r->getDevices()) {
            total++;
            if (d->getIsOn()) on++;
        }
    if (statusLabel)
        statusLabel->setText(QString("  Devices: %1 ON / %2 OFF  ").arg(on).arg(total - on));
}

void MainWindow::onRefreshStatus() {
    for (DeviceWidget* w : allDeviceWidgets) w->refresh();
    updateStatusBar();
}

void MainWindow::onTurnAllOn() {
    home->turnAllOn();
    onRefreshStatus();
}

void MainWindow::onTurnAllOff() {
    home->turnAllOff();
    onRefreshStatus();
}

void MainWindow::onSaveState() {
    fileManager->saveState(*home);
    statusBar()->showMessage("State saved to smarthome_state.dat", 3000);
}

void MainWindow::onLoadState() {
    fileManager->loadAndApplyState(*home);
    onRefreshStatus();
    statusBar()->showMessage("State loaded from smarthome_state.dat", 3000);
}

void MainWindow::appendLog(const QString& msg) {
    QString timestamp = QDateTime::currentDateTime().toString("hh:mm:ss");
    logView->append(
        "<span style='color:#484f58;'>[" + timestamp + "]</span> "
        "<span style='color:#cdd9e5;'>" + msg.toHtmlEscaped() + "</span>"
    );
    logView->verticalScrollBar()->setValue(logView->verticalScrollBar()->maximum());
}
