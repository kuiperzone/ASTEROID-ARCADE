# ASTEROID ARCADE #
ASTEROID ARCADE is a highly playable open source "retro vector" game written in C++ by Andy
Thomas. It requires the Qt Framework.

![ASTEROID ARCADE SCREENSHOT](/SCREENSHOT.png?raw=true "ASTEROID ARCADE SCREENSHOT")

## Running the Game ##
Download the latest "release" of ASTEROID ARCADE from Github (click "release" near the top of the page and download the zip file). This will contain binaries built for Windows. Run the program under the "bin" directory. Because the exe is not signed, Windows may warn you about running the program. Assuming you trust Github and me, click "More Info" and allow the program.

## More About the Game ##
Clearly, ASTEROID ARCADE takes inspiration from the 1979 classic, Atari Asteroids,
although there are some signficant differences (it would not be true to say that it 
is a straight-forward clone). In Atari Asteroids, for example, asteroids do not
collide but simply overlap when they encounter each other. In ASTEROID ARCADE,
however, things are a little more energetic as the game implements a
conservation of momentum rule(*) and, as a result, things have a tendency
to collide and explode.

The game was primarily written for fun and has served me as a way to learn new
programming frameworks. I originally wrote an early version of it around 2003.
I had made the decision to implement the game logic in pure C++ and this made
it easy to port it to different GUI development frameworks at various times
since. An early variant, for example, employed wxWidgets.

This version, called "ASTEROID ARCADE", links against the Qt Framwork. And here
it is. I hope you enjoy it!
 
(*) As a side-note. I was once contacted by a school teacher who was using the
program to teach both programming and the conservation of momentum.

## Source Code Overview ##
The source code for this game can be conceptualised as two
distinct areas:

* The game core
* Main application

The game core is located under the subdirectory "game" and employs the
namespace "Game". This code is not coupled in any way to Qt, but is restricted
to C++11 only. It was design decision to place all internal game complexity
away from the application code under a further "internal" namespace and
subdirectory.

A game is "played" by the Game::Player class. This class presents a relatively
simple interface which is to be polled by the application in order to drive
the game state and to receive user key press input.

Now, because the game code is framework and GDI independent, it does not know
how to render graphics or play sounds. Instead, it declares the method signatures
it needs as a pure virtual class called Game::CanvasInterface and leaves it to
the application level code to implement this. The application is, therefore,
expected to furnish the Game::Player class with a concrete instance of what it
needs to render the game display and play sounds on construction.

Given a simple drawing and sound implementation, and a regular polling call from
a timer, the Game::Player instance takes care of itself and the application need
do little else.

### The Game Universe ###
The Game::Player class presents and rotates intro pages and holds an instance
of Game::Internal::Universe. It is this class which maintains the game state
as a collection of entities and handles their interactions.

All "entities" within the game are derived from a base class called
Game::Internal::GameEntity. It has a couple methods of note:
* GameEntity::crunch()
* GameEntity::advanced()
The crunch() method is where each object determines whether it is in
collision with another and, signicantly, it is where the conservation
of momentum calculation takes place (*). The new velocity is stored until
advance() is called which increments the object's position in the game
space and determines whether it survives to the next iteration. All
game entities have position and velocity. Most entities have mass also,
with those of zero mass considered to be ghost particles which do
not interact. Further subclasses provide for angular momentum and the
ability to "explode".

(*) Side-note. You may notice that UFOs in the game seem to avoid
asteroids and have a limited tendency to flock. It is an override of the
GameEntity::crunch() method, namely Ufo::crunch(), which allows the UFO
the ability to "see" what is around it.

## Other Notes ##
The game does not store state between application launches.

I haven't yet tested building the software under LINUX, but would expect
no significant issues in doing so. Both Qt and the game code are
designed to be cross-platform.

The source code is documented using Doxygen comments. Look for the
"index.html" file under the "doc" directory.

## License ##
ASTEROID ARCADE is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by the
Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

ASTEROID ARCADE is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with ASTEROID ARCADE. If not, see <http://www.gnu.org/licenses/>.

## Credits and Attribution ##
ASTEROID ARCADE features music originally recorded by Seung Hee Oh
and used under a Creative Commons (CC-BY) license. Additionally,
sound effects files originate, I believe, from a game called "Golgotha"
and are in the public domain.
