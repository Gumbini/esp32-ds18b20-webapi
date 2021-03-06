#include "WiFiHandler.hpp"

#include <HardwareSerial.h>
#include <WiFi.h>

#include "deobfuscate.hpp"
#include "obfuscatedData.hpp"

// #define STATIC_IP

/////////////////////////////////////////////////////////////////////

namespace { // "static"
#ifdef STATIC_IP
	IPAddress localIP(172, 28, 116, 0);
	IPAddress gateway(172, 28, 0, 1);
	IPAddress subnet(255, 255, 0, 0);
	IPAddress primaryDNS(172, 28, 0, 1);
#endif
}; // namespace

// public constructors

WiFiHandler::WiFiHandler() {
#ifdef STATIC_IP
	if (!WiFi.config(localIP, gateway, subnet, primaryDNS, primaryDNS)) {
		Serial.println("STA failed to configure!");
	}
#endif
}

WiFiHandler::WiFiHandler(const char *hostname) {
	WiFi.config(INADDR_NONE, INADDR_NONE, INADDR_NONE, INADDR_NONE);
	WiFi.setHostname(hostname);
}

// public functions

void WiFiHandler::begin() {
	char ssid[33];
	char password[65];
	ObfuscatedCredentials::deobfuscate(ObfuscatedCredentials::data, 0, ssid);
	ObfuscatedCredentials::deobfuscate(ObfuscatedCredentials::data, 1, password);

	Serial.printf("Connecting to SSID \"%s\"...", ssid);

	WiFi.begin(ssid, password);
	memset(password, 0x00, 65);

	while (WiFi.status() != WL_CONNECTED) {
		delay(1500);
		Serial.print(".");
	}

	Serial.println();
	Serial.print("Successful. Current IP address: ");
	Serial.println(WiFi.localIP());
	Serial.print("Hostname: ");
	Serial.println(WiFi.getHostname());
}

void WiFiHandler::checkActiveConnection() {
	if (WiFi.status() == WL_CONNECTED)
		return;

	Serial.print("WiFi disconnected. Reconnecting...");
	WiFi.reconnect();

	while (WiFi.status() != WL_CONNECTED) {
		delay(1500);
		Serial.print(".");
	}

	Serial.println();
}
