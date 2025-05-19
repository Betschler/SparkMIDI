#line 1 "B:\\GIT\\SparkMIDI\\SparkMIDI17\\SparkComms.h"
#ifndef SparkComms_h
#define SparkComms_h

#ifdef CLASSIC
#include "BluetoothSerial.h"
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>
#else
#include "NimBLEDevice.h"
#include "NimBLEServer.h"
#include "NimBLEClient.h"
#include "NimBLEUtils.h"
#endif

#include "RingBuffer.h"

// Functions for logging changes for any UI updates
// Spark amp
#define SPK 0  
// Spark app                 
#define APP 1
// Buetooth MIDI controller  
#define BLE_MIDI 2
// USB Host MIDI
#define USB_MIDI 3
// Serial / DIN MIDI
#define SER_MIDI 4
// Bluetooth app MIDI
//#define NOTHING

#define NUM_CONNS 5

#define TO 0
#define FROM 1
#define STATUS 2

bool conn_status[NUM_CONNS];
unsigned long conn_last_changed[3][NUM_CONNS];

void set_conn_status_connected(int connection);
void set_conn_status_disconnected(int connection);
void set_conn_received(int connection);
void set_conn_sent(int connection);
//New
String last_command_sent="";

#define BLE_BUFSIZE 5000

#define C_SERVICE "ffc0"
#define C_CHAR1   "ffc1"
#define C_CHAR2   "ffc2"

#define S_SERVICE "ffc0"
#define S_CHAR1   "ffc1"
#define S_CHAR2   "ffc2"

#define MIDI_SERVICE     "03b80e5a-ede8-4b33-a751-6ce34ec4c700"
#define MIDI_CHAR        "7772e5db-3868-4112-a1a9-f2669d106bf3"

#define PEDAL_SERVICE    "03b80e5a-ede8-4b33-a751-6ce34ec4c700"
#define PEDAL_CHAR       "7772e5db-3868-4112-a1a9-f2669d106bf3"

#define SPARK_BT_NAME  "Spark 40"

#define MAX_SCAN_COUNT 2

bool connect_to_all();
void connect_spark();
#ifdef BLE_CONTROLLER
void connect_pedal();
#endif

bool sp_available();
bool app_available();
uint8_t sp_read();
uint8_t app_read();
void sp_write(byte *buf, int len);
void app_write(byte *buf, int len);
int ble_getRSSI();

#ifdef CLASSIC
BluetoothSerial *bt;
#endif

bool is_ble;

bool ble_app_connected;
bool classic_app_connected;

#ifdef BLE_CONTROLLER
bool connected_pedal;
bool found_pedal;
#endif

bool connected_sp;
bool found_sp;

NimBLEServer *pServer;
NimBLEService *pService;
NimBLECharacteristic *pCharacteristic_receive;
NimBLECharacteristic *pCharacteristic_send;

#ifdef BLE_APP_MIDI
NimBLEService *pServiceMIDI;
NimBLECharacteristic *pCharacteristicMIDI;
#endif

NimBLEAdvertising *pAdvertising;

NimBLEScan *pScan;
NimBLEScanResults pResults;
NimBLEAdvertisedDevice device;

NimBLEClient *pClient_sp;
NimBLERemoteService *pService_sp;
NimBLERemoteCharacteristic *pReceiver_sp;
NimBLERemoteCharacteristic *pSender_sp;
NimBLERemoteDescriptor* p2902_sp;
NimBLEAdvertisedDevice *sp_device;

#ifdef BLE_CONTROLLER
NimBLEClient *pClient_pedal;
NimBLERemoteService *pService_pedal;
NimBLERemoteCharacteristic *pReceiver_pedal;
NimBLERemoteCharacteristic *pSender_pedal;
NimBLERemoteDescriptor* p2902_pedal;
NimBLEAdvertisedDevice *pedal_device;
#endif

RingBuffer ble_in;
RingBuffer ble_app_in;

#ifdef BLE_CONTROLLER
RingBuffer midi_in;
#endif

#ifdef BLE_APP_MIDI
RingBuffer ble_midi_in;
#endif

#endif
