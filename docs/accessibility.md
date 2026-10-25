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
| No fast flashing | proposal | avoid strobing explosions (they last 0.4 s) |
| Independent volume sliders and a mute option | proposal | music and effects separate; the game also runs without any sound |
| Scalable interface text | proposal | readable at a distance |

## Mental and cognitive disabilities

| Solution | State | Notes |
|---|---|---|
| Few, simple rules | **done** | one enemy type, one boss, one goal per level |
| Clear and consistent feedback | **done** | health bars, sounds for hits and explosions, a message on victory or defeat |
| Slower game option | proposal | same mechanism as the difficulty settings above |
| Short levels | **done** | the boss arrives after 20 s of play |
| Plain language in menus | proposal | there is no menu yet |

## Documentation

The documentation follows the same principle:

- plain structured Markdown with headings, so screen readers can navigate it;
- short sentences, English only;
- diagrams are always accompanied by text and tables that carry the same
  information;
- no information given by colour alone.
