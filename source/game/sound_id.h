//---------------------------------------------------------------------------
// PROJECT      : Asteroid Style Game
// COPYRIGHT    : Andy Thomas (C) 2019
// WEB URL      : https://kuiper.zone
// LICENSE      : GPLv3
//---------------------------------------------------------------------------

#ifndef GAME_SOUND_ID_H
#define GAME_SOUND_ID_H

namespace Game {

//! Identifies a game related sound.
enum class SoundId
{
    IntroMusic, //!< Short intro music. This should be approx 5 to 10 seconds.
    Start, //!< Short "start" sound (new life).
    BigExplosion, //!< Big explosion sound.
    MediumExplosion, //!< Medium explosion sound.
    SmallExplosion, //!< Small explosion sound.
    Thrust, //!< Thrust sound..
    GunFire, //!< Gun fire sound.
    Count,  //!< Count only. Not a valid ID.
};

} // namespace
#endif
