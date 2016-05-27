#include <limits.h>
#include <AnalogEncoder.h>

AnalogEncoder::AnalogEncoder(int initialThreshold) {
  state = TRIGGER_DOWN;
  threshold = (initialThreshold << 4);
  oldWeight = 15;
  newWeight = 1;

  localMinimum = localMaximum = 0;
  lastLocalMinimum = lastLocalMaximum = 0;
  ticks = 0;
  totalUpdates = 0;
}

void AnalogEncoder::setOldNewWeight(int oldWt, int newWt) {
  oldWeight = oldWt;
  newWeight = newWt;
}

void AnalogEncoder::update(int measurement) {
  if (totalUpdates == 0) {
    cumValue = (measurement << 4);
  } else {
    cumValue = (newWeight*(measurement << 4) + oldWeight*cumValue)
      / (oldWeight + newWeight);
  }

  if (state == TRIGGER_UP) {
    localMinimum = min(localMinimum, cumValue);
    if (cumValue > localMinimum + threshold) {
      ++ticks;
      state = TRIGGER_DOWN;
      lastLocalMaximum = localMaximum / 16;
      localMaximum = cumValue;
    }
  } else {
    localMaximum = max(localMaximum, cumValue);
    if (cumValue < localMaximum - threshold) {
      ++ticks;
      state = TRIGGER_UP;
      lastLocalMinimum = localMinimum / 16;
      localMinimum = cumValue;
    }
  }
    
  ++totalUpdates;
}

int AnalogEncoder::getTicks() {
  return ticks;
}
  
void AnalogEncoder::resetTicks() {
  ticks = 0;
}

int AnalogEncoder::getLastLocalMinimum() {
  return lastLocalMinimum;
}

int AnalogEncoder::getLastLocalMaximum() {
  return lastLocalMaximum;
}
