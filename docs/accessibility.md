# Accessibility

> **Status: proposals.** Nothing below is implemented yet. For each category the
> subject lists, this page records what we plan to provide, ordered by how cheap
> it is. The team should confirm what will really be done.

## Physical and motor disabilities

| Solution | Notes |
|---|---|
| Playing with the keyboard only, no mouse | already the case: arrow keys and one fire key |
| **Remappable keys** | stored in a client setting |
| **Toggle fire** (press once to start, once to stop) instead of holding | the logic already treats fire as a held state; the client can turn a toggle into that state |
| Adjustable difficulty | speeds, health and fire rates are constants in `Config.hpp` (`rtype::config`), so a slower mode only changes numbers |
| Gamepad support | optional, through the input layer of the client |

## Audio and visual disabilities

| Solution | Notes |
|---|---|
| **Players are told apart by shape and position, not only by colour** | the four players have a slot 0 to 3, which can select a distinct sprite *and* a marker (number or symbol) |
| Colour-blind friendly palette | avoid red / green as the only difference between friend and enemy bullets |
| High-contrast option | bullets and ships clearly separated from the star-field |
| No fast flashing | avoid strobing explosions |
| Visual cues for sounds | an on-screen hint for events that otherwise only make a sound |
| Independent volume sliders and a mute option | music and effects separate |
| Scalable interface text | readable at a distance |

## Mental and cognitive disabilities

| Solution | Notes |
|---|---|
| Few, simple rules | one enemy type, one attack, one goal per level |
| Clear and consistent feedback | health and hits are visible |
| Slower game option | same mechanism as the difficulty settings above |
| Short levels | levels are short (the boss is planned after 60 s of scrolling) |
| Plain language in menus | short sentences, no jargon |

## Documentation

The documentation follows the same principle:

- plain structured Markdown with headings, so screen readers can navigate it;
- short sentences, English only;
- diagrams are always accompanied by text and tables that carry the same
  information;
- no information given by colour alone.
