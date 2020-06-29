#include <SerialCommand.h>
#include <RFControl.h>
#include <DHTlib.h>

#define BME_ENABLED

void argument_error();

SerialCommand sCmd;

#include "rfcontrol_command.h"
#ifdef KEYPAD_ENABLED
#include "keypad_command.h"
#endif
#include "dht_command.h"
#ifdef BME_ENABLED
#include "bme_command.h"
#endif

void digital_read_command();
void digital_write_command();
void analog_read_command();
void analog_write_command();
void reset_command();
void pin_mode_command();
void ping_command();
void unrecognized(const char *command);


void setup() {
	Serial.begin(115200);
  #ifdef BME_ENABLED
  bme_setup();
  #endif
	// Setup callbacks for SerialCommand commands
	sCmd.addCommand("DR", digital_read_command);
	sCmd.addCommand("DW", digital_write_command);
	sCmd.addCommand("AR", analog_read_command);
	sCmd.addCommand("AW", analog_write_command);
	sCmd.addCommand("PM", pin_mode_command);
	sCmd.addCommand("RF", rfcontrol_command);    
	sCmd.addCommand("PING", ping_command);
	sCmd.addCommand("DHT", dht_command);
  #ifdef BME_ENABLED
  sCmd.addCommand("BME", bme_command);
  #endif
  sCmd.addCommand("RESET", reset_command);
  #ifdef KEYPAD_ENABLED
  sCmd.addCommand("K", keypad_command);
  #endif
	sCmd.setDefaultHandler(unrecognized);
	Serial.print(F("ready\r\n"));
}

void loop() {
	// handle serial command
	sCmd.readSerial();
	// handle rf control receiving
	rfcontrol_loop();
  #ifdef KEYPAD_ENABLED
	// handle keypad keypress
	keypad_loop();
  #endif
}

void digital_read_command() {
	char* arg = sCmd.next();
  	if(arg == NULL) {
  		argument_error();
    	return;
  	}
  	int pin = atoi(arg);
  	int val = digitalRead(pin);
  	Serial.print(F("ACK "));
  	Serial.write('0' + val);
  	Serial.print(F("\r\n"));
}

void analog_read_command() {
	char* arg = sCmd.next();
  	if(arg == NULL) {
  		argument_error();
    	return;
  	}
  	int pin = atoi(arg);
  	int val = analogRead(pin);
  	Serial.print(F("ACK "));
  	Serial.print(val);
  	Serial.print(F("\r\n"));
}

void digital_write_command() {
	char* arg = sCmd.next();
  	if(arg == NULL) {
  		argument_error();
    	return;
  	}
  	int pin = atoi(arg);
  	arg = sCmd.next();
  	if(arg == NULL) {
  		argument_error();
    	return;
  	}
  	int val = atoi(arg);
  	digitalWrite(pin, val);
  	Serial.print(F("ACK\r\n"));
}

void analog_write_command() {
	char* arg = sCmd.next();
  	if(arg == NULL) {
  		argument_error();
    	return;
  	}
  	int pin = atoi(arg);
  	arg = sCmd.next();
  	if(arg == NULL) {
  		argument_error();
    	return;
  	}
  	int val = atoi(arg);
  	analogWrite(pin, val);
  	Serial.print(F("ACK\r\n"));
}

void pin_mode_command() {
	char* arg = sCmd.next();
  	if(arg == NULL) {
  		argument_error();
    	return;
  	}
  	int pin = atoi(arg);
  	arg = sCmd.next();
  	if(arg == NULL) {
  		argument_error();
    	return;
  	}
  	// INPUT 0x0
	// OUTPUT 0x1
  	int mode = atoi(arg);
  	pinMode(pin, mode);
    Serial.print(F("ACK\r\n"));	
}


void ping_command() {
  char *arg;
  Serial.print("PING");
  arg = sCmd.next();
  if (arg != NULL) {
    Serial.write(' ');
    Serial.print(arg);
  }
  Serial.print(F("\r\n"));
}


void reset_command() {
  RFControl::stopReceiving();
  Serial.print(F("ready\r\n"));
}

void argument_error() {
	Serial.print(F("ERR argument_error\r\n"));
}
// This gets set as the default handler, and gets called when no other command matches.
void unrecognized(const char *command) {
	Serial.print(F("ERR unknown_command\r\n"));
}
