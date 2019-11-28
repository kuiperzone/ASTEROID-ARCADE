//---------------------------------------------------------------------------
// PROJECT      : Asteroid Style Game
// COPYRIGHT    : Andy Thomas (C) 2019
// WEB URL      : https://kuiper.zone
// LICENSE      : GPLv3
//---------------------------------------------------------------------------

#ifndef GAME_KEY_ID_H
#define GAME_KEY_ID_H

namespace Game {

//! Identifies the game related key input.
enum class KeyId
{
    Left, //!< Rotate left. Map to LEFT_ARROW or 'J'.
    Right, //!< Rotate right. Map to RIGHT_ARROW or 'L'.
    Thrust, //!< Thrust. Map to UP_ARROW or 'I'.
    Fire, //!< Fire gun (also start game). Map to SPACE or CTRL.
    Quit, //!< Quit game. Map to 'Q'.
    Pause, //!< Pause/resume. Map to 'P'.
    Sound, //!< Toggle sound. Map to 'S'.
    Count, //!< Item count. Not a valid ID.
};

} // namespace
#endif
