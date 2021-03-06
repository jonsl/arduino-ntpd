/*
   File: config.h
   Description:
     Configuration for ArduinoNTPd.
   Author: Mooneer Salem <mooneer@gmail.com>
   License: New BSD License
*/

#ifndef CONFIG_H
#define CONFIG_H

#if defined(ARDUINO)

#include <Ethernet.h>

// ----------------------------------------------------------------------------------
// NOTE: please see config.cpp for all config options that begin with "extern" below.
// ----------------------------------------------------------------------------------

// The desired IP address for your time server.
// IPv4 only due to limitations of the Ethernet library.
extern IPAddress ip_;

// The desired MAC address for your time server.
// This should not conflict with any other devices on your network!
extern byte macAddress[];

// The port to run your time server on. This should not normally
// need to be changed.
#define NTP_PORT 123

// The port to run the built-in HTTP server on. This should not normally
// need to be changed.
#define HTTP_SERVER_PORT 80

// The pin that the timer uses to capture the interrupt.
#define PPS_PIN 49

// The pin that the timer uses to capture the Ethernet receive.
// (Comment out if not using.)
#define ETH_RX_PIN 48

// RX pin for the GPS receiver/shield. This should support change interrupts
// per NewSoftSerial documentation.
//#define GPS_RX_PIN 19

// TX pin for the GPS receiver/shield.
//#define GPS_TX_PIN 18

// Or uncomment and modify this to use one of the built-in UARTs.
// (possible choices: Serial1, Serial2, Serial3)
#define HARDWARE_SERIAL_CLASS Serial1

// Uncomment the below to use DHCP instead of static IP allocation.
//#define NETWORK_USE_DHCP 1

#endif // defined(ARDUINO)

#endif // CONFIG_H
