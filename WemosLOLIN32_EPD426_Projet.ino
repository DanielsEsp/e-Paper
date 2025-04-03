



//        mapping of Waveshare ESP32 Driver Board
//        BUSY -> 04, RST -> 16, DC -> 15, CS -> 05, CLK -> 14, DIN -> 13
//            Violet,     Blanc,     Vert,   Orange,     Jaune,      Bleu 
//        hspi.begin(14, 12, 13, 5);  // Wemos LOLIN32 
//        hspi.begin(14, 12, 13, 33); // ESP32

//        288 sequances de 5 minutes pour 24 heures, en rajouter 1 pour plus de surete
//        pour lecture de la date
//        compteur sequances a placer dans memoire preferences



#include <GxEPD2_BW.h>
#define   GxEPD2_DISPLAY_CLASS GxEPD2_BW
#define   USE_HSPI_FOR_EPD

//        Wemos_LOLIN32
#define   EPD_CS   05 
#define   EPD_DC   15
#define   EPD_RST  16
#define   EPD_BUSY 04 
// SCL/SCK/CLK     14
// SDA/DIN/MOSI    13

/*
//        ESP32
#define   EPD_CS   33  
#define   EPD_DC   27  
#define   EPD_RST  26  
#define   EPD_BUSY 25  
// SCL/SCK/CLK     14
// SDA/DIN/MOSI    13
*/

#define GxEPD2_DRIVER_CLASS GxEPD2_426_GDEQ0426T82 // GDEQ0426T82 480x800, SSD1677 (P426010-MF1-A)
#define GxEPD2_426_GDEQ0426T82_IS_BW true
        GxEPD2_BW<GxEPD2_426_GDEQ0426T82, GxEPD2_426_GDEQ0426T82::HEIGHT>epd(GxEPD2_426_GDEQ0426T82(EPD_CS, EPD_DC, EPD_RST, EPD_BUSY)); // GDEQ0426T82 480x800, SSD1677 (P426010-MF1-A)
        SPIClass hspi(HSPI);

#include <Adafruit_GFX.h>  
#include <U8g2_for_Adafruit_GFX.h>
          U8G2_FOR_ADAFRUIT_GFX affiche;


#include <WiFi.h>
          const char * ssid="Bbox-E295749E-2,4GHz";
          const char * wifipw="Mb&2427242711";
          int wifitime, esprestart;

#include <HTTPClient.h>
#include <ArduinoJson.h>

#include "time.h"
#include "time_routines.h"
     bool firstFlag = true;
     bool firstDailyFlag = true;
#include "bitmap_icones.h"
#include "icones_grille.h"
      int pointeur;
const unsigned char* icone;


//        Varialbles WEATHER DATAS LOAD
   String current = ("https://api.open-meteo.com/v1/forecast?latitude=48.477&longitude=7.6788&current=temperature_2m,relative_humidity_2m,apparent_temperature,is_day,precipitation,weather_code,surface_pressure,wind_speed_10m,wind_direction_10m,wind_gusts_10m&timezone=auto");    
   String alerte = ("https://api.weatherapi.com/v1/alerts.json?key=1110af7d6e0c4d21a99104458242011&q=ICHTRATZHEIM");      
   String url;
   String current_date;
   String last_weather_update;
   String temperature;
   String ressenti;
   String humidite;
   String pression;
   String vent_vitesse;
   String vent_rafale;
      int vent_direction, Direction;
   String vient_De; 
   String is_day;
     bool grande_icone = true;
   String weather_code;
   String weather_code_txt;
      int meteoCall = (5*60)*1000; //       pour 5 minutes en microsecondes
      int lastMeteoCall;
      int pressionCall = (120*60)*1000; //  pour 2 heures en microsecondes
      int lastPressionCall;
      int alerteCall = (30*60)*1000; //     pour 30 minutes en microsecondes
      int lastAlerteCall;
      int dailyCall = (120*60)*1000; //       pour 2 heures en microsecondes
      int lastDailyCall;

//         Varialbles DAILY DATAS LOAD
    String daily = ("https://api.open-meteo.com/v1/forecast?latitude=48.477&longitude=7.6788&daily=weather_code,temperature_2m_max,temperature_2m_min,apparent_temperature_max,apparent_temperature_min,wind_speed_10m_max,wind_gusts_10m_max,wind_direction_10m_dominant&forecast_days=5&models=icon_seamless");
       int daily_weather_code_0, daily_weather_code_1, daily_weather_code_2, daily_weather_code_3;
     float daily_temperature_2m_max_0, daily_temperature_2m_max_1, daily_temperature_2m_max_2, daily_temperature_2m_max_3;
     float daily_temperature_2m_min_0, daily_temperature_2m_min_1, daily_temperature_2m_min_2, daily_temperature_2m_min_3;
     float daily_apparent_temperature_max_0, daily_apparent_temperature_max_1, daily_apparent_temperature_max_2, daily_apparent_temperature_max_3;
     float daily_apparent_temperature_min_0, daily_apparent_temperature_min_1, daily_apparent_temperature_min_2, daily_apparent_temperature_min_3;
    String temp_max_Txt_0, temp_max_Txt_1, temp_max_Txt_2, temp_max_Txt_3;
    String temp_min_Txt_0, temp_min_Txt_1, temp_min_Txt_2, temp_min_Txt_3;    
    String ressenti_max_Txt_0, ressenti_max_Txt_1, ressenti_max_Txt_2, ressenti_max_Txt_3;
    String ressenti_min_Txt_0, ressenti_min_Txt_1, ressenti_min_Txt_2, ressenti_min_Txt_3;
   
    String oldWeatherCode, oldTemperatureFormatTxt, oldHumiditeFormatTxt, oldPressionFormatTxt, oldPressionDisplay, oldVentVitesse;
     float temperatureFormat, temperatureFloat, ressentiFormat, pressionFormat;
       int humiditeFormat;
    String temperatureFormatTxt, ressentiFormatTxt, humiditeFormatTxt, pressionFormatTxt;
    String description;
const char degree_symbol[] = "\u00B0C";

    String siAlerte;
       int numeroAlerte, nombreAlertes;
    String evenementAlerte[7];



void startWifi() {

     WiFi.begin(ssid, wifipw);
     Serial.println("Connecting Wifi");
     while (WiFi.status() != WL_CONNECTED) {
            wifitime ++;
            Serial.print("."); delay(100);
            if (wifitime > 20) { ESP.restart(); delay(50); }
           }
     Serial.println();
     Serial.print("Local IP  "); Serial.print(WiFi.localIP()); Serial.println();
     Serial.print("Wifi RSSI "); Serial.println(WiFi.RSSI()); Serial.println();
     
}



void setup() {

     Serial.begin(115200); Serial.println();

     startWifi();
     initTime("CET-1CEST,M3.5.0,M10.5.0/3");   // Set for Berlin
     WiFi.disconnect(); delay(50);    
     
     hspi.begin(14, 12, 13, 05); // Wemos LOLIN32
//     hspi.begin(14, 12, 13, 33); // ESP32

     epd.epd2.selectSPI(hspi, SPISettings(4000000, MSBFIRST, SPI_MODE0));

     epd.init(115200, true, 50, false);
     epd.setRotation(2); 
     epd.fillScreen(GxEPD_BLACK);
     epd.setTextColor(GxEPD_WHITE);
     
     affiche.begin(epd); // connect u8g2 procedures to Adafruit GFX
     affiche.setBackgroundColor(GxEPD_BLACK);  
     affiche.setForegroundColor(GxEPD_WHITE);
     affiche.setFont(u8g2_font_fub35_tr);

}


void loop() {

     printLocalTime();
          
     if (oldDateTxt != dateTxt || firstFlag == true) {         
         oldDateTxt = dateTxt;
Serial.println(dateTxt); delay(50); 
         epd.drawPaged(drawFull, 0);//               Grille de base
         current_datas_load(); delay(50); 
         alert_datas_load(); delay(50);        
         if (firstFlag == true) { oldPressionDisplay =  pressionFormatTxt; firstFlag = false; }            
         epd.drawPaged(drawPartial2, 0);//           Affiche icone
         epd.drawPaged(drawPartial3, 0);//           Affiche description
         epd.drawPaged(drawPartial5, 0);//           Affiche vent datas
         epd.drawPaged(drawPartial4, 0);//           Affiche temp. datas           
         epd.drawPaged(drawPartial1, 0);//           Affiche alertes 
        }

     if ((millis() - lastMeteoCall) > meteoCall) {
         lastMeteoCall = millis();         
         current_datas_load(); delay(50);               
         if (weather_code != oldWeatherCode) {
             oldWeatherCode = weather_code;
             epd.drawPaged(drawPartial2, 0);//           Affiche icone              
             epd.drawPaged(drawPartial3, 0);//           Affiche description
            }
         if (temperatureFormatTxt != oldTemperatureFormatTxt) {
             oldTemperatureFormatTxt = temperatureFormatTxt;
             epd.drawPaged(drawPartial4, 0);//           Affiche temp. datas              
            }
         if (humiditeFormatTxt != oldHumiditeFormatTxt) {
             oldHumiditeFormatTxt = humiditeFormatTxt;
             epd.drawPaged(drawPartial4, 0);//           Affiche temp. datas              
            }
         if (vent_vitesse != oldVentVitesse) {
             oldVentVitesse = vent_vitesse;
             epd.drawPaged(drawPartial5, 0);//           Affiche vent ou alerte datas 
            }
        }         
         
     if ((millis() - lastPressionCall) > pressionCall) {
         lastPressionCall = millis();           
         current_datas_load(); delay(50);                     
         oldPressionDisplay = oldPressionFormatTxt;
         oldPressionFormatTxt = pressionFormatTxt;
         epd.drawPaged(drawPartial4, 0);//           Affiche temp. datas                      
        }

     if ((millis() - lastAlerteCall) > alerteCall) {
         lastAlerteCall = millis();           
         alert_datas_load(); delay(50);
         epd.drawPaged(drawPartial1, 0);//           Affiche vent ou alerte datas 
        }


     if (((millis() - lastDailyCall) > dailyCall) || firstDailyFlag == true)  {
         lastDailyCall = millis(); firstDailyFlag = false;               
         daily_datas_load();     
         epd.drawPaged(drawPartial6, 0);
        }



     delay(50);
     epd.hibernate();
     delay(50);
     epd.powerOff();  
     delay(50);

Serial.println("Passage time sleep"); delay(50);
     esp_sleep_enable_timer_wakeup(5*60*1000000);
     esp_light_sleep_start();
     delay(50);

}



void drawFull(const void* pv) { //                                   Grille de base
  
     epd.setFullWindow();
     epd.fillRoundRect(5, 0, 780, 60, 10, GxEPD_BLACK);
     epd.setTextColor(GxEPD_WHITE);
     affiche.setFont(u8g2_font_fub35_tr);
     affiche.setCursor(165, 50);
     affiche.print(dateTxt);
     epd.drawRoundRect(5, 65, 150, 130, 10, GxEPD_BLACK); //         Grille icone
     epd.drawRoundRect(160, 65, 625, 35, 10, GxEPD_BLACK); //        Grille description
     epd.drawRoundRect(160, 105, 625, 90, 10, GxEPD_BLACK); //       Grille temp. datas
     epd.drawRoundRect(5, 200, 780, 70, 10, GxEPD_BLACK); //         Grille bas
     delay(50);
   
}


void drawPartial1(const void* pv) { //            Affiche alerte
 
     epd.setPartialWindow(0, 271, 790, 26);
     if (siAlerte != "[]") { 
         epd.fillRect(5, 272, 780, 26, GxEPD_BLACK);
         affiche.setBackgroundColor(GxEPD_BLACK);  
         affiche.setForegroundColor(GxEPD_WHITE);
         affiche.setFont(u8g2_font_profont22_mf);
         affiche.setCursor(5, 292);
         affiche.println(evenementAlerte[1]);
//         affiche.println("  " + evenementAlerte[2]);     
        } else {   
         epd.fillRect(0, 272, 790, 26, GxEPD_WHITE);
        }
     delay(50);
}


void drawPartial2(const void* pv) { //           Affiche icone
  
     epd.setPartialWindow(10, 70, 140, 120);
     epd.fillRect(10, 70, 140, 120, GxEPD_WHITE);
     epd.drawBitmap(10, 70, icone_name[pointeur], 140, 120, GxEPD_BLACK);
     delay(50);
 
}


void drawPartial3(const void* pv) { //           Affiche description
  
     epd.setPartialWindow(170, 70, 500, 25);
     epd.fillRect(170, 70, 600, 25, GxEPD_WHITE);
     affiche.setBackgroundColor(GxEPD_WHITE);  
     affiche.setForegroundColor(GxEPD_BLACK);
     affiche.setFont(u8g2_font_profont29_mf);
     affiche.setCursor(175, 90);
     affiche.print(data_description[pointeur]);
     delay(50);
  
}


void drawPartial4(const void* pv) { //           Affiche temp. datas
     
     epd.setPartialWindow(170, 110, 610, 80);
     epd.fillRect(170, 110, 610, 80, GxEPD_WHITE);
     affiche.setBackgroundColor(GxEPD_WHITE);  
     affiche.setForegroundColor(GxEPD_BLACK);
     affiche.setFont(u8g2_font_10x20_mf);
     affiche.setCursor(195, 125);
     affiche.print("Température");    
     affiche.setCursor(360, 125);
     affiche.print("Ressenti");    
     affiche.setCursor(505, 125);
     affiche.print("Humidité");    
     affiche.setCursor(665, 125);
     affiche.print("Pression");    

     affiche.setFont(u8g2_font_logisoso46_tf);
     affiche.setCursor(170, 185);
     affiche.print(temperatureFormatTxt + "\u00B0");     
     affiche.setCursor(320, 185);
     affiche.print(ressentiFormatTxt + "\u00B0");     
     affiche.setCursor(485, 185);
     affiche.print(humiditeFormatTxt);    
     affiche.setFont(u8g2_font_profont29_mf);
     affiche.setCursor(580, 155);
     affiche.print("%");
     affiche.setFont(u8g2_font_logisoso24_tf); 
     affiche.setCursor(635, 155);
     affiche.print("Act." + pressionFormatTxt);    
     affiche.setCursor(635, 185);   
     affiche.print("Old." + oldPressionDisplay);    
     delay(50);
                  
}


void drawPartial5(const void* pv) { //            Affiche vent datas
     
//       Affiche vent
         epd.setPartialWindow(10, 205, 770, 60);
         epd.fillRect(15, 205, 755, 60, GxEPD_WHITE);
         affiche.setBackgroundColor(GxEPD_WHITE);  
         affiche.setForegroundColor(GxEPD_BLACK);
         affiche.setFont(u8g2_font_profont29_mf);
         affiche.setCursor(15, 230);
         affiche.print("Vitesse actuelle du vent "); affiche.print(vent_vitesse); affiche.print(" km/h "); affiche.println(vient_De); 
         affiche.print(" avec des rafales de ");affiche.print(vent_rafale); affiche.print(" km/h");
//       Affiche alerte s'il y a lieu
//         epd.drawPaged(drawPartial1, 0);
        
}


void drawPartial6(const void* pv) {      

     epd.setPartialWindow(10, 299, 770, 190);
     epd.fillRoundRect(10, 300, 250, 40, 10, GxEPD_BLACK);
     affiche.setBackgroundColor(GxEPD_BLACK);  
     affiche.setForegroundColor(GxEPD_WHITE);
     affiche.setFont(u8g2_font_profont29_mf);
     affiche.setCursor(60,330);
     affiche.print("Aujourdhui");
     epd.drawRoundRect(10, 345, 150, 130, 10, GxEPD_BLACK); //           Grille icone
     weather_code = String(daily_weather_code_0);                             
     for (int boucle = 0; boucle <= 34; boucle ++) {
          pointeur = boucle;
          if (weather_code == icone_datas[pointeur]) {
              if (weather_code == "0") { 
                  if (is_day == "1") { ; } else { pointeur = pointeur + 1; }
                 }
              if (weather_code == "2") { 
                  if (is_day == "1") { ; } else { pointeur = pointeur + 1; }
                 }
              if (weather_code == "61") { 
                  if (is_day == "1") { ; } else { pointeur = pointeur + 1; }
                 }
              if (weather_code == "80") { 
                  if (is_day == "1") { ; } else { pointeur = pointeur + 1; }
                 }
               boucle = 35;
              }                                                                                               
         }                           
     epd.drawBitmap(15, 350, icone_name[pointeur], 140, 120, GxEPD_BLACK);

     epd.drawRoundRect(165, 345, 95, 130, 10, GxEPD_BLACK);
     affiche.setBackgroundColor(GxEPD_WHITE);  
     affiche.setForegroundColor(GxEPD_BLACK);
     affiche.setFont(u8g2_font_profont12_mf);
     affiche.setCursor(177,365);
     affiche.print("Temp max/min");     
     affiche.setFont(u8g2_font_profont22_mf);
     affiche.setCursor(175,385);
     affiche.print(temp_max_Txt_0); affiche.print("\u00B0");
     affiche.setCursor(175,405);
     affiche.print(temp_min_Txt_0); affiche.print("\u00B0");
     affiche.setFont(u8g2_font_profont12_mf);
     affiche.setCursor(177,425);
     affiche.print("Ress max/min");          
     affiche.setFont(u8g2_font_profont22_mf);
     affiche.setCursor(175,445);
     affiche.print(ressenti_max_Txt_0); affiche.print("\u00B0");
     affiche.setCursor(175,465);
     affiche.print(ressenti_min_Txt_0); affiche.print("\u00B0");


     epd.fillRoundRect(270, 300, 250, 40, 10, GxEPD_BLACK);
     affiche.setBackgroundColor(GxEPD_BLACK);  
     affiche.setForegroundColor(GxEPD_WHITE);
     affiche.setFont(u8g2_font_profont29_mf);
     affiche.setCursor(340,330);
     affiche.print("Demain");
     epd.drawRoundRect(270, 345, 150, 130, 10, GxEPD_BLACK);
     is_day = "1"; //  pour effacer is_day current 
     weather_code = String(daily_weather_code_1);                             
     for (int boucle = 0; boucle <= 34; boucle ++) {
          pointeur = boucle;
          if (weather_code == icone_datas[pointeur]) {
              if (weather_code == "0") { 
                  if (is_day == "1") { ; } else { pointeur = pointeur + 1; }
                 }
              if (weather_code == "2") { 
                  if (is_day == "1") { ; } else { pointeur = pointeur + 1; }
                 }
              if (weather_code == "61") { 
                  if (is_day == "1") { ; } else { pointeur = pointeur + 1; }
                 }
              if (weather_code == "80") { 
                  if (is_day == "1") { ; } else { pointeur = pointeur + 1; }
                 }
               boucle = 35;
              }                                                                                               
         }                           
     epd.drawBitmap(275, 350, icone_name[pointeur], 140, 120, GxEPD_BLACK);

     epd.drawRoundRect(425, 345, 95, 130, 10, GxEPD_BLACK);
     affiche.setBackgroundColor(GxEPD_WHITE);  
     affiche.setForegroundColor(GxEPD_BLACK);
     affiche.setFont(u8g2_font_profont12_mf);
     affiche.setCursor(437,365);
     affiche.print("Temp max/min");     
     affiche.setFont(u8g2_font_profont22_mf);
     affiche.setCursor(435,385);
     affiche.print(temp_max_Txt_1); affiche.print("\u00B0");
     affiche.setCursor(435,405);
     affiche.print(temp_min_Txt_1); affiche.print("\u00B0");
     affiche.setFont(u8g2_font_profont12_mf);
     affiche.setCursor(437,425);
     affiche.print("Ress max/min");          
     affiche.setFont(u8g2_font_profont22_mf);
     affiche.setCursor(435,445);
     affiche.print(ressenti_max_Txt_1); affiche.print("\u00B0");
     affiche.setCursor(435,465);
     affiche.print(ressenti_min_Txt_1); affiche.print("\u00B0");


     epd.fillRoundRect(530, 300, 250, 40, 10, GxEPD_BLACK);
     affiche.setBackgroundColor(GxEPD_BLACK);  
     affiche.setForegroundColor(GxEPD_WHITE);
     affiche.setFont(u8g2_font_profont29_mf);
     affiche.setCursor(563,330);
     affiche.print("Après demain");
     epd.drawRoundRect(530, 345, 150, 130, 10, GxEPD_BLACK);
     weather_code = String(daily_weather_code_2);                             
     for (int boucle = 0; boucle <= 34; boucle ++) {
          pointeur = boucle;
          if (weather_code == icone_datas[pointeur]) {
              if (weather_code == "0") { 
                  if (is_day == "1") { ; } else { pointeur = pointeur + 1; }
                 }
              if (weather_code == "2") { 
                  if (is_day == "1") { ; } else { pointeur = pointeur + 1; }
                 }
              if (weather_code == "61") { 
                  if (is_day == "1") { ; } else { pointeur = pointeur + 1; }
                 }
              if (weather_code == "80") { 
                  if (is_day == "1") { ; } else { pointeur = pointeur + 1; }
                 }
               boucle = 35;
              }                                                                                               
         }                           
     epd.drawBitmap(535, 350, icone_name[pointeur], 140, 120, GxEPD_BLACK);

     epd.drawRoundRect(685, 345, 95, 130, 10, GxEPD_BLACK);
     affiche.setBackgroundColor(GxEPD_WHITE);  
     affiche.setForegroundColor(GxEPD_BLACK);
     affiche.setFont(u8g2_font_profont12_mf);
     affiche.setCursor(697,365);
     affiche.print("Temp max/min");     
     affiche.setFont(u8g2_font_profont22_mf);
     affiche.setCursor(695,385);
     affiche.print(temp_max_Txt_2); affiche.print("\u00B0");
     affiche.setCursor(695,405);
     affiche.print(temp_min_Txt_2); affiche.print("\u00B0");
     affiche.setFont(u8g2_font_profont12_mf);
     affiche.setCursor(697,425);
     affiche.print("Ress max/min");          
     affiche.setFont(u8g2_font_profont22_mf);
     affiche.setCursor(695,445);
     affiche.print(ressenti_max_Txt_2); affiche.print("\u00B0");
     affiche.setCursor(695,465);
     affiche.print(ressenti_min_Txt_2); affiche.print("\u00B0");

}


//
