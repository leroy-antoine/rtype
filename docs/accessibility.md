# Accessibility

> **Status.** For each category the subject lists, this page records what the
> game provides today and what is only a proposal. The column *State* says
> which is which.

## Physical and motor disabilities

| Solution | State | Notes |
|---|---|---|
| Playing with the keyboard only, no mouse | **done** | arrow keys and one fire key, Enter to come back |
| **Remappable keys** | proposal | stored in a client setting; keys are fixed in `Controls.cpp` for now |
| **Toggle fire** (press once to start, once to stop) instead of holding | proposal | the logic already treats fire as a held state; the client can turn a toggle into that state |
| Adjustable difficulty | proposal | speeds, health and fire rates are constants in `Config.hpp` (`rtype::config`), so a slower mode only changes numbers |
| Gamepad support | proposal | optional, through the input layer of the client |

## Audio and visual disabilities

| Solution | State | Notes |
|---|---|---|
| **Players are told apart by something other than colour alone** | partly done | each slot 0 to 3 has its own sprite row; a number or marker above the ship is a proposal |
| Sounds have a visual counterpart | **done** for the important ones | health bars, a boss bar, and end-of-game messages on screen |
| Colour-blind friendly palette | proposal | avoid red / green as the only difference between friend and enemy bullets |
| High-contrast option | proposal | bullets and ships clearly separated from the star-field |
| No fast flashing | **done** for the scenery | the backgrounds and the boss auras only drift and pulse slowly; explosions still last 0.4 s |
| Independent volume sliders and a mute option | proposal | music and effects separate; the game also runs without any sound |
| Scalable interface text | proposal | readable at a distance |

## Mental and cognitive disabilities

| Solution | State | Notes |
|---|---|---|
| Few, simple rules | **done** | one goal (reach the end of the map), a handful of enemy types introduced one after the other, the same controls everywhere |
| Clear and consistent feedback | **done** | health bars, sounds for hits and explosions, a message on victory or defeat; a rocket launcher stops before it fires, which gives a visual warning |
| Slower game option | proposal | same mechanism as the difficulty settings above |
| Short levels | **done** | the first level lasts about 20 s, the *boss rush* map has short levels, and killing monsters shortens a level |
| Plain language in menus | **done** for the lobby | `SELECT A MAP`, a highlighted cursor and a one-line instruction at the bottom of the screen; there is no other menu |

## Documentation

The documentation follows the same principle:

- plain structured Markdown with headings, so screen readers can navigate it;
- short sentences, English only;
- diagrams are always accompanied by text and tables that carry the same
  information;
- no information given by colour alone.
