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
#if ENABLE_ICON_DISPLAY
  static char last_icon[STR_LEN] = "";
  static int last_preset = -1;
  static bool last_show_preset_name = false;
#else
  static int last_preset = -1;
#endif

  if (millis() - now > 50) { // only do this evey 50ms so we don't keep redrawing!
    now = millis();

#if ENABLE_ICON_DISPLAY
    // Display preset icon or name if it has changed
    #if defined M5CORE2 || defined M5CORE
    if (current_preset != last_preset || strcmp(current_preset_icon, last_icon) != 0 || show_preset_name != last_show_preset_name) {
      // Clear the entire display area for the icon/name
      M5.Lcd.fillRect(0, 0, 320, 64, TFT_BLACK);
      
      if (show_preset_name) {
        // Display preset name
        M5.Lcd.setTextColor(TFT_WHITE);
        M5.Lcd.setTextSize(2);
        M5.Lcd.setCursor(0, 0);
        M5.Lcd.print(presets[5].Name);
      } else {
        // Display the icon
        DEBUG("Displaying icon at position: x=0, y=0");
        display_preset_icon(current_preset_icon, 0, 0);
      }
      
      strncpy(last_icon, current_preset_icon, STR_LEN);
      last_preset = current_preset;
      last_show_preset_name = show_preset_name;
    }
    #elif defined M5STICK
    if (current_preset != last_preset || strcmp(current_preset_icon, last_icon) != 0 || show_preset_name != last_show_preset_name) {
      // Clear the entire display area for the icon/name
      M5.Lcd.fillRect(0, SER_MIDI_TEXT + 20, 80, 160 - (SER_MIDI_TEXT + 20), TFT_BLACK);
      
      if (show_preset_name) {
        // Display preset name
        M5.Lcd.setTextColor(TFT_WHITE);
        M5.Lcd.setTextSize(1);
        M5.Lcd.setCursor(0, SER_MIDI_TEXT + 20);
        M5.Lcd.print(presets[5].Name);
      } else {
        // Display the icon
        DEBUG("Displaying icon at position: " + String(SER_MIDI_TEXT + 20));
        display_preset_icon(current_preset_icon, 0, SER_MIDI_TEXT + 20);
      }
      
      strncpy(last_icon, current_preset_icon, STR_LEN);
      last_preset = current_preset;
      last_show_preset_name = show_preset_name;
    }
    #endif
#else
    // Always display preset name when icon display is disabled
    #if defined M5CORE2 || defined M5CORE
    if (current_preset != last_preset) {
      // Clear the preset name area
      M5.Lcd.fillRect(0, L_LAST_COMMAND_TEXT + 40, 64, 64, TFT_BLACK);
      
      // Display preset name
      M5.Lcd.setTextColor(TFT_WHITE);
      M5.Lcd.setTextSize(1);
      M5.Lcd.setCursor(0, 0);
      M5.Lcd.print(presets[5].Name);
      
      last_preset = current_preset;
    }
    #elif defined M5STICK
    if (current_preset != last_preset) {
      // Clear the preset name area
      M5.Lcd.fillRect(0, SER_MIDI_TEXT + 20, 80, 160 - (SER_MIDI_TEXT + 20), TFT_BLACK);
      
      // Display preset name
      M5.Lcd.setTextColor(TFT_WHITE);
      M5.Lcd.setTextSize(1);
      M5.Lcd.setCursor(0, SER_MIDI_TEXT + 20);
      M5.Lcd.print(presets[5].Name);
      
      last_preset = current_preset;
    }
    #endif
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

#if ENABLE_ICON_DISPLAY
void display_preset_icon(const char* icon_path, int x, int y) {
#if defined M5CORE2 || defined M5CORE
  DEBUG("=== Icon Display Debug ===");
  DEBUG("Attempting to display icon from path: ");
  DEBUG(icon_path);
  DEBUG("At position: x=" + String(x) + ", y=" + String(y));
  
  if (SD.exists(icon_path)) {
    DEBUG("Icon file exists, attempting to draw");
    File iconFile = SD.open(icon_path);
    if (iconFile) {
      DEBUG("Icon file opened successfully");
      size_t fileSize = iconFile.size();
      DEBUG("Icon file size: " + String(fileSize));
      iconFile.close();
      
      // Draw the PNG with explicit dimensions
      DEBUG("Attempting to draw PNG file...");
      M5.Lcd.drawPngFile(SD, icon_path, x, y, 64, 64);
      DEBUG("PNG draw completed");
    } else {
      DEBUG("Failed to open icon file");
      // Draw error placeholder
      M5.Lcd.fillRect(x, y, 64, 64, TFT_BLACK);
      M5.Lcd.drawRect(x, y, 64, 64, TFT_RED);
      M5.Lcd.setTextColor(TFT_RED);
      M5.Lcd.setTextSize(1);
      M5.Lcd.setCursor(x + 2, y + 2);
      M5.Lcd.print("Open Error");
    }
  } else {
    DEBUG("Icon file not found at path: " + String(icon_path));
    // Draw a placeholder if icon not found
    M5.Lcd.fillRect(x, y, 64, 64, TFT_BLACK);
    M5.Lcd.drawRect(x, y, 64, 64, TFT_WHITE);
    M5.Lcd.setTextColor(TFT_WHITE);
    M5.Lcd.setTextSize(1);
    M5.Lcd.setCursor(x + 2, y + 2);
    M5.Lcd.print("No Icon");
  }
  DEBUG("=== End Icon Display Debug ===");
#elif defined M5STICK
  DEBUG("=== Icon Display Debug ===");
  DEBUG("Attempting to display icon from path: ");
  DEBUG(icon_path);
  DEBUG("At position: x=" + String(x) + ", y=" + String(y));
  
  if (SD.exists(icon_path)) {
    DEBUG("Icon file exists, attempting to draw");
    File iconFile = SD.open(icon_path);
    if (iconFile) {
      DEBUG("Icon file opened successfully");
      size_t fileSize = iconFile.size();
      DEBUG("Icon file size: " + String(fileSize));
      iconFile.close();
      
      // Draw the PNG with explicit dimensions
      DEBUG("Attempting to draw PNG file...");
      M5.Lcd.drawPngFile(SD, icon_path, x, y, 32, 32);
      DEBUG("PNG draw completed");
    } else {
      DEBUG("Failed to open icon file");
      // Draw error placeholder
      M5.Lcd.fillRect(x, y, 32, 32, TFT_BLACK);
      M5.Lcd.drawRect(x, y, 32, 32, TFT_RED);
      M5.Lcd.setTextColor(TFT_RED);
      M5.Lcd.setTextSize(1);
      M5.Lcd.setCursor(x + 2, y + 2);
      M5.Lcd.print("Open Error");
    }
  } else {
    DEBUG("Icon file not found at path: " + String(icon_path));
    // Draw a placeholder if icon not found
    M5.Lcd.fillRect(x, y, 32, 32, TFT_BLACK);
    M5.Lcd.drawRect(x, y, 32, 32, TFT_WHITE);
    M5.Lcd.setTextColor(TFT_WHITE);
    M5.Lcd.setTextSize(1);
    M5.Lcd.setCursor(x + 2, y + 2);
    M5.Lcd.print("No Icon");
  }
  DEBUG("=== End Icon Display Debug ===");
#endif
}
#endif
