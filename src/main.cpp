#include <WiFi.h>

#include "ServerWrapper.hpp"
#include "decrypt.hpp"
#include "encData.hpp"

#define STATIC_IP  // enable if using static ip instead of DHCP

ServerWrapper server;

namespace WifiHandler {
	namespace {	 // "private"
#ifdef STATIC_IP
		IPAddress localIP(172, 28, 116, 0);
		IPAddress gateway(172, 28, 0, 1);
		IPAddress subnet(255, 255, 0, 0);
		IPAddress primaryDNS(172, 28, 0, 1);
#endif
	};	// namespace

	void init() {
#ifdef STATIC_IP
		if (!WiFi.config(localIP, gateway, subnet, primaryDNS, primaryDNS)) {
			Serial.println("STA Failed to configure");
		}
#endif

		char ssid[33];
		char password[65];
		decryptShuffled(ENCRYPTED_FIELD, 0, ssid);
		decryptShuffled(ENCRYPTED_FIELD, 1, password);

		Serial.printf("Connecting to SSID \"%s\"\n", ssid);

		WiFi.begin(ssid, password);
		while (WiFi.status() != WL_CONNECTED) {
			delay(1000);
			Serial.printf("Connecting to SSID \"%s\"\n", ssid);
		}
		Serial.print("Successful. Current IP address: ");
		Serial.println(WiFi.localIP());
	}
};	// namespace WifiHandler

void setup() {
	Serial.begin(115200);
	WifiHandler::init();
	server.init();
}

static void send(WiFiClient client) {
	client.println("<html><head><title>");
	client.println("Ventilation Temperature Control");
	client.println("</title></head><body>");
	client.println("<b>THIS IS A TEST VALUE!</b>");
	client.println("</body></html>");
}

void loop() {
	WiFiClient client = server.accept();

	if (server.isAvailable(client))
		server.handleConnection(client, &send);
	client.stop();
}