
/*
 * File: GPSTimeSource.h
 * Description:
 *   A time source that reads the time from a NMEA-compliant GPS receiver.
 * Author: Mooneer Salem <mooneer@gmail.com>
 * License: New BSD License
 */

#ifndef GPS_TIMESOURCE_H
#define GPS_TIMESOURCE_H

#if defined(ARDUINO)
#include "Arduino.h"
#else
#define micros() (0)
#define interrupts() 
#define noInterrupts()
#endif // defined(ARDUINO)

//#include <TinyGPS++.h>
#include <TinyGPS.h>
#include "config.h"
#include "ITimeSource.h"
#include "IDataSource.h"

/*
 * A time source based on GPS satellite time.
 * WARNING: only one instance of this class should be instantiated if PPS mode is enabled.
 */
class GPSTimeSource : public ITimeSource
{
public:
  GPSTimeSource(IDataSource &dataSource)
      : dataSource_(dataSource),
        tgps_(0),
        hasLocked_(false) {
      // empty
  }
  
  virtual ~GPSTimeSource() {
      // empty
  }
  
  /* 
   * Grabs latest time from the time source.
   */
  virtual void now(uint32_t *secs, uint32_t *fract);
  
  /*
   * Enables interrupts.
   */
  void enableInterrupts();
  
  /*
   * Interrupt handler for PPS input.
   */
  static void PpsInterrupt();
  
  /*
   * Interrupt handler for Ethernet input.
   */
  static void RecvInterrupt();
  
  /*
   * Retrieves current location.
   */
  float latitude() const { return lat_; }
  float longitude() const { return long_; }
  
  /*
   * get time now since epoch
   */
  void timePps(uint32_t *secs, uint32_t *fract) const;
  
  /*
   * get ethernet receive time since epoch
   */
  virtual uint32_t timeRecv(uint32_t *secs, uint32_t *fract) const;
    
private:
    static GPSTimeSource * singleton_;

    TinyGPS gps_;
    IDataSource &dataSource_;

    uint32_t tgps_;

    float lat_;
    float long_;

    bool hasLocked_;
};

#endif // GPS_TIMESOURCE_H
