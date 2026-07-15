# EV_Battery_Thermal_Management_Controller
## EV thermal management
### System Architecture
```
┌─────────────────────────────────────────────────────┐
│                 Vehicle Network Layer               │
├─────────────────────────────────────────────────────┤
│ CAN FD │ LIN │ Automotive Ethernet │ UDS Diagnostics│
└─────────────────────────────────────────────────────┘                         
                          ▼
┌─────────────────────────────────────────────────────┐
│             EV Thermal Management ECU              │
├─────────────────────────────────────────────────────         
│  Communication Manager                              │
│  ├─ CAN Rx                                          │
│  ├─ CAN Tx                                          │
│  ├─ UDS Diagnostics                                 │
│  └─ Software Update Interface                       │
│                                                     │
│  Thermal Controller                                 │
│  ├─ Temperature Control                             │
│  ├─ Pump Control                                    │
│  ├─ Fan Control                                     │
│  └─ Safe Mode Logic                                 │
│                                                     │
│  Diagnostics Manager                                │
│  ├─ Sensor Monitoring                               │
│  ├─ Actuator Monitoring                             │
│  ├─ Thermal Monitoring                              │
│  └─ DTC Generation                                  │
│                                                     │
│  Watchdog                                           
│  Fault Manager                                      │
│  Calibration Manager                                │
│  Telemetry Manager                                  │
│                                                     │
└─────────────────────────────────────────────────────┘
                ▼               ▼

      Physical Hardware     Simulation Models

      Sensors              Battery Model
      Pump                 Coolant Loop
      Fan                  Environment Model
```
Rx data: Battery Current, Battery Voltage, Vehicle Speed, Ambient Temperature, Charging Status
Tx data: Battery Temperature
Coolant Temperature, Pump Command, Fan Command, Active Faults, Health Status
Ethernet Interface: Telemetry Streaming, Cloud Analytics, High-Speed Logging

#### System functional architecture:
```
EV Battery Thermal Management System
│
├── Thermal Sensing
│   ├── Cell Temperature Acquisition
│   ├── Coolant Temperature Acquisition
│   └── Sensor Validation
│
├── Thermal Control
│   ├── Temperature Regulation
│   ├── Pump Control
│   ├── Fan Control
│   └── Safe Mode Control
│
├── Battery Thermal Model
│   ├── Heat Generation
│   ├── Heat Transfer
│   ├── Cooling Effectiveness
│   └── Thermal Mass Modeling
│
├── Diagnostics
│   ├── Sensor Monitoring
│   ├── Pump Monitoring
│   ├── Fan Monitoring
│   ├── Thermal Imbalance Detection
│   └── Overtemperature Detection
│
├── Functional Safety
│   ├── Watchdog Supervision
│   ├── Fault Management
│   ├── Degraded Operation
│   └── Safe Shutdown
│
├── Communication
│   ├── CAN Rx
│   ├── CAN Tx
│   ├── UDS Diagnostics
│   └── Calibration Download
│
├── Telemetry
│   ├── Runtime Logging
│   ├── Event Logging
│   └── CSV Export
│
└── Visualization
    ├── Temperature Trending
    ├── Cooling Performance
    ├── Fault Plots
    └── System Metrics
```

#### Context Diagram:
                     ┌───────────────┐
                     │      Cloud    │
                     └───────┬───────┘
                             │ Ethernet
                             ▼

┌─────────────┐     CAN FD      ┌────────────────┐
│     BMS     │◄──────────────►│ Thermal ECU    │
└─────────────┘                └────────────────┘
                                      │
                      ┌───────────────┼───────────────┐
                      ▼               ▼               ▼
              Temperature         Pump Driver      Fan Driver
                Sensors
                      │
                      ▼
                Coolant Loop
                      │
                      ▼
                 Battery Pack

FUSA architecture:
                     Safety Manager
                           │
       ┌───────────────────┼───────────────────┐
       ▼                   ▼                   ▼
 Watchdog          Diagnostics         Fault Manager

       └───────────────────┼───────────────────┘
                           ▼
                     Safe State

#### FUSA layer architecture:
Sensors
   │
   ▼
Diagnostics
   │ PASS
   ▼
Thermal Controller
   │
   ▼
Pump/Fan

Safety goals: 
Normal operating conditions:
Sensors
   │
   ▼
Diagnostics
   │ PASS
   ▼
Thermal Controller
   │
   ▼
Pump/Fan

Battery overheating - Mitigation: Fan Enable, Pump Enable, Reduced Power, Safe Shutdown
Failed pump - Mitigation: Fallback Cooling Mode, DTC Storage, Warning Message
Pump Command > 50%
  │
  ▼
Pump RPM < Threshold
  │
  ▼
Diagnostics
  │
  ▼
PUMP_FAILURE
  │
  ▼
Fault Manager
  │
  ▼
Vehicle Thermal Derating

Sensor failure - Mitigation: Use redundant sensor, Enter degraded mode
Sensor
  │
  ▼
Diagnostics
  ├── SENSOR_OUT_OF_RANGE
  ▼
Fault Manager
  │
  ▼
Safe Mode
  │
  ├── Pump = 100%
  ├── Fan  = 100%
  └── Fault Logged

watchdog failure:
Controller Task Stops
       │
       ▼
Watchdog Timeout
       │
       ▼
WATCHDOG_TIMEOUT
       │
       ▼
Emergency Safe State
       │
       ├── Pump = 100%
       ├── Fan = 100%
       └── Controller Disabled


Software Architecture:
+------------------------------------------------+
| Application Layer                              |
|------------------------------------------------|
| Thermal Controller                             |
| Diagnostics                                    |
| Fault Manager                                  |
+------------------------------------------------+
+------------------------------------------------+
| Service Layer                                  |
|------------------------------------------------|
| Telemetry                                      |
| Configuration                                  |
| UDS                                             |
| Logging                                         |
+------------------------------------------------+
+------------------------------------------------+
| Hardware Abstraction Layer                     |
|------------------------------------------------|
| Temperature Sensor Driver                      |
| Pump Driver                                    |
| Fan Driver                                     |
| CAN Driver                                     |
+------------------------------------------------+
+------------------------------------------------+
| Hardware                                       |
+------------------------------------------------+

Class Diagrams:
Thermal control module:
+------------------+
| ThermalController|
+------------------+
| calibration      |
+------------------+
| update()         |
| enterSafeMode()  |
+------------------+
      │ uses
      ▼
+-----------------------+
| ThermalInputs         |
+-----------------------+
| zoneTempsC            |
| coolantTempC          |
| batteryCurrentA       |
| pumpFeedback          |
| fanFeedback           |
+-----------------------+
+-----------------------+
| ThermalCommands       |
+-----------------------+
| pumpPercent           |
| fanPercent            |
+-----------------------+

Diagnostics module:
+----------------------+
| Diagnostics          |
+----------------------+
| stuckCounter         |
| pumpFaultCounter     |
| fanFaultCounter      |
| coolingCounter       |
+----------------------+
| checkRange()         |
| checkPlausibility()  |
| checkPumpFailure()   |
| checkFanFailure()    |
| evaluate()           |
+----------------------+
          ▼
+----------------------+
| FaultType            |
+----------------------+
| SENSOR_OUT_OF_RANGE  |
| SENSOR_IMPLAUSIBLE   |
| PUMP_FAILURE         |
| FAN_FAILURE          |
| THERMAL_IMBALANCE    |
| OVER_TEMPERATURE     |
+----------------------+

Telemetry module:
+----------------------+
| TelemetryLogger      |
+----------------------+
| m_file              |
+----------------------+
| log()               |
+----------------------+
          ▼
+----------------------+
| TelemetryRecord      |
+----------------------+
| timestampMs         |
| zoneTemps          |
| coolantTemp        |
| batteryCurrent     |
| pumpCommand        |
| fanCommand         |
| pumpRpm            |
| fanRpm             |
| fault              |
+----------------------+

Simulation module:
+----------------------+
| CoolantModel         |
+----------------------+
| coolantTemp         |
+----------------------+
| update()            |
+----------------------+
          │
          ▼
+----------------------+
| BatteryModel         |
+----------------------+
| batteryTemp         |
| thermalMass         |
+----------------------+
| generateHeat()      |
| updateTemp()        |
+----------------------+

Comms module:
+----------------------+
| CanManager           |
+----------------------+
| txQueue             |
| rxQueue             |
+----------------------+
| sendMessage()       |
| receiveMessage()    |
+----------------------+
          │
          ▼
+----------------------+
| CanMessage           |
+----------------------+
| id                  |
| dlc                 |
| data[64]            |
+----------------------+

Safety module:
+----------------------+
| Watchdog             |
+----------------------+
| lastKickTime        |
+----------------------+
| kick()              |
| expired()           |
+----------------------+
          │
          ▼
+----------------------+
| FaultManager         |
+----------------------+
| activeFaults        |
+----------------------+
| evaluate()          |
| clearFaults()       |
+----------------------+

System - Data flow:
Temperature Sensors
        │
        ▼
 ThermalInputs
        │
        ▼
 ThermalController
        │
        ├────────► PumpDriver
        │
        └────────► FanDriver
        │
        ▼
 Diagnostics
        │
        ▼
 Fault Manager
        │
        ▼
 Telemetry Logger
        │
        ▼
 telemetry.csv
        │
        ▼
 Python Visualization
        │
        ▼
 Temperature/Fault Dashboards

 Thermal controls:
 BatterySensor    Controller    PumpDriver   FanDriver   CoolantModel

     │               │             │           │            │
     │ Temperature   │             │           │            │
     ├──────────────►│             │           │            │
     │               │             │           │            │
     │               │ Calculate   │           │            │
     │               │ Commands    │           │            │
     │               │             │           │            │
     │               ├────────────►│           │            │
     │               │ Pump Cmd    │           │            │
     │               │             │           │            │
     │               ├────────────────────────►│            │
     │               │ Fan Cmd     │           │            │
     │               │             │           │            │
     │               │             │           ├───────────►│
     │               │             │           │ Update     │
     │               │             │           │            │
     │               │◄─────────────────────────────────────┤
     │               │ Coolant Temp                        │

Fault detection:
Sensor       Diagnostics      FaultManager      Controller

  │                │                │               │
  │ Temperature    │                │               │
  ├───────────────►│                │               │
  │                │                │               │
  │                │ Range Check    │               │
  │                │ Plausibility   │               │
  │                │                │               │
  │                │ Fault          │               │
  │                ├───────────────►│               │
  │                │                │               │
  │                │                │ Safe Mode     │
  │                │                ├──────────────►│
  │                │                │               │

  Telemetry:
  Main Loop     TelemetryRecord    TelemetryLogger      CSV File

    │                 │                 │               │
    │ Gather Data     │                 │               │
    ├────────────────►│                 │               │
    │                 │                 │               │
    │                 ├────────────────►│               │
    │                 │    log()        │               │
    │                 │                 │               │
    │                 │                 ├──────────────►│
    │                 │                 │ Write CSV     │
    │                 │                 │               │

CAN:
BMS                CAN Driver          Thermal ECU

 │                     │                    │
 │ Battery Current     │                    │
 ├────────────────────►│                    │
 │ CAN Frame           │                    │
 │                     ├───────────────────►│
 │                     │                    │
 │                     │  Update Inputs     │
 │                     │                    │

 Sartup:
 main()
 │
 ▼
Load Calibration
 │
 ▼
Initialize Sensors
 │
 ▼
Initialize Pump Driver
 │
 ▼
Initialize Fan Driver
 │
 ▼
Initialize Diagnostics
 │
 ▼
Initialize Watchdog
 │
 ▼
Initialize Telemetry
 │
 ▼
Start Control Loop