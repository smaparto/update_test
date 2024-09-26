#include <WiFi.h>
#include <HTTPClient.h>
#include <Update.h>

const char* ssid = "mparto";              // Replace with your Wi-Fi SSID
const char* password = "9128024126";      // Replace with your Wi-Fi password

// URL for the firmware file on Dropbox
//const char* firmwareURL = "https://raw.githubusercontent.com/smaparto/update_test/main/sketch_sep25b.ino.bin";
const char* firmwareURL = "https://raw.githubusercontent.com/smaparto/update_test/main/4.bin";

//const char* firmwareURL = "https://www.dropbox.com/scl/fi/th0ezmpvrfyxvg37pkyrv/1.bin?rlkey=eqlmmjruff02vcd5tux7j4jro&st=kuk7cfih&dl=1"; // Update with your URL
//const char* firmwareURL = "http://httpforever.com/"; // Update with your U

const int ledPin = 2; // LED pin
unsigned long previousMillis = 0; // Stores last time the LED was updated
const long interval = 500; // Interval to check Wi-Fi status (in milliseconds)
int blinkFrequency = 10; // Default to 10 Hz (not connected)

void setup() {
    Serial.begin(115200);
    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.println("Connecting to WiFi...");
    }
    Serial.println("Connected to WiFi1");

    checkForUpdates();
}

void checkForUpdates() {
    if (WiFi.status() == WL_CONNECTED) {
        HTTPClient http;
        http.begin(firmwareURL);
        int httpCode = http.GET();

        Serial.printf("HTTP Code: %d\n", httpCode);

        if (httpCode == HTTP_CODE_OK) {
            int contentLength = http.getSize();
            if (contentLength > 0) {
                WiFiClient *stream = http.getStreamPtr();
                if (Update.begin(contentLength)) { // Start the update process
                    size_t written = Update.writeStream(*stream); // Write the stream to flash

                    if (written == contentLength) {
                        Serial.println("Update successfully completed. Rebooting...");
                        Update.end();
                        // You might want to call ESP.restart() here
                    } else {
                        Serial.printf("Written only %d bytes. Update failed!\n", written);
                        Update.end();
                    }
                } else {
                    Serial.println("Not enough space to begin OTA");
                }
            }
        } else {
            Serial.printf("HTTP GET failed, error code: %d\n", httpCode);
        }

        http.end();
    } else {
        Serial.println("WiFi not connected!");
    }
}

void loop() {
    // Your main loop code
}