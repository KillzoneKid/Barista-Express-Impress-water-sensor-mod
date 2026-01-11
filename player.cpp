#include "player.h"

const Note LevelHighFX[]
{
   Note(G,32,6),
   Note(B,32,6),
   Note(G,32,6),
   Note(B,32,6),
   Note(G,32,6),
   Note(B,32,6),
};

const Note LevelLowFX[]
{
   Note(G,64,5),
   Note(B,64,5),
   Note(G,32,6),
   Note(B,32,6),
   Note(G,64,5),
   Note(B,64,5),
   Note(G,32,6),
   Note(B,32,6),
};

Player::Player()
    : _notes(nullptr), _beg(0), _end(0), _duration(0), _millis(0), _prescaler(0), _frequency(0), _state(DonePlaying)
{
    SetTempo(DEFAULT_TEMPO);
}

void Player::setup()
{
    PORTB &= ~(1 << PB4);
    DDRB |= 1 << PB4;
    TCCR1 |= 1 << CTC1;
}

void Player::loop(unsigned long millis)
{
    GPlayer.CheckState(millis);
}

void Player::CheckState(unsigned long millis)
{
    switch (_state)
    {
    case BusyPlaying:

        if (millis - _millis < _duration)
            return;

        Player::SetBuzzerState(OFF);

        if (_beg == _end)
        {
            _beg = _end = 0;
            _state = DonePlaying;
            return;
        }

    case ReadyToPlay:
    {
        _millis = millis;
        _state = BusyPlaying;

        const Note& note = _notes[_beg++];

        _frequency = note.frequency;
        Player::SetBuzzerState(_frequency > 0);

        _duration = _fullNoteDuration / (note.divider > 0 ? note.divider : DEFAULT_DIVIDER);
        _prescaler = F_CPU / 1000000UL; // support for 1MHz, 8MHz, 16MHz

        switch (note.octave)
        {
        case  2: _prescaler *= 32; break;
        case  3: _prescaler *= 16; break;
        case  4: _prescaler *= 8; break;
        case  5: _prescaler *= 4; break;
        case  6: _prescaler *= 2; break;
        case  7:
        default:
            break;
        }

        TCCR1 &= 0b11110000;
        switch (_prescaler)
        {
        case 512: TCCR1 |= 0b00001010; break;
        case 256: TCCR1 |= 0b00001001; break;
        case 128: TCCR1 |= 0b00001000; break;
        case  64: TCCR1 |= 0b00000111; break;
        case  32: TCCR1 |= 0b00000110; break;
        case  16: TCCR1 |= 0b00000101; break;
        case   8: TCCR1 |= 0b00000100; break;
        case   4: TCCR1 |= 0b00000011; break;
        case   2: TCCR1 |= 0b00000010; break;
        case   1: TCCR1 |= 0b00000001; break;
        default:
            break;
        }

        OCR1C = _frequency;
    }
    }
}

void Player::SetTempo(int tempo)
{
    _fullNoteDuration = 240000 / (tempo > 0 ? tempo : DEFAULT_TEMPO);
}

bool Player::IsBusy() const
{
    return _state > DonePlaying;
}

void Player::SetBuzzerState(bool on)
{
    GTCCR |= 1 << (on ? COM1B0 : COM1B1);
    GTCCR &= ~(1 << (on ? COM1B1 : COM1B0));
}

void Player::PlayLevelHighFX()
{
    GPlayer.Play(LevelHighFX, 120);
}

void Player::PlayLevelLowFX()
{
    GPlayer.Play(LevelLowFX, 120);
}

Player GPlayer;



