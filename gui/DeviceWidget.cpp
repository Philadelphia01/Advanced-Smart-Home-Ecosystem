#include "DeviceWidget.h"
#include <QMessageBox>
#include <QInputDialog>

static const QString DOT_ON  = "background:#2ea043; border-radius:7px;";
static const QString DOT_OFF = "background:#484f58; border-radius:7px;";

DeviceWidget::DeviceWidget(SmartDevice* dev, QWidget* parent)
    : QGroupBox(parent), device(dev)
{
    setTitle(deviceTypeLabel() + "  " + QString::fromStdString(dev->getName()));
    setStyleSheet("QGroupBox { font-weight:bold; color:#cdd9e5; border:1px solid #444c56;"
                  " border-radius:8px; margin-top:8px; padding:6px; }"
                  "QGroupBox::title { subcontrol-origin:margin; left:10px; padding:0 4px; }");

    auto* root = new QVBoxLayout(this);
    root->setSpacing(6);

    // ── Status row ────────────────────────────────────────────
    auto* statusRow = new QHBoxLayout;
    statusDot = new QLabel;
    statusDot->setFixedSize(14, 14);
    statusLabel = new QLabel("OFF");
    statusLabel->setStyleSheet("color:#8b949e; font-size:12px;");
    toggleBtn = new QPushButton("Turn ON");
    toggleBtn->setFixedWidth(90);
    connect(toggleBtn, &QPushButton::clicked, this, &DeviceWidget::onToggle);

    statusRow->addWidget(statusDot);
    statusRow->addWidget(statusLabel);
    statusRow->addStretch();
    statusRow->addWidget(toggleBtn);
    root->addLayout(statusRow);

    // ── Type-specific controls ────────────────────────────────
    Light*          l   = dynamic_cast<Light*>(device);
    Thermostat*     th  = dynamic_cast<Thermostat*>(device);
    Speaker*        sp  = dynamic_cast<Speaker*>(device);
    TV*             tv  = dynamic_cast<TV*>(device);
    DoorLock*       dl  = dynamic_cast<DoorLock*>(device);
    AlarmSystem*    al  = dynamic_cast<AlarmSystem*>(device);

    if (l) {
        auto* row = new QHBoxLayout;
        QLabel* lbl = new QLabel("Brightness:");
        lbl->setStyleSheet("color:#8b949e;");
        brightnessSlider = new QSlider(Qt::Horizontal);
        brightnessSlider->setRange(0, 100);
        brightnessSlider->setValue(l->getBrightness());
        brightnessLabel = new QLabel(QString::number(l->getBrightness()) + "%");
        brightnessLabel->setFixedWidth(40);
        brightnessLabel->setStyleSheet("color:#cdd9e5;");
        connect(brightnessSlider, &QSlider::valueChanged, this, &DeviceWidget::onBrightnessChanged);
        row->addWidget(lbl);
        row->addWidget(brightnessSlider);
        row->addWidget(brightnessLabel);
        root->addLayout(row);
    }

    if (th) {
        auto* row = new QHBoxLayout;
        QLabel* lbl = new QLabel("Temperature (°C):");
        lbl->setStyleSheet("color:#8b949e;");
        tempSpin = new QDoubleSpinBox;
        tempSpin->setRange(10.0, 35.0);
        tempSpin->setSingleStep(0.5);
        tempSpin->setValue(th->getTemperature());
        tempSpin->setSuffix(" °C");
        tempLabel = new QLabel;
        tempLabel->setStyleSheet("color:#cdd9e5;");
        connect(tempSpin, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
                this, &DeviceWidget::onTemperatureChanged);
        row->addWidget(lbl);
        row->addWidget(tempSpin);
        row->addStretch();
        root->addLayout(row);
    }

    if (sp) {
        auto* row = new QHBoxLayout;
        QLabel* lbl = new QLabel("Volume:");
        lbl->setStyleSheet("color:#8b949e;");
        volumeSlider = new QSlider(Qt::Horizontal);
        volumeSlider->setRange(0, 100);
        volumeSlider->setValue(sp->getVolume());
        volumeLabel = new QLabel(QString::number(sp->getVolume()));
        volumeLabel->setFixedWidth(36);
        volumeLabel->setStyleSheet("color:#cdd9e5;");
        connect(volumeSlider, &QSlider::valueChanged, this, &DeviceWidget::onVolumeChanged);
        row->addWidget(lbl);
        row->addWidget(volumeSlider);
        row->addWidget(volumeLabel);
        root->addLayout(row);
    }

    if (tv) {
        auto* r1 = new QHBoxLayout;
        QLabel* chLbl = new QLabel("Channel:");
        chLbl->setStyleSheet("color:#8b949e;");
        channelSpin = new QSpinBox;
        channelSpin->setRange(1, 999);
        channelSpin->setValue(tv->getChannel());
        connect(channelSpin, QOverload<int>::of(&QSpinBox::valueChanged),
                this, &DeviceWidget::onChannelChanged);
        r1->addWidget(chLbl);
        r1->addWidget(channelSpin);
        r1->addStretch();
        root->addLayout(r1);

        auto* r2 = new QHBoxLayout;
        QLabel* volLbl = new QLabel("Volume:");
        volLbl->setStyleSheet("color:#8b949e;");
        tvVolSlider = new QSlider(Qt::Horizontal);
        tvVolSlider->setRange(0, 100);
        tvVolSlider->setValue(tv->getVolume());
        tvVolLabel = new QLabel(QString::number(tv->getVolume()));
        tvVolLabel->setFixedWidth(36);
        tvVolLabel->setStyleSheet("color:#cdd9e5;");
        connect(tvVolSlider, &QSlider::valueChanged, this, &DeviceWidget::onVolumeChanged);
        r2->addWidget(volLbl);
        r2->addWidget(tvVolSlider);
        r2->addWidget(tvVolLabel);
        root->addLayout(r2);

        auto* r3 = new QHBoxLayout;
        QLabel* inLbl = new QLabel("Input:");
        inLbl->setStyleSheet("color:#8b949e;");
        inputCombo = new QComboBox;
        inputCombo->addItems({"HDMI 1", "HDMI 2", "AV", "USB", "Antenna"});
        inputCombo->setCurrentText(QString::fromStdString(tv->getInput()));
        connect(inputCombo, &QComboBox::currentTextChanged,
                this, &DeviceWidget::onInputChanged);
        r3->addWidget(inLbl);
        r3->addWidget(inputCombo);
        r3->addStretch();
        root->addLayout(r3);
    }

    if (dl) {
        auto* r1 = new QHBoxLayout;
        QLabel* pinLbl = new QLabel("PIN:");
        pinLbl->setStyleSheet("color:#8b949e;");
        pinEdit = new QLineEdit;
        pinEdit->setEchoMode(QLineEdit::Password);
        pinEdit->setPlaceholderText("Enter PIN...");
        pinEdit->setMaxLength(8);
        pinEdit->setFixedWidth(120);
        unlockBtn = new QPushButton("Unlock");
        unlockBtn->setStyleSheet("background:#238636; color:white; border-radius:4px; padding:3px 10px;");
        lockBtn   = new QPushButton("Lock");
        lockBtn->setStyleSheet("background:#da3633; color:white; border-radius:4px; padding:3px 10px;");
        connect(unlockBtn, &QPushButton::clicked, this, &DeviceWidget::onUnlock);
        connect(lockBtn,   &QPushButton::clicked, this, &DeviceWidget::onLock);
        r1->addWidget(pinLbl);
        r1->addWidget(pinEdit);
        r1->addWidget(unlockBtn);
        r1->addWidget(lockBtn);
        r1->addStretch();
        root->addLayout(r1);
    }

    if (al) {
        auto* r1 = new QHBoxLayout;
        QLabel* codeLbl = new QLabel("Code:");
        codeLbl->setStyleSheet("color:#8b949e;");
        codeEdit = new QLineEdit;
        codeEdit->setEchoMode(QLineEdit::Password);
        codeEdit->setPlaceholderText("Access code...");
        codeEdit->setMaxLength(8);
        codeEdit->setFixedWidth(120);
        r1->addWidget(codeLbl);
        r1->addWidget(codeEdit);
        r1->addStretch();
        root->addLayout(r1);

        auto* r2 = new QHBoxLayout;
        armHomeBtn = new QPushButton("Arm HOME");
        armHomeBtn->setStyleSheet("background:#9e6a03; color:white; border-radius:4px; padding:3px 8px;");
        armAwayBtn = new QPushButton("Arm AWAY");
        armAwayBtn->setStyleSheet("background:#da3633; color:white; border-radius:4px; padding:3px 8px;");
        disarmBtn  = new QPushButton("Disarm");
        disarmBtn->setStyleSheet("background:#238636; color:white; border-radius:4px; padding:3px 8px;");
        alarmStateLabel = new QLabel("DISARMED");
        alarmStateLabel->setStyleSheet("color:#2ea043; font-weight:bold;");
        connect(armHomeBtn, &QPushButton::clicked, this, &DeviceWidget::onArmHome);
        connect(armAwayBtn, &QPushButton::clicked, this, &DeviceWidget::onArmAway);
        connect(disarmBtn,  &QPushButton::clicked, this, &DeviceWidget::onDisarm);
        r2->addWidget(armHomeBtn);
        r2->addWidget(armAwayBtn);
        r2->addWidget(disarmBtn);
        r2->addStretch();
        r2->addWidget(alarmStateLabel);
        root->addLayout(r2);
    }

    updateStatusDot();
    updateToggleButton();
}

void DeviceWidget::updateStatusDot() {
    statusDot->setStyleSheet(device->getIsOn() ? DOT_ON : DOT_OFF);
    statusLabel->setText(device->getIsOn() ? "ON" : "OFF");
    statusLabel->setStyleSheet(device->getIsOn()
        ? "color:#2ea043; font-weight:bold;"
        : "color:#8b949e;");
}

void DeviceWidget::updateToggleButton() {
    if (device->getIsOn()) {
        toggleBtn->setText("Turn OFF");
        toggleBtn->setStyleSheet("background:#da3633; color:white; border-radius:4px; padding:4px 8px;");
    } else {
        toggleBtn->setText("Turn ON");
        toggleBtn->setStyleSheet("background:#238636; color:white; border-radius:4px; padding:4px 8px;");
    }
}

void DeviceWidget::refresh() {
    updateStatusDot();
    updateToggleButton();

    Light* l = dynamic_cast<Light*>(device);
    if (l && brightnessSlider) {
        brightnessSlider->blockSignals(true);
        brightnessSlider->setValue(l->getBrightness());
        brightnessLabel->setText(QString::number(l->getBrightness()) + "%");
        brightnessSlider->blockSignals(false);
    }

    Thermostat* th = dynamic_cast<Thermostat*>(device);
    if (th && tempSpin) {
        tempSpin->blockSignals(true);
        tempSpin->setValue(th->getTemperature());
        tempSpin->blockSignals(false);
    }

    Speaker* sp = dynamic_cast<Speaker*>(device);
    if (sp && volumeSlider) {
        volumeSlider->blockSignals(true);
        volumeSlider->setValue(sp->getVolume());
        volumeLabel->setText(QString::number(sp->getVolume()));
        volumeSlider->blockSignals(false);
    }

    AlarmSystem* al = dynamic_cast<AlarmSystem*>(device);
    if (al && alarmStateLabel) {
        switch (al->getAlarmState()) {
            case AlarmSystem::State::DISARMED:
                alarmStateLabel->setText("DISARMED");
                alarmStateLabel->setStyleSheet("color:#2ea043; font-weight:bold;");
                break;
            case AlarmSystem::State::ARMED_HOME:
                alarmStateLabel->setText("ARMED (HOME)");
                alarmStateLabel->setStyleSheet("color:#d29922; font-weight:bold;");
                break;
            case AlarmSystem::State::ARMED_AWAY:
                alarmStateLabel->setText("ARMED (AWAY)");
                alarmStateLabel->setStyleSheet("color:#da3633; font-weight:bold;");
                break;
            case AlarmSystem::State::TRIGGERED:
                alarmStateLabel->setText("!! TRIGGERED !!");
                alarmStateLabel->setStyleSheet("color:#da3633; font-weight:bold; font-size:14px;");
                break;
        }
    }

    DoorLock* dl = dynamic_cast<DoorLock*>(device);
    if (dl) {
        QString lockState = dl->getLocked() ? "LOCKED" : "UNLOCKED";
        setTitle(deviceTypeLabel() + "  " +
                 QString::fromStdString(device->getName()) +
                 "  [" + lockState + "]");
    }
}

void DeviceWidget::onToggle() {
    if (device->getIsOn()) device->turnOff();
    else device->turnOn();
    refresh();
}

void DeviceWidget::onBrightnessChanged(int val) {
    Light* l = dynamic_cast<Light*>(device);
    if (l) { l->setBrightness(val); brightnessLabel->setText(QString::number(val) + "%"); }
}

void DeviceWidget::onTemperatureChanged(double val) {
    Thermostat* th = dynamic_cast<Thermostat*>(device);
    if (th) th->setTemperature(val);
}

void DeviceWidget::onVolumeChanged(int val) {
    Speaker* sp = dynamic_cast<Speaker*>(device);
    if (sp) { sp->setVolume(val); if (volumeLabel) volumeLabel->setText(QString::number(val)); }

    TV* tv = dynamic_cast<TV*>(device);
    if (tv) { tv->setVolume(val); if (tvVolLabel) tvVolLabel->setText(QString::number(val)); }
}

void DeviceWidget::onChannelChanged(int val) {
    TV* tv = dynamic_cast<TV*>(device);
    if (tv) tv->setChannel(val);
}

void DeviceWidget::onInputChanged(const QString& input) {
    TV* tv = dynamic_cast<TV*>(device);
    if (tv) tv->setInput(input.toStdString());
}

void DeviceWidget::onUnlock() {
    DoorLock* dl = dynamic_cast<DoorLock*>(device);
    if (dl && pinEdit) {
        bool ok = dl->unlock(pinEdit->text().toStdString());
        pinEdit->clear();
        refresh();
        if (!ok) QMessageBox::warning(this, "Wrong PIN", "Incorrect PIN entered.");
    }
}

void DeviceWidget::onLock() {
    DoorLock* dl = dynamic_cast<DoorLock*>(device);
    if (dl) { dl->lock(); refresh(); }
}

void DeviceWidget::onArmHome() {
    AlarmSystem* al = dynamic_cast<AlarmSystem*>(device);
    if (al && codeEdit) {
        bool ok = al->armHome(codeEdit->text().toStdString());
        codeEdit->clear();
        refresh();
        if (!ok) QMessageBox::warning(this, "Wrong Code", "Incorrect access code.");
    }
}

void DeviceWidget::onArmAway() {
    AlarmSystem* al = dynamic_cast<AlarmSystem*>(device);
    if (al && codeEdit) {
        bool ok = al->armAway(codeEdit->text().toStdString());
        codeEdit->clear();
        refresh();
        if (!ok) QMessageBox::warning(this, "Wrong Code", "Incorrect access code.");
    }
}

void DeviceWidget::onDisarm() {
    AlarmSystem* al = dynamic_cast<AlarmSystem*>(device);
    if (al && codeEdit) {
        bool ok = al->disarm(codeEdit->text().toStdString());
        codeEdit->clear();
        refresh();
        if (!ok) QMessageBox::warning(this, "Wrong Code", "Incorrect access code.");
    }
}

QString DeviceWidget::deviceTypeLabel() const {
    if (dynamic_cast<Light*>(device))          return "💡";
    if (dynamic_cast<Thermostat*>(device))     return "🌡";
    if (dynamic_cast<Speaker*>(device))        return "🔊";
    if (dynamic_cast<SecurityCamera*>(device)) return "📷";
    if (dynamic_cast<TV*>(device))             return "📺";
    if (dynamic_cast<DoorLock*>(device))       return "🔒";
    if (dynamic_cast<AlarmSystem*>(device))    return "🚨";
    return "📱";
}
