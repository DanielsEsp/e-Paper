



void current_datas_load() {
  
     WiFi.disconnect(); delay(50);
     Serial.println("Connect Current"); delay(100);
     WiFi.begin(ssid, wifipw);
     Serial.println("Connecting Wifi");
     while (WiFi.status() != WL_CONNECTED) {
            wifitime ++;
            Serial.print("."); delay(100);
            if (wifitime > 20) { WiFi.disconnect(); wifitime = 0; esprestart ++; Serial.println("Exit--->"); current_datas_load(); }
            if (esprestart >= 5) { WiFi.disconnect(); Serial.println("Restart--->"); delay(500); ESP.restart(); delay(50); }
           }
     Serial.println();
     Serial.print("Local IP "); Serial.print(WiFi.localIP()); Serial.println();
     Serial.print("Wifi RSSI "); Serial.println(WiFi.RSSI()); Serial.println();                  
     delay(1000);

     Serial.println("Get Current"); delay(100);
     url = current; 
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
                     const char* datetime = doc["current"]["time"];
                    Serial.println(String(doc["current"]["time"])); Serial.println(); 

                     temperature = String(doc["current"]["temperature_2m"]);
                     temperatureFormat = temperature.toFloat();
                     temperatureFormatTxt = " " + String(temperatureFormat, 1); 
                     if (temperatureFormat < 10) { temperatureFormatTxt = "  " + String(temperatureFormat, 1); }
                     if (temperatureFormat < 0) { temperatureFormatTxt = " " + String(temperatureFormat, 1); }
                     if (temperatureFormat < -10) { temperatureFormatTxt = String(temperatureFormat, 1); }

                     ressenti = String(doc["current"]["apparent_temperature"]);
                     ressentiFormat = ressenti.toFloat();
                     ressentiFormatTxt = " " + String(ressentiFormat, 1); 
                     if (ressentiFormat < 10) { ressentiFormatTxt = "  " + String(ressentiFormat, 1); }
                     if (ressentiFormat < 0) { ressentiFormatTxt = " " + String(ressentiFormat, 1); }
                     if (ressentiFormat < -10) { ressentiFormatTxt = String(ressentiFormat, 1); }
 
                     humidite = String(doc["current"]["relative_humidity_2m"]);
                     humiditeFormat = humidite.toInt();
                     humiditeFormatTxt = String(humiditeFormat); 
                     if (humiditeFormat < 100) { humiditeFormatTxt = " " + String(humiditeFormat); }

                     pression = String(doc["current"]["surface_pressure"]);
                     pressionFormat = pression.toFloat();
                     pressionFormatTxt = String(pressionFormat, 1);
                     if (pressionFormat < 1000) { pressionFormatTxt = " " + String(pressionFormat,1); }

                     is_day = String(doc["current"]["is_day"])/*.toInt()*/;

                     vent_vitesse = String(doc["current"]["wind_speed_10m"]);      
                     vent_rafale = String(doc["current"]["wind_gusts_10m"]);
                     vent_direction = String(doc["current"]["wind_direction_10m"]).toInt();
                     Direction = vent_direction;
                     if (Direction > 337.5 || Direction <= 22.5) { vient_De = "du Nord"; }
                     if (Direction > 22.5 && Direction <= 67.5) { vient_De = "du Nord Est"; }
                     if (Direction > 67.5 && Direction <= 112.5) { vient_De = "de l'Est"; }
                     if (Direction > 112.5 && Direction <= 157.5) { vient_De = "du Sud Est"; }
                     if (Direction > 157.5 && Direction <= 202.5) { vient_De = "du Sud"; }
                     if (Direction > 202.5 && Direction <= 247.5) { vient_De = "du Sud Ouest"; }
                     if (Direction > 247.5 && Direction <= 292.5) { vient_De = "de l'Ouest"; }
                     if (Direction > 292.5 && Direction <= 337.5) { vient_De = "du Nord Ouest"; }                       

                     weather_code = String(doc["current"]["weather_code"])/*.toInt()*/;        
                     for (pointeur = 0; pointeur <= 34; pointeur ++) {
                          if (weather_code == icone_datas[pointeur]) {
                              if (weather_code == "0") { 
                                  if (is_day == "1") { return; } else { pointeur = pointeur + 1; return; }
                                 }
                              if (weather_code == "2") { 
                                  if (is_day == "1") { return; } else { pointeur = pointeur + 1; return; }
                                 }
                              if (weather_code == "61") { 
                                  if (is_day == "1") { return; } else { pointeur = pointeur + 1; return; }
                                  }
                              if (weather_code == "80") { 
                                  if (is_day == "1") { return; } else { pointeur = pointeur + 1; return; }
                                  }
                              return;
                             }                                                                       
                         }  

                     String datetime_str = String(datetime);
                     int splitIndex = datetime_str.indexOf('T');
                     current_date = datetime_str.substring(0, splitIndex);
                     last_weather_update = datetime_str.substring(splitIndex + 1, splitIndex + 9); // Extract time portion
                     Serial.print("Derniere mise a jour "); Serial.println(last_weather_update); Serial.println();
                    } else {
                     Serial.print("deserializeJson() failed: ");
                     Serial.println(error.c_str());
                    }
                }
           } else {
            Serial.printf("GET request failed, error: %s\n", http.errorToString(httpCode).c_str());
            WiFi.disconnect(); delay(50); current_datas_load();
           }
       http.end(); // Close connection
      } else {
       Serial.println("Not connected to Wi-Fi");
      }

     WiFi.disconnect();
     delay(50);        
  
}


//
