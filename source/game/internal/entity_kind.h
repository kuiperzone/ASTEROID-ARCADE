//---------------------------------------------------------------------------
// PROJECT      : Asteroid Style Game
// COPYRIGHT    : Andy Thomas (C) 2019
// WEB URL      : https://kuiper.zone
// LICENSE      : GPLv3
//---------------------------------------------------------------------------

#ifndef GAME_ENTITY_KIND_H
#define GAME_ENTITY_KIND_H

namespace Game { namespace Internal {

//! Identifies the GameEntity kind.
enum class EntityKind
{
    Ship, //!< The user ship.
    BigRock,  //!< A big rock.
    MediumRock,  //!< A medium sized rock.
    SmallRock,  //!< A small rock.
    Bullet, //!< Bullet from ship.
    Debris, //!< Short-lived debris.
    Spark, //!< Short-lived sparks associated with debris and ship exhaust.
    Ufo, //!< Alien UFO.
    Label //!< Floating label.
};

}} // namespace
#endif
