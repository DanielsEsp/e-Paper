



void daily_datas_load() {
  
     WiFi.disconnect(); delay(50);
     Serial.println("Connect Current"); delay(100);
     WiFi.begin(ssid, wifipw);
     Serial.println("Connecting Wifi");
     while (WiFi.status() != WL_CONNECTED) {
            wifitime ++;
            Serial.print("."); delay(100);
            if (wifitime > 20) { WiFi.disconnect(); wifitime = 0; esprestart ++; Serial.println("Exit--->"); daily_datas_load(); }
            if (esprestart >= 5) { WiFi.disconnect(); Serial.println("Restart--->"); delay(500); ESP.restart(); delay(50); }
           }
     Serial.println();
     Serial.print("Local IP "); Serial.print(WiFi.localIP()); Serial.println();
     Serial.print("Wifi RSSI "); Serial.println(WiFi.RSSI()); Serial.println();                  
     delay(1000);

     Serial.println("Get Current"); delay(100);
     url = daily; 
     if (WiFi.status() == WL_CONNECTED) {
         HTTPClient http; 
         http.begin(url);
         int httpCode = http.GET(); 
         Serial.println(httpCode);
         
         if (httpCode > 0) {
             if (httpCode == HTTP_CODE_OK) {
                 String payload = http.getString();
                 Serial.println("Request information:");
                 Serial.println(payload); Serial.println();
                 StaticJsonDocument<1024> doc;
                 DeserializationError error = deserializeJson(doc, payload);
                 if (!error) {

float latitude = doc["latitude"]; 
float longitude = doc["longitude"];
double generationtime_ms = doc["generationtime_ms"];
int utc_offset_seconds = doc["utc_offset_seconds"];
const char* timezone = doc["timezone"]; // "Europe/London"
const char* timezone_abbreviation = doc["timezone_abbreviation"];
int elevation = doc["elevation"];

JsonObject daily_units = doc["daily_units"];
const char* daily_units_time = daily_units["time"];
const char* daily_units_weather_code = daily_units["weather_code"];
const char* daily_units_temperature_2m_max = daily_units["temperature_2m_max"]; 
const char* daily_units_temperature_2m_min = daily_units["temperature_2m_min"];
const char* daily_units_apparent_temperature_max = daily_units["apparent_temperature_max"];
const char* daily_units_apparent_temperature_min = daily_units["apparent_temperature_min"];
const char* daily_units_wind_speed_10m_max = daily_units["wind_speed_10m_max"];
const char* daily_units_wind_direction_10m_dominant = daily_units["wind_direction_10m_dominant"];

JsonObject daily = doc["daily"];

JsonArray daily_time = daily["time"];
const char* daily_time_0 = daily_time[0];
const char* daily_time_1 = daily_time[1];
const char* daily_time_2 = daily_time[2];
const char* daily_time_3 = daily_time[3]; 

Serial.println("Weather code");
JsonArray daily_weather_code = daily["weather_code"];
 daily_weather_code_0 = daily_weather_code[0];
Serial.print(daily_time_0); Serial.print(" -> "); Serial.println(daily_weather_code_0);
 daily_weather_code_1 = daily_weather_code[1]; // 3
Serial.print(daily_time_1); Serial.print(" -> "); Serial.println(daily_weather_code_1);
 daily_weather_code_2 = daily_weather_code[2]; // 3
Serial.print(daily_time_2); Serial.print(" -> "); Serial.println(daily_weather_code_2);
 daily_weather_code_3 = daily_weather_code[3]; // 45
Serial.print(daily_time_3); Serial.print(" -> "); Serial.println(daily_weather_code_3);

Serial.println("Temperature maxi");
JsonArray daily_temperature_2m_max = daily["temperature_2m_max"];
 daily_temperature_2m_max_0 = daily_temperature_2m_max[0]; // 7.8
Serial.print(daily_time_0); Serial.print(" -> "); Serial.print(daily_temperature_2m_max_0);
                     temp_max_Txt_0 = " " + String(daily_temperature_2m_max_0, 1); 
                     if (daily_temperature_2m_max_0 < 10) { temp_max_Txt_0 = "  " + String(daily_temperature_2m_max_0, 1); }
                     if (daily_temperature_2m_max_0 < 0) { temp_max_Txt_0 = " " + String(daily_temperature_2m_max_0, 1); }
                     if (daily_temperature_2m_max_0 < -10) { temp_max_Txt_0 = String(daily_temperature_2m_max_0, 1); }
                     Serial.print(" = "); Serial.println(temp_max_Txt_0);
 daily_temperature_2m_max_1 = daily_temperature_2m_max[1]; // 7.5
Serial.print(daily_time_1); Serial.print(" -> "); Serial.print(daily_temperature_2m_max_1);
                     temp_max_Txt_1 = " " + String(daily_temperature_2m_max_1, 1); 
                     if (daily_temperature_2m_max_1 < 10) { temp_max_Txt_1 = "  " + String(daily_temperature_2m_max_1, 1); }
                     if (daily_temperature_2m_max_1 < 0) { temp_max_Txt_1 = " " + String(daily_temperature_2m_max_1, 1); }
                     if (daily_temperature_2m_max_1 < -10) { temp_max_Txt_1 = String(daily_temperature_2m_max_1, 1); }
                     Serial.print(" = "); Serial.println(temp_max_Txt_1);    
 daily_temperature_2m_max_2 = daily_temperature_2m_max[2]; // 9
Serial.print(daily_time_2); Serial.print(" -> "); Serial.print(daily_temperature_2m_max_2);
                     temp_max_Txt_2 = " " + String(daily_temperature_2m_max_2, 1); 
                     if (daily_temperature_2m_max_2 < 10) { temp_max_Txt_2 = "  " + String(daily_temperature_2m_max_2, 1); }
                     if (daily_temperature_2m_max_2 < 0) { temp_max_Txt_2 = " " + String(daily_temperature_2m_max_2, 1); }
                     if (daily_temperature_2m_max_2 < -10) { temp_max_Txt_2 = String(daily_temperature_2m_max_2, 1); }
                     Serial.print(" = "); Serial.println(temp_max_Txt_2);    
 daily_temperature_2m_max_3 = daily_temperature_2m_max[3]; // 9.7
Serial.print(daily_time_3); Serial.print(" -> "); Serial.print(daily_temperature_2m_max_3);
                     temp_max_Txt_3 = " " + String(daily_temperature_2m_max_3, 1); 
                     if (daily_temperature_2m_max_3 < 10) { temp_max_Txt_3 = "  " + String(daily_temperature_2m_max_3, 1); }
                     if (daily_temperature_2m_max_3 < 0) { temp_max_Txt_3 = " " + String(daily_temperature_2m_max_3, 1); }
                     if (daily_temperature_2m_max_3 < -10) { temp_max_Txt_3 = String(daily_temperature_2m_max_3, 1); }
                     Serial.print(" = "); Serial.println(temp_max_Txt_3);    

Serial.println("Temperature mini");
JsonArray daily_temperature_2m_min = daily["temperature_2m_min"];
 daily_temperature_2m_min_0 = daily_temperature_2m_min[0]; // 1.5
Serial.print(daily_time_0); Serial.print(" -> "); Serial.print(daily_temperature_2m_min_0);
                     temp_min_Txt_0 = " " + String(daily_temperature_2m_min_0, 1); 
                     if (daily_temperature_2m_min_0 < 10) { temp_min_Txt_0 = "  " + String(daily_temperature_2m_min_0, 1); }
                     if (daily_temperature_2m_min_0 < 0) { temp_min_Txt_0 = " " + String(daily_temperature_2m_min_0, 1); }
                     if (daily_temperature_2m_min_0 < -10) { temp_min_Txt_0 = String(daily_temperature_2m_min_0, 1); }
                     Serial.print(" = "); Serial.println(temp_min_Txt_0);
 daily_temperature_2m_min_1 = daily_temperature_2m_min[1]; // 0.9
Serial.print(daily_time_1); Serial.print(" -> "); Serial.print(daily_temperature_2m_min_1);
                     temp_min_Txt_1 = " " + String(daily_temperature_2m_min_1, 1); 
                     if (daily_temperature_2m_min_1 < 10) { temp_min_Txt_1 = "  " + String(daily_temperature_2m_min_1, 1); }
                     if (daily_temperature_2m_min_1 < 0) { temp_min_Txt_1 = " " + String(daily_temperature_2m_min_1, 1); }
                     if (daily_temperature_2m_min_1 < -10) { temp_min_Txt_1 = String(daily_temperature_2m_min_1, 1); }
                     Serial.print(" = "); Serial.println(temp_min_Txt_1);
 daily_temperature_2m_min_2 = daily_temperature_2m_min[2]; // 0
Serial.print(daily_time_2); Serial.print(" -> "); Serial.print(daily_temperature_2m_min_2);
                     temp_min_Txt_2 = " " + String(daily_temperature_2m_min_2, 1); 
                     if (daily_temperature_2m_min_2 < 10) { temp_min_Txt_2 = "  " + String(daily_temperature_2m_min_2, 1); }
                     if (daily_temperature_2m_min_2 < 0) { temp_min_Txt_2 = " " + String(daily_temperature_2m_min_2, 1); }
                     if (daily_temperature_2m_min_2 < -10) { temp_min_Txt_2 = String(daily_temperature_2m_min_2, 1); }
                     Serial.print(" = "); Serial.println(temp_min_Txt_2);
 daily_temperature_2m_min_3 = daily_temperature_2m_min[3]; // -0.6
Serial.print(daily_time_3); Serial.print(" -> "); Serial.print(daily_temperature_2m_min_3);
                     temp_min_Txt_3 = " " + String(daily_temperature_2m_min_3, 1); 
                     if (daily_temperature_2m_min_3 < 10) { temp_min_Txt_3 = "  " + String(daily_temperature_2m_min_3, 1); }
                     if (daily_temperature_2m_min_3 < 0) { temp_min_Txt_3 = " " + String(daily_temperature_2m_min_3, 1); }
                     if (daily_temperature_2m_min_3 < -10) { temp_min_Txt_3 = String(daily_temperature_2m_min_3, 1); }
                     Serial.print(" = "); Serial.println(temp_min_Txt_3);

Serial.println("Ressenti maxi");
JsonArray daily_apparent_temperature_max = daily["apparent_temperature_max"];
 daily_apparent_temperature_max_0 = daily_apparent_temperature_max[0]; // 4.2
Serial.print(daily_time_0); Serial.print(" -> "); Serial.print(daily_apparent_temperature_max_0);
                     ressenti_max_Txt_0 = " " + String(daily_apparent_temperature_max_0, 1); 
                     if (daily_apparent_temperature_max_0 < 10) { ressenti_max_Txt_0 = "  " + String(daily_apparent_temperature_max_0, 1); }
                     if (daily_apparent_temperature_max_0 < 0) { ressenti_max_Txt_0 = " " + String(daily_apparent_temperature_max_0, 1); }
                     if (daily_apparent_temperature_max_0 < -10) { ressenti_max_Txt_0 = String(daily_apparent_temperature_max_0, 1); }
                     Serial.print(" = "); Serial.println(ressenti_max_Txt_0);
 daily_apparent_temperature_max_1 = daily_apparent_temperature_max[1]; // 2.2
Serial.print(daily_time_1); Serial.print(" -> "); Serial.print(daily_apparent_temperature_max_1);
                     ressenti_max_Txt_1 = " " + String(daily_apparent_temperature_max_1, 1); 
                     if (daily_apparent_temperature_max_1 < 10) { ressenti_max_Txt_1 = "  " + String(daily_apparent_temperature_max_1, 1); }
                     if (daily_apparent_temperature_max_1 < 0) { ressenti_max_Txt_1 = " " + String(daily_apparent_temperature_max_1, 1); }
                     if (daily_apparent_temperature_max_1 < -10) { ressenti_max_Txt_1 = String(daily_apparent_temperature_max_1, 1); }
                     Serial.print(" = "); Serial.println(ressenti_max_Txt_1);
 daily_apparent_temperature_max_2 = daily_apparent_temperature_max[2]; // 4.8
Serial.print(daily_time_2); Serial.print(" -> "); Serial.print(daily_apparent_temperature_max_2);
                     ressenti_max_Txt_2 = " " + String(daily_apparent_temperature_max_2, 1); 
                     if (daily_apparent_temperature_max_2 < 10) { ressenti_max_Txt_2 = "  " + String(daily_apparent_temperature_max_2, 1); }
                     if (daily_apparent_temperature_max_2 < 0) { ressenti_max_Txt_2 = " " + String(daily_apparent_temperature_max_2, 1); }
                     if (daily_apparent_temperature_max_2 < -10) { ressenti_max_Txt_2 = String(daily_apparent_temperature_max_2, 1); }
                     Serial.print(" = "); Serial.println(ressenti_max_Txt_2);
 daily_apparent_temperature_max_3 = daily_apparent_temperature_max[3]; // 6.3
Serial.print(daily_time_3); Serial.print(" -> "); Serial.print(daily_apparent_temperature_max_3);
                     ressenti_max_Txt_3 = " " + String(daily_apparent_temperature_max_3, 1); 
                     if (daily_apparent_temperature_max_3 < 10) { ressenti_max_Txt_3 = "  " + String(daily_apparent_temperature_max_3, 1); }
                     if (daily_apparent_temperature_max_3 < 0) { ressenti_max_Txt_3 = " " + String(daily_apparent_temperature_max_3, 1); }
                     if (daily_apparent_temperature_max_3 < -10) { ressenti_max_Txt_3 = String(daily_apparent_temperature_max_3, 1); }
                     Serial.print(" = "); Serial.println(ressenti_max_Txt_3);

Serial.println("Ressenti mini");
JsonArray daily_apparent_temperature_min = daily["apparent_temperature_min"];
 daily_apparent_temperature_min_0 = daily_apparent_temperature_min[0]; // -2.1
Serial.print(daily_time_0); Serial.print(" -> "); Serial.print(daily_apparent_temperature_min_0);
                     ressenti_min_Txt_0 = " " + String(daily_apparent_temperature_min_0, 1); 
                     if (daily_apparent_temperature_min_0 < 10) { ressenti_min_Txt_0 = "  " + String(daily_apparent_temperature_min_0, 1); }
                     if (daily_apparent_temperature_min_0 < 0) { ressenti_min_Txt_0 = " " + String(daily_apparent_temperature_min_0, 1); }
                     if (daily_apparent_temperature_min_0 < -10) { ressenti_min_Txt_0 = String(daily_apparent_temperature_min_0, 1); }
                     Serial.print(" = "); Serial.println(ressenti_min_Txt_0);
 daily_apparent_temperature_min_1 = daily_apparent_temperature_min[1]; // -3.2
Serial.print(daily_time_1); Serial.print(" -> "); Serial.print(daily_apparent_temperature_min_1);
                     ressenti_min_Txt_1 = " " + String(daily_apparent_temperature_min_1, 1); 
                     if (daily_apparent_temperature_min_1 < 10) { ressenti_min_Txt_1 = "  " + String(daily_apparent_temperature_min_1, 1); }
                     if (daily_apparent_temperature_min_1 < 0) { ressenti_min_Txt_1 = " " + String(daily_apparent_temperature_min_1, 1); }
                     if (daily_apparent_temperature_min_1 < -10) { ressenti_min_Txt_1 = String(daily_apparent_temperature_min_1, 1); }
                     Serial.print(" = "); Serial.println(ressenti_min_Txt_1);
 daily_apparent_temperature_min_2 = daily_apparent_temperature_min[2]; // -3.9
Serial.print(daily_time_2); Serial.print(" -> "); Serial.print(daily_apparent_temperature_min_2);
                     ressenti_min_Txt_2 = " " + String(daily_apparent_temperature_min_2, 1); 
                     if (daily_apparent_temperature_min_2 < 10) { ressenti_min_Txt_2 = "  " + String(daily_apparent_temperature_min_2, 1); }
                     if (daily_apparent_temperature_min_2 < 0) { ressenti_min_Txt_2 = " " + String(daily_apparent_temperature_min_2, 1); }
                     if (daily_apparent_temperature_min_2 < -10) { ressenti_min_Txt_2 = String(daily_apparent_temperature_min_2, 1); }
                     Serial.print(" = "); Serial.println(ressenti_min_Txt_2);
 daily_apparent_temperature_min_3 = daily_apparent_temperature_min[3]; // -3.7
Serial.print(daily_time_3); Serial.print(" -> "); Serial.print(daily_apparent_temperature_min_3);
                     ressenti_min_Txt_3 = " " + String(daily_apparent_temperature_min_3, 1); 
                     if (daily_apparent_temperature_min_3 < 10) { ressenti_min_Txt_3 = "  " + String(daily_apparent_temperature_min_3, 1); }
                     if (daily_apparent_temperature_min_3 < 0) { ressenti_min_Txt_3 = " " + String(daily_apparent_temperature_min_3, 1); }
                     if (daily_apparent_temperature_min_3 < -10) { ressenti_min_Txt_3 = String(daily_apparent_temperature_min_3, 1); }
                     Serial.print(" = "); Serial.println(ressenti_min_Txt_3);

Serial.println("Vent vitesse");
JsonArray daily_wind_speed_10m_max = daily["wind_speed_10m_max"];
float daily_wind_speed_10m_max_0 = daily_wind_speed_10m_max[0]; // 13.9
Serial.print(daily_time_0); Serial.print(" -> "); Serial.println(daily_wind_speed_10m_max_0);
float daily_wind_speed_10m_max_1 = daily_wind_speed_10m_max[1]; // 22.6
Serial.print(daily_time_1); Serial.print(" -> "); Serial.println(daily_wind_speed_10m_max_1);
float daily_wind_speed_10m_max_2 = daily_wind_speed_10m_max[2]; // 18.8
Serial.print(daily_time_2); Serial.print(" -> "); Serial.println(daily_wind_speed_10m_max_2);
float daily_wind_speed_10m_max_3 = daily_wind_speed_10m_max[3]; // 11.2
Serial.print(daily_time_3); Serial.print(" -> "); Serial.println(daily_wind_speed_10m_max_3);

Serial.println("Vent direction");
JsonArray daily_wind_direction_10m_dominant = daily["wind_direction_10m_dominant"];
int daily_wind_direction_10m_dominant_0 = daily_wind_direction_10m_dominant[0]; // 261
Serial.print(daily_time_0); Serial.print(" -> "); Serial.println(daily_wind_direction_10m_dominant_0);
int daily_wind_direction_10m_dominant_1 = daily_wind_direction_10m_dominant[1]; // 27
Serial.print(daily_time_1); Serial.print(" -> "); Serial.println(daily_wind_direction_10m_dominant_1);
int daily_wind_direction_10m_dominant_2 = daily_wind_direction_10m_dominant[2]; // 22
Serial.print(daily_time_2); Serial.print(" -> "); Serial.println(daily_wind_direction_10m_dominant_2);
int daily_wind_direction_10m_dominant_3 = daily_wind_direction_10m_dominant[3]; // 1
Serial.print(daily_time_3); Serial.print(" -> "); Serial.println(daily_wind_direction_10m_dominant_3);

                    } else {
                     Serial.print("deserializeJson() failed: ");
                     Serial.println(error.c_str());
                    }
                }
           } else {
            Serial.printf("GET request failed, error: %s\n", http.errorToString(httpCode).c_str());
            WiFi.disconnect(); delay(50); daily_datas_load();
           }
       http.end(); // Close connection
      } else {
       Serial.println("Not connected to Wi-Fi");
      }

     WiFi.disconnect();
     delay(50);        
  
}


//
