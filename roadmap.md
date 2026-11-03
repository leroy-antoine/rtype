# Roadmap

Problems found while playing, in the order they will be handled.

| # | Problem | Status |
|---|---|---|
| 1 | Once a map is chosen, there is no way to go back to the map selection menu (Backspace now opens a pause menu, with a way to the map list) | Done |
| 2 | After a defeat the game returns to the map selection instead of restarting the match, as the end screen says | To do |
| 3 | The game is boring overall. New sprites for the bosses and the projectiles will be provided | To do |
| 4 | The death runs are not real death runs. They will be reworked, with a deep look at the gameplay | To do |
| 5 | Almost no obstacles in the normal levels, except during a death run | To do |
| 6 | The boss animations are broken | To do |
| 7 | Everything looks too much alike (levels, enemies, scenery) | To do |
| 8 | The bosses spawn all in a row at the end of the game, while few appear in the levels before. The same boss type comes back all the time: a type must spawn only once in a whole map | To do |

## Notes

- **1**: the host pauses with Backspace; from the pause menu Enter leads
  back to the map list.
- **2**: the end screen announces a restart; the match must restart on the
  same map, and only the pause menu should lead back to the map list.
- **3**: wait for the sprites from the team; the sprite bank must load them
  without code changes elsewhere.
- **4**: rethink the mode from the gameplay: the current gates are only
  a reflex test. This covers the death runs of the DEATH RUNS map and the
  ones of the campaign (levels 10, 15 and 20), which are not improved yet.
- **5**: add rocks, spikes and moving blocks to the regular levels of the
  campaign, with a difficulty that grows.
- **6**: check the frames, the timing and the offsets of every boss sprite.
- **7**: vary the enemy patterns, the colours and the pace between levels.
- **8**: spread the bosses over the whole map instead of stacking them at
  the end, and never use the same boss type twice in a map.
