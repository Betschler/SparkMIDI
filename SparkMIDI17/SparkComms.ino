#include "Spark.h"
#include "SparkComms.h"

const uint8_t notifyOn[] = {0x1, 0x0};

// client callback for connection to Spark
class MyClientCallback : public NimBLEClientCallbacks
{
  void onConnect(NimBLEClient *pclient)
  {
    DEBUG("callback: Spark connected");
    set_conn_status_connected(SPK);
  }

  void onDisconnect(NimBLEClient *pclient)
  {
    connected_sp = false;         
    DEBUG("callback: Spark disconnected");   
    set_conn_status_disconnected(SPK);   
  }
};

// server callback for connection to BLE app
class MyServerCallback : public NimBLEServerCallbacks
{
  void onConnect(NimBLEServer *pserver)
  {
     if (pserver->getConnectedCount() == 1) {
      set_conn_status_connected(APP);
      DEBUG("callback: BLE app connection event and is connected"); 
    }
    else {
      DEBUG("callback: BLE app connection event and is not really connected");   
    }
  }

  void onDisconnect(NimBLEServer *pserver)
  {
    DEBUG("callback: BLE app disconnected");
    set_conn_status_disconnected(APP);
  }
};

// BLE MIDI
#ifdef BLE_APP_MIDI
class MyMIDIServerCallback : public NimBLEServerCallbacks
{
  void onConnect(NimBLEServer *pserver)
  {
    DEBUG("callback: BLE MIDI connected");
  }

  void onDisconnect(NimBLEServer *pserver)
  {
    DEBUG("callback: BLE MIDI disconnected");
  }
};
#endif

#ifdef CLASSIC
// server callback for connection to BT classic app
void bt_callback(esp_spp_cb_event_t event, esp_spp_cb_param_t *param){
  if(event == ESP_SPP_SRV_OPEN_EVT){
    DEBUG("callback: Classic BT Spark app connected");
    set_conn_status_connected(APP);
  }
 
  if(event == ESP_SPP_CLOSE_EVT ){
    DEBUG("callback: Classic BT Spark app disconnected");
    set_conn_status_disconnected(APP);
  }
}
#endif

void notifyCB_sp(NimBLERemoteCharacteristic* pRemoteCharacteristic, uint8_t* pData, size_t length, bool isNotify) {
  int i;
  byte b;
  
  for (i = 0; i < length; i++) {
    b = pData[i];
    ble_in.add(b);
  }
  ble_in.commit();
}

#ifdef BLE_CONTROLLER
void notifyCB_pedal(NimBLERemoteCharacteristic* pRemoteCharacteristic, uint8_t* pData, size_t length, bool isNotify){
  int i;
  byte b;

  for (i = 0; i < length; i++) {
    b = pData[i];
    midi_in.add(b);
  }
  midi_in.commit();

  set_conn_received(BLE_MIDI);  
}
#endif

class CharacteristicCallbacks: public NimBLECharacteristicCallbacks {
  void onWrite(NimBLECharacteristic* pCharacteristic) {
    for (auto & it : pCharacteristic->getValue()) {
      ble_app_in.add(it);
    }
    ble_app_in.commit();
  };
};

static CharacteristicCallbacks chrCallbacks_s, chrCallbacks_r;

#ifdef BLE_APP_MIDI
class MIDICharacteristicCallbacks: public NimBLECharacteristicCallbacks {
  void onWrite(NimBLECharacteristic* pCharacteristic) {
    for (auto & it : pCharacteristic->getValue()) {
      ble_midi_in.add(it);
    }
    ble_midi_in.commit();
  };
};

static MIDICharacteristicCallbacks chrCallbacksMIDI;
#endif

NimBLEUUID SpServiceUuid(C_SERVICE);
#ifdef BLE_CONTROLLER  
NimBLEUUID PedalServiceUuid(PEDAL_SERVICE);
#endif

void connect_spark() {
  if (found_sp && !connected_sp) {
    if (pClient_sp != nullptr && pClient_sp->isConnected())
       DEBUG("HMMMM - connect_spark() SAYS I WAS CONNECTED ANYWAY");
    
    if (pClient_sp->connect(sp_device)) {
#if defined CLASSIC  && !defined HELTEC_WIFI
      pClient_sp->setMTU(517);  
#endif
      connected_sp = true;
      pService_sp = pClient_sp->getService(SpServiceUuid);
      if (pService_sp != nullptr) {
        pSender_sp   = pService_sp->getCharacteristic(C_CHAR1);
        pReceiver_sp = pService_sp->getCharacteristic(C_CHAR2);
        if (pReceiver_sp && pReceiver_sp->canNotify()) {
#ifdef CLASSIC
          pReceiver_sp->registerForNotify(notifyCB_sp);
          p2902_sp = pReceiver_sp->getDescriptor(NimBLEUUID((uint16_t)0x2902));
          if (p2902_sp != nullptr)
             p2902_sp->writeValue((uint8_t*)notifyOn, 2, true);
#else
          if (!pReceiver_sp->subscribe(true, notifyCB_sp, true)) {
            connected_sp = false;
            DEBUG("Spark disconnected");
            NimBLEDevice::deleteClient(pClient_sp);
          }   
#endif
        } 
      }
      DEBUG("connect_spark(): Spark connected");
    }
  }
}

#ifdef BLE_CONTROLLER
void connect_pedal() {
  if (found_pedal && !connected_pedal) {
    if (pClient_pedal->connect(pedal_device)) {  
#if defined CLASSIC && !defined HELTEC_WIFI
      pClient_sp->setMTU(517);
#endif
      connected_pedal = true;
      pService_pedal = pClient_pedal->getService(PedalServiceUuid);
      if (pService_pedal != nullptr) {
        pReceiver_pedal = pService_pedal->getCharacteristic(PEDAL_CHAR);

        if (pReceiver_pedal && pReceiver_pedal->canNotify()) {
#ifdef CLASSIC
          pReceiver_pedal->registerForNotify(notifyCB_pedal);
          p2902_pedal = pReceiver_pedal->getDescriptor(NimBLEUUID((uint16_t)0x2902));
          if(p2902_pedal != nullptr)
            p2902_pedal->writeValue((uint8_t*)notifyOn, 2, true);
#else
          if (!pReceiver_pedal->subscribe(true, notifyCB_pedal, true)) {
            connected_pedal = false;
            DEBUG("Pedal disconnected");
            NimBLEDevice::deleteClient(pClient_pedal);
          } 
#endif
        }
      }
      DEBUG("connect_pedal(): pedal connected");
      set_conn_status_connected(BLE_MIDI);
    }
  }
}
#endif

bool connect_to_all() {
  int i, j;
  int counts;
  uint8_t b;
  unsigned long t;

  DEBUG("DEBUG: Starting connect_to_all()");
  
  // Clean up any existing BLE connections
  DEBUG("DEBUG: Cleaning up existing BLE connections");
  if (pClient_sp != nullptr) {
    if (pClient_sp->isConnected()) {
      pClient_sp->disconnect();
    }
    NimBLEDevice::deleteClient(pClient_sp);
    pClient_sp = nullptr;
  }
  
#ifdef BLE_CONTROLLER
  if (pClient_pedal != nullptr) {
    if (pClient_pedal->isConnected()) {
      pClient_pedal->disconnect();
    }
    NimBLEDevice::deleteClient(pClient_pedal);
    pClient_pedal = nullptr;
  }
#endif

  if (pServer != nullptr) {
    // pServer->stop();  // This method doesn't exist in NimBLE
    pServer = nullptr;
  }

  if (pAdvertising != nullptr) {
    pAdvertising->stop();
    pAdvertising = nullptr;
  }

  // Deinitialize BLE stack
  DEBUG("DEBUG: Deinitializing BLE stack");
  NimBLEDevice::deinit();
  DEBUG("DEBUG: Waiting for BLE stack to deinitialize...");
  delay(1000); // Give more time for cleanup
  
  // set up connection status tracking array
  t = millis();
  for (i = 0; i < NUM_CONNS; i++) {
    conn_status[i] = false;
    for (j = 0; j < 3; j++)
      conn_last_changed[j][i] = t;
  }

  is_ble = true;

  DEBUG("DEBUG: About to initialize BLE device");
  NimBLEDevice::init("Spark 40 MIDI BLE");
  DEBUG("DEBUG: Waiting for BLE stack to initialize...");
  delay(1000); // Give more time for initialization to complete
  DEBUG("DEBUG: BLE device initialized");
  
  DEBUG("DEBUG: Creating Spark client");
  pClient_sp = NimBLEDevice::createClient();
  if (pClient_sp == nullptr) {
    DEBUG("DEBUG: Failed to create Spark client!");
    return false;
  }
  
  DEBUG("DEBUG: Setting Spark client callbacks");
  pClient_sp->setClientCallbacks(new MyClientCallback());
  
#ifdef BLE_CONTROLLER  
  DEBUG("DEBUG: Creating pedal client");
  pClient_pedal = NimBLEDevice::createClient();
  if (pClient_pedal == nullptr) {
    DEBUG("DEBUG: Failed to create pedal client!");
    return false;
  }
#endif
  
  DEBUG("DEBUG: Creating BLE server");
  pServer = NimBLEDevice::createServer();
  if (pServer == nullptr) {
    DEBUG("DEBUG: Failed to create BLE server!");
    return false;
  }
  
  DEBUG("DEBUG: Setting server callbacks");
  pServer->setCallbacks(new MyServerCallback());  
  DEBUG("DEBUG: Creating service");
  pService = pServer->createService(S_SERVICE);
  if (pService == nullptr) {
    DEBUG("DEBUG: Failed to create service!");
    return false;
  }

  DEBUG("DEBUG: Creating characteristics");
#ifdef CLASSIC  
  pCharacteristic_receive = pService->createCharacteristic(S_CHAR1, BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE | BLECharacteristic::PROPERTY_WRITE_NR);
  pCharacteristic_send = pService->createCharacteristic(S_CHAR2, BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_NOTIFY);
#else
  pCharacteristic_receive = pService->createCharacteristic(S_CHAR1, NIMBLE_PROPERTY::READ | NIMBLE_PROPERTY::WRITE | NIMBLE_PROPERTY::WRITE_NR);
  pCharacteristic_send = pService->createCharacteristic(S_CHAR2, NIMBLE_PROPERTY::READ | NIMBLE_PROPERTY::NOTIFY); 
#endif

  if (pCharacteristic_receive == nullptr || pCharacteristic_send == nullptr) {
    DEBUG("DEBUG: Failed to create characteristics!");
    return false;
  }

  DEBUG("DEBUG: Setting characteristic callbacks");
  pCharacteristic_receive->setCallbacks(&chrCallbacks_r);
  pCharacteristic_send->setCallbacks(&chrCallbacks_s);

#ifdef BLE_APP_MIDI
  DEBUG("DEBUG: Setting up MIDI service");
  pServiceMIDI = pServer->createService(MIDI_SERVICE);
  if (pServiceMIDI == nullptr) {
    DEBUG("DEBUG: Failed to create MIDI service!");
    return false;
  }

#ifdef CLASSIC  
  pCharacteristicMIDI = pServiceMIDI->createCharacteristic(MIDI_CHAR, BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE | BLECharacteristic::PROPERTY_WRITE_NR | BLECharacteristic::PROPERTY_NOTIFY);
#else
  pCharacteristicMIDI = pServiceMIDI->createCharacteristic(MIDI_CHAR, NIMBLE_PROPERTY::READ | NIMBLE_PROPERTY::WRITE | NIMBLE_PROPERTY::WRITE_NR | NIMBLE_PROPERTY::NOTIFY);
#endif

  if (pCharacteristicMIDI == nullptr) {
    DEBUG("DEBUG: Failed to create MIDI characteristic!");
    return false;
  }

  pCharacteristicMIDI->setCallbacks(&chrCallbacksMIDI);
#endif

  DEBUG("DEBUG: Starting services");
  pService->start();
#ifdef BLE_APP_MIDI
  pServiceMIDI->start();
#endif

#ifndef CLASSIC
  DEBUG("DEBUG: Starting server");
  pServer->start(); 
#endif

  DEBUG("DEBUG: Setting up advertising");
  pAdvertising = NimBLEDevice::getAdvertising();
  if (pAdvertising == nullptr) {
    DEBUG("DEBUG: Failed to create advertising object!");
    return false;
  }
  
  pAdvertising->addServiceUUID(pService->getUUID());
#ifdef BLE_APP_MIDI
  pAdvertising->addServiceUUID(pServiceMIDI->getUUID());
#endif

  // Create scan response data
  NimBLEAdvertisementData scanResponse;
  scanResponse.setName("Spark 40 MIDI BLE");
  pAdvertising->setScanResponseData(scanResponse);

  // Connect to Spark
  connected_sp = false;
  found_sp = false;
 
#ifdef BLE_CONTROLLER
  connected_pedal = false;
  found_pedal = false;
#endif

  DEBUG("DEBUG: Starting BLE scan loop");

  int scan_count = 0;
  while (!found_sp && scan_count < MAX_SCAN_COUNT) {
    scan_count++;
    String scanMsg = "Scan attempt " + String(scan_count) + " of " + String(MAX_SCAN_COUNT);
    DEBUG(scanMsg.c_str());
    
    DEBUG("DEBUG: Creating new scan object");
    // Create a new scan object for each attempt
    NimBLEScan* pScan = NimBLEDevice::getScan();
    if (pScan == nullptr) {
      DEBUG("DEBUG: Failed to create scan object!");
      return false;
    }

    
    DEBUG("DEBUG: Getting scan results (will wait up to 10 seconds)");
    NimBLEScanResults results = pScan->getResults(10000, true);  // Changed to true to wait for scan to complete
    DEBUG("DEBUG: Scan completed");
    
    String deviceMsg = "Found " + String(results.getCount()) + " devices";
    DEBUG(deviceMsg.c_str());
    
    DEBUG("DEBUG: Processing scan results");
    for(i = 0; i < results.getCount(); i++) {
      const NimBLEAdvertisedDevice* device = results.getDevice(i);
      if (device == nullptr) {
        DEBUG("DEBUG: Null device in results!");
        continue;
      }
      
      String deviceInfo = "Device " + String(i) + ": " + String(device->getName().c_str()) + " (Address: " + String(device->getAddress().toString().c_str()) + ")";
      DEBUG(deviceInfo.c_str());
      
      if (device->isAdvertisingService(SpServiceUuid)) {
        String sparkMsg = "Found Spark device: " + String(device->getName().c_str());
        DEBUG(sparkMsg.c_str());
        found_sp = true;
        connected_sp = false;
        sp_device = const_cast<NimBLEAdvertisedDevice*>(device);
        break;  // Found a Spark device, no need to continue scanning
      }
#ifdef BLE_CONTROLLER
      else if (device->isAdvertisingService(PedalServiceUuid) || strcmp(device->getName().c_str(),"iRig BlueBoard") == 0) {
        String pedalMsg = "Found pedal device: " + String(device->getName().c_str());
        DEBUG(pedalMsg.c_str());
        found_pedal = true;
        connected_pedal = false;
        pedal_device = const_cast<NimBLEAdvertisedDevice*>(device);
      }
#endif
    }

    if (!found_sp) {
      DEBUG("DEBUG: No Spark devices found in this scan");
    }
    
    DEBUG("DEBUG: Cleaning up scan results");
    pScan->clearResults();
    delay(1000); // Give more time between scans
  }

  if (!found_sp) {
    String failMsg = "Failed to find Spark after " + String(MAX_SCAN_COUNT) + " attempts";
    DEBUG(failMsg.c_str());
    return false;
  }
  
  DEBUG("DEBUG: Attempting to connect to Spark");
  connect_spark();
#ifdef BLE_CONTROLLER
  DEBUG("DEBUG: Attempting to connect to pedal");
  connect_pedal();
#endif    

#ifdef CLASSIC
  DEBUG("DEBUG: Starting classic bluetooth");
  bt = new BluetoothSerial();
  bt->register_callback(bt_callback);
  if (!bt->begin(SPARK_BT_NAME)) {
    DEBUG("DEBUG: Classic bluetooth init failed");
    while (true);
  }

  while (bt->available())
    b = bt->read(); 
  DEBUG("DEBUG: Spark 40 Audio set up");
#endif

  DEBUG("DEBUG: BLE setup complete, starting advertising");
  pAdvertising->start(); 
  return true;
}

bool app_available() {
  if (!ble_app_in.is_empty()) {
    is_ble = true;
    return true;
  }
#ifdef CLASSIC    
  if (bt->available()) {
    is_ble = false;
    return true;
  }
#endif
  return false;
}

uint8_t app_read() {
  set_conn_received(APP);
  if (is_ble) {
     uint8_t b;
     ble_app_in.get(&b);
     return b;
  }
#ifdef CLASSIC  
  else
    return bt->read();
#endif
  return 0;
}

void app_write(byte *buf, int len) {
  set_conn_sent(APP);
  if (is_ble) {
    pCharacteristic_send->setValue(buf, len);
    pCharacteristic_send->notify(true);
  }
#ifdef CLASSIC 
  else {
    bt->write(buf, len);
  }
#endif
}

void app_write_timed(byte *buf, int len) {
  set_conn_sent(APP);
  if (is_ble) {
    pCharacteristic_send->setValue(buf, len);
    pCharacteristic_send->notify(true);
  }
#ifdef CLASSIC 
  else {
    bt->write(buf, len);
    delay(50);
  }
#endif
}

bool sp_available() {
  return !ble_in.is_empty();
}

uint8_t sp_read() {
  uint8_t b;
  
  set_conn_received(SPK);
  ble_in.get(&b);
  return b;
}

void sp_write(byte *buf, int len) {
  set_conn_sent(SPK);  
  pSender_sp->writeValue(buf, len, false);
}

int ble_getRSSI() { 
#ifdef BLE_CONTROLLER  
  return 0;
#else
  return pClient_sp->getRssi();
#endif
}

void set_conn_received(int connection) {
  conn_last_changed[FROM][connection] = millis();
}

void set_conn_sent(int connection) {
  conn_last_changed[TO][connection] = millis();
}

void set_conn_status_connected(int connection) {
  if (conn_status[connection] == false) {
    conn_status[connection] = true;
    conn_last_changed[STATUS][connection] = millis();
  }
}

void set_conn_status_disconnected(int connection) {
  if (conn_status[connection] == true) {
    conn_status[connection] = false;
    conn_last_changed[STATUS][connection] = millis();
  }
}
