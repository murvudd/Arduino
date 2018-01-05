//https://www.wemos.cc/verify_products Chip ID = 0033010E
// put your setup code here, to run once:

#include <dummy.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

const char* ssid = "ssid";
const char* password = "pass";
const IPAddress LocalIPAdress = IPAddress(192,168,0,131);
int LED = BUILTIN_LED;
int Vel = 1000;
int gpio0_pin = 0;
int gpio2_pin = 2;

// ESP8266WebServer server(LocalIPAdress,80);
ESP8266WebServer server(LocalIPAdress,80);
String webPage = "";
String RootText = "<h1>Hello from esp8266 œæ¿Ÿ¹ñó³ê!</h1>\n<p></p>";
String indexPage = "<h1>Index Page</h1>\n<p></p>";


void Blink( int v ) {
	digitalWrite(LED, !digitalRead(LED));
	delay(v); 
	digitalWrite(LED, !digitalRead(LED));
	delay(v);
}
void handleRoot() {	
		
		server.send(200, "text/html", RootText);	
}

void handleNotFound() {
	
	String message = "File Not Found\n\n";
	message += "URI: ";
	message += server.uri();
	message += "\nMethod: ";
	message += (server.method() == HTTP_GET) ? "GET" : "POST";
	message += "\nArguments: ";
	message += server.args();
	message += "\n";
	for (uint8_t i = 0; i<server.args(); i++) {
		message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
	}
	server.send(404, "text/plain", message);
	
}


void setup(void) {
	pinMode(LED, OUTPUT);
	/*server setup*/ {
		Serial.begin(115200);
		WiFi.begin(ssid, password);
		Serial.println("");

		// Wait for connection
		while (WiFi.status() != WL_CONNECTED) {
			delay(500);
			Serial.print(".");
		}
		Serial.println("");
		Serial.println("Connected to ");
		Serial.println(ssid);
		Serial.println("IP address: ");
		Serial.println(WiFi.localIP());
		Serial.println("MAC: ");
		Serial.println(WiFi.macAddress());

		if (MDNS.begin("esp8266")) {
			Serial.println("MDNS responder started");
		}
	}	// setup

	//html pages
	
	webPage += "<h1>ESP8266 Web Server</h1>";
	webPage+= "<p>Socket #1 <a href=\"socket1On\"><button onclick=\"buttonFunction()\">ON</button><script>function buttonFunction() { alert(\"On\"); } </script></a>&nbsp;<a href=\"socket1Off\"><button>OFF</button></a></p>";
	webPage += "<p>Socket #2 <a href=\"socket2On\"><button>ON</button></a>&nbsp;<a href=\"socket2Off\"><button>OFF</button></a></p>";

	// preparing GPIOs
	pinMode(gpio0_pin, OUTPUT);
	digitalWrite(gpio0_pin, LOW);
	pinMode(gpio2_pin, OUTPUT);
	digitalWrite(gpio2_pin, LOW);

	server.on("/root", []() {
		Vel = 1000;
		server.send(200, "text/html", RootText);
	});

	server.on("/inline", []() {
		Vel = 100;
		server.send(200, "text/html", "<h1>this works as well</h1>");
	});

	server.on("/", []() {
		Vel = 250;
		server.send(200, "text/html", webPage);
		Serial.println("IndexPage");
	});
	
	server.on("/socket1On", []() {
		server.send(200, "text/html", webPage);
		digitalWrite(gpio0_pin, HIGH);
		Serial.println("socket1On");
		delay(1000);
		
	});

	server.on("/socket1Off", []() {
		server.send(200, "text/html", webPage);
		digitalWrite(gpio0_pin, LOW);
		Serial.println("socket1Off");
		delay(1000);
	});

	server.on("/socket2On", []() {
		server.send(200, "text/html", webPage);
		digitalWrite(gpio2_pin, HIGH);
		Serial.println("socket2On");
		delay(1000);
	});

	server.on("/socket2Off", []() {
		server.send(200, "text/html", webPage);
		digitalWrite(gpio2_pin, LOW);
		Serial.println("socket2Off");
		delay(1000);
	});

		server.onNotFound(handleNotFound);
		server.begin();
		Serial.println("HTTP server started");
}
	

void loop(void) {
	server.handleClient();
	Blink(Vel);
}