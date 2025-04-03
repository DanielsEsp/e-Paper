



void alert_datas_load() {
  
     WiFi.disconnect(); delay(50);
     Serial.println("Connect Alerte"); delay(100);
     WiFi.begin(ssid, wifipw);
     Serial.println("Connecting Wifi");
     while (WiFi.status() != WL_CONNECTED) {
            wifitime ++;
            Serial.print("."); delay(100);
            if (wifitime > 20) { WiFi.disconnect(); wifitime = 0; esprestart ++; Serial.println("Exit--->"); alert_datas_load(); }
            if (esprestart >= 5) { WiFi.disconnect(); Serial.println("Restart--->"); delay(500); ESP.restart(); delay(50); }
           }
     Serial.println();
     Serial.print("Local IP "); Serial.print(WiFi.localIP()); Serial.println();
     Serial.print("Wifi RSSI "); Serial.println(WiFi.RSSI()); Serial.println();                  
     delay(1000);
          
     Serial.println("Get Alerte"); delay(50);
     url = alerte;         
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
                              JsonObject location = doc["location"];
                              const char* location_name = location["name"];
                              siAlerte = String(doc["alerts"]["alert"]);
                              if (siAlerte == "[]") { evenementAlerte[1] = ""; evenementAlerte[2] = ""; numeroAlerte = 0; return; } 
                              for (JsonObject alerts_alert_item : doc["alerts"]["alert"].as<JsonArray>()) {
                                   numeroAlerte ++;
//                                   const char* alerts_alert_item_event = alerts_alert_item["event"];
//                                   evenementAlerte[numeroAlerte] = alerts_alert_item_event;
                                   const char* alerts_alert_item_event = alerts_alert_item["desc"];
                                   evenementAlerte[numeroAlerte] = alerts_alert_item_event;

                                  }
                               nombreAlertes = numeroAlerte; 
                              } else {
                               Serial.print("deserializeJson() failed: ");
                               Serial.println(error.c_str());
                              }

                }
           } else {
            Serial.printf("GET request failed, error: %s\n", http.errorToString(httpCode).c_str());
            WiFi.disconnect(); delay(50); alert_datas_load();
           }
       http.end(); // Close connection
      } else {
       Serial.println("Not connected to Wi-Fi");
      }

     WiFi.disconnect();
     delay(50);        
  
}


//
