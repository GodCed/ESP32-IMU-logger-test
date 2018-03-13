#include <Arduino.h>
#include "Adafruit_MCP23008.h"

#include "display.h"
#include "menu.h"

Display display;
Menu menu;
Adafruit_MCP23008 mcp;

TickType_t lastButtonRead;

int actionCtn = 0;
int previousCtn = 0;
int nextCtn = 0;

bool lastAction = false;
bool lastPrevious = false;
bool lastNext = false;

void readButton(void *pvParameters) {
    bool action, previous, next;

    lastButtonRead = xTaskGetTickCount();

    while(1) {
        vTaskDelayUntil(&lastButtonRead, 100 / portTICK_RATE_MS);

        action = mcp.digitalRead(2) != 0;
        previous = mcp.digitalRead(6) != 0;
        next = mcp.digitalRead(7) != 0;

        if(action && action != lastAction) {
            actionCtn++;
        }

        if(previous && previous != lastPrevious) {
            previousCtn++;
        }

        if(next && next != lastNext) {
            nextCtn++;
        }

        lastAction = action;
        lastPrevious = previous;
        lastNext = next;
    }
}

void setup() {

    // Start serial
    Serial.begin(115200);

    // Enable IO expander
    mcp.begin();

    mcp.pinMode(2, INPUT);
    mcp.pullUp(2, HIGH);

    mcp.pinMode(6, INPUT);
    mcp.pullUp(6, HIGH);

    mcp.pinMode(7, INPUT);
    mcp.pullUp(7, HIGH);

    // Start display
    Serial.println("Initializing display...");
    delay(1000);

    display.begin();
    display.showSplashScreen();

    // Wait for keypress
    while(mcp.digitalRead(2) != 0) {
        delay(100);
    }

    display.showMenu(&menu);
    xTaskCreate(&readButton, "Buttons", 2048, NULL, 5, NULL);

    Serial.println("System ready");
}

void loop() {

    //if(actionCtn != 0 || previousCtn != 0 || nextCtn != 0) {
        while(actionCtn > 0) {
            menu.action();
            actionCtn--;
        }

        while(previousCtn > 0) {
            menu.previous();
            previousCtn--;
        }

        while(nextCtn > 0) {
            menu.next();
            nextCtn--;
        }

        display.updateMenu(&menu);
    //}
}
