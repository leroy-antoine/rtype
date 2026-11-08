# Graph Report - RTYPE  (2026-11-08)

## Corpus Check
- 220 files · ~170,439 words
- Verdict: corpus is large enough that graph structure adds value.
- Unclassified: 5 file(s) not represented in the graph (top: (none) 5)

## Summary
- 3836 nodes · 8226 edges · 199 communities (163 shown, 36 thin omitted)
- Extraction: 87% EXTRACTED · 13% INFERRED · 0% AMBIGUOUS · INFERRED: 1082 edges (avg confidence: 0.86)
- Token cost: 0 input · 0 output

## Graph Freshness
- Built from commit: `661a6fde`
- Run `git rev-parse HEAD` and compare to check if the graph is stale.
- Run `graphify update .` after code changes (no API cost).

## Community Hubs (Navigation)
- src/App.cpp
- IntRect
- string
- Components.hpp
- Background.cpp
- Client Connection
- vector
- Component.hpp
- Delivery and Sessions
- Build and Protocol Docs
- Server Events
- Level
- Game Tests
- Components.hpp
- TEST
- Connection Channel
- Game
- Address
- .updatePlaying
- Hud
- Entity State Payload
- Prediction
- Binary Writer
- TestHarness.hpp
- Pending Messages
- Replica
- BinaryReader
- DeathRuns.hpp
- GameView
- Collider
- ShopPanel
- LeviathanState
- Audio Player Impl
- TEST
- Registry
- SpriteBank.cpp
- Design Choices Docs
- Audio Player
- AugmentStyle.hpp
- Network
- Build Tooling Docs
- EntitySpawned
- Reliable Channel
- Shop.hpp
- Zone
- Phase
- Network protocol doc
- Architecture Targets
- HudLobby.cpp
- TEST
- Segments
- Key
- addGates
- WaveStep
- map3
- Vec2Pos
- SpriteId
- Window::Window
- SpriteBank
- TEST
- AudioPlayer::Impl
- Background
- Message Types
- CustomInfo
- BossKind
- Controls.cpp
- describeSpawn
- SpriteId
- Obstacles.hpp
- Connection.cpp
- Client.cpp
- Color
- SpriteBank
- EntityState
- BackdropId
- BinaryReader
- Window Impl Header
- LobbyMenu
- TEST
- Phase
- TEST
- Systems.hpp
- Sound Manager Impl
- Reader
- MonsterDef
- TEST
- Connection Send
- UdpSocket
- .update
- DisconnectReason
- TEST
- GameStatus
- Create your own map
- Hud::drawSettingsStep
- RunRecord
- Zone
- .operator()
- Track
- CLAUDE.md
- .isUsedSlot
- LevelDef
- cstddef
- App
- UdpSocket::Impl
- map1
- buildRun
- ObstacleStep
- Audio::play
- App::run
- vcpkg.json
- window/Event.hpp
- Gameplay
- Archetype
- The reader: `rtype::mapjson`
- ShopPanel.cpp
- BossHazards.hpp
- formatTime
- .width
- TEST
- CustomMaps
- Piece
- GameView.cpp
- game/App.cpp
- LifeBook
- TEST
- StrikeSpec
- isValidDifficulty
- LeviathanMove
- LaserCycle
- Signature
- CustomMapList
- AugmentId
- Aura
- PlayerInput
- Case
- collectMonsters
- MapUpload
- TEST
- TEST
- parseLevel
- CustomLeaderboard
- Resized
- Reader.hpp
- WeaponKind
- Regeneration
- convert
- Drone.hpp
- App::useCustomStep
- Message
- CoinCounter
- CustomMapList.cpp
- time_point
- ObstacleMotion
- Renderer.cpp
- Server App class (only place including both network and game)
- CustomMap
- checkTotals
- Hud::drawPlayers
- Folder.cpp
- Packet
- DifficultyScale
- size_t
- Sample
- RarityOdds
- Build and run doc
- FixedTimestep
- Session
- TextureManager::Impl
- Hud::drawCustomStep
- Streak
- HudVictory.cpp
- ShopSound
- .startMap
- MonsterState
- wrap
- Audio::Audio
- Shield
- BulletLook
- TurretFire
- CustomMaps/README.md
- Star
- Area
- BossTag
- DroneBody
- NetId
- Pierce
- Weapon
- isPrintableText

## God Nodes (most connected - your core abstractions)
1. `Registry` - 247 edges
2. `TEST()` - 202 edges
3. `Entity` - 187 edges
4. `Game` - 159 edges
5. `Replica` - 108 edges
6. `TEST()` - 85 edges
7. `Hud` - 71 edges
8. `AugmentId` - 60 edges
9. `TEST()` - 59 edges
10. `App` - 56 edges

## Surprising Connections (you probably didn't know these)
- `What you need` --references--> `CustomMaps`  [INFERRED]
  docs/custom-maps.md → src/server/game/CustomMaps.hpp
- `Your first map in two minutes` --references--> `CustomMaps`  [INFERRED]
  docs/custom-maps.md → src/server/game/CustomMaps.hpp
- `Add a field` --references--> `Reader`  [INFERRED]
  docs/custom-maps-internals.md → src/client/game/Rtype/src/mapjson/Reader.hpp
- `Add a kind of obstacle` --references--> `kindOf()`  [INFERRED]
  docs/custom-maps-internals.md → src/client/game/Rtype/src/mapjson/ObstacleParser.cpp
- `Decisions already made` --references--> `applyDamages()`  [INFERRED]
  roadmap.md → src/client/game/Rtype/include/logic/Damages.hpp

## Import Cycles
- None detected.

## Communities (199 total, 36 thin omitted)

### Community 0 - "src/App.cpp"
Cohesion: 0.09
Nodes (8): App::App(), App::currentSettings(), App::isReady(), App::listening(), App::playSounds(), App::pressed(), App::sendReady(), App::useShop()

### Community 1 - "IntRect"
Cohesion: 0.15
Nodes (14): Animation, firstFrame, frameCount, frameDuration, step, IntRect, height, left (+6 more)

### Community 2 - "string"
Cohesion: 0.12
Nodes (6): Window, _impl, handle(), App::handleBack(), App::loop(), App::run()

### Community 3 - "Components.hpp"
Cohesion: 0.10
Nodes (28): Charge, time, Damage, value, DroneLevel, level, Health, hp (+20 more)

### Community 4 - "Background.cpp"
Cohesion: 0.11
Nodes (18): Renderer, Background::drawAura(), Background::drawBand(), Background::drawBars(), Background::drawDebris(), Background::drawFade(), Background::drawNebula(), Background::drawSky() (+10 more)

### Community 5 - "Client Connection"
Cohesion: 0.06
Nodes (15): Client, _attempts, _connection, _events, _handshakeSequence, _lastAttempt, _server, _socket (+7 more)

### Community 6 - "vector"
Cohesion: 0.11
Nodes (5): SoundManager::Impl, buffers, ids, SoundManager::load(), TextureManager::load()

### Community 8 - "Delivery and Sessions"
Cohesion: 0.09
Nodes (21): Refused, reason, Delivery, Reliable, Unreliable, RefuseReason, Full, Version (+13 more)

### Community 9 - "Build and Protocol Docs"
Cohesion: 0.23
Nodes (13): engine_audio library, SFML::Audio, engine build root, engine_core library, asio::asio, engine_network library, engine_window library, SFML::Graphics (+5 more)

### Community 10 - "Server Events"
Cohesion: 0.05
Nodes (13): _messages, Server, _events, _maxClients, _nextClientId, _random, _sessions, _socket (+5 more)

### Community 11 - "Level"
Cohesion: 0.40
Nodes (4): Level, Scenery and death runs, length(), normalize()

### Community 14 - "TEST"
Cohesion: 0.09
Nodes (13): chargeFactor(), chargeRatio(), chargeSize(), chargeSystem(), createChargedBullet(), growBullet(), isCharging(), releaseShot() (+5 more)

### Community 15 - "Connection Channel"
Cohesion: 0.07
Nodes (11): Connection, _channel, _lastPing, _lastReceived, _outbox, _pingId, _pingSentAt, _received (+3 more)

### Community 16 - "Game"
Cohesion: 0.04
Nodes (35): resetCharges(), countBefore(), Game, _alive, _augments, _coinCounter, _countdown, _customZones (+27 more)

### Community 17 - "Address"
Cohesion: 0.14
Nodes (12): Address, ip, port, toAddress(), toEndpoint(), UdpSocket::Impl::Impl(), UdpSocket::Impl::store(), UdpSocket::port() (+4 more)

### Community 18 - ".updatePlaying"
Cohesion: 0.04
Nodes (25): Replica, _augments, _buyResult, CLOCK_CORRECTION_RATE, _cues, _customInfo, _customRecords, _entities (+17 more)

### Community 19 - "Hud"
Cohesion: 0.07
Nodes (23): Hud, AUGMENT_GAP, AUGMENT_PADDING, AUGMENT_TEXT_SCALE, BAR_HEIGHT, BAR_WIDTH, BAR_X, BOSS_BAR_WIDTH (+15 more)

### Community 20 - "Entity State Payload"
Cohesion: 0.13
Nodes (34): decodeAugments(), decodeBuy(), decodeBuyResult(), decodeDestroy(), decodeHistory(), decodeInput(), decodeOwnState(), decodePause() (+26 more)

### Community 21 - "Prediction"
Cohesion: 0.07
Nodes (16): Pending, dt, input, sequence, Prediction, _box, _input, MAX_PENDING (+8 more)

### Community 22 - "Binary Writer"
Cohesion: 0.09
Nodes (17): BinaryWriter, _data, MAX_VARINT, BinaryWriter::writeBytes(), BinaryWriter::writeFloat(), BinaryWriter::writeU16(), BinaryWriter::writeU32(), BinaryWriter::writeU8() (+9 more)

### Community 23 - "TestHarness.hpp"
Cohesion: 0.11
Nodes (3): main(), parsePort(), parsePort()

### Community 24 - "Pending Messages"
Cohesion: 0.11
Nodes (8): Pending, message, sentAt, ReliableChannel, _early, _expected, _nextId, _pending

### Community 25 - "Replica"
Cohesion: 0.07
Nodes (18): GameView, ANIMATION_PHASE, _background, _hud, HURT_DURATION, HURT_SHAKE, _hurtUntil, _lastBossHp (+10 more)

### Community 26 - "BinaryReader"
Cohesion: 0.20
Nodes (8): BinaryReader::BinaryReader(), BinaryReader::readBytes(), BinaryReader::readFloat(), BinaryReader::readU16(), BinaryReader::readU32(), BinaryReader::readU8(), BinaryReader::readVarint(), BinaryReader::remaining()

### Community 27 - "DeathRuns.hpp"
Cohesion: 0.16
Nodes (21): addCourse(), addCoursePortal(), checkPattern(), Course, after, gapFrom, gapTo, motion (+13 more)

### Community 28 - "GameView"
Cohesion: 0.08
Nodes (16): App, _assets, _connectedSince, _controls, _custom, FRAMERATE, _last, LISTEN_DELAY (+8 more)

### Community 29 - "Collider"
Cohesion: 0.11
Nodes (38): applyContact(), canHit(), canStrike(), centerOfBox(), collectProjectiles(), collectTargets(), Collider, box (+30 more)

### Community 30 - "ShopPanel"
Cohesion: 0.06
Nodes (24): ShopPanel, BORDER, CARD_GAP, CARD_HEIGHT, CARD_PADDING, CARD_TOP, CARD_WIDTH, COIN_GAP (+16 more)

### Community 31 - "LeviathanState"
Cohesion: 0.16
Nodes (19): LeviathanState, move, step, swing, timer, aimVelocity(), bossStopX(), horizontalVelocity() (+11 more)

### Community 32 - "Audio Player Impl"
Cohesion: 0.18
Nodes (4): AudioPlayer::play(), AudioPlayer::playMusic(), AudioPlayer::setMusicVolume(), toSfmlVolume()

### Community 33 - "TEST"
Cohesion: 0.07
Nodes (4): escortFor(), escortSize(), TEST(), with()

### Community 34 - "Registry"
Cohesion: 0.03
Nodes (93): add(), CommandBuffer, _commands, create(), Entity, id, EntityManager, _freeList (+85 more)

### Community 35 - "SpriteBank.cpp"
Cohesion: 0.13
Nodes (5): SpriteBank::add(), SpriteBank::addBackdrop(), SpriteBank::loadBackdrops(), SpriteBank::loadMisc(), SpriteBank::texture()

### Community 36 - "Design Choices Docs"
Cohesion: 0.17
Nodes (16): Accessibility doc, Audio/visual accessibility (health bars, per-slot sprites; palette, contrast proposed), Cognitive accessibility (simple rules, short levels, clear feedback), rtype::config (Config.hpp constants), Motor accessibility (keyboard only done; remap, toggle fire, gamepad proposed), Choice: per-message reliability, Choice: event-based replication with NetId, Choice: sessions with token, ping, 5s timeout (+8 more)

### Community 38 - "AugmentStyle.hpp"
Cohesion: 0.40
Nodes (11): augmentInfo(), augmentLabel(), augmentName(), droneInfo(), percentLess(), rarityName(), skillInfo(), statInfo() (+3 more)

### Community 39 - "Network"
Cohesion: 0.07
Nodes (12): Unreachable, applyDecoded(), Network, CHUNKS_PER_UPDATE, _client, Network::Network(), _open, _upload (+4 more)

### Community 40 - "Build Tooling Docs"
Cohesion: 0.15
Nodes (15): vcpkg submodule (external/vcpkg, manifest vcpkg.json, Asio + SFML 3), Technical and comparative study doc, Choice: AABB collision, Choice: hand-written binary big-endian encoding, Choice: CMake with presets, Choice: C++20, Choice: collect first, act after loop (deferred structural changes), Long-term monitoring (pinned vcpkg baseline, TODO CI/sanitizers/fuzzing) (+7 more)

### Community 41 - "EntitySpawned"
Cohesion: 0.08
Nodes (25): EntityDestroyed, id, EntitySpawned, box, id, pos, slot, sprite (+17 more)

### Community 42 - "Reliable Channel"
Cohesion: 0.23
Nodes (5): ReliableChannel::acknowledge(), ReliableChannel::receive(), ReliableChannel::resend(), ReliableChannel::send(), ReliableChannel::takeReady()

### Community 43 - "Shop.hpp"
Cohesion: 0.11
Nodes (20): AugmentDef, apply, id, price, rarity, Rarity, Common, Epic (+12 more)

### Community 44 - "Zone"
Cohesion: 0.18
Nodes (9): Who knows whom, crowded(), scatter(), scatterLasers(), scatterLimit(), shapesOf(), shapeWaves(), customZone() (+1 more)

### Community 45 - "Phase"
Cohesion: 0.09
Nodes (14): Audio, _dir, _enabled, _ids, _phase, _player, SHOP_SOUND_COUNT, _shopIds (+6 more)

### Community 46 - "Network protocol doc"
Cohesion: 0.28
Nodes (12): Architecture doc, Contributing doc, Branch prefixes and commit types, Coding conventions (80 cols, 25-line functions, 5 params, 2 nested ifs, no ternary, _ private prefix), Part ownership (engine/core, server, client, game logic each have an owner), Network protocol doc, engine/docs/network.md (transport layer description), Transport layer header (magic 0x5254, sequence, token) and transport message types 0x00-0x06 (+4 more)

### Community 47 - "Architecture Targets"
Cohesion: 0.15
Nodes (17): r-type_client (display executable), Client replica of the world (entity kind + position), engine::audio (sounds and music, wraps SFML), engine::core (Registry, Scheduler, CommandBuffer, FixedTimestep), engine::network (UDP transport, sessions, ping, reliability, wraps Asio), engine::window (rendering, wraps SFML), Three decoupled layers: rendering, networking, game logic, rtype::common (shared headers: Vec2, return codes) (+9 more)

### Community 48 - "HudLobby.cpp"
Cohesion: 0.16
Nodes (8): firstVisible(), Hud::drawCustomList(), Hud::drawCustomRow(), Hud::drawLines(), Hud::drawNoCustomScores(), Hud::drawTabs(), Hud::settingsMapName(), labelOf()

### Community 50 - "Segments"
Cohesion: 0.10
Nodes (18): firstLevel(), isDeathRun(), lastSpawnPoint(), levelAt(), levelEnd(), levelSpan(), levelStart(), levelStartOf() (+10 more)

### Community 51 - "Key"
Cohesion: 0.10
Nodes (21): Key, A, Backspace, C, D, Down, E, Enter (+13 more)

### Community 52 - "addGates"
Cohesion: 0.11
Nodes (28): addCave(), addGates(), addRun(), addSweepers(), CavePiece, bottom, top, Gap (+20 more)

### Community 53 - "WaveStep"
Cohesion: 0.05
Nodes (43): MonsterType, kind, EscortSpawn, delay, kind, y, fittedBase(), fittedSpacing() (+35 more)

### Community 54 - "map3"
Cohesion: 0.54
Nodes (6): gauntlet(), map3(), minefield(), pinch(), rhythm(), run()

### Community 55 - "Vec2Pos"
Cohesion: 0.11
Nodes (21): dasherTarget(), DashTarget, pos, velocity, roamBoss(), bulletStartPos(), isShooter(), createBullet() (+13 more)

### Community 56 - "SpriteId"
Cohesion: 0.12
Nodes (7): Controls, _back, _held, _input, _pause, _pressed, _respawn

### Community 58 - "SpriteBank"
Cohesion: 0.09
Nodes (11): explosionCue(), grown(), Replica::applyPrediction(), Replica::bossSprite(), Replica::localPlayer(), Replica::mapCursor(), Replica::record(), Replica::recordSpawnCue() (+3 more)

### Community 59 - "TEST"
Cohesion: 0.06
Nodes (13): createPlayer(), secondarySystem(), beams(), holdX(), laserShip(), releaseX(), tankAt(), TEST() (+5 more)

### Community 60 - "AudioPlayer::Impl"
Cohesion: 0.12
Nodes (15): AudioPlayer::Impl, music, musicVolume, next, soundVolume, voices, Sound, BossAlert (+7 more)

### Community 61 - "Background"
Cohesion: 0.09
Nodes (17): Background, _aura, _auraLevel, _boosted, _fade, _height, _last, LAYERS (+9 more)

### Community 62 - "Message Types"
Cohesion: 0.22
Nodes (5): Message, id, payload, reliable, type

### Community 63 - "CustomInfo"
Cohesion: 0.12
Nodes (24): Replica::takeUploadResult(), CustomInfo, hash, name, zones, CustomRecords, hash, records (+16 more)

### Community 64 - "BossKind"
Cohesion: 0.20
Nodes (10): BossType, kind, BossKind, Count, Dasher, Fortress, Standard, Summoner (+2 more)

### Community 66 - "describeSpawn"
Cohesion: 0.12
Nodes (10): Packet::add(), Packet::isHandshake(), Packet::isValid(), Packet::Packet(), Packet::parse(), Packet::parseMessage(), Packet::sequence(), Packet::size() (+2 more)

### Community 67 - "SpriteId"
Cohesion: 0.05
Nodes (41): isBossSprite(), Renderable, id, SpriteId, AlienShard, BeamOn, BeamWarn, Bomb (+33 more)

### Community 68 - "Obstacles.hpp"
Cohesion: 0.06
Nodes (48): spawnBossLaser(), coinPickupSystem(), collectCoins(), touchesAny(), PlayerEffects, inverted, lastPortal, speedFactor (+40 more)

### Community 69 - "Connection.cpp"
Cohesion: 0.13
Nodes (13): Connection::Connection(), Connection::handlePong(), Connection::handleUser(), Connection::hasHandshakeMessage(), Connection::latency(), Connection::nextSequence(), Connection::peerDisconnect(), Connection::readId() (+5 more)

### Community 70 - "Client.cpp"
Cohesion: 0.07
Nodes (20): DisconnectReason, Kicked, Quit, Shutdown, Timeout, Client::checkConnection(), Client::Client(), Client::close() (+12 more)

### Community 71 - "Color"
Cohesion: 0.09
Nodes (20): Color, a, b, g, r, Font, GLYPH_HEIGHT, GLYPH_WIDTH (+12 more)

### Community 72 - "SpriteBank"
Cohesion: 0.10
Nodes (9): TextureManager, _impl, SpriteBank, _assets, _backdrops, _clock, _sprites, Background::Background() (+1 more)

### Community 73 - "EntityState"
Cohesion: 0.22
Nodes (10): EntityState, id, pos, encodeShop(), encodeStateChunk(), encodeStates(), StatesSnapshot, states (+2 more)

### Community 74 - "BackdropId"
Cohesion: 0.13
Nodes (15): BackdropId, Count, Girders, NebulaBlue, NebulaGreen, NebulaOrange, NebulaPurple, NebulaRed (+7 more)

### Community 75 - "BinaryReader"
Cohesion: 0.14
Nodes (10): BinaryReader, _cursor, PlayerStatus, alive, hp, lives, ready, slot (+2 more)

### Community 77 - "LobbyMenu"
Cohesion: 0.08
Nodes (16): Client side, LobbyMenu, _draft, _forCustom, _scoresFromCustom, _shownRank, LobbyStep, Custom (+8 more)

### Community 79 - "Phase"
Cohesion: 0.29
Nodes (7): Phase, Defeat, Lobby, Playing, Shop, Victory, Hud::trackPhase()

### Community 80 - "TEST"
Cohesion: 0.07
Nodes (4): applyAugment(), findAugment(), TEST(), TEST()

### Community 81 - "Systems.hpp"
Cohesion: 0.06
Nodes (34): bossSystem(), fan(), volley(), createBolt(), centerOf(), dive(), lockOn(), calculatePos() (+26 more)

### Community 82 - "Sound Manager Impl"
Cohesion: 0.25
Nodes (3): SoundManager, _impl, AudioPlayer::AudioPlayer()

### Community 83 - "Reader"
Cohesion: 0.19
Nodes (9): Range, max, min, readName(), Reader, _json, _path, Reader::Reader() (+1 more)

### Community 84 - "MonsterDef"
Cohesion: 0.11
Nodes (18): MonsterDef, damage, fireCooldown, height, hp, projectile, speed, sprite (+10 more)

### Community 85 - "TEST"
Cohesion: 0.14
Nodes (3): folder(), TEST(), write()

### Community 86 - "Connection Send"
Cohesion: 0.38
Nodes (3): Connection::flush(), Connection::send(), Connection::sendEngine()

### Community 88 - ".update"
Cohesion: 0.15
Nodes (5): collectStates(), collectUntracked(), run(), run(), skipToBoss()

### Community 89 - "DisconnectReason"
Cohesion: 0.18
Nodes (10): Cue, BossAppears, BossExplosion, CoinPickup, Explosion, Hit, PlayerShot, Purchase (+2 more)

### Community 90 - "TEST"
Cohesion: 0.09
Nodes (7): failsWith(), freshFolder(), mapOf(), replaced(), TEST(), tinyLevel(), write()

### Community 91 - "GameStatus"
Cohesion: 0.09
Nodes (17): GameStatus, bossHp, bossMaxHp, coins, countdown, deaths, elapsedMs, level (+9 more)

### Community 92 - "Create your own map"
Cohesion: 0.13
Nodes (15): A complete example, Bosses, Create your own map, Distances and time, How a map is built, Limits, Play it, The end of a map (+7 more)

### Community 93 - "Hud::drawSettingsStep"
Cohesion: 0.11
Nodes (12): difficultyColor(), difficultyName(), multiplierText(), entryName(), Hud::drawMapList(), Hud::drawRecords(), Hud::drawRunRow(), Hud::drawScaleSummary() (+4 more)

### Community 94 - "RunRecord"
Cohesion: 0.11
Nodes (12): addRecord(), Leaderboard, _history, _records, RunEntry, map, record, RunRecord (+4 more)

### Community 95 - "Zone"
Cohesion: 0.12
Nodes (16): Scene, aura, boosted, zone, Zone, Arena, Asteroids, Core (+8 more)

### Community 96 - ".operator()"
Cohesion: 0.40
Nodes (5): Install, Keep it up to date: the git hook, Knowledge graph (graphify), Use the graph, With an AI agent (Claude Code)

### Community 97 - "Track"
Cohesion: 0.11
Nodes (16): Track, BossDasher, BossFortress, BossStandard, BossSummoner, Core, Count, DeathRun (+8 more)

### Community 98 - "CLAUDE.md"
Cohesion: 0.12
Nodes (16): HistoryPage, entries, start, total, StatusHeader, bossHp, bossMaxHp, coins (+8 more)

### Community 99 - ".isUsedSlot"
Cohesion: 0.14
Nodes (3): findPlayerBySlot(), hpOf(), kill()

### Community 100 - "LevelDef"
Cohesion: 0.16
Nodes (33): LevelDef, bosses, deathRun, length, obstacles, waves, wave(), asteroidField() (+25 more)

### Community 102 - "App"
Cohesion: 0.09
Nodes (15): App, _custom, CUSTOM_LEADERBOARD_FILE, _game, _lastInputs, _lastPhase, _leaderboard, LEADERBOARD_FILE (+7 more)

### Community 103 - "UdpSocket::Impl"
Cohesion: 0.11
Nodes (11): UdpSocket::Impl::startReceive(), UdpSocket::Impl, buffer, BUFFER_SIZE, io, localPort, MAX_QUEUED, mutex (+3 more)

### Community 105 - "buildRun"
Cohesion: 0.18
Nodes (6): Overview, appendLevel(), buildMap(), buildRun(), shifted(), map2()

### Community 106 - "ObstacleStep"
Cohesion: 0.09
Nodes (26): addGate(), GateSpec, at, gapHeight, gapY, kind, motion, pinch (+18 more)

### Community 110 - "window/Event.hpp"
Cohesion: 0.20
Nodes (8): Closed, FocusGained, FocusLost, KeyPressed, key, KeyReleased, key, Controls::handle()

### Community 111 - "Gameplay"
Cohesion: 0.11
Nodes (18): Bosses, Charged shot, Coins, time and level skip, Controls, Difficulty and settings, Drone, Enemies, Gameplay (+10 more)

### Community 112 - "Archetype"
Cohesion: 0.06
Nodes (23): Archetype, _columns, _columnsIndex, _entities, NO_COLUMN, _signature, Column, _data (+15 more)

### Community 113 - "The reader: `rtype::mapjson`"
Cohesion: 0.13
Nodes (19): Add a field, Add a kind of obstacle, Add a monster, a boss, a zone, Change the format, Custom maps: for developers, From the file to the match, How-to, Records (+11 more)

### Community 114 - "ShopPanel.cpp"
Cohesion: 0.12
Nodes (11): ShopPanel::affordable(), ShopPanel::confirm(), ShopPanel::handle(), ShopPanel::hint(), ShopPanel::infoColumns(), ShopPanel::isOpen(), ShopPanel::priceShown(), ShopPanel::selection() (+3 more)

### Community 116 - "BossHazards.hpp"
Cohesion: 0.29
Nodes (7): bossHazardSystem(), HazardClock, laser, meteor, time, hazardInterval(), spawnMeteor()

### Community 117 - "formatTime"
Cohesion: 0.31
Nodes (5): formatTime(), twoDigits(), Hud::drawCoins(), Hud::drawLevel(), Hud::drawTimer()

### Community 118 - ".width"
Cohesion: 0.16
Nodes (13): Gates, Map, Obstacle ranges, Obstacles, Portals, Reference, Solid pieces, rarityColor() (+5 more)

### Community 119 - "TEST"
Cohesion: 0.11
Nodes (5): hpOf(), missiles(), monsterAt(), rocketAt(), TEST()

### Community 120 - "CustomMaps"
Cohesion: 0.11
Nodes (10): Lobby and maps, CustomMaps, _board, _current, CustomMaps::CustomMaps(), _recordsFile, _upload, LoadedMap (+2 more)

### Community 121 - "Piece"
Cohesion: 0.17
Nodes (18): isPortalPiece(), kindOf(), parseObstacle(), Piece, FastPortal, Gate, Laser, Mine (+10 more)

### Community 122 - "GameView.cpp"
Cohesion: 0.10
Nodes (22): Sprite, animation, texture, Vector2f, x, y, ScreenRect, position (+14 more)

### Community 124 - "LifeBook"
Cohesion: 0.19
Nodes (4): LifeBook, _down, SLOTS, _timers

### Community 125 - "TEST"
Cohesion: 0.04
Nodes (9): _shopOffers, findFreeSlot(), clearEnemies(), gameAtTheEndOfALevel(), reachShop(), runFor(), shopWithCoins(), TEST() (+1 more)

### Community 126 - "StrikeSpec"
Cohesion: 0.17
Nodes (12): Strike, deadly, spec, time, StrikeSpec, active, height, vy (+4 more)

### Community 127 - "isValidDifficulty"
Cohesion: 0.25
Nodes (5): isValidDifficulty(), decodeLeaderboard(), isOfDifficulty(), isValid(), readRecords()

### Community 128 - "LeviathanMove"
Cohesion: 0.25
Nodes (8): LeviathanMove, Aim, Columns, Dash, Rest, Return, Sweep, Windup

### Community 129 - "LaserCycle"
Cohesion: 0.50
Nodes (4): LaserCycle, age, beam, hasBeam

### Community 130 - "Signature"
Cohesion: 0.29
Nodes (4): each(), Signature, WORD_BITS, _words

### Community 131 - "CustomMapList"
Cohesion: 0.11
Nodes (6): CustomMapList, _entries, _folder, _message, Hud::drawScoresStep(), Hud::drawCustomScores()

### Community 132 - "AugmentId"
Cohesion: 0.12
Nodes (17): AugmentId, Count, DamageUp, Drone, DroneBoost, DroneTwin, FireRateUp, Laser (+9 more)

### Community 133 - "Aura"
Cohesion: 0.29
Nodes (7): Aura, Dasher, Fortress, None, Standard, Summoner, GameView::auraOf()

### Community 134 - "PlayerInput"
Cohesion: 0.09
Nodes (13): PlayerInput, alt, down, fire, left, right, up, Prediction::advance() (+5 more)

### Community 135 - "Case"
Cohesion: 0.33
Nodes (4): Case, name, run, Registrar

### Community 136 - "collectMonsters"
Cohesion: 0.19
Nodes (9): collectMonsters(), collectObstacles(), collectProjectileEntities(), collectWith(), getCollider(), deleteUnitsOutOfBounds(), destroyFarEntities(), killPlayersOutOfBounds() (+1 more)

### Community 137 - "MapUpload"
Cohesion: 0.18
Nodes (7): MapUpload, _owner, _size, _text, bytes(), truncated(), uploaded()

### Community 139 - "TEST"
Cohesion: 0.12
Nodes (10): Difficulty, Brutal, Easy, Hard, Normal, Settings, difficulty, friendlyFire (+2 more)

### Community 140 - "parseLevel"
Cohesion: 0.18
Nodes (12): parseLevel(), readBosses(), readObstacles(), readWaves(), ParsedLevel, level, zone, checkReach() (+4 more)

### Community 141 - "CustomLeaderboard"
Cohesion: 0.18
Nodes (3): CustomLeaderboard, MAX_MAPS, _records

### Community 142 - "Resized"
Cohesion: 0.50
Nodes (3): Resized, height, width

### Community 143 - "Reader.hpp"
Cohesion: 0.29
Nodes (6): choice(), choiceOr(), Name, name, value, namesOf()

### Community 144 - "WeaponKind"
Cohesion: 0.07
Nodes (26): Decisions already made, Method, Open points to settle step by step, Roadmap: weapons, Steps, W1. Charged shot on the base weapon, W2. Secondary weapon slot (done), W3. Secondary weapons (+18 more)

### Community 145 - "Regeneration"
Cohesion: 0.50
Nodes (4): Regeneration, hp, interval, timer

### Community 146 - "convert"
Cohesion: 0.22
Nodes (5): convert(), toKey(), translate(), Window::clear(), Window::pollEvent()

### Community 147 - "Drone.hpp"
Cohesion: 0.18
Nodes (15): cooldownOf(), createDrone(), droneFire(), droneFollowSystem(), dronePerch(), droneSystem(), followOwner(), hasDrone() (+7 more)

### Community 148 - "App::useCustomStep"
Cohesion: 0.32
Nodes (3): App::useCustomScoresPage(), App::useCustomStep(), App::useMapStep()

### Community 149 - "Message"
Cohesion: 0.15
Nodes (9): Connected, client, Disconnected, client, reason, Message, client, payload (+1 more)

### Community 150 - "CoinCounter"
Cohesion: 0.33
Nodes (5): CoinCounter, quantity, total, coins, value

### Community 151 - "CustomMapList.cpp"
Cohesion: 0.17
Nodes (5): CustomMapList::cursor(), CustomMapList::CustomMapList(), CustomMapList::readSelected(), CustomMapList::selected(), CustomMapList::setMessage()

### Community 152 - "time_point"
Cohesion: 0.25
Nodes (3): Connection::receive(), Connection::timedOut(), Connection::update()

### Community 153 - "ObstacleMotion"
Cohesion: 0.40
Nodes (5): ObstacleMotion, age, amplitude, baseY, omega

### Community 154 - "Renderer.cpp"
Cohesion: 0.29
Nodes (4): Renderer::draw(), Renderer::drawRect(), Renderer::drawResized(), Renderer::Renderer()

### Community 155 - "Server App class (only place including both network and game)"
Cohesion: 0.33
Nodes (6): Dependency rules, Game::setInput / Game::update(dt), Server App class (only place including both network and game), Server threads: network thread and game thread, Choice: mediator in main instead of IGame interface, Choice: network thread + game thread

### Community 156 - "CustomMap"
Cohesion: 0.20
Nodes (8): CustomMap, author, levels, name, zones, ParseResult, error, map

### Community 157 - "checkTotals"
Cohesion: 0.27
Nodes (7): checkTotals(), countAll(), tooMany(), Totals, bosses, monsters, obstacles

### Community 159 - "Folder.cpp"
Cohesion: 0.19
Nodes (10): MapEntry, error, file, hash, map, isJson(), listJson(), loadEntry() (+2 more)

### Community 160 - "Packet"
Cohesion: 0.11
Nodes (7): Packet, Datagram, bytes, from, Client::handleDatagram(), Server::handleDatagram(), Server::handleSession()

### Community 161 - "DifficultyScale"
Cohesion: 0.20
Nodes (10): DifficultyScale, allyDamage, density, economy, enemyDamage, enemyFireRate, enemyHp, extraOffers (+2 more)

### Community 163 - "size_t"
Cohesion: 0.40
Nodes (4): EntityRecord, archetype, row, IterationGuard

### Community 164 - "Sample"
Cohesion: 0.22
Nodes (5): Sample, pos, tick, Replica::interpolate(), Replica::update()

### Community 165 - "RarityOdds"
Cohesion: 0.50
Nodes (4): RarityOdds, bossBonus, perLevel, weight

### Community 167 - "Build and run doc"
Cohesion: 0.40
Nodes (5): rtype::protocol (Protocol.hpp message encoding), Build and run doc, CMake presets linux-debug / windows-debug (Ninja on Linux), rtype_tests executable (game logic + protocol encoding tests, ctest), Build troubleshooting table

### Community 168 - "FixedTimestep"
Cohesion: 0.25
Nodes (3): FixedTimestep, _accumulator, _maxAccumulator

### Community 169 - "Session"
Cohesion: 0.25
Nodes (6): Session, address, connection, id, Server::findByAddress(), Server::findByToken()

### Community 170 - "TextureManager::Impl"
Cohesion: 0.25
Nodes (3): TextureManager::Impl, ids, textures

### Community 171 - "Hud::drawCustomStep"
Cohesion: 0.22
Nodes (3): Hud::drawMapStep(), Hud::drawCustomDetail(), Hud::drawCustomStep()

### Community 172 - "Streak"
Cohesion: 0.40
Nodes (5): Streak, length, speed, x, y

### Community 173 - "HudVictory.cpp"
Cohesion: 0.14
Nodes (7): count, clock(), confettiColor(), Hud::drawConfetti(), Hud::drawVictoryScreen(), Hud::drawVictoryStats(), Hud::timeColor()

### Community 174 - "ShopSound"
Cohesion: 0.40
Nodes (5): ShopSound, Accept, Count, Refuse, Audio::playShopSound()

### Community 176 - "MonsterState"
Cohesion: 0.50
Nodes (4): MonsterState, age, locked, summoned

### Community 179 - "Shield"
Cohesion: 0.67
Nodes (3): Shield, cooldown, timer

### Community 180 - "BulletLook"
Cohesion: 0.22
Nodes (9): BulletLook, height, sprite, width, Team, id, TeamId, Monster (+1 more)

### Community 181 - "TurretFire"
Cohesion: 0.67
Nodes (3): TurretFire, interval, timer

### Community 183 - "Star"
Cohesion: 0.50
Nodes (4): Star, layer, x, y

### Community 184 - "Area"
Cohesion: 0.67
Nodes (3): Area, box, pos

### Community 185 - "BossTag"
Cohesion: 0.67
Nodes (3): BossTag, age, summonTimer

### Community 186 - "DroneBody"
Cohesion: 0.67
Nodes (3): DroneBody, owner, timer

### Community 189 - "Weapon"
Cohesion: 0.67
Nodes (3): Weapon, cooldown, timer

## Knowledge Gaps
- **852 isolated node(s):** `PANEL_WIDTH`, `PANEL_HEIGHT`, `CARD_WIDTH`, `CARD_HEIGHT`, `CARD_GAP` (+847 more)
  These have ≤1 connection - possible missing edges or undocumented components. (Counts symbols only; 1810 node(s) total have ≤1 connection when file, concept and rationale nodes are included.)
- **36 thin communities (<3 nodes) omitted from report** — run `graphify query` to explore isolated nodes.

## Suggested Questions
_Questions this graph is uniquely positioned to answer:_

- **Why does `Game` connect `Game` to `Components.hpp`, `AugmentId`, `PlayerInput`, `collectMonsters`, `Game Tests`, `TEST`, `TEST`, `Registry`, `Shop.hpp`, `.startMap`, `Segments`, `WaveStep`, `TEST`, `Obstacles.hpp`, `Phase`, `TEST`, `Systems.hpp`, `.update`, `Zone`, `.isUsedSlot`, `App`, `BossHazards.hpp`, `TEST`, `LifeBook`, `TEST`?**
  _High betweenness centrality (0.102) - this node is a cross-community bridge._
- **Are the 9 inferred relationships involving `Registry` (e.g. with `TEST()` and `TEST()`) actually correct?**
  _`Registry` has 9 INFERRED edges - model-reasoned connections that need verification._
- **What connects `PANEL_WIDTH`, `PANEL_HEIGHT`, `CARD_WIDTH` to the rest of the system?**
  _852 weakly-connected nodes found - possible documentation gaps or missing edges._
- **Should `src/App.cpp` be split into smaller, more focused modules?**
  _Cohesion score 0.08695652173913043 - nodes in this community are weakly interconnected._
- **Why does `Replica` connect `.updatePlaying` to `CustomMapList`, `AugmentId`, `PlayerInput`, `Level`, `Components.hpp`, `Hud`, `Prediction`, `Replica`, `GameView`, `ShopPanel`, `Registry`, `Sample`, `Network`, `EntitySpawned`, `Hud::drawCustomStep`, `Phase`, `HudVictory.cpp`, `HudLobby.cpp`, `SpriteBank`, `CustomInfo`, `LobbyMenu`, `DisconnectReason`, `GameStatus`, `Hud::drawSettingsStep`, `RunRecord`, `Track`, `ShopPanel.cpp`, `GameView.cpp`?**
  _High betweenness centrality (0.091) - this node is a cross-community bridge._
- **Are the 80 inferred relationships involving `TEST()` (e.g. with `Registry` and `.alive()`) actually correct?**
  _`TEST()` has 80 INFERRED edges - model-reasoned connections that need verification._
- **Should `IntRect` be split into smaller, more focused modules?**
  _Cohesion score 0.14619883040935672 - nodes in this community are weakly interconnected._