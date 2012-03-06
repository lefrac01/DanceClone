// play_logic
//
//TODO: .ini for rating times
#define BOO_MS 180
#define GOOD_MS 135
#define GREAT_MS 90
#define PERFECT_MS 45
#define MARVELLOUS_MS 23

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

#define JUMP_ALLOW_MS 100 // how many milliseconds are allowed between 
  // inputs before they are not considered as valid for a jump
void fart(int boo);
