# Roadmap

Problems found while playing, in the order they will be handled.

**Rule for every gameplay item** (2, 3, 4, 5, 7, 8, 9 and any other): before
changing anything, ask the team for ideas and feedback on how to do it. No
gameplay choice is made alone.

| #  | Problem                                                                                                                                                                                                                                                           | Status |
|----|-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|---|
| 1  | Once a map is chosen, there is no way to go back to the map selection menu (Backspace now opens a pause menu, with a way to the map list)                                                                                                                         | Done |
| 2  | After a defeat the game returns to the map selection instead of restarting the match, as the end screen says                                                                                                                                                      | Done |
| 3  | The game is boring overall. New sprites for the bosses and the projectiles will be provided                                                                                                                                                                       | Bosses done, projectiles waiting for sprites |
| 4  | The death runs are not real death runs. They will be reworked, with a deep look at the gameplay                                                                                                                                                                   | Second pass done (no monster, mixed traps, no waiting after a death), waiting for your feedback |
| 5  | Almost no obstacles in the normal levels, except during a death run                                                                                                                                                                                               | First pass done, waiting for your feedback |
| 6  | The boss animations are broken                                                                                                                                                                                                                                    | To do |
| 7  | Everything looks too much alike (levels, enemies, scenery)                                                                                                                                                                                                        | To do |
| 8  | The bosses spawn all in a row at the end of the game, while few appear in the levels before. The same boss type comes back all the time: a type must spawn only once in a whole map                                                                               | To do |
| 9  | Make the death runs of the campaign (levels 10, 15 and 20) harder                                                                                                                                                                                                 | First pass done, waiting for your feedback |
| 10 | Delete the violet circle animation                                                                                                                                                                                                                                | To do |
| 11 | Les monstres ne doivent plus spawn dans les murs                                                                                                                                                                                                                  | Done |
| 12  | Le boss final doit être super dur, là c'est juste un gros mob qui fait pas bcp de dégâts et qui bouge pas. Limite, vu que le dernier boss c'est le tank je crois, fais en sorte qu'il soit agile comme un dasher, tanky comme un tank, avec des attaques de zones | |
| 13 | Add obstacles to the boss fights, especially the last boss: lasers and a rain of fast diagonal meteors | First pass done, waiting for your feedback |
## Notes

- **1**: the host pauses with Backspace; from the pause menu Enter leads
  back to the map list.
- **2**: a defeat restarts the same map; a victory shows an animated summary
  (deaths, coins, time green if it beats the best time, red otherwise) then
  returns to the lobby.
- **3**: wait for the sprites from the team; the sprite bank must load them
  without code changes elsewhere.
- **4**: rethink the mode from the gameplay: the current gates are only
  a reflex test. This covers the death runs of the DEATH RUNS map and the
  ones of the campaign (levels 10, 15 and 20); the campaign ones are item 9.
- **5**: add rocks, spikes and moving blocks to the regular levels of the
  campaign, with a difficulty that grows.
- **6**: check the frames, the timing and the offsets of every boss sprite.
- **7**: vary the enemy patterns, the colours and the pace between levels.
- **8**: spread the bosses over the whole map instead of stacking them at
  the end, and never use the same boss type twice in a map.
- **9**: raise the difficulty of the three campaign death runs (wall speed,
  closer gates, moving blocks, portals), checked with the reachability
  analysis. Ask for the ideas and the feedback first.

