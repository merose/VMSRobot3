#if !defined(AnalogEncoder_h)
#define AnalogEncoder_h

#include "Arduino.h"

class AnalogEncoder {
  
 private:
  enum {TRIGGER_DOWN, TRIGGER_UP};

  long totalUpdates;
  long threshold;
  int state;
  int oldWeight;
  int newWeight;
  volatile long ticks;
  long cumValue;
  long localMinimum;
  long localMaximum;
  volatile int lastLocalMinimum;
  volatile int lastLocalMaximum;

 public:

  AnalogEncoder(int initialThreshold = 125);
  
  void setOldNewWeight(int oldWt, int newWt);

  void update(int measurement);
  
  int getTicks();
  
  void resetTicks();
  
  int getLastLocalMinimum();

  int getLastLocalMaximum();

};

#endif //!defined(AnalogEncoder_h)
