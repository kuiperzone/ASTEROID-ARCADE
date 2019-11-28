//---------------------------------------------------------------------------
// PROJECT      : Asteroid Style Game
// COPYRIGHT    : Andy Thomas (C) 2019
// WEB URL      : https://kuiper.zone
// LICENSE      : GPLv3
//---------------------------------------------------------------------------

#include "label.h"

#include "universe.h"
#include "scaled_canvas.h"

using namespace Game;
using namespace Game::Internal;

//---------------------------------------------------------------------------
// CLASS Label : PUBLIC MEMBERS
//---------------------------------------------------------------------------
Label::Label(Universe *owner, const std::string &text, double seconds)
    : GameEntity(owner)
{
    _text = text;
    setMaxSeconds(seconds);
}

std::string Label::text() const
{
    return _text;
}

void Label::setText(const std::string &s)
{
    _text = s;
}

double Label::rem() const
{
    return _rem;
}

void Label::setRem(double rem)
{
    _rem = rem;
}

void Label::draw()
{
    if (!_text.empty() && _rem > 0)
    {
        owner()->canvas()->drawText(position(), AlignHorz::Center, AlignVert::Middle, _rem, _text);
    }
}
