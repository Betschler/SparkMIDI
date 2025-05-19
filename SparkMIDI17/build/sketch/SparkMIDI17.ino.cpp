#include <Arduino.h>
#line 1 "B:\\GIT\\SparkMIDI\\SparkMIDI17\\SparkMIDI17.ino"
//       .----------------.  .----------------.  .----------------.  .----------------.  .----------------.      .----------------.  .----------------.  .----------------.  .----------------. 
//     | .--------------. || .--------------. || .--------------. || .--------------. || .--------------. |    | .--------------. || .--------------. || .--------------. || .--------------. |
//     | |    _______   | || |   ______     | || |      __      | || |  _______     | || |  ___  ____   | |    | | ____    ____ | || |     _____    | || |  ________    | || |     _____    | |
//     | |   /  ___  |  | || |  |_   __ \   | || |     /  \     | || | |_   __ \    | || | |_  ||_  _|  | |    | ||_   \  /   _|| || |    |_   _|   | || | |_   ___ `.  | || |    |_   _|   | |
//     | |  |  (__ \_|  | || |    | |__) |  | || |    / /\ \    | || |   | |__) |   | || |   | |_/ /    | |    | |  |   \/   |  | || |      | |     | || |   | |   `. \ | || |      | |     | |
//     | |   '.___`-.   | || |    |  ___/   | || |   / ____ \   | || |   |  __ /    | || |   |  __'.    | |    | |  | |\  /| |  | || |      | |     | || |   | |    | | | || |      | |     | |
//     | |  |`\____) |  | || |   _| |_      | || | _/ /    \ \_ | || |  _| |  \ \_  | || |  _| |  \ \_  | |    | | _| |_\/_| |_ | || |     _| |_    | || |  _| |___.' / | || |     _| |_    | |
//     | |  |_______.'  | || |  |_____|     | || ||____|  |____|| || | |____| |___| | || | |____||____| | |    | ||_____||_____|| || |    |_____|   | || | |________.'  | || |    |_____|   | |
//     | |              | || |              | || |              | || |              | || |              | |    | |              | || |              | || |              | || |              | |
//     | '--------------' || '--------------' || '--------------' || '--------------' || '--------------' |    | '--------------' || '--------------' || '--------------' || '--------------' |
//      '----------------'  '----------------'  '----------------'  '----------------'  '----------------'      '----------------'  '----------------'  '----------------'  '----------------' 

//
// Spark MIDI
//
// Open source MIDI control for the Spark 40 amp from Positive Grid
// See    https://github.com/paulhamsh/SparkMIDI for more details
// Also   https://github.com/paulhamsh/Spark     for documentation on the API and the message format to the Spark amp   
//

//Dependencies:
// NimBLE                          >=1.2.0 (last tested with 1.3.6)
// SSD1306 by Adafruit             >=2.3.0  
// ESP32 Dev Module board          =2.0.4
// Heltec ESP32 Series Dev-boards  =0.0.5

// For ESP32 Dev Kit update the preferences for board managers to point to: https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
// And load the latest board library - 2.0.2 when built on 23 Jan 2022
// This has the latest BLE code which has setMTU 
// The Heltec WIFI does not have a library supporting setMTU so that is removed by the preprocessor conditional (in SparkComms)


// Select the BLE type 
// -------------------
// if CLASSIC is defined, will compile with BLE stack which allows serial bluetooth
// - if not defined will use NimBLE which has no serial bluetooth
// it matters because Android Spark apps use serial bluetooth
// but BLE doesn't handle Spark disconnection well, whereas NimBLE does

//#define CLASSIC

// Select the BLE devices you need
// -------------------------------
// if BLE_APP_MIDI is defined it will allow a BLE MIDI device to connect (much like the app)
// if BLE_CONTROLLER is defined it will search for a bluetooth controller (like Akai LPD8 Wireless or IK Multimedia iRig Blueboard)

#define BLE_APP_MIDI
#define BLE_CONTROLLER

// Select whether to use external OLED
// -----------------------------------
#define OLED_ON

// Select the USB host device type
// -------------------------------
// USB_HOST is the generic MAX 2341 host
// USB_TRINKET is a Trinket M0 device programmed as a USB Host

#define USB_HOST
//#define USB_TRINKET

// Select the ESP32 board type
// ---------------------------
// M5STICK, M5CORE2, ESP_DEVKIT, HELTEC_WIFI
// Don't forget to set your board type in Tools -> Board as well!!
//
// Allowed combinations are:
// M5STICK && USB_TRINKET
// M5CORE2 && USB_TRINKET
// M5CORE && USB_HOST  (the USB Host base from M5)
// ESP_DEVKIT && (USB_TRINKET || USB_HOST)  can be either - the only one with a circuit for both so far
// HELTEC_WIFI && USB_HOST
//
// OLED_ON is acceptable with all boards except HELTEC_WIFI

//#define ESP_DEVKIT
//#define HELTEC_WIFI
#define M5CORE
//#define M5CORE2
//#define M5STICK

#define SDMOD

// Some checks in the pre-processing to ensure no invalid combinations (probably not exhaustive)
// The combinations are only excluded because there has been no sample circuit made with them yet, 
// so GPIO pin assignments are not determined

#if defined HELTEC_WIFI && defined OLED_ON
#error ** Cannot use a Heltec WIFI and have OLED_ON set **
#endif

#if defined USB_TRINKET && defined HELTEC_WIFI
#error ** Cannot use a Heltec WIFI with a Tinket M0 USB Host **
#endif

#if defined USB_HOST && (defined M5STICK || defined M5CORE2)
#error ** Cannot use a M5 board with USB Host **
#endif 

// Board specific #includes
#if defined HELTEC_WIFI
  #include "heltec.h"
#elif defined M5STICK
  #include <M5StickC.h>
#elif defined M5CORE2
  #include <M5Core2.h>
#elif defined M5CORE
  #include <M5Stack.h>
#endif

#include "Spark.h"
#include "Screen.h"
#include "MIDI.h"
#include "SparkPresets.h"
#if defined SDMOD 
  #include "SparkPresetFromSD.h"
#endif

#line 119 "B:\\GIT\\SparkMIDI\\SparkMIDI17\\SparkMIDI17.ino"
void setup();
#line 151 "B:\\GIT\\SparkMIDI\\SparkMIDI17\\SparkMIDI17.ino"
void loop();
#line 51 "B:\\GIT\\SparkMIDI\\SparkMIDI17\\MIDI.ino"
void setup_midi();
#line 83 "B:\\GIT\\SparkMIDI\\SparkMIDI17\\MIDI.ino"
bool update_midi(byte *mid);
#line 25 "B:\\GIT\\SparkMIDI\\SparkMIDI17\\Screen.ino"
void splash_screen();
#line 95 "B:\\GIT\\SparkMIDI\\SparkMIDI17\\Screen.ino"
void setup_screen();
#line 200 "B:\\GIT\\SparkMIDI\\SparkMIDI17\\Screen.ino"
void show_status();
#line 345 "B:\\GIT\\SparkMIDI\\SparkMIDI17\\Screen.ino"
void display_preset_icon(const char* icon_path, int x, int y);
#line 9 "B:\\GIT\\SparkMIDI\\SparkMIDI17\\Spark.ino"
int get_effect_index(char *str);
#line 22 "B:\\GIT\\SparkMIDI\\SparkMIDI17\\Spark.ino"
bool spark_state_tracker_start();
#line 39 "B:\\GIT\\SparkMIDI\\SparkMIDI17\\Spark.ino"
bool update_spark_state();
#line 196 "B:\\GIT\\SparkMIDI\\SparkMIDI17\\Spark.ino"
void update_ui();
#line 207 "B:\\GIT\\SparkMIDI\\SparkMIDI17\\Spark.ino"
void change_generic_model(char *new_eff, int slot);
#line 218 "B:\\GIT\\SparkMIDI\\SparkMIDI17\\Spark.ino"
void change_comp_model(char *new_eff);
#line 222 "B:\\GIT\\SparkMIDI\\SparkMIDI17\\Spark.ino"
void change_drive_model(char *new_eff);
#line 226 "B:\\GIT\\SparkMIDI\\SparkMIDI17\\Spark.ino"
void change_amp_model(char *new_eff);
#line 238 "B:\\GIT\\SparkMIDI\\SparkMIDI17\\Spark.ino"
void change_mod_model(char *new_eff);
#line 242 "B:\\GIT\\SparkMIDI\\SparkMIDI17\\Spark.ino"
void change_delay_model(char *new_eff);
#line 248 "B:\\GIT\\SparkMIDI\\SparkMIDI17\\Spark.ino"
void change_generic_onoff(int slot,bool onoff);
#line 257 "B:\\GIT\\SparkMIDI\\SparkMIDI17\\Spark.ino"
void change_noisegate_onoff(bool onoff);
#line 261 "B:\\GIT\\SparkMIDI\\SparkMIDI17\\Spark.ino"
void change_comp_onoff(bool onoff);
#line 265 "B:\\GIT\\SparkMIDI\\SparkMIDI17\\Spark.ino"
void change_drive_onoff(bool onoff);
#line 269 "B:\\GIT\\SparkMIDI\\SparkMIDI17\\Spark.ino"
void change_amp_onoff(bool onoff);
#line 273 "B:\\GIT\\SparkMIDI\\SparkMIDI17\\Spark.ino"
void change_mod_onoff(bool onoff);
#line 277 "B:\\GIT\\SparkMIDI\\SparkMIDI17\\Spark.ino"
void change_delay_onoff(bool onoff);
#line 281 "B:\\GIT\\SparkMIDI\\SparkMIDI17\\Spark.ino"
void change_reverb_onoff(bool onoff);
#line 286 "B:\\GIT\\SparkMIDI\\SparkMIDI17\\Spark.ino"
void change_generic_toggle(int slot);
#line 299 "B:\\GIT\\SparkMIDI\\SparkMIDI17\\Spark.ino"
void change_noisegate_toggle();
#line 303 "B:\\GIT\\SparkMIDI\\SparkMIDI17\\Spark.ino"
void change_comp_toggle();
#line 307 "B:\\GIT\\SparkMIDI\\SparkMIDI17\\Spark.ino"
void change_drive_toggle();
#line 311 "B:\\GIT\\SparkMIDI\\SparkMIDI17\\Spark.ino"
void change_amp_toggle();
#line 315 "B:\\GIT\\SparkMIDI\\SparkMIDI17\\Spark.ino"
void change_mod_toggle();
#line 319 "B:\\GIT\\SparkMIDI\\SparkMIDI17\\Spark.ino"
void change_delay_toggle();
#line 323 "B:\\GIT\\SparkMIDI\\SparkMIDI17\\Spark.ino"
void change_reverb_toggle();
#line 328 "B:\\GIT\\SparkMIDI\\SparkMIDI17\\Spark.ino"
void change_generic_param(int slot, int param, float val);
#line 344 "B:\\GIT\\SparkMIDI\\SparkMIDI17\\Spark.ino"
void change_noisegate_param(int param, float val);
#line 348 "B:\\GIT\\SparkMIDI\\SparkMIDI17\\Spark.ino"
void change_comp_param(int param, float val);
#line 352 "B:\\GIT\\SparkMIDI\\SparkMIDI17\\Spark.ino"
void change_drive_param(int param, float val);
#line 356 "B:\\GIT\\SparkMIDI\\SparkMIDI17\\Spark.ino"
void change_amp_param(int param, float val);
#line 360 "B:\\GIT\\SparkMIDI\\SparkMIDI17\\Spark.ino"
void change_mod_param(int param, float val);
#line 364 "B:\\GIT\\SparkMIDI\\SparkMIDI17\\Spark.ino"
void change_delay_param(int param, float val);
#line 368 "B:\\GIT\\SparkMIDI\\SparkMIDI17\\Spark.ino"
void change_reverb_param(int param, float val);
#line 373 "B:\\GIT\\SparkMIDI\\SparkMIDI17\\Spark.ino"
void change_hardware_preset(int pres_num);
#line 385 "B:\\GIT\\SparkMIDI\\SparkMIDI17\\Spark.ino"
void change_custom_preset(SparkPreset *preset, int preset_num);
#line 400 "B:\\GIT\\SparkMIDI\\SparkMIDI17\\Spark.ino"
void tuner_on_off(bool on_off);
#line 406 "B:\\GIT\\SparkMIDI\\SparkMIDI17\\Spark.ino"
void tuner_on_toogle();
#line 75 "B:\\GIT\\SparkMIDI\\SparkMIDI17\\SparkComms.ino"
void notifyCB_sp(NimBLERemoteCharacteristic* pRemoteCharacteristic, uint8_t* pData, size_t length, bool isNotify);
#line 87 "B:\\GIT\\SparkMIDI\\SparkMIDI17\\SparkComms.ino"
void notifyCB_pedal(NimBLERemoteCharacteristic* pRemoteCharacteristic, uint8_t* pData, size_t length, bool isNotify);
#line 130 "B:\\GIT\\SparkMIDI\\SparkMIDI17\\SparkComms.ino"
void connect_spark();
#line 165 "B:\\GIT\\SparkMIDI\\SparkMIDI17\\SparkComms.ino"
void connect_pedal();
#line 198 "B:\\GIT\\SparkMIDI\\SparkMIDI17\\SparkComms.ino"
bool connect_to_all();
#line 465 "B:\\GIT\\SparkMIDI\\SparkMIDI17\\SparkComms.ino"
bool app_available();
#line 479 "B:\\GIT\\SparkMIDI\\SparkMIDI17\\SparkComms.ino"
uint8_t app_read();
#line 493 "B:\\GIT\\SparkMIDI\\SparkMIDI17\\SparkComms.ino"
void app_write(byte *buf, int len);
#line 506 "B:\\GIT\\SparkMIDI\\SparkMIDI17\\SparkComms.ino"
void app_write_timed(byte *buf, int len);
#line 520 "B:\\GIT\\SparkMIDI\\SparkMIDI17\\SparkComms.ino"
bool sp_available();
#line 524 "B:\\GIT\\SparkMIDI\\SparkMIDI17\\SparkComms.ino"
uint8_t sp_read();
#line 532 "B:\\GIT\\SparkMIDI\\SparkMIDI17\\SparkComms.ino"
void sp_write(byte *buf, int len);
#line 537 "B:\\GIT\\SparkMIDI\\SparkMIDI17\\SparkComms.ino"
int ble_getRSSI();
#line 545 "B:\\GIT\\SparkMIDI\\SparkMIDI17\\SparkComms.ino"
void set_conn_received(int connection);
#line 549 "B:\\GIT\\SparkMIDI\\SparkMIDI17\\SparkComms.ino"
void set_conn_sent(int connection);
#line 553 "B:\\GIT\\SparkMIDI\\SparkMIDI17\\SparkComms.ino"
void set_conn_status_connected(int connection);
#line 560 "B:\\GIT\\SparkMIDI\\SparkMIDI17\\SparkComms.ino"
void set_conn_status_disconnected(int connection);
#line 67 "B:\\GIT\\SparkMIDI\\SparkMIDI17\\SparkIO.ino"
void dump_buf(char *hdr, uint8_t *buf, int len);
#line 94 "B:\\GIT\\SparkMIDI\\SparkMIDI17\\SparkIO.ino"
void uint_to_bytes(unsigned int i, uint8_t *h, uint8_t *l);
#line 99 "B:\\GIT\\SparkMIDI\\SparkMIDI17\\SparkIO.ino"
void bytes_to_uint(uint8_t h, uint8_t l,unsigned int *i);
#line 109 "B:\\GIT\\SparkMIDI\\SparkMIDI17\\SparkIO.ino"
void spark_start(bool passthru);
#line 133 "B:\\GIT\\SparkMIDI\\SparkMIDI17\\SparkIO.ino"
void spark_process();
#line 151 "B:\\GIT\\SparkMIDI\\SparkMIDI17\\SparkIO.ino"
void app_process();
#line 4 "B:\\GIT\\SparkMIDI\\SparkMIDI17\\SparkPresetFromSD.ino"
bool loadSparkPresetFromSD(SparkPreset* preset, const char* filename);
#line 57 "B:\\GIT\\SparkMIDI\\SparkMIDI17\\SparkPresetFromSD.ino"
bool loadSparkPresetCustom(int genre, int index);
#line 116 "B:\\GIT\\SparkMIDI\\SparkMIDI17\\SparkPresetFromSD.ino"
void printDirectory(File dir, int numTabs);
#line 140 "B:\\GIT\\SparkMIDI\\SparkMIDI17\\SparkPresetFromSD.ino"
void debugOutput(SparkPreset* preset);
#line 119 "B:\\GIT\\SparkMIDI\\SparkMIDI17\\SparkMIDI17.ino"
void setup() {

#if defined HELTEC_WIFI
  Heltec.begin(true /*DisplayEnable Enable*/, false /*LoRa Enable*/, true /*Serial Enable*/);
#elif defined M5STICK || defined M5CORE2 || defined M5CORE
  M5.begin(true /*DisplayEnable Enable*/, true /*SD Enable*/, true /*Serial Enable*/, false /*I2C Enable*/);
#endif
#if defined M5CORE
  M5.Power.begin();
#endif

  Serial.begin(115200);
  while (!spark_state_tracker_start()) {  // set up data to track Spark and app state, if it fails to find the Spark it will return false
    DEBUG("No Spark found - perhaps sleep?");// think about a deep sleep here if needed
  }
  DEBUG("Spark found and connected - starting");

  splash_screen();
  setup_midi();
  setup_screen();

#if defined SDMOD 
  if(!SD.begin()) { 
    DEBUG("Card failed, or not present");  
  }else{
    DEBUG("TF card initialized.");
  }
#endif

}


void loop() {

  byte mi[3];
  int midi_chan, midi_cmd;
  bool onoff;

#if defined M5STICK || defined M5CORE2 || defined M5CORE
  M5.update();
  if(M5.BtnA.wasPressed()) {
    DEBUG("Button A pressed!");  
    M5.powerOFF();
  }

  if(M5.BtnB.wasPressed()) {
      DEBUG("Button B pressed!");  
  }

  if(M5.BtnC.wasPressed()) {
      DEBUG("Button C pressed!");  
  }  
#endif

  show_status();

  if (update_midi(mi)) {
    midi_chan = (mi[0] & 0x0f) + 1;
    midi_cmd = mi[0] & 0xf0;
    onoff = (mi[2] == 127 ? true : false);
   
    if (midi_cmd == 0xc0) { 
      switch (mi[1]) {
        case 0:              change_hardware_preset(0);                 break; // MIDI Commander BIFX A
        case 1:              change_hardware_preset(1);                 break; // MIDI Commander BIFX B
        case 2:              change_hardware_preset(2);                 break; // MIDI Commander BIFX C
        case 3:              change_hardware_preset(3);                 break; // MIDI Commander BIFX D 
        case 4:              change_noisegate_toggle();                 break; // MIDI Commander BIFX E
        case 5:              change_drive_toggle();                     break; // MIDI Commander BIFX F
        case 6:              change_comp_toggle();                      break; // MIDI Commander BIFX G
        case 7:              change_mod_toggle();                       break; // MIDI Commander BIFX H 
        case 8:              change_delay_toggle();                     break; // MIDI Commander BIFX I
        case 9:              change_reverb_toggle();                    break; // MIDI Commander BIFX J 
#if defined SDMOD 
        default:             loadSparkPresetCustom((int) mi[1]/10, mi[1]-10*((int) mi[1]/10));   break; // Use SD load function
#elif 
        default:             change_custom_preset(my_presets[max(mi[1]-10,(sizeof(my_presets) / sizeof(my_presets[0])-1)], 3);  break; // try to load preset number mi[1]-10
#endif  
      }
    }

    if (midi_cmd == 0xb0 && midi_chan == 4) {     
      switch (mi[1]) {
        case 1:              change_amp_param(AMP_GAIN, mi[2]/127.0);   break; // MIDI Commander CUS2 EXP l          
        case 2:              change_amp_param(AMP_MASTER, mi[2]/127.0); break; // MIDI Commander CUS2 EXP 2  
        case 3:              change_drive_toggle();                     break; // MIDI Commander CUS2 1  
        case 5:              change_comp_toggle();                      break; // MIDI Commander CUS2 1  
        case 6:              change_noisegate_toggle();                 break; // MIDI Commander CUS2 1  
        case 9:              change_mod_toggle();                       break; // MIDI Commander CUS2 2       
        case 14:             change_delay_toggle();                     break; // MIDI Commander CUS2 3
        case 29:             change_reverb_toggle();                    break; // MIDI Commander CUS2 4
        case 22:             change_hardware_preset(0);                 break; // MIDI Commander CUS2 A
        case 23:             change_hardware_preset(1);                 break; // MIDI Commander CUS2 B
        case 24:             change_hardware_preset(2);                 break; // MIDI Commander CUS2 C
        case 105:            change_hardware_preset(3);                 break; // MIDI Commander CUS2 D
        }
    }
    
    if (midi_cmd == 0xb0) {     
      switch (mi[1]) {
        case 4:              change_amp_param(AMP_GAIN, mi[2]/127.0);   break; // MIDI Commander BIFX EXP 1
        case 7:              change_amp_param(AMP_MASTER, mi[2]/127.0); break; // MIDI Commander BIFX EXP 2  
        }
    }

    if (midi_cmd == 0xb0) 
      switch (midi_chan) {
        case 1:
          switch (mi[1]) {
            case 80:             change_comp_onoff(onoff);               break; // MIDI Commander BIFX 1
            case 81:             change_drive_onoff(onoff);              break; // MIDI Commander BIFX 2      
            case 82:             change_mod_onoff(onoff);                break; // MIDI Commander BIFX 3     
            case 83:             change_delay_onoff(onoff);              break; // MIDI Commander BIFX 4
          };
          break;
        case 2:
          switch (mi[1]) {
            case 80:             change_comp_onoff(onoff);               break; // MIDI Commander BIFX 1
            case 81:             change_drive_onoff(onoff);              break; // MIDI Commander BIFX 2      
            case 82:             change_mod_onoff(onoff);                break; // MIDI Commander BIFX 3     
            case 83:             change_reverb_onoff(onoff);             break; // MIDI Commander BIFX 4
          };
          break;
        case 3:
          switch (mi[1]) {
            case 80:             change_comp_onoff(onoff);               break; // MIDI Commander BIFX 1
            case 81:             change_drive_onoff(onoff);              break; // MIDI Commander BIFX 2      
            case 82:             change_delay_onoff(onoff);              break; // MIDI Commander BIFX 3     
            case 83:             change_reverb_onoff(onoff);             break; // MIDI Commander BIFX 4
          };
          break;
        case 4:
          switch (mi[1]) {
            case 80:             change_comp_onoff(onoff);               break; // MIDI Commander BIFX 1
            case 81:             change_mod_onoff(onoff);                break; // MIDI Commander BIFX 2      
            case 82:             change_delay_onoff(onoff);              break; // MIDI Commander BIFX 3     
            case 83:             change_reverb_onoff(onoff);             break; // MIDI Commander BIFX 4
          };
          break;
        case 5:
          switch (mi[1]) {
            case 80:             change_drive_onoff(onoff);              break; // MIDI Commander BIFX 1      
            case 81:             change_mod_onoff(onoff);                break; // MIDI Commander BIFX 2     
            case 82:             change_delay_onoff(onoff);              break; // MIDI Commander BIFX 3
            case 83:             change_reverb_onoff(onoff);             break; // MIDI Commander BIFX 4
          };
          break;
        default:
          switch (mi[1]) {
            case 80:             change_drive_onoff(onoff);              break; // MIDI Commander BIFX 1      
            case 81:             change_mod_onoff(onoff);                break; // MIDI Commander BIFX 2     
            case 82:             change_delay_onoff(onoff);              break; // MIDI Commander BIFX 3
            case 83:             change_reverb_onoff(onoff);             break; // MIDI Commander BIFX 4
          };
      }
  }

  if (update_spark_state()) {
  }
}



#line 1 "B:\\GIT\\SparkMIDI\\SparkMIDI17\\MIDI.ino"
#include "MIDI.h"

// MIDI State processing for BLE streams

void MIDIState::initialise(RingBuffer *rb)
{
  midi_stream = rb;
  midi_data_count = 0;
  midi_cmd_count = 0;
  midi_status = 0;
}

bool MIDIState::process(byte mi_data[3]) 
{
  byte b;
  bool got_data;

  got_data = false;
  
  if (!midi_stream->is_empty()) {
    midi_stream->get(&b);
    
    if (b <= 0x7F) {
      midi_data[midi_data_count] = b;
      midi_data_count++;
      midi_cmd_count = 0;
      if ((midi_status == 0xC0 || midi_status == 0xD0) || midi_data_count >= 2) {
        mi_data[0] = midi_status;
        mi_data[1] = midi_data[0];
        if (midi_data_count == 2)
          mi_data[2] = midi_data[1];
        else
          mi_data[2] = 0;
        midi_data_count = 0;
        got_data = true;
      }
    } 
    else {
      midi_cmd_count++;
      if (midi_cmd_count > 1) {
        midi_status = b;            // the last byte before a non-cmd byte will always be status unless it was a single timestamp
      }
    }
  } 
  return got_data;
}

MIDIState ble_app;
MIDIState ble_midi;

void setup_midi() {
  byte b;

#ifdef USB_HOST
  if (Usb.Init() == -1) {
    DEBUG("USB host init failed");
    usb_connected = false;
  }
  else {
    DEBUG("USB host running");
    usb_connected = true;   
  }
#endif
  
  ser1 = new HardwareSerial(1); 
  ser1->begin(31250, SERIAL_8N1, SER_RX, -1);
  while (ser1->available())
    b = ser1->read();

#ifdef USB_TRINKET
  ser2 = new HardwareSerial(2); 
  ser2->begin(115200, SERIAL_8N1, SER2_RX, SER2_TX);
  while (ser2->available())    
    b = ser2->read();
#endif

  ble_app.initialise(&ble_midi_in);
#ifdef BLE_CONTROLLER
  ble_midi.initialise(&midi_in);
#endif
}

bool update_midi(byte *mid) {
  bool got_midi;
  byte b;
  
  got_midi = false;

  // process MIDI - both USB and BLE and  Serial DIN

#ifdef BLE_APP_MIDI
  if (ble_app.process(mid)) got_midi = true;
#endif

  // Serial 1 - DIN MIDI
  if (ser1->available()) {
    mid[0] = ser1->read();
    mid[1] = ser1->read();
    
    if (mid[0] == 0xC0 || mid[0] == 0xD0)
      mid[2] = 0;
    else 
      mid[2] = ser1->read();
      
    if (mid[0] != 0xFE) {
      got_midi = true;
      set_conn_received(SER_MIDI);
    }
  }


#ifdef USB_TRINKET
  // Serial 2 - Trinket M0 USB
  if (ser2->available()) {
    mid[0] = ser2->read();
    mid[1] = ser2->read();
    
    if (mid[0] == 0xC0 || mid[0] == 0xD0)
      mid[2] = 0;
    else 
      mid[2] = ser2->read();
      
    if (mid[0] != 0xFE) {
      got_midi = true;
      set_conn_received(USB_MIDI);
    }
  }
#endif

#ifdef BLE_CONTROLLER
  // BLE MIDI controller
  if (ble_midi.process(mid)) got_midi = true;
#endif

#ifdef USB_HOST
  // USB MIDI  
  if (usb_connected) {
    Usb.Task();

    if (Midi) {                                                  // USB Midi
      rcvd = Midi.RecvData(midi_buf, false);
      if (rcvd > 0 && !(midi_buf[0] == 0 && midi_buf[1] == 0 && midi_buf[2] == 0)) {
        set_conn_received(USB_MIDI);
        mid[0] = midi_buf[0];
        mid[1] = midi_buf[1];
        mid[2] = midi_buf[2];
        got_midi = true;
      }
    }
  }
#endif

  if (got_midi) {
    Serial.print("MIDI ");
    Serial.print(mid[0], HEX);
    Serial.print(" ");
    Serial.print(mid[1]);
    Serial.print(" ");   
    Serial.println(mid[2]);
  }
  return got_midi;
}

#line 1 "B:\\GIT\\SparkMIDI\\SparkMIDI17\\RingBuffer.ino"
#include "RingBuffer.h" 

//
// RingBuffer class
//

/* Implementation of a ring buffer - with a difference
 * New data is written to a temp area with add_to_buffer() and committed into the buffer once done - commit()
 * Commited data is read from the head of the buffer with get_from_buffer()
 * Data in this area can be read and updated by index - get_at_index() and set_at_index()
 * 
 * If the temp data is not required it can be ignored using drop() rather than commit()
 * 
 *    +----------------------------------++----------------------------------------------------------------------+
 *    |  0 |  1 |  2 |  3 |  4 |  5 |  6 ||  7 |  8 |  9 | 10 | 11 | 12 | 13 || 14 | 15 | 16 | 17 | 18 | 19 | 20 |                                              |
 *    +----------------------------------++----------------------------------------------------------------------+  
 *       ^                             ^     ^                             ^
 *       st ---------- len ------------+     +----------- t_len --------- en
 * 
 *           committed data                      temporary data                                   empty
 */

RingBuffer::RingBuffer() {
  st = 0;
  en = 0;
  len = 0;
  t_len = 0;
}

bool RingBuffer::add(uint8_t b) {
  if (len + t_len < RB_BUFF_MAX) {
    rb[en] = b;
    t_len++;
    en++; 
    if (en >= RB_BUFF_MAX) en = 0; 
    return true;
  }
  else
    return false;
}

bool RingBuffer::get(uint8_t *b) {
  if (len > 0) {
    *b = rb[st];
    len--;
    st++; 
    if (st >= RB_BUFF_MAX) st = 0; 
    return true;
  }
  else
    return false;  
}

// set a value at a location in the temp area
bool RingBuffer::set_at_index(int ind, uint8_t b) {
  if (ind >= 0 && ind < t_len) {
    rb[(st+len+ind) % RB_BUFF_MAX] = b;
    return true;
  }
  else
    return false;
}

// get a value from a location in the temp area
bool RingBuffer::get_at_index(int ind, uint8_t *b) {
  if (ind >= 0 && ind < t_len) {
    *b = rb[(st+len+ind) % RB_BUFF_MAX];
    return true;
  }
  else
    return false;
}

bool RingBuffer::set_bit_at_index(int ind, uint8_t b) {
  if (ind >= 0 && ind < t_len) {
    rb[(st+len+ind) % RB_BUFF_MAX] |= b;    
    return true;
  }
  else
    return false; 
}

int RingBuffer::get_len() { // total temp len
  return t_len;
}

int RingBuffer::get_pos() { // current position
  return t_len;
}

void RingBuffer::commit() {
  len += t_len;
  t_len = 0;
}

void RingBuffer::drop() {
  en = st + len;
  t_len = 0;
}

void RingBuffer::clear() {
  en = st;
  len = 0;
}

bool RingBuffer::is_empty() {
  return (len == 0);
}

void RingBuffer::dump() {
  int i;

  for (i=0; i<len; i++) {
    Serial.print("S ");
    Serial.print(st+i);
    Serial.print(" ");
    Serial.print((st+i) % RB_BUFF_MAX);
    Serial.print(" ");    
    Serial.println(rb[(st+i) % RB_BUFF_MAX], HEX);
  };
  for (i=0; i<t_len; i++) {
    Serial.print("T ");
    Serial.print(st+len+i);
    Serial.print(" ");
    Serial.print((st+len+i) % RB_BUFF_MAX);
    Serial.print(" ");    
    Serial.println(rb[(st+len+i) % RB_BUFF_MAX], HEX);
  };
}

void RingBuffer::dump2() {
  int i;
  uint8_t v;

//  Serial.println();
  for (i=0; i<len; i++) {
    v=rb[(st+i) % RB_BUFF_MAX];
    if (v < 16) Serial.print("0");
    Serial.print(v, HEX);
    Serial.print(" ");
  };
  for (i=0; i<t_len; i++) {
    v=rb[(st+len+i) % RB_BUFF_MAX];
    if (v < 16) Serial.print("0");
    Serial.print(v, HEX);
    Serial.print(" ");
  };
  Serial.println();
}

void RingBuffer::dump3() {
  int i;
  uint8_t v;

  Serial.print("               ");
  for (i=0; i<len; i++) {
    v=rb[(st+i) % RB_BUFF_MAX];
    if (v < 16) Serial.print("0");
    Serial.print(v, HEX);
    Serial.print(" ");
    if (i % 16 == 15) {
      Serial.println();
      Serial.print("               ");
    };
  };
  Serial.println();
}

#line 1 "B:\\GIT\\SparkMIDI\\SparkMIDI17\\Screen.ino"
#include "Screen.h"

#ifdef CLASSIC
char ble_type[10] = "Classic";
#else
char ble_type[10] = "NimBLE";
#endif

#define SCR_TITLE      "Spark MIDI"
#define SCR_SUBTITLE1  "MIDI control for"
#define SCR_SUBTITLE2  "PG Spark 40"
#define SCR_VER        "V1.6"

#define SCR_COLS        "Con To Frm" 
#define SCR_COLS_L      " Conn  To  From"
#define SCR_SPARK       "Spark"
#define SCR_APP         "App"
#define SCR_BLE_MIDI    "BLE MIDI"
#define SCR_USB_MIDI    "USB MIDI"
#define SCR_SER_MIDI    "SER MIDI"


unsigned long now, last_time;

void splash_screen() {
#ifdef OLED_ON
  Wire.begin(OLED_SDA, OLED_SCL);  // set my SDA, SCL
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { 
    Serial.println("SSD1306 allocation failed");
    while (true); 
  }
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(2);
  display.setCursor(5, 0);
  display.print(SCR_TITLE);
  display.setCursor(10, 22);
  display.print(SCR_VER);
  display.setTextSize(1);
  display.setCursor(14, 42); 
  display.print(SCR_SUBTITLE1);
  display.setCursor(28, 52); 
  display.print(SCR_SUBTITLE2); 
  display.display();
#endif

#if defined M5STICK
  M5.Lcd.setRotation(-1);
  M5.Lcd.fillScreen(TFT_BLACK);
  M5.Lcd.setTextColor(TFT_WHITE);
  M5.Lcd.setTextSize(2);
  M5.Lcd.setCursor(5, 0);
  M5.Lcd.print(SCR_TITLE);
  M5.Lcd.setCursor(10, 22);
  M5.Lcd.print(SCR_VER);
  M5.Lcd.setTextSize(1);
  M5.Lcd.setCursor(14, 42); 
  M5.Lcd.print(SCR_SUBTITLE1);
  M5.Lcd.setCursor(24, 52); 
  M5.Lcd.print(SCR_SUBTITLE2); 
#endif

#if defined M5CORE2  || defined M5CORE
  M5.Lcd.fillScreen(TFT_BLACK);
  M5.Lcd.setTextColor(TFT_WHITE);
  M5.Lcd.setTextSize(3);
  M5.Lcd.setCursor(60, 0);
  M5.Lcd.print(SCR_TITLE);
  M5.Lcd.setCursor(90, 66);
  M5.Lcd.print(SCR_VER);
  M5.Lcd.setTextSize(2);
  M5.Lcd.setCursor(52, 126); 
  M5.Lcd.print(SCR_SUBTITLE1);
  M5.Lcd.setCursor(82, 156); 
  M5.Lcd.print(SCR_SUBTITLE2); 
#endif

#if defined HELTEC_WIFI
  Heltec.display->clear();
  Heltec.display->setColor(WHITE);
  Heltec.display->setFont(ArialMT_Plain_24);
  Heltec.display->drawString(5, 0, SCR_TITLE); 
  Heltec.display->drawString(10, 22, SCR_VER); 
  Heltec.display->setFont(ArialMT_Plain_10);
  Heltec.display->drawString(24, 42, SCR_SUBTITLE1); 
  Heltec.display->drawString(30, 52, SCR_SUBTITLE2); 
  Heltec.display->display();
#endif

  now = millis();
  last_time = millis();
}


void setup_screen() {
#ifdef OLED_ON
  display.clearDisplay();
  display.drawRect(0, 0, 45, 10, WHITE);  
  display.setCursor(2, 2);
  display.print(ble_type);
  for (int i = 0; i < NUM_CONNS ; i++) {
    display.drawCircle(CON_COL, SPK_ICON + LINES_GAP * i, CIRC_RAD, WHITE);
    display.drawCircle(TO_COL, SPK_ICON + LINES_GAP * i, CIRC_RAD, WHITE);
    display.drawCircle(FROM_COL, SPK_ICON + LINES_GAP * i, CIRC_RAD, WHITE);
  }
  display.setCursor(CON_COL-10, TITLES);
  display.print(SCR_COLS); 
  display.setCursor(0, SPK_TEXT);
  display.print(SCR_SPARK); 
  display.setCursor(0, APP_TEXT);
  display.print(SCR_APP); 
  display.setCursor(0, BLE_MIDI_TEXT); 
  display.print(SCR_BLE_MIDI); 
  display.setCursor(0, USB_MIDI_TEXT);
  display.print(SCR_USB_MIDI);
  display.setCursor(0, SER_MIDI_TEXT);
  display.print(SCR_SER_MIDI);
  display.display();
#endif

#if defined M5STICK
  M5.Lcd.fillScreen(TFT_BLACK);
  M5.Lcd.drawRect(0, 0, 45, 10, TFT_WHITE);   
  M5.Lcd.setCursor(2, 2);
  M5.Lcd.print(ble_type);
  for (int i = 0; i < NUM_CONNS ; i++) {
    M5.Lcd.drawCircle(CON_COL, SPK_ICON + LINES_GAP * i, CIRC_RAD, TFT_WHITE);
    M5.Lcd.drawCircle(TO_COL, SPK_ICON + LINES_GAP * i, CIRC_RAD, TFT_WHITE);
    M5.Lcd.drawCircle(FROM_COL, SPK_ICON + LINES_GAP * i, CIRC_RAD, TFT_WHITE); 
  }
  M5.Lcd.setCursor(CON_COL-10, TITLES);
  M5.Lcd.print(SCR_COLS); 
  M5.Lcd.setCursor(0, SPK_TEXT);
  M5.Lcd.print(SCR_SPARK); 
  M5.Lcd.setCursor(0, APP_TEXT);
  M5.Lcd.print(SCR_APP); 
  M5.Lcd.setCursor(0, BLE_MIDI_TEXT); 
  M5.Lcd.print(SCR_BLE_MIDI); 
  M5.Lcd.setCursor(0, USB_MIDI_TEXT);
  M5.Lcd.print(SCR_USB_MIDI);
  M5.Lcd.setCursor(0, SER_MIDI_TEXT);
  M5.Lcd.print(SCR_SER_MIDI);   
#endif

#if defined M5CORE2  || defined M5CORE
  M5.Lcd.fillScreen(TFT_BLACK);
  M5.Lcd.fillRect(0, 0, L_BOX_SPLIT, 30, TFT_WHITE);   
  M5.Lcd.drawRect(L_BOX_SPLIT+2, 0, 320-L_BOX_SPLIT-2, 30, TFT_WHITE); 
  M5.Lcd.setCursor(6,6);
  M5.Lcd.setTextColor(TFT_BLACK);
  M5.Lcd.print(SCR_TITLE);
  M5.Lcd.setTextColor(TFT_WHITE);
  M5.Lcd.setCursor(L_TO_COL, 6);
  M5.Lcd.print(ble_type);
  for (int i = 0; i < NUM_CONNS ; i++) {
    M5.Lcd.drawCircle(L_CON_COL, L_SPK_ICON + L_LINES_GAP * i, L_CIRC_RAD, TFT_WHITE);
    M5.Lcd.drawCircle(L_TO_COL, L_SPK_ICON + L_LINES_GAP * i, L_CIRC_RAD, TFT_WHITE);
    M5.Lcd.drawCircle(L_FROM_COL, L_SPK_ICON + L_LINES_GAP * i, L_CIRC_RAD, TFT_WHITE);  
  }   
  M5.Lcd.setCursor(L_CON_COL-40, L_TITLES);
  M5.Lcd.print(SCR_COLS_L); 
  M5.Lcd.setCursor(0, L_SPK_TEXT);
  M5.Lcd.print(SCR_SPARK); 
  M5.Lcd.setCursor(0, L_APP_TEXT);
  M5.Lcd.print(SCR_APP); 
  M5.Lcd.setCursor(0, L_BLE_MIDI_TEXT); 
  M5.Lcd.print(SCR_BLE_MIDI); 
  M5.Lcd.setCursor(0, L_USB_MIDI_TEXT);
  M5.Lcd.print(SCR_USB_MIDI);
  M5.Lcd.setCursor(0, L_SER_MIDI_TEXT);
  M5.Lcd.print(SCR_SER_MIDI);
#endif

#if defined HELTEC_WIFI
  Heltec.display->clear();
  Heltec.display->setColor(WHITE);
  Heltec.display->setFont(ArialMT_Plain_10);
  Heltec.display->drawRect(0, 0, 45, 12);  
  Heltec.display->drawString(2,0, ble_type);
  for (int i = 0; i < NUM_CONNS ; i++) {
    Heltec.display->drawCircle(CON_COL, SPK_ICON + LINES_GAP * i, CIRC_RAD);
    Heltec.display->drawCircle(TO_COL, SPK_ICON + LINES_GAP * i, CIRC_RAD);
    Heltec.display->drawCircle(FROM_COL, SPK_ICON + LINES_GAP * i, CIRC_RAD);
  }

  Heltec.display->drawString(CON_COL-10, TITLES, SCR_COLS); 
  Heltec.display->drawString(0, SPK_TEXT, SCR_SPARK); 
  Heltec.display->drawString(0, APP_TEXT, SCR_APP); 
  Heltec.display->drawString(0, BLE_MIDI_TEXT, SCR_BLE_MIDI); 
  Heltec.display->drawString(0, USB_MIDI_TEXT, SCR_USB_MIDI);
  Heltec.display->drawString(0, SER_MIDI_TEXT, SCR_SER_MIDI);
  Heltec.display->display();
#endif


}



void show_status() {
  int rad;
  static char last_icon[STR_LEN] = "";
  static int last_preset = -1;

  if (millis() - now > 50) { // only do this evey 50ms so we don't keep redrawing!
    now = millis();

    // Display preset icon if it has changed
    #if defined M5CORE2 || defined M5CORE
    if (current_preset != last_preset || strcmp(current_preset_icon, last_icon) != 0) {
      // Clear the icon area
      M5.Lcd.fillRect(0, L_LAST_COMMAND_TEXT + 40, 64, 64, TFT_BLACK);
      // Display the new icon
      display_preset_icon(current_preset_icon, 0, L_LAST_COMMAND_TEXT + 40);
      strncpy(last_icon, current_preset_icon, STR_LEN);
      last_preset = current_preset;
    }
    #elif defined M5STICK
    if (current_preset != last_preset || strcmp(current_preset_icon, last_icon) != 0) {
      // Clear the icon area
      M5.Lcd.fillRect(0, SER_MIDI_TEXT + 20, 32, 32, TFT_BLACK);
      // Display the new icon
      display_preset_icon(current_preset_icon, 0, SER_MIDI_TEXT + 20);
      strncpy(last_icon, current_preset_icon, STR_LEN);
      last_preset = current_preset;
    }
    #endif

    #ifdef OLED_ON
    for (int i = 0; i <= 1; i++) 
      for (int j = 0; j < NUM_CONNS; j++) 
        if (now - conn_last_changed[i][j] <= 900) {
          rad = FILL_RAD - int(FILL_RAD * (now - conn_last_changed[i][j]) / 800); 
          display.fillCircle(i == TO ? TO_COL : FROM_COL, SPK_ICON + j * LINES_GAP, CIRC_RAD, BLACK);
          display.drawCircle(i == TO ? TO_COL : FROM_COL, SPK_ICON + j * LINES_GAP, CIRC_RAD, WHITE);  
          if (rad > 0) {
            display.fillCircle(i == TO ? TO_COL : FROM_COL, SPK_ICON + j * LINES_GAP, rad, WHITE); 
          }
          display.display();
        }

    for (int j = 0; j <= 2; j++) 
      if (conn_last_changed[STATUS][j] >= last_time)
        if (conn_status[j]) {
          display.fillCircle(CON_COL, SPK_ICON + j * LINES_GAP, CIRC_RAD, WHITE);
          display.display();
        }
        else {
          display.fillCircle(CON_COL, SPK_ICON + j * LINES_GAP, CIRC_RAD, BLACK);
          display.drawCircle(CON_COL, SPK_ICON + j * LINES_GAP, CIRC_RAD, WHITE);
          display.display();
        }
    #endif

    #ifdef M5STICK
    for (int i = 0; i <= 1; i++) 
      for (int j = 0; j < NUM_CONNS; j++) 
        if (now - conn_last_changed[i][j] <= 900) {
          rad = FILL_RAD - int(FILL_RAD * (now - conn_last_changed[i][j]) / 800); 
          M5.Lcd.fillCircle(i == TO ? TO_COL : FROM_COL, SPK_ICON + j * LINES_GAP, CIRC_RAD, TFT_BLACK);
          M5.Lcd.drawCircle(i == TO ? TO_COL : FROM_COL, SPK_ICON + j * LINES_GAP, CIRC_RAD, TFT_WHITE);  
          if (rad > 0) {
            M5.Lcd.fillCircle(i == TO ? TO_COL : FROM_COL, SPK_ICON + j * LINES_GAP, rad, TFT_WHITE);
          }
        }

    for (int j = 0; j <= 2; j++) 
      if (conn_last_changed[STATUS][j] >= last_time)
        if (conn_status[j]) {
          M5.Lcd.fillCircle(CON_COL, SPK_ICON + j * LINES_GAP, CIRC_RAD, TFT_WHITE);      
        }
        else {
          M5.Lcd.fillCircle(CON_COL, SPK_ICON + j * LINES_GAP, CIRC_RAD, TFT_BLACK);
          M5.Lcd.drawCircle(CON_COL, SPK_ICON + j * LINES_GAP, CIRC_RAD, TFT_WHITE);
        }
    #endif

    #if defined M5CORE2 || defined M5CORE
    //new
    if (conn_last_changed[0][1] >= last_time){
      M5.Lcd.setCursor(0, L_LAST_COMMAND_TEXT);
      M5.Lcd.setTextColor(TFT_BLACK);
      M5.Lcd.fillRect(0, L_LAST_COMMAND_TEXT-2, 320, L_LAST_COMMAND_TEXT+32, TFT_WHITE);   
      M5.Lcd.print(last_command_sent);
    }


    for (int i = 0; i <= 1; i++) 
      for (int j = 0; j < NUM_CONNS; j++) 
        if (now - conn_last_changed[i][j] <= 900) {
          rad = L_FILL_RAD - int(L_FILL_RAD * (now - conn_last_changed[i][j]) / 800); 
          M5.Lcd.fillCircle(i == TO ? L_TO_COL : L_FROM_COL, L_SPK_ICON + j * L_LINES_GAP, L_CIRC_RAD, TFT_BLACK);
          M5.Lcd.drawCircle(i == TO ? L_TO_COL : L_FROM_COL, L_SPK_ICON + j * L_LINES_GAP, L_CIRC_RAD, TFT_WHITE);  
          if (rad > 0) {
            M5.Lcd.fillCircle(i == TO ? L_TO_COL : L_FROM_COL, L_SPK_ICON + j * L_LINES_GAP, rad, TFT_WHITE);
          }
        }

    for (int j = 0; j <= 2; j++) 
      if (conn_last_changed[STATUS][j] >= last_time)
        if (conn_status[j]) {
          M5.Lcd.fillCircle(L_CON_COL, L_SPK_ICON + j * L_LINES_GAP, L_CIRC_RAD, TFT_WHITE);      
        }
        else {
          M5.Lcd.fillCircle(L_CON_COL, L_SPK_ICON + j * L_LINES_GAP, L_CIRC_RAD, TFT_BLACK);
          M5.Lcd.drawCircle(L_CON_COL, L_SPK_ICON + j * L_LINES_GAP, L_CIRC_RAD, TFT_WHITE);
        }
    #endif

    #ifdef HELTEC_WIFI
    for (int i = 0; i <= 1; i++) 
      for (int j = 0; j < NUM_CONNS; j++) 
        if (now - conn_last_changed[i][j] <= 900) {
          rad = FILL_RAD - int(FILL_RAD * (now - conn_last_changed[i][j]) / 800); 
          Heltec.display->setColor(BLACK);
          Heltec.display->fillCircle(i == TO ? TO_COL : FROM_COL, SPK_ICON + j * LINES_GAP, CIRC_RAD);
          Heltec.display->setColor(WHITE);
          Heltec.display->drawCircle(i == TO ? TO_COL : FROM_COL, SPK_ICON + j * LINES_GAP, CIRC_RAD);  
          if (rad > 0) {
            Heltec.display->fillCircle(i == TO ? TO_COL : FROM_COL, SPK_ICON + j * LINES_GAP, rad);  
            }
          Heltec.display->display();
        }

    for (int j = 0; j <= 2; j++) 
      if (conn_last_changed[STATUS][j] >= last_time)
        if (conn_status[j]) {
          Heltec.display->setColor(WHITE);
          Heltec.display->fillCircle(CON_COL, SPK_ICON + j * LINES_GAP, CIRC_RAD);
          Heltec.display->display();
        }
        else {
          Heltec.display->setColor(BLACK);
          Heltec.display->fillCircle(CON_COL, SPK_ICON + j * LINES_GAP, CIRC_RAD);
          Heltec.display->setColor(WHITE);
          Heltec.display->drawCircle(CON_COL, SPK_ICON + j * LINES_GAP, CIRC_RAD);
          Heltec.display->display();
        }
    #endif
    
    last_time = now;
  }
}

void display_preset_icon(const char* icon_path, int x, int y) {
#if defined M5CORE2 || defined M5CORE
  if (SD.exists(icon_path)) {
    M5.Lcd.drawJpgFile(SD, icon_path, x, y);
  } else {
    // Draw a placeholder if icon not found
    M5.Lcd.fillRect(x, y, 64, 64, TFT_BLACK);
    M5.Lcd.drawRect(x, y, 64, 64, TFT_WHITE);
    M5.Lcd.setTextColor(TFT_WHITE);
    M5.Lcd.setTextSize(1);
    M5.Lcd.setCursor(x + 2, y + 2);
    M5.Lcd.print("No Icon");
  }
#elif defined M5STICK
  if (SD.exists(icon_path)) {
    M5.Lcd.drawJpgFile(SD, icon_path, x, y);
  } else {
    // Draw a placeholder if icon not found
    M5.Lcd.fillRect(x, y, 32, 32, TFT_BLACK);
    M5.Lcd.drawRect(x, y, 32, 32, TFT_WHITE);
    M5.Lcd.setTextColor(TFT_WHITE);
    M5.Lcd.setTextSize(1);
    M5.Lcd.setCursor(x + 2, y + 2);
    M5.Lcd.print("No Icon");
  }
#endif
}

#line 1 "B:\\GIT\\SparkMIDI\\SparkMIDI17\\Spark.ino"
///// ROUTINES TO SYNC TO AMP SETTINGS

int selected_preset;
bool ui_update_in_progress;

int current_preset = -1;
char current_preset_icon[STR_LEN] = "";

int get_effect_index(char *str) {
  int ind, i;

  ind = -1;
  for (i = 0; ind == -1 && i <= 6; i++) {
    if (strcmp(presets[5].effects[i].EffectName, str) == 0) {
      ind  = i;
    }
  }
  return ind;
}


bool  spark_state_tracker_start() {
  spark_state = SPARK_DISCONNECTED;
  // try to find and connect to Spark - returns false if failed to find Spark
  if (!connect_to_all()) return false;    
                
  spark_start(true);                  // set up the classes to communicate with Spark and app
  spark_state = SPARK_CONNECTED;     // it has to be to have reached here

  spark_ping_timer = millis();
  selected_preset = 0;
  ui_update_in_progress = false;
  return true;
}

// get changes from app or Spark and update internal state to reflect this
// this function has the side-effect of loading cmdsub, msg and preset which can be used later

bool  update_spark_state() {
  int pres, ind;

  // sort out connection and sync progress
  if (conn_status[SPK] == false && spark_state != SPARK_DISCONNECTED) {
    spark_state = SPARK_DISCONNECTED;
    spark_ping_timer = millis();
    DEBUG("SPARK DISCONNECTED, TRY TO CONNECT");
  }


  if (spark_state == SPARK_DISCONNECTED) {
    if (millis() - spark_ping_timer > 100) {
      spark_ping_timer = millis();
      connect_spark();  // reconnects if any disconnects happen    
    }
  }

  if (conn_status[SPK] == true && spark_state == SPARK_DISCONNECTED) {
    spark_state = SPARK_CONNECTED;
    DEBUG("SPARK CONNECTED");
  }

  if (spark_state == SPARK_CONNECTED) {
    if (millis() - spark_ping_timer > 500) {
      // every 0.5s ping the Spark amp to see if it will respond
      spark_ping_timer = millis();
      spark_msg_out.get_serial();
      DEBUG("PINGING SPARK");  
    }  
  }
  
  if (spark_state == SPARK_COMMUNICATING) {
    spark_msg_out.get_preset_details(0x0000);
    spark_msg_out.get_preset_details(0x0001);
    spark_msg_out.get_preset_details(0x0002);
    spark_msg_out.get_preset_details(0x0003);
    spark_msg_out.get_preset_details(0x0100);
    spark_msg_out.get_hardware_preset_number();
    spark_state = SPARK_SYNCING;
    DEBUG("REQUESTED PRESETS");
  }

  spark_process();
  app_process();
  
  // K&R: Expressions connected by && or || are evaluated left to right, 
  // and it is guaranteed that evaluation will stop as soon as the truth or falsehood is known.
  
  if (spark_msg_in.get_message(&cmdsub, &msg, &preset) || app_msg_in.get_message(&cmdsub, &msg, & preset)) {
    Serial.print("Message: ");
    Serial.println(cmdsub, HEX);

    // all the processing for sync
    switch (cmdsub) {
      // if serial number response then we know the connection is good
      case 0x0323:
        if (spark_state == SPARK_CONNECTED) {
          spark_state = SPARK_COMMUNICATING;
          DEBUG("RECEIVED SERIAL NUMBER - GOT CONNECTION");
        }
        break;
      // full preset details
      case 0x0301:  
      case 0x0101:
        pres = (preset.preset_num == 0x7f) ? 4 : preset.preset_num;
        if (preset.curr_preset == 0x01) {
          pres = 5;
          // if we receive preset data for 0x0100 then we are fully synced
          if (spark_state == SPARK_SYNCING) {
            spark_state = SPARK_SYNCED;
            DEBUG("FULLY SYNCED NOW");
          }
        }
        presets[pres] = preset;
        break;
      // change of amp model
      case 0x0306:
        strcpy(presets[5].effects[3].EffectName, msg.str2);
        break;
      // change of effect
      case 0x0106:
        ind = get_effect_index(msg.str1);
        if (ind >= 0) 
          strcpy(presets[5].effects[ind].EffectName, msg.str2);
        break;
      // effect on/off  
      case 0x0315:
      case 0x0115:
        ind = get_effect_index(msg.str1);
        if (ind >= 0) 
          presets[5].effects[ind].OnOff = msg.onoff;
        break;
      // change parameter value  
      case 0x0337:
      case 0x0104:
        ind = get_effect_index(msg.str1);
        if (ind >= 0)
          presets[5].effects[ind].Parameters[msg.param1] = msg.val;
        break;  
      // change to preset  
      case 0x0338:
      case 0x0138:
        selected_preset = (msg.param2 == 0x7f) ? 4 : msg.param2;
        presets[5] = presets[selected_preset];
        break;
      // store to preset  
      case 0x0327:
        selected_preset = (msg.param2 == 0x7f) ? 4 : msg.param2;
        presets[selected_preset] = presets[5];
        break;
      // current selected preset
      case 0x0310:
        selected_preset = (msg.param2 == 0x7f) ? 4 : msg.param2;
        if (msg.param1 == 0x01) 
          selected_preset = 5;
        presets[5] = presets[selected_preset];
        break;
      default:
        break;
    }

    // all the processing for UI update
    switch (cmdsub) {
      case 0x0201:  
         if (ui_update_in_progress) {
           Serial.println("Updating UI");

           strcpy(presets[5].Name, "SyncPreset");
           strcpy(presets[5].UUID, "F00DF00D-FEED-0123-4567-987654321000");  
           presets[5].curr_preset = 0x00;
           presets[5].preset_num = 0x03;
           app_msg_out.create_preset(&presets[5]);
           app_process();
           delay(100);
           
           app_msg_out.change_hardware_preset(0x00, 0x00);
           app_process();
           app_msg_out.change_hardware_preset(0x00, 0x03);     
           app_process();

//           spark_msg_out.change_hardware_preset(0x00, 0x7f);     
//           spark_process();

           sp_bin.pass_through = true;
           app_bin.pass_through = true;   
           ui_update_in_progress = false;
         }
       break;
    }
          
    return true;
  }
  else
    return false;
}

void update_ui() {
  sp_bin.pass_through = false;
  app_bin.pass_through = false;
    
  app_msg_out.save_hardware_preset(0x00, 0x03);
  app_process();
  ui_update_in_progress = true;
}

///// ROUTINES TO CHANGE AMP SETTINGS

void change_generic_model(char *new_eff, int slot) {
  if (strcmp(presets[5].effects[slot].EffectName, new_eff) != 0) {
    spark_msg_out.change_effect(presets[5].effects[slot].EffectName, new_eff);
    strcpy(presets[5].effects[slot].EffectName, new_eff);
    spark_process();
    
    last_command_sent="Set "+String(presets[5].effects[slot].EffectName)+" to: "+ String(new_eff);
    delay(100);
  }
}

void change_comp_model(char *new_eff) {
  change_generic_model(new_eff, 1);
}

void change_drive_model(char *new_eff) {
  change_generic_model(new_eff, 2);
}

void change_amp_model(char *new_eff) {
  if (strcmp(presets[5].effects[3].EffectName, new_eff) != 0) {
    spark_msg_out.change_effect(presets[5].effects[3].EffectName, new_eff);
    app_msg_out.change_effect(presets[5].effects[3].EffectName, new_eff);
    strcpy(presets[5].effects[3].EffectName, new_eff);
    last_command_sent="Set "+String(presets[5].effects[3].EffectName)+" to: "+ String(new_eff);
    spark_process();
    app_process();
    delay(100);
  }
}

void change_mod_model(char *new_eff) {
  change_generic_model(new_eff, 4);
}

void change_delay_model(char *new_eff) {
  change_generic_model(new_eff, 5);
}



void change_generic_onoff(int slot,bool onoff) {
  spark_msg_out.turn_effect_onoff(presets[5].effects[slot].EffectName, onoff);
  app_msg_out.turn_effect_onoff(presets[5].effects[slot].EffectName, onoff);
  presets[5].effects[slot].OnOff = onoff;
  last_command_sent=String(presets[5].effects[slot].EffectName)+": "+ String(presets[5].effects[slot].OnOff);
  spark_process();
  app_process();  
}

void change_noisegate_onoff(bool onoff) {
  change_generic_onoff(0, onoff);  
}

void change_comp_onoff(bool onoff) {
  change_generic_onoff(1, onoff);  
}

void change_drive_onoff(bool onoff) {
  change_generic_onoff(2, onoff);  
}

void change_amp_onoff(bool onoff) {
  change_generic_onoff(3, onoff);  
}

void change_mod_onoff(bool onoff) {
  change_generic_onoff(4, onoff);  
}

void change_delay_onoff(bool onoff) {
  change_generic_onoff(5, onoff);  
}

void change_reverb_onoff(bool onoff) {
  change_generic_onoff(6, onoff);  
}


void change_generic_toggle(int slot) {
  bool new_onoff;

  new_onoff = !presets[5].effects[slot].OnOff;
  
  spark_msg_out.turn_effect_onoff(presets[5].effects[slot].EffectName, new_onoff);
  app_msg_out.turn_effect_onoff(presets[5].effects[slot].EffectName, new_onoff);
  last_command_sent=String(presets[5].effects[slot].EffectName)+": "+ String(presets[5].effects[slot].OnOff);
  presets[5].effects[slot].OnOff = new_onoff;
  spark_process();
  app_process();  
}

void change_noisegate_toggle() {
  change_generic_toggle(0);  
}

void change_comp_toggle() {
  change_generic_toggle(1);  
}

void change_drive_toggle() {
  change_generic_toggle(2);  
}

void change_amp_toggle() {
  change_generic_toggle(3);  
}

void change_mod_toggle() {
  change_generic_toggle(4);  
}

void change_delay_toggle() {
  change_generic_toggle(5);  
}

void change_reverb_toggle() {
  change_generic_toggle(6);  
}


void change_generic_param(int slot, int param, float val) {
  float diff;

  // some code to reduce the number of changes
  diff = presets[5].effects[slot].Parameters[param] - val;
  if (diff < 0) diff = -diff;
  if (diff > 0.04) {
    spark_msg_out.change_effect_parameter(presets[5].effects[slot].EffectName, param, val);
    app_msg_out.change_effect_parameter(presets[5].effects[slot].EffectName, param, val);
    presets[5].effects[slot].Parameters[param] = val;
    last_command_sent=String(presets[5].effects[slot].EffectName)+"- Param"+String(param)+": "+ String(presets[5].effects[slot].Parameters[param]);
    spark_process();  
    app_process();
  }
}

void change_noisegate_param(int param, float val) {
  change_generic_param(0, param, val);
}

void change_comp_param(int param, float val) {
  change_generic_param(1, param, val);
}

void change_drive_param(int param, float val) {
  change_generic_param(2, param, val);
}

void change_amp_param(int param, float val) {
  change_generic_param(3, param, val);
}

void change_mod_param(int param, float val) {
  change_generic_param(4, param, val);
}

void change_delay_param(int param, float val) {
  change_generic_param(5, param, val);
}

void change_reverb_param(int param, float val){
  change_generic_param(6, param, val);
}


void change_hardware_preset(int pres_num) {
  if (pres_num >= 0 && pres_num <= 3) {  
    presets[5] = presets[pres_num];
    
    spark_msg_out.change_hardware_preset(0, pres_num);
    app_msg_out.change_hardware_preset(0, pres_num);  
    last_command_sent="Set preset Nr."+ String(pres_num+1);
    spark_process();  
    app_process();
  }
}

void change_custom_preset(SparkPreset *preset, int preset_num) {
  current_preset = preset_num;
  strncpy(current_preset_icon, preset->Icon, STR_LEN);
  
  if (preset_num >= 0 && preset_num <= 4) {
    preset->preset_num = (preset_num < 4) ? preset_num : 0x7f;
    presets[5] = *preset;
    presets[preset_num] = *preset;
    
    last_command_sent="Set preset: "+ String(preset->Name);
    spark_msg_out.create_preset(preset);
    spark_msg_out.change_hardware_preset(0, preset->preset_num);
  }
}

void tuner_on_off(bool on_off) {
  spark_msg_out.tuner_on_off(on_off); 
  last_command_sent="Turned Tuner "+ String((on_off) ? "On":"Off");
  spark_process();  
}

void tuner_on_toogle() {
  tuner=!tuner;
  spark_msg_out.tuner_on_off(tuner); 
  last_command_sent="Turned Tuner "+ String((tuner) ? "On":"Off");
  spark_process();  
}

#line 1 "B:\\GIT\\SparkMIDI\\SparkMIDI17\\SparkComms.ino"
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

#line 1 "B:\\GIT\\SparkMIDI\\SparkMIDI17\\SparkIO.ino"
#include "SparkIO.h"

/*  SparkIO
 *  
 *  SparkIO handles communication to and from the Positive Grid Spark amp over bluetooth for ESP32 boards
 *  
 *  From the programmers perspective, you create and read two formats - a Spark Message or a Spark Preset.
 *  The Preset has all the data for a full preset (amps, effects, values) and can be sent or received from the amp.
 *  The Message handles all other changes - change amp, change effect, change value of an effect parameter, change hardware preset and so on
 *  
 *  The class is initialized by creating an instance such as:
 *  
 *  SparkClass sp;
 *  
 *  Conection is handled with the two commands:
 *  
 *    sp.start_bt();
 *    sp.connect_to_spark();
 *    
 *  The first starts up bluetooth, the second connects to the amp
 *  
 *  
 *  Messages and presets to and from the amp are then queued and processed.
 *  The essential thing is the have the process() function somewhere in loop() - this handles all the processing of the input and output queues
 *  
 *  loop() {
 *    ...
 *    sp.process()
 *    ...
 *    do something
 *    ...
 *  }
 * 
 * Sending functions:
 *     void create_preset(SparkPreset *preset);    
 *     void get_serial();    
 *     void turn_effect_onoff(char *pedal, bool onoff);    
 *     void change_hardware_preset(uint8_t preset_num);    
 *     void change_effect(char *pedal1, char *pedal2);    
 *     void change_effect_parameter(char *pedal, int param, float val);
 *     
 *     These all create a message or preset to be sent to the amp when they reach the front of the 'send' queue
 *  
 * Receiving functions:
 *     bool get_message(unsigned int *cmdsub, SparkMessage *msg, SparkPreset *preset);
 * 
 *     This receives the front of the 'received' queue - if there is nothing it returns false
 *     
 *     Based on whatever was in the queue, it will populate fields of the msg parameter or the preset parameter.
 *     Eveything apart from a full preset sent from the amp will be a message.
 *     
 *     You can determine which by inspecting cmdsub - this will be 0x0301 for a preset.
 *     
 *     Other values are:
 *     
 *     cmdsub       str1                   str2              val           param1             param2                onoff
 *     0323         amp serial #
 *     0337         effect name                              effect val    effect number
 *     0306         old effect             new effect
 *     0338                                                                0                  new hw preset (0-3)
 * 
 * 
 * 
 */


void dump_buf(char *hdr, uint8_t *buf, int len) {
 
  Serial.print(hdr);
  Serial.print(" <");
  Serial.print(buf[18], HEX);
  Serial.print("> ");
  Serial.print(buf[len-2], HEX);
  Serial.print(" ");
  Serial.print(buf[len-1], HEX);  
  
  int i;
  for (i = 0; i < len ; i++) {
    if (buf[i]<16) Serial.print("0");
    Serial.print(buf[i], HEX);
    Serial.print(" ");
    if (i % 16 == 15) Serial.println();
  };
  Serial.println(); 
}


// TO FIX
// *ok_to_send shared across classes, and also checked whilst sending to the app
// app_rc_seq / sp_rc_seq - anaylse and work out how it should work!!!

// UTILITY FUNCTIONS

void uint_to_bytes(unsigned int i, uint8_t *h, uint8_t *l) {
  *h = (i & 0xff00) / 256;
  *l = i & 0xff;
}

void bytes_to_uint(uint8_t h, uint8_t l,unsigned int *i) {
  *i = (h & 0xff) * 256 + (l & 0xff);
}

 
// MAIN SparkIO CLASS

uint8_t chunk_header_from_spark[16]{0x01, 0xfe, 0x00, 0x00, 0x41, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
uint8_t chunk_header_to_spark[16]  {0x01, 0xfe, 0x00, 0x00, 0x53, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

void spark_start(bool passthru) {
  sp_bin.set(passthru, &sp_in_chunk, chunk_header_from_spark);
  app_bin.set(passthru, &app_in_chunk, chunk_header_to_spark);

  sp_cin.set(&sp_in_chunk, &sp_in_message, &sp_ok_to_send, &sp_rec_seq);
  app_cin.set(&app_in_chunk, &app_in_message, &app_ok_to_send, &app_rec_seq);

  spark_msg_in.set(&sp_in_message);
  app_msg_in.set(&app_in_message);

  spark_msg_out.set(&sp_out_message);
  app_msg_out.set(&app_out_message);

  sp_cout.set(&sp_out_chunk, &sp_out_message, &sp_rec_seq);
  app_cout.set(&app_out_chunk, &app_out_message, &app_rec_seq);

  sp_bout.set(&sp_out_chunk, chunk_header_to_spark, &sp_ok_to_send);
  app_bout.set(&app_out_chunk, chunk_header_from_spark, &app_ok_to_send);
}

// 
// Main processing routine
//

void spark_process() 
{
  // process inputs
  sp_bin.process();
  sp_cin.process();

  if (!sp_ok_to_send && (millis() - sp_bout.last_sent_time > 500)) {
    DEBUG("Timeout on send");
    sp_ok_to_send = true;
  }

  // process outputs
  sp_cout.process();
  sp_bout.process();

}


void app_process() 
{
  // process inputs
  app_bin.process();
  app_cin.process();

  // process outputs
  app_cout.process();
  app_bout.process();
}

// BLOCK INPUT ROUTINES 
// Routine to read the block and put into the in_chunk ring buffer

//////////////////// TEMPORARY TO DEBUG BAD BLOCK PROCESSING
bool in_bad_block = false;

void BlockIn::process() {
  uint8_t b;
  bool boo;

  while (data_available()) {
    b = data_read();
   
    // **** PASSTHROUGH APP AND AMP ****

    if (pass_through) {
      if (io_state == 0) {
        if (b == 0x01) {  // block passthough
          io_state = 1;
        } 
        else if (b == 0xf0) { // chunk passthrough (for Spark Mini)
          io_state = 101;
          io_buf[0] = 0xf0;
          io_pos = 1;
          io_len = MAX_IO_BUFFER;          
        }
      }
      else if (io_state == 1) {
        if (b == 0xfe) {
          io_state = 2;
          io_buf[0] = 0x01;
          io_buf[1] = 0xfe;
          io_pos = 2;
          io_len = MAX_IO_BUFFER;
        }
        else if (b == 0x01) {
          io_state = 1;
        }
        else
          io_state = 0;
      }
      else if (io_state == 2) {
        if (io_pos == 6) {
          io_len = b;
        }
        io_buf[io_pos++] = b;
        if (io_pos >= io_len) {         // could only be > if the earlier matching was fooled and len is <= 6
          data_write(io_buf, io_pos);

          io_pos = 0;
          io_len = MAX_IO_BUFFER; 
          io_state = 0; 
        }
      }
      else if (io_state == 101) { // chunk passthrough only
        io_buf[io_pos++] = b;
        if (b == 0xf7) { 
          data_write(io_buf, io_pos);

          io_pos = 0;
          io_len = MAX_IO_BUFFER; 
          io_state = 0; 
        }
      }

      if (io_pos > MAX_IO_BUFFER) {
        DEBUG("SPARKIO IO_PROCESS_IN_BLOCKS OVERRUN");
        while (true);
      }
    }
    // **** END PASSTHROUGH ****

   
    // check the 7th byte which holds the block length

    if (rb_state == 0 && b == 0xf0) { // chunk header not block header
      rb_state = 101;
    }
    else if (rb_state == 6) {
      rb_len = b - 16;
      rb_state++;
    }  // check every other byte in the block header for a match to the header standard
    else if (rb_state >= 0 && rb_state < 16) {
      if (b == blk_hdr[rb_state]) {
        rb_state++;
      }
      else {
        DEBUG("Bad block header at position: ");
        // DEBUG(rb_state);
        // DEBUG(" data: ");
        // DEBUG(b);
        // DEBUG(" expected: ");
        // DEBUG(blk_hdr[rb_state]);
        // DEBUG();
        rb_state = 0;
        if (b == blk_hdr[rb_state])  // in case we found 0x01, the start of the header
          rb_state++;
      }
    } 
    // and once past the header just read the next bytes as defined by rb_len
    // store these to the chunk buffer
    else if (rb_state == 16) {
      rb->add(b);
      
/*        
      if (b < 16) Serial.print("0");
      Serial.print(b, HEX);
      Serial.print("+");     
*/
      rb_len--;
      if (rb_len == 0) {
        rb_state = 0;
        rb->commit();
//        Serial.println();
      }
    }
    
    if (rb_state == 101) {
      rb->add(b);     
/*
      if (b < 16) Serial.print("0");
      Serial.print(b, HEX);
      Serial.print("-");
*/
      if (b == 0xf7) {
        rb_state == 0;
        rb->commit(); 
//        Serial.println();              
      }
    }
  }
}

void SparkBlockIn::set(bool pass, RingBuffer *ring_buffer, uint8_t *hdr) {
  rb = ring_buffer;
  blk_hdr = hdr;
  pass_through = pass;
}

bool SparkBlockIn::data_available() {
  return sp_available();
}

uint8_t SparkBlockIn::data_read(){
  return sp_read();
}
void SparkBlockIn::data_write(uint8_t *buf, int len){
  app_write(buf, len);
}

void AppBlockIn::set(bool pass, RingBuffer *ring_buffer, uint8_t *hdr) {
  rb = ring_buffer;
  blk_hdr = hdr;
  pass_through = pass;
}

bool AppBlockIn::data_available() {
  return app_available();
}

uint8_t AppBlockIn::data_read(){
  return app_read();
}

void AppBlockIn::data_write(uint8_t *buf, int len){
  sp_write(buf, len);
}

// CHUNK INPUT ROUTINES
// Routine to read chunks from the in_chunk ring buffer and copy to a in_message msgpack buffer


void SparkChunkIn::set(RingBuffer *chunks, RingBuffer *messages, bool *ok, uint8_t *seq) {
  in_chunk = chunks;
  in_message = messages;
  ok_to_send = ok;
  *ok_to_send = true;
  rec_seq = seq;
}

void AppChunkIn::set(RingBuffer *chunks, RingBuffer *messages, bool *ok, uint8_t *seq) {
  in_chunk = chunks;
  in_message = messages;
  ok_to_send = ok;
  *ok_to_send = true;
  rec_seq = seq;
}

void ChunkIn::process() {
  uint8_t b;
  bool boo;
  unsigned int len;
  uint8_t len_h, len_l;

  while (!in_chunk->is_empty()) {      
    boo = in_chunk->get(&b);
    if (!boo) DEBUG("Chunk is_empty was false but the buffer was empty!");

    switch (rc_state) {
      case 1:
        if (b == 0x01) 
          rc_state++; 
        else 
          rc_state = 0; 
        break;
      case 2:
        rc_seq = b; 
        *rec_seq = b;
        rc_state++; 
        break;
      case 3:
        rc_checksum = b;
        rc_state++; 
        break;
      case 4:
        rc_cmd = b; 
        rc_state++; 
        break;
      case 5:
        rc_sub = b; 
        rc_state = 10;

        // flow control for blocking sends - put here in case we want to check rc_sub too
        // in here for amp responses - only triggered by the amp
        if ((rc_cmd == 0x04 || rc_cmd == 0x05) && rc_sub == 0x01) {
          ////////////// THIS FEELS CLUNKY - HOW CAN THIS BE DONE BETTER? <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
          if (*ok_to_send == false) {
            *ok_to_send = true;
            DEBUG("Unblocked");
          }
        }
        
        // set up for the main data loop - rc_state 10
        rc_bitmask = 0x80;
        rc_calc_checksum = 0;
        rc_data_pos = 0;
        
        // check for multi-chunk
        if (rc_sub == 1 && (rc_cmd == 3 || rc_cmd == 1))  
          rc_multi_chunk = true;
        else {
          rc_multi_chunk = false;
          in_message_bad = false;
          in_message->add(rc_cmd);
          in_message->add(rc_sub);
          in_message->add(0);
          in_message->add(0);
        }
        break;
      case 10:                    // the main loop which ends on an 0xf7
        if (b == 0xf7) {
          if (rc_calc_checksum != rc_checksum) 
            in_message_bad = true;
          rc_state = 0;
          if (!rc_multi_chunk || (rc_this_chunk == rc_total_chunks-1)) { //last chunk in message
            if (in_message_bad) {
              DEBUG("Bad message, dropped");
              in_message->drop();
            }
            else {
              len = in_message->get_len();
              uint_to_bytes(len, &len_h, &len_l);

              in_message->set_at_index(2, len_h);
              in_message->set_at_index(3, len_l);
              in_message->commit();
            }  
          }
        }
        else if (rc_bitmask == 0x80) { // if the bitmask got to this value it is now a new bits 
          rc_calc_checksum ^= b;
          rc_bits = b;
          rc_bitmask = 1;
        }
        else {
          rc_data_pos++;
          rc_calc_checksum ^= b;          
          if (rc_bits & rc_bitmask) 
            b |= 0x80;
          rc_bitmask *= 2;
          
          if (rc_multi_chunk && rc_data_pos == 1) 
            rc_total_chunks = b;
          else if (rc_multi_chunk && rc_data_pos == 2) {
            rc_last_chunk = rc_this_chunk;
            rc_this_chunk = b;
            if (rc_this_chunk == 0) {
              in_message_bad = false;
              in_message->add(rc_cmd);
              in_message->add(rc_sub);
              in_message->add(0);
              in_message->add(0);
            }
            else if (rc_this_chunk != rc_last_chunk+1)
              in_message_bad = true;
          }
          else if (rc_multi_chunk && rc_data_pos == 3) 
            rc_chunk_len = b;
          else {  
            in_message->add(b);             
          }
          
        };
        break;
    }

    // checking for rc_state 0 is done separately so that if a prior step finds a mismatch
    // and resets the state to 0 it can be processed here for that byte - saves missing the 
    // first byte of the header if that was misplaced
    
    if (rc_state == 0) {
      if (b == 0xf0) 
        rc_state++;
    }
  }
}



// MESSAGE INPUT ROUTINES
// Routine to read messages from the in_message ring buffer and copy to a message C structurer


void SparkMessageIn::set(RingBuffer *messages) {
  in_message = messages;
}

void AppMessageIn::set(RingBuffer *messages) {
  in_message = messages;
}

void MessageIn::read_byte(uint8_t *b)
{
  uint8_t a;
  in_message->get(&a);
  *b = a;
}   

void MessageIn::read_uint(uint8_t *b)
{
  uint8_t a;
  in_message->get(&a);
  if (a == 0xCC)
    in_message->get(&a);
  *b = a;
}
   
void MessageIn::read_string(char *str)
{
  uint8_t a, len;
  int i;

  read_byte(&a);
  if (a == 0xd9) {
    read_byte(&len);
  }
  else if (a >= 0xa0) {
    len = a - 0xa0;
  }
  else {
    read_byte(&a);
    if (a < 0xa0 || a >= 0xc0) DEBUG("Bad string");
    len = a - 0xa0;
  }

  if (len > 0) {
    // process whole string but cap it at STR_LEN-1
    for (i = 0; i < len; i++) {
      read_byte(&a);
      if (a<0x20 || a>0x7e) a=0x20; // make sure it is in ASCII range - to cope with get_serial 
      if (i < STR_LEN -1) str[i]=a;
    }
    str[i > STR_LEN-1 ? STR_LEN-1 : i]='\0';
  }
  else {
    str[0]='\0';
  }
}   

void MessageIn::read_prefixed_string(char *str)
{
  uint8_t a, len;
  int i;

  read_byte(&a); 
  read_byte(&a);

  if (a < 0xa0 || a >= 0xc0) DEBUG("Bad string");
  len = a-0xa0;

  if (len > 0) {
    for (i = 0; i < len; i++) {
      read_byte(&a);
      if (a<0x20 || a>0x7e) a=0x20; // make sure it is in ASCII range - to cope with get_serial 
      if (i < STR_LEN -1) str[i]=a;
    }
    str[i > STR_LEN-1 ? STR_LEN-1 : i]='\0';
  }
  else {
    str[0]='\0';
  }
}   

void MessageIn::read_float(float *f)
{
  union {
    float val;
    byte b[4];
  } conv;   
  uint8_t a;
  int i;

  read_byte(&a);  // should be 0xca
  if (a != 0xca) return;

  // Seems this creates the most significant byte in the last position, so for example
  // 120.0 = 0x42F00000 is stored as 0000F042  
   
  for (i=3; i>=0; i--) {
    read_byte(&a);
    conv.b[i] = a;
  } 
  *f = conv.val;
}

void MessageIn::read_onoff(bool *b)
{
  uint8_t a;
   
  read_byte(&a);
  if (a == 0xc3)
    *b = true;
  else // 0xc2
    *b = false;
}

// The functions to get the message

bool MessageIn::get_message(unsigned int *cmdsub, SparkMessage *msg, SparkPreset *preset)
{
  uint8_t cmd, sub, len_h, len_l;
  unsigned int len;
  unsigned int cs;
   
  uint8_t junk;
  int i, j;
  uint8_t num;

  if (in_message->is_empty()) return false;
  
  read_byte(&cmd);
  read_byte(&sub);
  read_byte(&len_h);
  read_byte(&len_l);
  
  bytes_to_uint(len_h, len_l, &len);
  bytes_to_uint(cmd, sub, &cs);

  *cmdsub = cs;
  switch (cs) {

    // 0x02 series - requests
    // get preset information
    case 0x0201:
      read_byte(&msg->param1);
      read_byte(&msg->param2);
      for (i=0; i < 30; i++) read_byte(&junk); // 30 bytes of 0x00
      break;            
    // get current hardware preset number - this is a request with no payload
    case 0x0210:
      break;
    // get amp name - no payload
    case 0x0211:
      break;
    // get name - this is a request with no payload
    case 0x0221:
      break;
    // get serial number - this is a request with no payload
    case 0x0223:
      break;
    // the UNKNOWN command - 0x0224 00 01 02 03
    case 0x0224:
    case 0x022a:
    case 0x032a:
      // the data is a fixed array of four bytes (0x94 00 01 02 03)
      read_byte(&junk);
      read_uint(&msg->param1);
      read_uint(&msg->param2);
      read_uint(&msg->param3);
      read_uint(&msg->param4);
    // get firmware version - this is a request with no payload
    case 0x022f:
      break;
    // change effect parameter
    case 0x0104:
      read_string(msg->str1);
      read_byte(&msg->param1);
      read_float(&msg->val);
      break;
    // change of effect model
    case 0x0306:
    case 0x0106:
      read_string(msg->str1);
      read_string(msg->str2);
      break;
    // get current hardware preset number
    case 0x0310:
      read_byte(&msg->param1);
      read_byte(&msg->param2);
      break;
    // get name
    case 0x0311:
      read_string(msg->str1);
      break;
    // enable / disable an effect
    // and 0x0128 amp info command
    case 0x0315:
    case 0x0115:
    case 0x0128:
      read_string(msg->str1);
      read_onoff(&msg->onoff);
      break;
    // get serial number
    case 0x0323:
      read_string(msg->str1);
      break;
    // store into hardware preset
    case 0x0327:
      read_byte(&msg->param1);
      read_byte(&msg->param2);
      break;
    // amp info   
    case 0x0328:
      read_float(&msg->val);
      break;  
    // firmware version number
    case 0x032f:
      // really this is a uint32 but it is just as easy to read into 4 uint8 - a bit of a cheat
      read_byte(&junk);           // this will be 0xce for a uint32
      read_byte(&msg->param1);      
      read_byte(&msg->param2); 
      read_byte(&msg->param3); 
      read_byte(&msg->param4); 
      break;
    // change of effect parameter
    case 0x0337:
      read_string(msg->str1);
      read_byte(&msg->param1);
      read_float(&msg->val);
      break;
    // tuner
    case 0x0364:
      read_byte(&msg->param1);
      read_float(&msg->val);
      break;
    case 0x0365:
      read_onoff(&msg->onoff);
      break;
    // change of preset number selected on the amp via the buttons
    case 0x0338:
    case 0x0138:
      read_byte(&msg->param1);
      read_byte(&msg->param2);
      break;
    // license key
    case 0x0170:
      for (i = 0; i < 64; i++)
        read_uint(&license_key[i]);
      break;
    // response to a request for a full preset
    case 0x0301:
    case 0x0101:
      read_byte(&preset->curr_preset);
      read_byte(&preset->preset_num);
      read_string(preset->UUID); 
      read_string(preset->Name);
      read_string(preset->Version);
      read_string(preset->Description);
      read_string(preset->Icon);
      read_float(&preset->BPM);

      for (j=0; j<7; j++) {
        read_string(preset->effects[j].EffectName);
        read_onoff(&preset->effects[j].OnOff);
        read_byte(&num);
        preset->effects[j].NumParameters = num - 0x90;
        for (i = 0; i < preset->effects[j].NumParameters; i++) {
          read_byte(&junk);
          read_byte(&junk);
          read_float(&preset->effects[j].Parameters[i]);
        }
      }
      read_byte(&preset->chksum);  

      break;
    // tap tempo!
    case 0x0363:
      read_float(&msg->val);  
      break;
    case 0x0470:
    case 0x0428:
      read_byte(&junk);
      break;
    // acks - no payload to read - no ack sent for an 0x0104
    case 0x0401:
    case 0x0501:
    case 0x0406:
    case 0x0415:
    case 0x0438:
    case 0x0465:
//      Serial.print("Got an ack ");
//      Serial.println(cs, HEX);
      break;
    default:
      Serial.print("Unprocessed message SparkIO ");
      Serial.print (cs, HEX);
      Serial.print(":");
      for (i = 0; i < len - 4; i++) {
        read_byte(&junk);
        Serial.print(junk, HEX);
        Serial.print(" ");
      }
      Serial.println();
  }
  return true;
}

//
// Output routines
//

void MessageOut::start_message(int cmdsub)
{
  int om_cmd, om_sub;
  
  om_cmd = (cmdsub & 0xff00) >> 8;
  om_sub = cmdsub & 0xff;

  out_message->add(om_cmd);
  out_message->add(om_sub);
  out_message->add(0);      // placeholder for length
  out_message->add(0);      // placeholder for length

  out_msg_chksum = 0;
}

void MessageOut::end_message()
{
  unsigned int len;
  uint8_t len_h, len_l;
  
  len = out_message->get_len();
  uint_to_bytes(len, &len_h, &len_l);
  
  out_message->set_at_index(2, len_h);   
  out_message->set_at_index(3, len_l);
  out_message->commit();
}

void MessageOut::write_byte_no_chksum(byte b)
{
  out_message->add(b);
}

void MessageOut::write_byte(byte b)
{
  out_message->add(b);
  out_msg_chksum += int(b);
}

void MessageOut::write_uint(byte b)
{
  if (b > 127) {
    out_message->add(0xCC);
    out_msg_chksum += int(0xCC);  
  }
  out_message->add(b);
  out_msg_chksum += int(b);
}

void MessageOut::write_uint32(uint32_t w)
{
  int i;
  uint8_t b;
  uint32_t mask;

  mask = 0xFF000000;
  
  out_message->add(0xCE);
  out_msg_chksum += int(0xCE);  

  for (i = 3; i >= 0; i--) {
    b = (w & mask) >> (8*i);
    mask >>= 8;
    write_uint(b);
//    out_message->add(b);
//    out_msg_chksum += int(b);
  }
}


void MessageOut::write_prefixed_string(const char *str)
{
  int len, i;

  len = strnlen(str, STR_LEN);
  write_byte(byte(len));
  write_byte(byte(len + 0xa0));
  for (i=0; i<len; i++)
    write_byte(byte(str[i]));
}

void MessageOut::write_string(const char *str)
{
  int len, i;

  len = strnlen(str, STR_LEN);
  write_byte(byte(len + 0xa0));
  for (i=0; i<len; i++)
    write_byte(byte(str[i]));
}      
  
void MessageOut::write_long_string(const char *str)
{
  int len, i;

  len = strnlen(str, STR_LEN);
  write_byte(byte(0xd9));
  write_byte(byte(len));
  for (i=0; i<len; i++)
    write_byte(byte(str[i]));
}

void MessageOut::write_float (float flt)
{
  union {
    float val;
    byte b[4];
  } conv;
  int i;
   
  conv.val = flt;
  // Seems this creates the most significant byte in the last position, so for example
  // 120.0 = 0x42F00000 is stored as 0000F042  
   
  write_byte(0xca);
  for (i=3; i>=0; i--) {
    write_byte(byte(conv.b[i]));
  }
}

void MessageOut::write_onoff (bool onoff)
{
  byte b;

  if (onoff)
  // true is 'on'
    b = 0xc3;
  else
    b = 0xc2;
  write_byte(b);
}


void MessageOut::change_effect_parameter (char *pedal, int param, float val)
{
   if (cmd_base == 0x0100) 
     start_message (cmd_base + 0x04);
   else
     start_message (cmd_base + 0x37);
   write_prefixed_string (pedal);
   write_byte (byte(param));
   write_float(val);
   end_message();
}

void MessageOut::change_effect (char *pedal1, char *pedal2)
{
   start_message (cmd_base + 0x06);
   write_prefixed_string (pedal1);
   write_prefixed_string (pedal2);
   end_message();
}



void MessageOut::change_hardware_preset (uint8_t curr_preset, uint8_t preset_num)
{
   // preset_num is 0 to 3

   start_message (cmd_base + 0x38);
   write_byte (curr_preset);
   write_byte (preset_num)  ;     
   end_message();  
}


void MessageOut::turn_effect_onoff (char *pedal, bool onoff)
{
   start_message (cmd_base + 0x15);
   write_prefixed_string (pedal);
   write_onoff (onoff);
   end_message();
}

void MessageOut::get_serial()
{
   start_message (0x0223);
   end_message();  
}

void MessageOut::get_name()
{
   start_message (0x0211);
   end_message();  
}

void MessageOut::get_hardware_preset_number()
{
   start_message (0x0210);
   end_message();  
}

void MessageOut::get_checksum_info() {
   start_message (0x022a);
   write_byte(0x94);
   write_uint(0);
   write_uint(1);
   write_uint(2);
   write_uint(3);   
   end_message();   
}

void MessageOut::get_firmware() {
   start_message (0x022f);
   end_message(); 
}

void MessageOut::save_hardware_preset(uint8_t curr_preset, uint8_t preset_num)
{
   start_message (cmd_base + 0x27);
//   start_message (0x0327);
   write_byte (curr_preset);
   write_byte (preset_num);  
   end_message();
}

void MessageOut::send_firmware_version(uint32_t firmware)
{
   start_message (0x032f);
   write_uint32(firmware);  
   end_message();
}

void MessageOut::send_serial_number(char *serial)
{
   start_message (0x0323);
   write_prefixed_string(serial);
   end_message();
}

void MessageOut::send_ack(unsigned int cmdsub) {
   start_message (cmdsub);
   end_message();
}

void MessageOut::send_0x022a_info(byte v1, byte v2, byte v3, byte v4)
{
   start_message (0x032a);
   write_byte(0x94);
   write_uint(v1);
   write_uint(v2);
   write_uint(v3);
   write_uint(v4);      
   end_message();
}

void MessageOut::send_key_ack()
{
   start_message (0x0470);
   write_byte(0x00);
   end_message();
}

void MessageOut::send_preset_number(uint8_t preset_h, uint8_t preset_l)
{
   start_message (0x0310);
   write_byte(preset_h);
   write_byte(preset_l);
   end_message();
}

void MessageOut::tuner_on_off(bool onoff)
{
   start_message (0x0165);
   write_onoff (onoff);
   end_message();
}

void MessageOut::get_preset_details(unsigned int preset)
{
   int i;
   uint8_t h, l;

   uint_to_bytes(preset, &h, &l);
   
   start_message (0x0201);
   write_byte(h);
   write_byte(l);

   for (i=0; i<30; i++) {
     write_byte(0);
   }
   
   end_message(); 
}

void MessageOut::create_preset(SparkPreset *preset)
{
  int i, j, siz;

  start_message (cmd_base + 0x01);

  write_byte_no_chksum (0x00);
  write_byte_no_chksum (preset->preset_num);   
  write_long_string (preset->UUID);
  //write_string (preset->Name);
  if (strnlen (preset->Name, STR_LEN) > 31)
    write_long_string (preset->Name);
  else
    write_string (preset->Name);
    
  write_string (preset->Version);
  if (strnlen (preset->Description, STR_LEN) > 31)
    write_long_string (preset->Description);
  else
    write_string (preset->Description);
  write_string(preset->Icon);
  write_float (preset->BPM);
   
  write_byte (byte(0x90 + 7));       // always 7 pedals
  for (i=0; i<7; i++) {
    write_string (preset->effects[i].EffectName);
    write_onoff(preset->effects[i].OnOff);

    siz = preset->effects[i].NumParameters;
    write_byte ( 0x90 + siz); 
      
    for (j=0; j<siz; j++) {
      write_byte (j);
      write_byte (byte(0x91));
      write_float (preset->effects[i].Parameters[j]);
    }
  }
  write_byte_no_chksum (uint8_t(out_msg_chksum % 256));  
  end_message();
  
//  DEBUG("");
//  out_message->dump3();
}

void SparkMessageOut::set(RingBuffer *messages) {
  out_message = messages;
  cmd_base = 0x0100;
}

void AppMessageOut::set(RingBuffer *messages) {
  out_message = messages;
  cmd_base = 0x0300;
}

void ChunkOut::out_store(uint8_t b)
{
  uint8_t bits;
  
  if (oc_bit_mask == 0x80) {
    oc_bit_mask = 1;
    oc_bit_pos = out_chunk->get_pos();
    out_chunk->add(0);
  }
  
  if (b & 0x80) {
    out_chunk->set_bit_at_index(oc_bit_pos, oc_bit_mask);
    oc_checksum ^= oc_bit_mask;
  }
  out_chunk->add(b & 0x7f);
  oc_checksum ^= (b & 0x7f);

  oc_len++;
  oc_bit_mask *= 2;
}

void ChunkOut::process() {
  int i, j, len;
  int checksum_pos;
  uint8_t b;
  uint8_t len_h, len_l;

  uint8_t num_chunks, this_chunk, this_len;
 
  while (!out_message->is_empty()) {
    out_message->get(&oc_cmd);
    out_message->get(&oc_sub);
    out_message->get(&len_h);
    out_message->get(&len_l);
    bytes_to_uint(len_h, len_l, &oc_len);
    len = oc_len - 4;

    if (len > chunk_size) { //this is a multi-chunk message
      num_chunks = int((len - 1) / chunk_size) + 1;
      for (this_chunk = 0; this_chunk < num_chunks; this_chunk++) {
       
        // create chunk header
        out_chunk->add(0xf0);
        out_chunk->add(0x01);

        // WATCH OUT THIS CODE IS IN TWO PLACES!!! NEED TO CHANGE BOTH IF CHANGING EITHER
        // Feels clunky to use a 'global' variable but how else to get the sequence number from the input to the output?
        if (oc_cmd == 0x04 || oc_cmd == 0x05 || oc_cmd == 0x03) { // response, so use other sequence counter
          out_chunk->add(*rec_seq);
        }
        else {
          out_chunk->add(oc_seq);
          oc_seq++;
          if (oc_seq == 0x7f) oc_seq = 0x40;  // for sending from amp to app
          if (oc_seq == 0x3f) oc_seq = 0x01;  // for sending from app to amp
        };

        
        checksum_pos = out_chunk->get_pos();
        out_chunk->add(0); // checksum
        
        out_chunk->add(oc_cmd);
        out_chunk->add(oc_sub);

        this_len = len % chunk_size;
        if ((num_chunks != this_chunk + 1) || (this_len == 0)) 
          this_len = chunk_size;

        oc_bit_mask = 0x80;
        oc_checksum = 0;
        
        // create chunk sub-header          
        out_store(num_chunks);
        out_store(this_chunk);
        out_store(this_len);
        
        for (i = 0; i < this_len; i++) {
          out_message->get(&b);
          out_store(b);
        }
        out_chunk->set_at_index(checksum_pos, oc_checksum);        
        out_chunk->add(0xf7);
      }
    } 
    else { 
    // create chunk header
      out_chunk->add(0xf0);
      out_chunk->add(0x01);

      // Feels clunky to use a 'global' variable but how else to get the sequence number from the input to the output?
      if (oc_cmd == 0x04 || oc_cmd == 0x05 || (oc_cmd == 0x03 && (oc_sub != 0x27 && oc_sub != 0x37 && oc_sub != 0x38 && oc_sub != 0x15 && oc_sub != 0x06))) { // response, so use other sequence counter
        out_chunk->add(*rec_seq);
      }
      else {
        out_chunk->add(oc_seq);
        oc_seq++;
        if (oc_seq == 0x7f) oc_seq = 0x40;  // for sending from amp to app
        if (oc_seq == 0x3f) oc_seq = 0x01;  // for sending from app to amp
      };

      checksum_pos = out_chunk->get_pos();
      out_chunk->add(0); // checksum

      out_chunk->add(oc_cmd);
      out_chunk->add(oc_sub);

      oc_bit_mask = 0x80;
      oc_checksum = 0;
      for (i = 0; i < len; i++) {
        out_message->get(&b);
        out_store(b);
      }
     out_chunk->set_at_index(checksum_pos, oc_checksum);        
     out_chunk->add(0xf7);
    }
    out_chunk->commit();
    
    DEBUG("");
    //out_chunk->dump3();
  }
}

void SparkChunkOut::set(RingBuffer *chunks, RingBuffer *messages, uint8_t *seq) {
  out_chunk = chunks;
  out_message = messages;
  chunk_size = 0x80;
  oc_seq = 0x01;
  rec_seq = seq;
}

void AppChunkOut::set(RingBuffer *chunks, RingBuffer *messages, uint8_t *seq) {
  out_chunk = chunks;
  out_message = messages;
  chunk_size = 0x19;
  oc_seq = 0x40;
  rec_seq = seq;
}

/*
void dumpit(uint8_t *buf, int len) {
  int i;
  uint8_t v;

  Serial.print("               ");
  for (i=0; i<len; i++) {
    v = buf[i];
    if (v < 16) Serial.print("0");
    Serial.print(v, HEX);
    Serial.print(" ");
    if (i % 16 == 15) {
      Serial.println();
      Serial.print("               ");
    };
  };
  Serial.println();
}
*/

void BlockOut::process() {
  int i;
  int len;
  uint8_t b;  
  uint8_t cmd, sub;

  while (!out_chunk->is_empty() && *ok_to_send) {
    ob_pos = 16;

    for (i=0; i < 16; i++) 
      out_block[i]= blk_hdr[i];
      
    b = 0;

    // This condition is complex because sending to the Spark is always a block ending in 0xf7 and chunk aligned
    // but the Spark sends a slew of data until it runs out, and the block contains multiple and partial
    // chunks, and with a different block size - inconsistent
    
    while (    (!to_spark && (ob_pos < block_size && !out_chunk->is_empty())) 
            || ( to_spark && (b != 0xf7))   ) {
      out_chunk->get(&b);
      
      // look for cmd and sub in the stream and set blocking to true if 0x0101 found - multi chunk
      // not sure if that should be here because it means the block send needs to understand the chunks content
      // perhaps it should be between converting msgpack to chunks and put flow control in there
      if (ob_pos == 20) 
        cmd = b;
      if (ob_pos == 21)
        sub = b;

      out_block[ob_pos++] = b;
    }
    
    out_block[6] = ob_pos;
    
    if (is_spark_mini)
      data_write(out_block + 16, ob_pos - 16); // skip the block header
    else 
      data_write(out_block, ob_pos);
      
    //dumpit(out_block, ob_pos);  


    if (to_spark) {
      if (cmd == 0x01 && sub == 0x01) {// need to ensure we are only blocking on send to spark!
          *ok_to_send = false;
          last_sent_time = millis();  
          DEBUG("Blocked");
      }
    }
  }
}

void SparkBlockOut::set(RingBuffer *chunks, uint8_t *hdr, bool *ok) {
  out_chunk = chunks;
  blk_hdr = hdr;
  block_size = 0xad;
  to_spark = true;
  
  ok_to_send = ok;
  *ok_to_send = true;    
  last_sent_time = millis();
}

void SparkBlockOut::data_write(uint8_t *buf, int len){
  sp_write(buf, len);
}

void AppBlockOut::set(RingBuffer *chunks, uint8_t *hdr, bool *ok) {
  out_chunk = chunks;
  blk_hdr = hdr;
  block_size = 0x6a;
  to_spark = false;

  // not sure we need flow control to the app
  ok_to_send = ok;
  *ok_to_send = true;
  last_sent_time = millis();
}

void AppBlockOut::data_write(uint8_t *buf, int len){
  app_write_timed(buf, len);
}

#line 1 "B:\\GIT\\SparkMIDI\\SparkMIDI17\\SparkPresetFromSD.ino"
#include "SparkPresetFromSD.h"


bool loadSparkPresetFromSD(SparkPreset* preset, const char* filename) {
  File configFile = SD.open(filename);
  if (!configFile) {
    DEBUG("Failed to open config file");
    return false;
  }

  size_t size = configFile.size();
  if (size > 4096) {
    DEBUG("Config file size is too large");
    return false;
  }

  std::unique_ptr<char[]> buf(new char[size]);
  configFile.readBytes(buf.get(), size);
  configFile.close();

  DynamicJsonDocument doc(4096);
  DeserializationError error = deserializeJson(doc, buf.get(), size);
  if (error) {
    DEBUG("Failed to parse config file");
    return false;
  }

  // Parse meta data
  DEBUG("Parse meta data...");
  strncpy(preset->UUID, doc["meta"]["id"].as<const char*>(), sizeof(preset->UUID));
  strncpy(preset->Name, doc["meta"]["name"].as<const char*>(), sizeof(preset->Name));
  strncpy(preset->Version, doc["meta"]["version"].as<const char*>(), sizeof(preset->Version));
  strncpy(preset->Description, doc["meta"]["description"].as<const char*>(), sizeof(preset->Description));
  strncpy(preset->Icon, doc["meta"]["icon"].as<const char*>(), sizeof(preset->Icon));
  preset->BPM = doc["bpm"].as<float>();

  const JsonArray& sigpath = doc["sigpath"];
  size_t sigpathSize = sigpath.size();

  // Parse sigpath array
    DEBUG("Parse sigpath array...");
  int i = 0;
  for (auto fx : sigpath) {
    strncpy(preset->effects[i].EffectName, fx["dspId"].as<const char*>(), sizeof(preset->effects[i].EffectName));
    preset->effects[i].OnOff = fx["active"].as<bool>();
    preset->effects[i].NumParameters = fx["params"].size();
    auto paramsArray = fx["params"].as<JsonArray>();
    for (int l = 0; l < preset->effects[i].NumParameters; l++) {
      preset->effects[i].Parameters[l] = paramsArray[l]["value"].as<float>();
    }
    i++;
  }
  return true;
}


bool loadSparkPresetCustom(int genre, int index) {
  if (genre < 1 || genre > numGenres) {
    // Invalid genre
    DEBUG("Invalid genre!");
    return false;
  }
  
  String folderName = "/" + String(genreFolders[genre - 1]);
  
  if (!SD.exists(folderName)) {
    // Genre folder doesn't exist
    DEBUG("Genre folder :" +folderName+ "doesn't exist!");
    File root= SD.open("/");
    //printDirectory(root,0);
    return false;
  }
  
  File folder = SD.open(folderName);
  
  if (!folder.isDirectory()) {
    // Invalid folder
     DEBUG("Invalid folder!");
    folder.close();
    return false;
  }
  
  File presetFolder = folder.openNextFile();
  int count = 0;
  
  while (presetFolder) {
    if (presetFolder.isDirectory()) {
      if (count == index) {
        // Found the correct folder
        DEBUG("Found the correct folder:");
        DEBUG( (folderName+"/"+presetFolder.name()+"/preset.json").c_str());
        SparkPreset preset;

        if (!loadSparkPresetFromSD(&preset, (folderName+"/"+presetFolder.name()+"/preset.json").c_str())) {
            DEBUG("Failed to load preset!");
            return false;
        }
        change_custom_preset(&preset,3);
        presetFolder.close();
        folder.close();
        //debugOutput(&preset);
        return true;
      }
      count++;
    }
    
    presetFolder = folder.openNextFile();
  }
  
  // Couldn't find folder at specified index
  DEBUG("Couldn't find folder at specified index!");
  folder.close();
  return false;
}

void printDirectory(File dir, int numTabs) {
    while(true) {
    File entry = dir.openNextFile();

    if (!entry) {
      dir.rewindDirectory();
      break;
    }
    
    for (uint8_t i=0; i<numTabs; i++)   Serial.print('\t');
    Serial.print(entry.name());
        
    if (entry.isDirectory()) {
      DEBUG("/");
      printDirectory(entry, numTabs+1);
    } 
  else {
      Serial.print("\t\t");
      DEBUG(entry.size());
    }
    entry.close();
  }
}

void debugOutput(SparkPreset* preset) {
  DEBUG("Current Preset: " + String(preset->curr_preset));
  DEBUG("Total Presets: " + String(preset->preset_num));
  DEBUG("UUID: " + String(preset->UUID));
  DEBUG("Name: " + String(preset->Name));
  DEBUG("Version: " + String(preset->Version));
  DEBUG("Description: " + String(preset->Description));
  DEBUG("Icon: " + String(preset->Icon));
  DEBUG("BPM: " + String(preset->BPM));
  
  for (int i = 0; i < 7; i++) {
    DEBUG("Effect " + String(i+1) + " Name: " + String(preset->effects[i].EffectName));
    DEBUG("Effect " + String(i+1) + " On/Off: " + String(preset->effects[i].OnOff));
    DEBUG("Effect " + String(i+1) + " NumParameters: " + String(preset->effects[i].NumParameters));
    for (int j = 0; j < preset->effects[i].NumParameters; j++) {
      DEBUG("Effect " + String(i+1) + " Parameter " + String(j+1) + ": " + String(preset->effects[i].Parameters[j]));
    }
  }
  
  DEBUG("Checksum: " + String(preset->chksum));
}

