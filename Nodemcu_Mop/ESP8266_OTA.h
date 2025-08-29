/*

* ESP8266 OTA Library
* Copyright (c) 2024 Kunal. All rights reserved.
* 
* This library provides Over-The-Air (OTA) firmware update functionality
* for ESP8266 NodeMCU with a modern web interface.
* 
* Author: Kunal
* License: All Rights Reserved
-------------------------------------------------------------------------
* SYNTAX Object()
->   ESP8266_OTA OTA;
-------------------------------------------------------------------------
* SYNTAX void setup()
->   OTA.setupWiFi_MIX("some","caughtcaught","NODEMCU-ESP","12345678");
->   OTA.setupWiFi_STA("some","caughtcaught");
->   OTA.setupWiFi_AP("NODEMCU-ESP-AP","12345678");
->   OTA.begin();
------------------------------------------------------------------------- 
* SYNTAX void loop()
->   OTA.handle();

*/

#ifndef ESP8266_OTA_H
#define ESP8266_OTA_H

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPUpdateServer.h>

class ESP8266_OTA {
private:
  ESP8266WebServer server;
  ESP8266HTTPUpdateServer httpUpdater;

public:
  ESP8266_OTA()
    : server(80) {}

  void begin() {

    httpUpdater.setup(&server, "/update");
    setupRoutes();
    server.begin();
    Serial.println("OTA Server started at: http://" + WiFi.localIP().toString() + "/ota");
  }

  void handle() {
    server.handleClient();
  }

  void setupWiFi_STA(const char* ssid, const char* password) {
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
  }

  void setupWiFi_AP(const char* ssidAP, const char* passwordAP) {
    WiFi.mode(WIFI_AP);
    WiFi.softAP(ssidAP, passwordAP);
  }


  void setupWiFi_MIX(const char* ssid, const char* password, const char* ssidAP, const char* passwordAP) {
    WiFi.mode(WIFI_AP_STA);
    WiFi.softAP(ssidAP, passwordAP);
    WiFi.begin(ssid, password);

    int attempts = 0;
    while (WiFi.status() != WL_CONNECTED && attempts < 10) {
      delay(500);
      attempts++;
    }
  }

private:
  void setupRoutes() {
    server.on("/ota", HTTP_GET, [this]() {
      String html = "<!DOCTYPE html><html><head>";
      html += "<meta charset=\"UTF-8\"><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">";
      html += "<title>NodeMCU OTA</title>";
      html += "<style>";
      html += "* { margin: 0; padding: 0; box-sizing: border-box; }";
      html += "body { font-family: -apple-system, BlinkMacSystemFont, 'Segoe UI', Roboto, sans-serif; background: linear-gradient(135deg, #0f172a 0%, #1e293b 100%); color: #f8fafc; min-height: 100vh; display: flex; align-items: center; justify-content: center; padding: 20px; }";
      html += "body::before { content: ''; position: fixed; top: 0; left: 0; width: 100%; height: 100%; background: radial-gradient(circle at 30% 70%, rgba(59, 130, 246, 0.1) 0%, transparent 50%), radial-gradient(circle at 70% 30%, rgba(16, 185, 129, 0.08) 0%, transparent 50%); pointer-events: none; z-index: -1; }";
      html += ".container { max-width: 500px; width: 100%; }";
      html += ".card { background: rgba(30, 41, 59, 0.8); backdrop-filter: blur(20px); border: 1px solid rgba(51, 65, 85, 0.6); border-radius: 20px; padding: 32px; margin-bottom: 20px; box-shadow: 0 25px 50px -12px rgba(0, 0, 0, 0.4); }";
      html += ".title { font-size: 32px; font-weight: 800; text-align: center; margin-bottom: 8px; background: linear-gradient(135deg, #3b82f6, #06b6d4); -webkit-background-clip: text; -webkit-text-fill-color: transparent; background-clip: text; }";
      html += ".subtitle { text-align: center; color: #cbd5e1; margin-bottom: 24px; font-size: 16px; }";
      html += ".credits { text-align: center; margin-bottom: 32px; padding: 16px; background: linear-gradient(135deg, rgba(51, 65, 85, 0.5), rgba(71, 85, 105, 0.3)); border-radius: 12px; border: 1px solid rgba(71, 85, 105, 0.4); }";
      html += ".credits p:first-child { font-size: 13px; color: #94a3b8; margin-bottom: 4px; font-weight: 600; }";
      html += ".credits p:last-child { font-size: 12px; color: #64748b; }";
      html += ".upload-area { border: 2px dashed rgba(71, 85, 105, 0.6); border-radius: 16px; padding: 48px 24px; text-align: center; cursor: pointer; transition: all 0.3s ease; background: rgba(51, 65, 85, 0.2); position: relative; overflow: hidden; }";
      html += ".upload-area::before { content: ''; position: absolute; top: 0; left: -100%; width: 100%; height: 100%; background: linear-gradient(90deg, transparent, rgba(59, 130, 246, 0.1), transparent); transition: left 0.5s; }";
      html += ".upload-area:hover { border-color: #3b82f6; background: rgba(59, 130, 246, 0.1); transform: translateY(-2px); box-shadow: 0 10px 25px rgba(59, 130, 246, 0.2); }";
      html += ".upload-area:hover::before { left: 100%; }";
      html += ".upload-icon { width: 56px; height: 56px; margin: 0 auto 20px; color: #64748b; transition: all 0.3s ease; }";
      html += ".upload-area:hover .upload-icon { color: #3b82f6; transform: scale(1.1); }";
      html += ".upload-area h3 { font-size: 20px; font-weight: 700; margin-bottom: 8px; color: #f1f5f9; }";
      html += ".upload-area p { color: #94a3b8; margin-bottom: 24px; }";
      html += ".btn { background: linear-gradient(135deg, #3b82f6, #2563eb); color: white; border: none; padding: 14px 28px; border-radius: 12px; cursor: pointer; font-weight: 600; font-size: 15px; transition: all 0.3s ease; box-shadow: 0 4px 15px rgba(59, 130, 246, 0.3); }";
      html += ".btn:hover { transform: translateY(-2px); box-shadow: 0 8px 25px rgba(59, 130, 246, 0.4); background: linear-gradient(135deg, #2563eb, #1d4ed8); }";
      html += ".btn:active { transform: translateY(0); }";
      html += ".btn-success { background: linear-gradient(135deg, #10b981, #059669); box-shadow: 0 4px 15px rgba(16, 185, 129, 0.3); }";
      html += ".btn-success:hover { background: linear-gradient(135deg, #059669, #047857); box-shadow: 0 8px 25px rgba(16, 185, 129, 0.4); }";
      html += ".file-info { background: linear-gradient(135deg, rgba(51, 65, 85, 0.8), rgba(71, 85, 105, 0.6)); border-radius: 16px; padding: 20px; margin: 24px 0; display: none; border: 1px solid rgba(71, 85, 105, 0.4); }";
      html += ".file-info svg { width: 24px; height: 24px; margin-right: 12px; color: #3b82f6; }";
      html += ".file-info p:first-child { font-weight: 600; color: #f1f5f9; margin-bottom: 4px; }";
      html += ".file-info p:last-child { color: #94a3b8; font-size: 14px; }";
      html += ".progress-card { display: none; }";
      html += ".progress-card h3 { font-size: 20px; font-weight: 700; margin-bottom: 24px; color: #f1f5f9; }";
      html += ".progress-bar { width: 100%; height: 12px; background: rgba(51, 65, 85, 0.8); border-radius: 8px; overflow: hidden; margin: 20px 0; position: relative; }";
      html += ".progress-bar::before { content: ''; position: absolute; top: 0; left: 0; width: 100%; height: 100%; background: linear-gradient(90deg, transparent, rgba(255, 255, 255, 0.1), transparent); animation: shimmer 2s infinite; }";
      html += ".progress-fill { height: 100%; background: linear-gradient(90deg, #3b82f6, #06b6d4, #10b981); width: 0%; transition: width 0.5s ease; border-radius: 8px; position: relative; }";
      html += ".progress-fill::after { content: ''; position: absolute; top: 0; right: 0; width: 20px; height: 100%; background: linear-gradient(90deg, transparent, rgba(255, 255, 255, 0.3)); }";
      html += ".status { padding: 20px; border-radius: 16px; margin: 20px 0; border-left: 4px solid; backdrop-filter: blur(10px); }";
      html += ".status-info { background: rgba(30, 41, 59, 0.8); border-color: #3b82f6; color: #93c5fd; }";
      html += ".status-success { background: rgba(6, 78, 59, 0.8); border-color: #10b981; color: #6ee7b7; }";
      html += ".status-error { background: rgba(127, 29, 29, 0.8); border-color: #ef4444; color: #fca5a5; }";
      html += ".hidden { display: none; }";
      html += ".flex { display: flex; align-items: center; }";
      html += ".text-center { text-align: center; }";
      html += ".mt-4 { margin-top: 16px; }";
      html += "input[type='file'] { display: none; }";
      html += "@keyframes shimmer { 0% { transform: translateX(-100%); } 100% { transform: translateX(100%); } }";
      html += "@media (max-width: 640px) { .container { max-width: 100%; } .card { padding: 24px; } .upload-area { padding: 32px 16px; } }";
      html += "</style>";
      html += "</head><body>";

      html += "<div class=\"container\">";
      html += "<div class=\"card\">";
      html += "<h1 class=\"title\">NodeMCU OTA</h1>";
      html += "<p class=\"subtitle\">Firmware Update System</p>";
      html += "<div class=\"credits\">";
      html += "<p>© 2024 Kunal - All Rights Reserved</p>";
      html += "<p>ESP8266 OTA Library by Kunal</p>";
      html += "</div>";

      html += "<div id=\"uploadArea\" class=\"upload-area\" onclick=\"document.getElementById('fileInput').click()\">";
      html += "<svg class=\"upload-icon\" fill=\"none\" stroke=\"currentColor\" viewBox=\"0 0 24 24\">";
      html += "<path stroke-linecap=\"round\" stroke-linejoin=\"round\" stroke-width=\"2\" d=\"M7 16a4 4 0 01-.88-7.903A5 5 0 1115.9 6L16 6a5 5 0 011 9.9M15 13l-3-3m0 0l-3 3m3-3v12\"></path></svg>";
      html += "<h3>Select Firmware File</h3>";
      html += "<p>Choose a .bin file to upload</p>";
      html += "<input type=\"file\" id=\"fileInput\" accept=\".bin\" onchange=\"handleFileSelect(this)\" />";
      html += "<button class=\"btn mt-4\">Browse Files</button>";
      html += "</div>";

      html += "<div id=\"fileInfo\" class=\"file-info\">";
      html += "<div class=\"flex\">";
      html += "<svg class=\"w-6 h-6 mr-3\" fill=\"none\" stroke=\"currentColor\" viewBox=\"0 0 24 24\">";
      html += "<path stroke-linecap=\"round\" stroke-linejoin=\"round\" stroke-width=\"2\" d=\"M9 12h6m-6 4h6m2 5H7a2 2 0 01-2-2V5a2 2 0 012-2h5.586a1 1 0 01.707.293l5.414 5.414a1 1 0 01.293.707V19a2 2 0 01-2 2z\"></path></svg>";
      html += "<div>";
      html += "<p id=\"fileName\"></p>";
      html += "<p id=\"fileSize\" style=\"color: #94a3b8; font-size: 14px;\"></p>";
      html += "</div></div></div>";

      html += "<div class=\"text-center\">";
      html += "<button id=\"uploadBtn\" onclick=\"uploadFirmware()\" class=\"btn btn-success hidden\">Upload Firmware</button>";
      html += "</div>";

      html += "</div>";

      html += "<div id=\"progressCard\" class=\"card progress-card\">";
      html += "<h3 class=\"text-center\">Uploading Firmware</h3>";
      html += "<div class=\"progress-bar\">";
      html += "<div id=\"progressBar\" class=\"progress-fill\"></div>";
      html += "</div>";
      html += "<div class=\"flex\" style=\"justify-content: space-between;\">";
      html += "<span>Progress</span>";
      html += "<span id=\"progressText\">0%</span>";
      html += "</div></div>";

      html += "<div id=\"status\"></div>";
      html += "</div>";

      html += "<script>";
      html += "function handleFileSelect(input) {";
      html += "const file = input.files[0];";
      html += "if (file) {";
      html += "document.getElementById('fileName').textContent = file.name;";
      html += "document.getElementById('fileSize').textContent = (file.size / 1024 / 1024).toFixed(2) + ' MB';";
      html += "document.getElementById('fileInfo').style.display = 'block';";
      html += "document.getElementById('uploadBtn').classList.remove('hidden');";
      html += "} }";

      html += "function uploadFirmware() {";
      html += "const fileInput = document.getElementById('fileInput');";
      html += "const file = fileInput.files[0];";
      html += "if (!file) { showStatus('Please select a firmware file first!', 'error'); return; }";
      html += "if (!file.name.endsWith('.bin')) { showStatus('Please select a valid .bin firmware file!', 'error'); return; }";
      html += "const formData = new FormData(); formData.append('firmware', file);";
      html += "const xhr = new XMLHttpRequest();";
      html += "document.getElementById('progressCard').style.display = 'block';";
      html += "xhr.upload.addEventListener('progress', function(e) {";
      html += "if (e.lengthComputable) { const percentComplete = Math.round((e.loaded / e.total) * 100); updateProgress(percentComplete); }";
      html += "});";
      html += "xhr.addEventListener('load', function() {";
      html += "if (xhr.status === 200) {";
      html += "showStatus('Firmware uploaded successfully! Device is rebooting...', 'success');";
      html += "setTimeout(() => { showStatus('Update complete! Please wait for device to restart.', 'info'); }, 3000);";
      html += "} else { showStatus('Upload failed: ' + xhr.responseText, 'error'); }";
      html += "});";
      html += "xhr.addEventListener('error', function() { showStatus('Upload failed: Network error', 'error'); });";
      html += "showStatus('Uploading firmware...', 'info');";
      html += "xhr.open('POST', '/update'); xhr.send(formData); }";

      html += "function updateProgress(percent) {";
      html += "document.getElementById('progressBar').style.width = percent + '%';";
      html += "document.getElementById('progressText').textContent = percent + '%'; }";

      html += "function showStatus(message, type) {";
      html += "const statusDiv = document.getElementById('status');";
      html += "let className = 'status status-info';";
      html += "if (type === 'success') className = 'status status-success';";
      html += "if (type === 'error') className = 'status status-error';";
      html += "statusDiv.innerHTML = '<div class=\"' + className + '\">' + message + '</div>'; }";

      html += "</script></body></html>";
      server.send(200, "text/html", html);
    });
  }
};

#endif