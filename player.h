#pragma once
#include <Arduino.h>

class Player;
extern Player GPlayer;

#define DEFAULT_DIVIDER 4
#define DEFAULT_OCTAVE 5
#define DEFAULT_TEMPO 60

enum Frequencies : byte
{
    C   =   244,  /* C     */
    CD  =   230,  /* C#,Db */
    D   =   217,  /* D     */
    DE  =   205,  /* D#,Eb */
    E   =   193,  /* E     */
    F   =   182,  /* F     */
    FG  =   172,  /* F#,Gb */
    G   =   162,  /* G     */
    GA  =   153,  /* G#,Ab */
    A   =   144,  /* A     */
    AB  =   136,  /* A#,Bb */
    B   =   129,  /* B     */
    PP  =   000,  /* Pause */
};

struct Note
{
    const byte frequency;
    const byte divider;   // 1,2,4,8,16,32,64,128
    const byte octave;    // 2,3,4,5,6,7

public:
    Note(const byte frequency, const byte divider = DEFAULT_DIVIDER, const byte octave = DEFAULT_OCTAVE)
        : frequency(frequency), divider(divider), octave(octave)
    {
    }
};

class Player
{
    enum State
    {
        DonePlaying,
        ReadyToPlay,
        BusyPlaying,
        NStates
    };

    static constexpr bool OFF = false;
    const Note* _notes;
    unsigned long _fullNoteDuration, _duration, _millis;
    int _beg, _end, _prescaler, _frequency;
    State _state;

public: 
    Player();

    static void setup();
    static void loop(unsigned long millis);

    void CheckState(unsigned long millis);
    void SetTempo(int tempo);
    bool IsBusy() const;
    
    static void SetBuzzerState(bool on);

    static void PlayLevelHighFX();
    static void PlayLevelLowFX();

    template<typename T, int size>
    void Play(T(&array)[size], int tempo = DEFAULT_TEMPO)
    {
        if (IsBusy())
            return;
        
        _notes = array;
        _beg = 0;
        _end = size;
        _state = _beg < _end ? ReadyToPlay : DonePlaying;
        
        SetTempo(tempo);
    }
};
