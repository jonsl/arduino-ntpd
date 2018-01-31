/*
 * File: GPSTimeSource.cpp
 * Description:
 *   A time source that reads the time from a NMEA-compliant GPS receiver.
 * Author: Mooneer Salem <mooneer@gmail.com>
 * License: New BSD License
 */
#include <time.h>
#include "config.h"

#ifdef ETH_RX_PIN
#include "utility/w5100.h"
#endif // ETH_RX_PIN

#include "GPSTimeSource.h"
#include "TimeUtilities.h"

GPSTimeSource* GPSTimeSource::singleton_ = NULL;

volatile unsigned t4ovfcnt_;
volatile unsigned t5ovfcnt_;

volatile uint32_t trecvsec_;
volatile uint32_t trecvfract_;

volatile bool isProcessingPps = false;

void GPSTimeSource::enableInterrupts() {
#ifdef ETH_RX_PIN
    // Enable Ethernet interrupt first to reduce difference between the two timers.
    // NOTE: NTP server must _always_ be initialized first to ensure that it occupies socket 0.
    W5100.writeIMR(0x01);
#endif

    singleton_ = this;
    pinMode(PPS_PIN, INPUT);
    
    TCCR4A = 0 ;                    // Normal counting mode
    TCCR4B = B010;                  // set prescale bits
    TCCR4B |= _BV(ICES4);           // enable input capture when pin goes high
    TIMSK4 |= _BV(ICIE4);           // enable input capture interrupt for timer 4
    TIMSK4 |= _BV(TOIE4);           // overflow interrupt

#ifdef ETH_RX_PIN
    pinMode(ETH_RX_PIN, INPUT);
    
    TCCR5A = 0 ;                    // Normal counting mode
    TCCR5B = B010;                  // set prescale bits
                                    // enable input capture when pin goes low (default).
    TIMSK5 |= _BV(ICIE5);           // enable input capture interrupt for timer 5
    TIMSK5 |= _BV(TOIE5);           // overflow interrupt
#endif

    Serial.println("interrupts enabled");
}

void GPSTimeSource::PpsInterrupt() {
  if (!isProcessingPps) {
    TCNT4 = 0;
    t4ovfcnt_ = 0;
    isProcessingPps = true;
  }
}

void GPSTimeSource::RecvInterrupt() {
  timePps(&trecvsec_, &trecvfract_);
}

ISR(TIMER4_OVF_vect) {
  ++t4ovfcnt_;
}

ISR(TIMER4_CAPT_vect) {
  GPSTimeSource::singleton_->GPSTimeSource::PpsInterrupt();
}

ISR(TIMER5_OVF_vect) {
    ++t5ovfcnt_;
}

ISR(TIMER5_CAPT_vect) {
    GPSTimeSource::singleton_->GPSTimeSource::RecvInterrupt();
}

void GPSTimeSource::timePps(uint32_t *secs, uint32_t *fract) const {
  unsigned long t4now = ((unsigned long)t4ovfcnt_ << 16) | TCNT4;
  unsigned long t4diff = (t4now >> 1);
  if (secs) {
    *secs = t4diff / 1000000;
    if (isProcessingPps) {
      ++(*secs);
    }
  }
  if (fract) {
    *fract = t4diff % 1000000;
  }
}

uint32_t GPSTimeSource::timeRecv(uint32_t *secs, uint32_t *fract) const {
  if (secs) {
    *secs = tgps_ + trecvsec_;
  }
  if (fract) {
    *fract = (0xffffffff / 1000000) * trecvfract_;
  }
  return 0;
}

void GPSTimeSource::now(uint32_t *secs, uint32_t *fract) {
  unsigned long elapsed_seconds, elapsed_fraction;
  timePps(&elapsed_seconds, &elapsed_fraction);

  if (isProcessingPps) {
    while (dataSource_.available()) {
      int c = dataSource_.read();
      if (gps_.encode(c)) {
        // Grab time from now-valid data.
        int year;
        byte month, day, hour, minutes, second, hundredths;
        unsigned long fix_age;
    
        gps_.crack_datetime(&year, &month, &day, &hour, &minutes, &second, &hundredths, &fix_age);
        gps_.f_get_position(&lat_, &long_);
    
        // We don't want to use the time we've received if 
        // the fix is invalid.
        if (fix_age != TinyGPS::GPS_INVALID_AGE && fix_age < 5000 && year >= 2013) {
          tgps_ = TimeUtilities::numberOfSecondsSince1900Epoch(year, month, day, hour, minutes, second);
          hasLocked_ = true;
        } else {
          tgps_ = 0;
          hasLocked_ = false;
        }

        isProcessingPps = false;

        // recalculate utc now
        timePps(&elapsed_seconds, &elapsed_fraction);

//        Serial.print("tgps_ is ");
//        Serial.println(tgps_);
      }
    }
  }

//  Serial.print("elapsed_seconds is ");
//  Serial.println(elapsed_seconds);
//  Serial.print("elapsed_fraction is ");
//  Serial.println(elapsed_fraction);

  if (secs) {
    *secs = tgps_ + elapsed_seconds;
  }
  if (fract) {
    *fract = (0xffffffff / 1000000) * elapsed_fraction;
  }

//  Serial.print("secs is ");
//  Serial.println(*secs);
//  Serial.print("fract is ");
//  Serial.println(*fract);
}

