//---------------------------------------------------------------------------
// PROJECT      : Asteroid Style Game
// COPYRIGHT    : Andy Thomas (C) 2019
// WEB URL      : https://kuiper.zone
// LICENSE      : GPLv3
//---------------------------------------------------------------------------

#ifndef GAME_LABEL_H
#define GAME_LABEL_H

#include "game_entity.h"

#include <string>

namespace Game { namespace Internal {

//! A floating label entity. It can have motion, but is massless.
class Label : public GameEntity
{
public:

    //! Constructor with text and max seconds.
    Label(Universe *owner, const std::string &text = "", double seconds = 1.0);

    //! The label string to be displayed. Nothing is drawn if empty.
    //! The default is an empty string.
    std::string text() const;
    void setText(const std::string &s);

    //! The text font size expressed in relative units where a value of 1.0
    //! indicates normal default window text size. No text label is drawn if
    //! the valule is 0 or less. The initial value is 0.8.
    double rem() const;
    void setRem(double rem);

    // Overrides
    void draw() override;


    // Overrides
    EntityKind kind() const override { return EntityKind::Label; }

protected:

private:

    std::string _text;
    double _rem {0.8};

};

}} // namespace
#endif

