// PlayTune - An example of playing a tune on a piezo buzzer.
//
// An example for the VMS Robotics elective.

// Pitch frequencies (in octave 4), taken from:
// https://en.wikipedia.org/wiki/Scientific_pitch_notation
const float PITCH_C = 261.63;
const float PITCH_C_SHARP = 277.18;
const float PITCH_D_FLAT = PITCH_C_SHARP;
const float PITCH_D = 293.66;
const float PITCH_D_SHARP = 311.13;
const float PITCH_E_FLAT = PITCH_D_SHARP;
const float PITCH_E = 329.63;
const float PITCH_F = 349.23;
const float PITCH_F_SHARP = 369.99;
const float PITCH_G_FLAT = PITCH_F_SHARP;
const float PITCH_G = 392.0;
const float PITCH_G_SHARP = 415.30;
const float PITCH_A_FLAT = PITCH_G_SHARP;
const float PITCH_A = 440.0;
const float PITCH_A_SHARP = 466.16;
const float PITCH_B_FLAT = PITCH_A_SHARP;
const float PITCH_B = 493.88;

// Style of note. Affects the separation between the current note
// and the next.
enum NoteStyle {
  NORMAL, SLURRED, STACCATO
};

// Implements a data structure to hold a single note, which has a
// frequency, duration, and style.
class Note {
public:
  Note(float pitch, int octave, float duration, NoteStyle style=NORMAL);
  int getFrequency() const;
  void getDuration(float tempo,
		   unsigned long &duration,
		   unsigned long &separation) const;
    
private:
  int frequency;
  float duration;
  NoteStyle style;
};

// The standard note durations, as fractions of a whole note.
const float WHOLE = 1.0;
const float HALF = 1.0 / 2.0;
const float QUARTER = 1.0 / 4.0;
const float EIGHTH = 1.0 / 8.0;
const float SIXTEENTH = 1.0 / 16.0;
const float THIRTY_SECOND = 1.0 / 32.0;

// The tune to play.
const Note NOTES[] = {
  Note(PITCH_E, 5, QUARTER),
  Note(PITCH_E, 5, QUARTER),
  Note(PITCH_F, 5, QUARTER),
  Note(PITCH_G, 5, QUARTER),
  Note(PITCH_G, 5, QUARTER),
  Note(PITCH_F, 5, QUARTER),
  Note(PITCH_E, 5, QUARTER),
  Note(PITCH_D, 5, QUARTER),
  Note(PITCH_C, 5, QUARTER),
  Note(PITCH_C, 5, QUARTER),
  Note(PITCH_D, 5, QUARTER),
  Note(PITCH_E, 5, QUARTER),
  Note(PITCH_E, 5, QUARTER + EIGHTH),
  Note(PITCH_D, 5, EIGHTH),
  Note(PITCH_D, 5, HALF),
};
const int NOTE_COUNT = sizeof(NOTES) / sizeof(NOTES[0]);

// The temp, in beats per minute, where a single beat is a quarter note.
const float TEMPO = 120.0;

// Sets up the serial port and plays the tune.
void setup() {
  Serial.begin(115200);
  
  for (int i=0; i < NOTE_COUNT; ++i) {
    unsigned long duration, separation;
    NOTES[i].getDuration(TEMPO, duration, separation);

    Serial.print("duration=");
    Serial.print(duration);
    Serial.print(" separation=");
    Serial.print(separation);
    Serial.print(" frequency=");
    Serial.print(NOTES[i].getFrequency());
    Serial.println();
    
    tone(8, NOTES[i].getFrequency());
    delay(duration);

    noTone(8);
    delay(separation);
  }
}

// Does nothing - all work is done in setup().
void loop() {

}

// Creates a note with a given pitch and octave, duration, and style.
// Converts the pitch, within the 4th octave, to the actual frequency
// for the specified octave.
Note::Note(float pitch, int octave, float duration, NoteStyle style) {
  this->frequency = pitch;
  this->duration = duration;
  this->style = style;

  if (octave < 4) {
    while (octave < 4) {
      frequency /= 2.0;
      ++octave;
    }
  } else if (octave > 4) {
    while (octave > 4) {
      frequency *= 2.0;
      --octave;
    }
  }
}

// Gets the note frequency.
int Note::getFrequency() const {
  return round(frequency);
}

// Gets the note duration and separation. The play duration and separation
// duration sum to the total note duration.
void Note::getDuration(float tempo,
		       unsigned long &noteDuration,
		       unsigned long &noteSeparation) const {
  // Duration of a whole note, in milliseconds (4 quarter notes).
  float wholeNoteDuration = 60.0 * 1000.0 / tempo * 4.0;
  unsigned long fullDuration = round(wholeNoteDuration * duration);

  switch (style) {
  case NORMAL:
    // Default to 1/50 of a second = 20ms.
    noteSeparation = 20;
    break;

  case SLURRED:
    noteSeparation = 0;
    break;

  case STACCATO:
    noteSeparation = fullDuration / 2;
    break;
  }
  
  noteDuration = fullDuration - noteSeparation;
}
