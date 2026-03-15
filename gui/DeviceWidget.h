#ifndef DEVICEWIDGET_H
#define DEVICEWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QSlider>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QLineEdit>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QComboBox>

#include "../SmartDevice.h"
#include "../Light.h"
#include "../Thermostat.h"
#include "../Speaker.h"
#include "../SecurityCamera.h"
#include "../TV.h"
#include "../DoorLock.h"
#include "../AlarmSystem.h"

class DeviceWidget : public QGroupBox {
    Q_OBJECT

public:
    explicit DeviceWidget(SmartDevice* device, QWidget* parent = nullptr);
    void refresh();

private slots:
    void onToggle();
    void onBrightnessChanged(int val);
    void onTemperatureChanged(double val);
    void onVolumeChanged(int val);
    void onChannelChanged(int val);
    void onUnlock();
    void onLock();
    void onArmHome();
    void onArmAway();
    void onDisarm();
    void onInputChanged(const QString& input);

private:
    SmartDevice*    device;
    QLabel*         statusDot;
    QLabel*         statusLabel;
    QPushButton*    toggleBtn;

    // Light
    QSlider*        brightnessSlider  = nullptr;
    QLabel*         brightnessLabel   = nullptr;

    // Thermostat
    QDoubleSpinBox* tempSpin          = nullptr;
    QLabel*         tempLabel         = nullptr;

    // Speaker
    QSlider*        volumeSlider      = nullptr;
    QLabel*         volumeLabel       = nullptr;

    // TV
    QSpinBox*       channelSpin       = nullptr;
    QSlider*        tvVolSlider       = nullptr;
    QLabel*         tvVolLabel        = nullptr;
    QComboBox*      inputCombo        = nullptr;

    // Door Lock
    QLineEdit*      pinEdit           = nullptr;
    QPushButton*    unlockBtn         = nullptr;
    QPushButton*    lockBtn           = nullptr;

    // Alarm
    QLineEdit*      codeEdit          = nullptr;
    QPushButton*    armHomeBtn        = nullptr;
    QPushButton*    armAwayBtn        = nullptr;
    QPushButton*    disarmBtn         = nullptr;
    QLabel*         alarmStateLabel   = nullptr;

    void updateToggleButton();
    void updateStatusDot();
    QString deviceTypeLabel() const;
};

#endif
