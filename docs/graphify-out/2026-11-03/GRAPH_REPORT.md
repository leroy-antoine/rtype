# Graph Report - RTYPE  (2026-11-03)

## Corpus Check
- 146 files · ~145,511 words
- Verdict: corpus is large enough that graph structure adds value.
- Unclassified: 5 file(s) not represented in the graph (top: (none) 5)

## Summary
- 2521 nodes · 5091 edges · 161 communities (119 shown, 42 thin omitted)
- Extraction: 87% EXTRACTED · 13% INFERRED · 0% AMBIGUOUS · INFERRED: 667 edges (avg confidence: 0.88)
- Token cost: 0 input · 0 output

## Graph Freshness
- Built from commit: `26188d5e`
- Run `git rev-parse HEAD` and compare to check if the graph is stale.
- Run `graphify update .` after code changes (no API cost).

## Community Hubs (Navigation)
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
- Game Tests
- Components.hpp
- Registry
- Connection Channel
- Game
- UdpSocket::Impl
- .updatePlaying
- Hud
- Entity State Payload
- Prediction
- Binary Writer
- Packet
- Pending Messages
- Replica
- BinaryReader
- DeathRuns.hpp
- GameView
- App
- Client.cpp
- Audio Player Impl
- deathSystem
- Command Buffer
- BackdropId
- Design Choices Docs
- Audio Player
- Connection Impl
- Replica Events
- Build Tooling Docs
- Message
- Reliable Channel
- AugmentId
- Zone
- Phase
- Network protocol doc
- Architecture Targets
- vector
- TEST
- Segments
- Key
- addGates
- MonsterKind
- map3.cpp
- Components
- SpriteId
- Window.cpp
- SpriteBank
- movementSystem
- src/App.cpp
- Background
- Message Types
- EntitySpawned
- BossKind
- Controls.cpp
- describeSpawn
- SpriteId
- time_point
- Connection::handle
- Session Info
- Renderer.cpp
- SpriteBank
- EntityState
- WaveStep
- Layers
- Window Impl Header
- BinaryWriter.cpp
- TEST
- client/main.cpp
- Phase
- Sound Manager Impl
- HudVictory.cpp
- MonsterDef
- RefuseReason
- Connection Send
- createPlayer
- SlotTable
- DisconnectReason
- Knowledge graph (graphify)
- GameStatus
- GameView.cpp
- OwnState
- game/App.cpp
- Aura
- .operator()
- Track
- CLAUDE.md
- findPlayerBySlot
- LevelDef
- App
- src/App.cpp
- map1
- Archetype
- ObstacleStep
- Audio.cpp
- App::run
- vcpkg.json
- Gameplay
- SpriteBank.cpp
- Hud.cpp
- bossHazardSystem
- Prediction.cpp
- Vec2
- Streak
- Vector2f
- UdpSocket
- FixedTimestep
- .restartDeathRun
- Scatter.hpp
- ObstacleKind
- Hud::drawTimer
- buildMap
- Star
- Vec2Pos
- Roadmap
- size_t
- formatTime
- Area
- wall
- LaserCycle
- PendingSpawn
- Hud::drawLobby
- BossTag
- TeamId
- TurretFire
- spawnBoss
- zoneOf

## God Nodes (most connected - your core abstractions)
1. `TEST()` - 180 edges
2. `Registry` - 145 edges
3. `Entity` - 117 edges
4. `Game` - 112 edges
5. `Replica` - 60 edges
6. `SpriteId` - 46 edges
7. `LevelDef` - 46 edges
8. `Hud` - 44 edges
9. `TEST()` - 44 edges
10. `Server` - 43 edges

## Surprising Connections (you probably didn't know these)
- `Use the graph` --references--> `Entity`  [INFERRED]
  docs/graphify.md → engine/core/include/engine/core/Entity.hpp
- `The ECS rules to remember` --references--> `each()`  [INFERRED]
  src/client/game/Rtype/doc/README.md → engine/core/include/engine/core/Registry.hpp
- `The first map: 20 levels` --references--> `map1()`  [INFERRED]
  src/client/game/Rtype/doc/README.md → src/client/game/Rtype/src/maps/map1.cpp
- `Add a system` --references--> `each()`  [INFERRED]
  src/client/game/Rtype/doc/README.md → engine/core/include/engine/core/Registry.hpp
- `Current status` --references--> `LevelDef`  [INFERRED]
  src/client/game/Rtype/doc/README.md → src/client/game/Rtype/include/logic/LevelBuilder.hpp

## Import Cycles
- None detected.

## Communities (161 total, 42 thin omitted)

### Community 1 - "IntRect"
Cohesion: 0.13
Nodes (15): Animation, firstFrame, frameCount, frameDuration, step, IntRect, height, left (+7 more)

### Community 2 - "string"
Cohesion: 0.18
Nodes (4): Window, _impl, handle(), App::loop()

### Community 4 - "Background.cpp"
Cohesion: 0.19
Nodes (15): Renderer, Background::drawBand(), Background::drawBars(), Background::drawFade(), Background::drawNebula(), Background::drawRings(), Background::drawSky(), Background::drawStar() (+7 more)

### Community 5 - "Client Connection"
Cohesion: 0.06
Nodes (15): Client, _attempts, _connection, _events, _handshakeSequence, _lastAttempt, _server, _socket (+7 more)

### Community 6 - "vector"
Cohesion: 0.06
Nodes (11): SoundManager::Impl, buffers, ids, SoundManager::load(), TextureManager::Impl, ids, textures, TextureManager::load() (+3 more)

### Community 7 - "Component.hpp"
Cohesion: 0.13
Nodes (6): add(), create(), remove(), infoOf(), swapRemove(), hash<Engine::Entity>

### Community 8 - "Delivery and Sessions"
Cohesion: 0.10
Nodes (18): Delivery, Reliable, Unreliable, Server::admit(), Server::broadcast(), Server::checkSessions(), Server::clientCount(), Server::close() (+10 more)

### Community 9 - "Build and Protocol Docs"
Cohesion: 0.16
Nodes (17): engine_audio library, SFML::Audio, engine build root, engine_core library, Handshake and connection, Network module (engine::network), ReliableChannel, UDP wire format (+9 more)

### Community 10 - "Server Events"
Cohesion: 0.06
Nodes (9): Server, _events, _maxClients, _nextClientId, _sessions, _socket, _version, Server::handleConnect() (+1 more)

### Community 14 - "Registry"
Cohesion: 0.06
Nodes (47): Entity, id, EntityManager, _freeList, _generations, eachRow(), has(), Registry (+39 more)

### Community 15 - "Connection Channel"
Cohesion: 0.07
Nodes (12): Use the graph, With an AI agent (Claude Code), Connection, _channel, _lastPing, _lastReceived, _outbox, _pingId (+4 more)

### Community 16 - "Game"
Cohesion: 0.05
Nodes (27): Game, _alive, _augments, _coinCounter, _deaths, _effectLevel, _elapsed, _events (+19 more)

### Community 17 - "UdpSocket::Impl"
Cohesion: 0.06
Nodes (26): Address, ip, port, Datagram, bytes, from, toAddress(), toEndpoint() (+18 more)

### Community 18 - ".updatePlaying"
Cohesion: 0.07
Nodes (13): Replica, CLOCK_CORRECTION_RATE, _cues, _entities, INTERPOLATION_DELAY, _latestTick, MAX_CLOCK_ERROR, _prediction (+5 more)

### Community 19 - "Hud"
Cohesion: 0.09
Nodes (17): Hud, BAR_HEIGHT, BAR_WIDTH, BAR_X, BOSS_BAR_WIDTH, _font, _height, ICON_GAP (+9 more)

### Community 20 - "Entity State Payload"
Cohesion: 0.18
Nodes (19): decodeInput(), decodeLeaderboard(), decodeOwnState(), decodePause(), decodeSelectMap(), decodeSpawn(), decodeStates(), decodeStatus() (+11 more)

### Community 21 - "Prediction"
Cohesion: 0.07
Nodes (16): Pending, dt, input, sequence, Prediction, _box, _input, MAX_PENDING (+8 more)

### Community 22 - "Binary Writer"
Cohesion: 0.21
Nodes (8): BinaryWriter, _data, MAX_VARINT, Packet::serialize(), Server::sendAccept(), encode(), encodeInput(), writeVec2()

### Community 23 - "Packet"
Cohesion: 0.10
Nodes (11): Packet, _messages, Client::checkConnection(), Client::handleDatagram(), Client::handleHandshake(), Client::handleSession(), Client::tryConnect(), Connection::flush() (+3 more)

### Community 24 - "Pending Messages"
Cohesion: 0.11
Nodes (8): Pending, message, sentAt, ReliableChannel, _early, _expected, _nextId, _pending

### Community 25 - "Replica"
Cohesion: 0.12
Nodes (12): GameView, ANIMATION_PHASE, _background, _hud, PLAYER_FRAME_DOWN, PLAYER_FRAME_IDLE, PLAYER_FRAME_UP, _screenHeight (+4 more)

### Community 26 - "BinaryReader"
Cohesion: 0.21
Nodes (8): BinaryReader::BinaryReader(), BinaryReader::readBytes(), BinaryReader::readFloat(), BinaryReader::readU16(), BinaryReader::readU32(), BinaryReader::readU8(), BinaryReader::readVarint(), BinaryReader::remaining()

### Community 27 - "DeathRuns.hpp"
Cohesion: 0.18
Nodes (18): addCourse(), addCoursePortal(), checkPattern(), Course, after, gapFrom, gapTo, motion (+10 more)

### Community 28 - "GameView"
Cohesion: 0.10
Nodes (14): App, _assets, _connectedSince, _controls, FRAMERATE, _last, LISTEN_DELAY, _network (+6 more)

### Community 29 - "App"
Cohesion: 0.07
Nodes (49): Layers, The tick: `Game::update`, applyContact(), canHit(), collectProjectiles(), collectTargets(), Collider, box (+41 more)

### Community 30 - "Client.cpp"
Cohesion: 0.09
Nodes (13): DisconnectReason, Kicked, Quit, Shutdown, Timeout, Client::Client(), Client::close(), Client::flush() (+5 more)

### Community 32 - "Audio Player Impl"
Cohesion: 0.18
Nodes (4): AudioPlayer::play(), AudioPlayer::playMusic(), AudioPlayer::setMusicVolume(), toSfmlVolume()

### Community 33 - "deathSystem"
Cohesion: 0.14
Nodes (13): each(), Add a system, Architecture, Level skip, The ECS rules to remember, isSummoned(), spawnCoin(), createCoin() (+5 more)

### Community 34 - "Command Buffer"
Cohesion: 0.14
Nodes (5): CommandBuffer, _commands, Scheduler, _commands, _systems

### Community 35 - "BackdropId"
Cohesion: 0.12
Nodes (16): BackdropId, Count, Girders, NebulaBlue, NebulaGreen, NebulaOrange, NebulaPurple, NebulaRed (+8 more)

### Community 36 - "Design Choices Docs"
Cohesion: 0.17
Nodes (16): Accessibility doc, Audio/visual accessibility (health bars, per-slot sprites; palette, contrast proposed), Cognitive accessibility (simple rules, short levels, clear feedback), rtype::config (Config.hpp constants), Motor accessibility (keyboard only done; remap, toggle fire, gamepad proposed), Choice: per-message reliability, Choice: event-based replication with NetId, Choice: sessions with token, ping, 5s timeout (+8 more)

### Community 38 - "Connection Impl"
Cohesion: 0.21
Nodes (8): Connection::Connection(), Connection::handleUser(), Connection::latency(), Connection::peerDisconnect(), Connection::resendDelay(), Connection::sendPing(), Connection::token(), duration

### Community 39 - "Replica Events"
Cohesion: 0.13
Nodes (6): Unreachable, applyDecoded(), Network, _client, Network::Network(), _open

### Community 40 - "Build Tooling Docs"
Cohesion: 0.11
Nodes (21): Dependency rules, Game::setInput / Game::update(dt), Server App class (only place including both network and game), Server threads: network thread and game thread, vcpkg submodule (external/vcpkg, manifest vcpkg.json, Asio + SFML 3), Technical and comparative study doc, Choice: AABB collision, Choice: hand-written binary big-endian encoding (+13 more)

### Community 41 - "Message"
Cohesion: 0.15
Nodes (9): Connected, client, Disconnected, client, reason, Message, client, payload (+1 more)

### Community 42 - "Reliable Channel"
Cohesion: 0.23
Nodes (5): ReliableChannel::acknowledge(), ReliableChannel::receive(), ReliableChannel::resend(), ReliableChannel::send(), ReliableChannel::takeReady()

### Community 43 - "AugmentId"
Cohesion: 0.08
Nodes (19): applyAugment(), AugmentDef, apply, id, price, rarity, AugmentId, Count (+11 more)

### Community 44 - "Zone"
Cohesion: 0.12
Nodes (15): Scene, aura, boosted, zone, Zone, Arena, Asteroids, Core (+7 more)

### Community 45 - "Phase"
Cohesion: 0.11
Nodes (10): Audio, _dir, _enabled, _ids, _phase, _player, SOUND_COUNT, _sounds (+2 more)

### Community 46 - "Network protocol doc"
Cohesion: 0.18
Nodes (21): Architecture doc, Build and run doc, CMake presets linux-debug / windows-debug (Ninja on Linux), Build troubleshooting table, Contributing doc, Branch prefixes and commit types, Coding conventions (80 cols, 25-line functions, 5 params, 2 nested ifs, no ternary, _ private prefix), Part ownership (engine/core, server, client, game logic each have an owner) (+13 more)

### Community 47 - "Architecture Targets"
Cohesion: 0.14
Nodes (19): r-type_client (display executable), Client replica of the world (entity kind + position), engine::audio (sounds and music, wraps SFML), engine::core (Registry, Scheduler, CommandBuffer, FixedTimestep), engine::network (UDP transport, sessions, ping, reliability, wraps Asio), engine::window (rendering, wraps SFML), Three decoupled layers: rendering, networking, game logic, rtype::common (shared headers: Vec2, return codes) (+11 more)

### Community 49 - "TEST"
Cohesion: 0.06
Nodes (7): Leaderboard, _records, RunRecord, players, timeMs, sampleStatus(), TEST()

### Community 50 - "Segments"
Cohesion: 0.13
Nodes (16): firstLevel(), isDeathRun(), lastSpawnPoint(), levelAt(), levelSpan(), levelStart(), Segments, bossSpawn (+8 more)

### Community 51 - "Key"
Cohesion: 0.20
Nodes (10): Key, Backspace, Down, Enter, Escape, Left, Right, Space (+2 more)

### Community 52 - "addGates"
Cohesion: 0.11
Nodes (28): addCave(), addGates(), addRun(), addSweepers(), CavePiece, bottom, top, Gap (+20 more)

### Community 53 - "MonsterKind"
Cohesion: 0.10
Nodes (20): Add a new kind of monster, Behaviours, Kinds of monsters and bosses, MonsterState, age, locked, summoned, MonsterType (+12 more)

### Community 54 - "map3.cpp"
Cohesion: 0.54
Nodes (6): gauntlet(), map3(), minefield(), pinch(), rhythm(), run()

### Community 55 - "Components"
Cohesion: 0.06
Nodes (47): Add a component, Add a map or a level, Add a player action (a new key), Coding conventions, Collisions, Components, Configuration, Current status (+39 more)

### Community 56 - "SpriteId"
Cohesion: 0.22
Nodes (4): Controls, _input, _pause, _respawn

### Community 57 - "Window.cpp"
Cohesion: 0.10
Nodes (14): Closed, FocusGained, FocusLost, KeyReleased, key, Resized, height, width (+6 more)

### Community 58 - "SpriteBank"
Cohesion: 0.13
Nodes (9): explosionCue(), Replica::applyPrediction(), Replica::bossSprite(), Replica::localPlayer(), Replica::mapCursor(), Replica::predict(), Replica::recordSpawnCue(), Replica::refreshSolids() (+1 more)

### Community 59 - "movementSystem"
Cohesion: 0.40
Nodes (5): Position, velocity, speed, direction, bossSystem(), calculatePos(), movementSystem(), runBoss()

### Community 60 - "src/App.cpp"
Cohesion: 0.24
Nodes (6): AudioPlayer::Impl, music, musicVolume, next, soundVolume, voices

### Community 61 - "Background"
Cohesion: 0.08
Nodes (19): Background, _aura, _auraLevel, _boosted, _fade, _height, _last, LAYERS (+11 more)

### Community 62 - "Message Types"
Cohesion: 0.22
Nodes (5): Message, id, payload, reliable, type

### Community 63 - "EntitySpawned"
Cohesion: 0.14
Nodes (11): EntityDestroyed, id, EntitySpawned, box, id, pos, slot, sprite (+3 more)

### Community 64 - "BossKind"
Cohesion: 0.25
Nodes (8): BossType, kind, BossKind, Count, Dasher, Fortress, Standard, Summoner

### Community 65 - "Controls.cpp"
Cohesion: 0.18
Nodes (5): KeyPressed, key, Controls::applyKey(), Controls::handle(), Controls::poll()

### Community 66 - "describeSpawn"
Cohesion: 0.16
Nodes (9): Packet::add(), Packet::isHandshake(), Packet::isValid(), Packet::Packet(), Packet::parseMessage(), Packet::sequence(), Packet::size(), Packet::sizeOf() (+1 more)

### Community 67 - "SpriteId"
Cohesion: 0.06
Nodes (34): isBossSprite(), Renderable, id, SpriteId, AlienShard, BeamOn, BeamWarn, Bomb (+26 more)

### Community 68 - "time_point"
Cohesion: 0.25
Nodes (3): Connection::receive(), Connection::timedOut(), Connection::update()

### Community 69 - "Connection::handle"
Cohesion: 0.20
Nodes (5): Connection::handle(), Connection::handlePong(), Connection::nextSequence(), Connection::readId(), Connection::sendId()

### Community 70 - "Session Info"
Cohesion: 0.18
Nodes (8): _random, Session, address, connection, id, Server::findByAddress(), Server::findByToken(), Server::makeToken()

### Community 71 - "Renderer.cpp"
Cohesion: 0.09
Nodes (19): Color, a, b, g, r, Font, GLYPH_HEIGHT, GLYPH_WIDTH (+11 more)

### Community 72 - "SpriteBank"
Cohesion: 0.09
Nodes (10): TextureManager, _impl, SpriteBank, _assets, _backdrops, _clock, _sprites, Background::Background() (+2 more)

### Community 73 - "EntityState"
Cohesion: 0.16
Nodes (12): EntityState, id, pos, Replica::record(), encodeStateChunk(), encodeStates(), InputMessage, input (+4 more)

### Community 74 - "WaveStep"
Cohesion: 0.29
Nodes (7): WaveStep, at, count, dy, interval, kind, y

### Community 75 - "Layers"
Cohesion: 0.12
Nodes (9): BinaryReader, _cursor, PlayerStatus, alive, hp, slot, readPlayers(), readPlayerStatus() (+1 more)

### Community 77 - "BinaryWriter.cpp"
Cohesion: 0.20
Nodes (6): BinaryWriter::writeBytes(), BinaryWriter::writeFloat(), BinaryWriter::writeU16(), BinaryWriter::writeU32(), BinaryWriter::writeU8(), BinaryWriter::writeVarint()

### Community 80 - "client/main.cpp"
Cohesion: 0.33
Nodes (4): Case, name, run, Registrar

### Community 81 - "Phase"
Cohesion: 0.29
Nodes (6): Phase, Defeat, Lobby, Playing, Victory, Hud::trackPhase()

### Community 82 - "Sound Manager Impl"
Cohesion: 0.25
Nodes (3): SoundManager, _impl, AudioPlayer::AudioPlayer()

### Community 83 - "HudVictory.cpp"
Cohesion: 0.14
Nodes (7): clock(), confettiColor(), Hud::drawConfetti(), Hud::drawVictoryScreen(), Hud::drawVictoryStats(), Hud::timeColor(), count

### Community 84 - "MonsterDef"
Cohesion: 0.17
Nodes (13): addMonsterStats(), createBoss(), createMonster(), setBossStats(), MonsterDef, damage, fireCooldown, height (+5 more)

### Community 85 - "RefuseReason"
Cohesion: 0.22
Nodes (8): Refused, reason, RefuseReason, Full, Version, Client::flushConnection(), Server::flush(), Server::sendRefuse()

### Community 86 - "Connection Send"
Cohesion: 0.47
Nodes (3): Connection::send(), Connection::sendEngine(), Connection::takeMessages()

### Community 87 - "createPlayer"
Cohesion: 0.29
Nodes (6): Game rules, CoinCounter, quantity, total, coins, value

### Community 88 - "SlotTable"
Cohesion: 0.22
Nodes (9): Sound, BossAlert, Coin, Count, Defeat, Explosion, Hit, Shoot (+1 more)

### Community 89 - "DisconnectReason"
Cohesion: 0.17
Nodes (9): Cue, BossAppears, BossExplosion, CoinPickup, Explosion, Hit, PlayerShot, Audio::play() (+1 more)

### Community 90 - "Knowledge graph (graphify)"
Cohesion: 0.22
Nodes (5): Sample, pos, tick, Replica::interpolate(), Replica::update()

### Community 91 - "GameStatus"
Cohesion: 0.11
Nodes (14): GameStatus, bossHp, bossMaxHp, coins, deaths, elapsedMs, level, map (+6 more)

### Community 92 - "GameView.cpp"
Cohesion: 0.18
Nodes (7): GameView::drawLayer(), GameView::drawWorld(), GameView::GameView(), GameView::isBlock(), GameView::isPortalSprite(), GameView::sceneOf(), GameView::tiltFrame()

### Community 93 - "OwnState"
Cohesion: 0.33
Nodes (6): Prediction::reconcile(), OwnState, inverted, lastInput, pos, speedPercent

### Community 95 - "Aura"
Cohesion: 0.29
Nodes (7): Aura, Dasher, Fortress, None, Standard, Summoner, GameView::auraOf()

### Community 96 - ".operator()"
Cohesion: 0.50
Nodes (3): Install, Keep it up to date: the git hook, Knowledge graph (graphify)

### Community 97 - "Track"
Cohesion: 0.22
Nodes (9): Track, BossDasher, BossFortress, BossStandard, BossSummoner, Count, DeathRun, Level (+1 more)

### Community 98 - "CLAUDE.md"
Cohesion: 0.17
Nodes (10): isValid(), StatusHeader, bossHp, bossMaxHp, coins, deaths, elapsedMs, level (+2 more)

### Community 99 - "findPlayerBySlot"
Cohesion: 0.12
Nodes (4): Maps and lobby, findPlayerBySlot(), kill(), playerOf()

### Community 100 - "LevelDef"
Cohesion: 0.17
Nodes (30): LevelDef, bosses, deathRun, length, obstacles, waves, wave(), deathRun() (+22 more)

### Community 102 - "App"
Cohesion: 0.13
Nodes (11): App, _game, _lastInputs, _lastPhase, _leaderboard, LEADERBOARD_FILE, _server, _slots (+3 more)

### Community 103 - "src/App.cpp"
Cohesion: 0.11
Nodes (4): App::App(), App::listening(), App::playSounds(), App::run()

### Community 105 - "Archetype"
Cohesion: 0.06
Nodes (24): Archetype, _columns, _columnsIndex, _entities, NO_COLUMN, _signature, Column, _data (+16 more)

### Community 106 - "ObstacleStep"
Cohesion: 0.09
Nodes (24): addGate(), GateSpec, at, gapHeight, gapY, kind, motion, pinch (+16 more)

### Community 107 - "Audio.cpp"
Cohesion: 0.22
Nodes (6): Audio::Audio(), Audio::onPhase(), Audio::playCue(), Audio::playMusic(), Audio::playSound(), Audio::trackFor()

### Community 111 - "Gameplay"
Cohesion: 0.15
Nodes (12): Bosses, Coins, time and level skip, Controls, Enemies, Gameplay, Goal, Leaderboard, Lobby and maps (+4 more)

### Community 113 - "SpriteBank.cpp"
Cohesion: 0.15
Nodes (4): SpriteBank::add(), SpriteBank::addBackdrop(), SpriteBank::loadBackdrops(), SpriteBank::texture()

### Community 115 - "Hud.cpp"
Cohesion: 0.29
Nodes (4): Hud::drawMapList(), Hud::drawMenuHint(), Hud::drawPauseMenu(), Hud::Hud()

### Community 116 - "bossHazardSystem"
Cohesion: 0.22
Nodes (8): bossHazardSystem(), HazardClock, laser, meteor, time, hazardInterval(), spawnBossLaser(), spawnMeteor()

### Community 118 - "Prediction.cpp"
Cohesion: 0.20
Nodes (3): Prediction::advance(), Prediction::position(), Prediction::record()

### Community 119 - "Vec2"
Cohesion: 0.12
Nodes (22): volley(), centerOf(), dive(), lockOn(), ProjectileDef, drop, height, shots (+14 more)

### Community 121 - "Streak"
Cohesion: 0.40
Nodes (5): Streak, length, speed, x, y

### Community 122 - "Vector2f"
Cohesion: 0.12
Nodes (17): Sprite, animation, texture, Vector2f, x, y, Renderer::draw(), Renderer::drawRect() (+9 more)

### Community 124 - "FixedTimestep"
Cohesion: 0.25
Nodes (3): FixedTimestep, _accumulator, _maxAccumulator

### Community 125 - ".restartDeathRun"
Cohesion: 0.20
Nodes (6): countBefore(), levelEnd(), collectMonsters(), collectObstacles(), collectProjectileEntities(), collectWith()

### Community 126 - "Scatter.hpp"
Cohesion: 0.39
Nodes (5): asteroid(), crowded(), scatter(), scatterLasers(), scatterLimit()

### Community 127 - "ObstacleKind"
Cohesion: 0.07
Nodes (33): Obstacles, ObstacleMotion, age, amplitude, baseY, omega, ObstacleTag, kind (+25 more)

### Community 128 - "Hud::drawTimer"
Cohesion: 0.47
Nodes (3): Hud::drawCoins(), Hud::drawLevel(), Hud::drawTimer()

### Community 129 - "buildMap"
Cohesion: 0.40
Nodes (4): appendLevel(), buildMap(), buildRun(), shifted()

### Community 130 - "Star"
Cohesion: 0.50
Nodes (4): Star, layer, x, y

### Community 131 - "Vec2Pos"
Cohesion: 0.14
Nodes (15): dasherTarget(), DashTarget, pos, velocity, roamBoss(), BulletLook, height, sprite (+7 more)

### Community 134 - "size_t"
Cohesion: 0.40
Nodes (4): EntityRecord, archetype, row, IterationGuard

### Community 138 - "formatTime"
Cohesion: 0.47
Nodes (3): formatTime(), Hud::drawRecords(), twoDigits()

### Community 141 - "Area"
Cohesion: 0.40
Nodes (4): Area, box, pos, GameView::drawTiled()

### Community 142 - "wall"
Cohesion: 0.60
Nodes (4): wall(), asteroidField(), pillars(), tankCover()

### Community 144 - "LaserCycle"
Cohesion: 0.50
Nodes (4): LaserCycle, age, beam, hasBeam

### Community 148 - "PendingSpawn"
Cohesion: 0.50
Nodes (4): PendingSpawn, delay, kind, y

### Community 150 - "BossTag"
Cohesion: 0.67
Nodes (3): BossTag, age, summonTimer

### Community 151 - "TeamId"
Cohesion: 0.67
Nodes (3): TeamId, Monster, Player

### Community 152 - "TurretFire"
Cohesion: 0.67
Nodes (3): TurretFire, interval, timer

## Knowledge Gaps
- **607 isolated node(s):** `PLAYER_FRAME_DOWN`, `PLAYER_FRAME_IDLE`, `PLAYER_FRAME_UP`, `ANIMATION_PHASE`, `pos` (+602 more)
  These have ≤1 connection - possible missing edges or undocumented components. (Counts symbols only; 1205 node(s) total have ≤1 connection when file, concept and rationale nodes are included.)
- **42 thin communities (<3 nodes) omitted from report** — run `graphify query` to explore isolated nodes.

## Suggested Questions
_Questions this graph is uniquely positioned to answer:_

- **Why does `Game` connect `Game` to `Components.hpp`, `findPlayerBySlot`, `App`, `AugmentId`, `Game Tests`, `Registry`, `vector`, `Phase`, `App`, `Segments`, `bossHazardSystem`, `PendingSpawn`, `spawnBoss`, `.restartDeathRun`?**
  _High betweenness centrality (0.101) - this node is a cross-community bridge._
- **Are the 70 inferred relationships involving `TEST()` (e.g. with `Registry` and `.alive()`) actually correct?**
  _`TEST()` has 70 INFERRED edges - model-reasoned connections that need verification._
- **What connects `PLAYER_FRAME_DOWN`, `PLAYER_FRAME_IDLE`, `PLAYER_FRAME_UP` to the rest of the system?**
  _607 weakly-connected nodes found - possible documentation gaps or missing edges._
- **Should `IntRect` be split into smaller, more focused modules?**
  _Cohesion score 0.13333333333333333 - nodes in this community are weakly interconnected._
- **Why does `TEST()` connect `Game Tests` to `buildMap`, `Registry`, `wall`, `Game`, `spawnBoss`, `App`, `Levels.hpp`, `deathSystem`, `AugmentId`, `vector`, `TEST`, `Segments`, `MonsterKind`, `Components`, `movementSystem`, `Phase`, `MonsterDef`, `findPlayerBySlot`, `LevelDef`, `bossHazardSystem`, `Vec2`, `.restartDeathRun`, `ObstacleKind`?**
  _High betweenness centrality (0.083) - this node is a cross-community bridge._
- **Are the 2 inferred relationships involving `Registry` (e.g. with `TEST()` and `TEST()`) actually correct?**
  _`Registry` has 2 INFERRED edges - model-reasoned connections that need verification._
- **Should `Client Connection` be split into smaller, more focused modules?**
  _Cohesion score 0.058823529411764705 - nodes in this community are weakly interconnected._