// play_logic
//
//TODO: .ini for rating times
#define ARROWS_HIT_ANIM_MS 200.0

#define NUM_DIFFICULTIES 5

#define SONG_START_OFFSET 100 // an offset to handle the delay between mp3 
    // start request and the time the sound is actually heard
#define PRE_START_DELAY 3000 // time to prepare before the song starts
#define SONG_ABORT_DELAY 3000   // time abort button(s) held before abort

#define JUMP_ALLOW_MS 100 // how many milliseconds are allowed between 
  // inputs before they are not considered as valid for a jump

float pixels_per_ms_at_1_bpm = 0.0; // calculate once screen height is known
// based on the observed rate of 4000ms per screen height per quarter note
// at 120 BPM

int goal_offset = 0;
int arrow_height = 64;

#define MAX_PLAYERS 1

#define BOO_MS 200
#define GOOD_MS 200
#define GREAT_MS 100
#define PERFECT_MS 50
#define MARVELLOUS_MS 25

#define FREEZE_FAIL_MS 200

#define NOTE_TYPE_HOLD 1              
#define NOTE_TYPE_QUARTER 2           // 1 per beat, 4 per measure
#define NOTE_TYPE_EIGHTH 3            // 2 per beat, 8 per measure
#define NOTE_TYPE_QUARTER_TRIPLET 4   // 3 per beat, 12 per measure
#define NOTE_TYPE_SIXTEENTH 5         // 4 per beat, 16 per measure
#define NOTE_TYPE_EIGHTH_TRIPLET 6    // 6 per beat, 24 per measure
#define NOTE_TYPE_THIRTYSECOND 7      // 8 per beat, 32 per measure
#define NOTE_TYPE_SIXTEENTH_TRIPLET 8 // 12 per beat, 48 per measure
#define NOTE_TYPE_SIXTYFOURTH 9       // 16 per beat, 64 per measure
#define NOTE_TYPE_THIRTYSECOND_TRIPLET 10  // 24 per beat, 96 per measure
#define NOTE_TYPE_SIXTYFOURTH_TRIPLET 11   // 48 per beat, 192 per measure

#define RATING_NONE -1
#define RATING_MISS 0
#define RATING_GOOD 1
#define RATING_GREAT 2
#define RATING_PERFECT 3
#define RATING_MARVELLOUS 4

#define FREEZE_RATING_NONE -1
#define FREEZE_RATING_FAILED 0
#define FREEZE_RATING_OK 1

#define FREEZE_LENGTH_ALLOW 25 // how many pixels before the end of a freeze
 // arrow the player is allowed to lift off without failing the freeze
