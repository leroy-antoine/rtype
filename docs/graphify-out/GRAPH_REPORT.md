# Graph Report - RTYPE  (2026-11-03)

## Corpus Check
- 145 files · ~125,965 words
- Verdict: corpus is large enough that graph structure adds value.
- Unclassified: 5 file(s) not represented in the graph (top: (none) 5)

## Summary
- 2617 nodes · 5350 edges · 166 communities (118 shown, 48 thin omitted)
- Extraction: 87% EXTRACTED · 13% INFERRED · 0% AMBIGUOUS · INFERRED: 715 edges (avg confidence: 0.87)
- Token cost: 0 input · 0 output

## Graph Freshness
- Built from commit: `06d320af`
- Run `git rev-parse HEAD` and compare to check if the graph is stale.
- Run `graphify update .` after code changes (no API cost).

## Community Hubs (Navigation)
- src/App.cpp
- IntRect
- string
- Background.cpp
- Client Connection
- vector
- Component.hpp
- Delivery and Sessions
- Build and Protocol Docs
- Server Events
- Game Tests
- Components.hpp
- Entity
- Connection Channel
- Game
- Address
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
- Layers
- Client.cpp
- Leviathan.hpp
- Audio Player Impl
- Collectibles.hpp
- Registry
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
- Public API: the `Game` class
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
- GameTests.cpp
- src/App.cpp
- Background
- Message Types
- EntitySpawned
- BossKind
- Controls.cpp
- describeSpawn
- SpriteId
- time_point
- Connection::readId
- Session Info
- Renderer.cpp
- SpriteBank
- EntityState
- WaveStep
- Layers
- Window Impl Header
- BinaryWriter.cpp
- TEST
- stdexcept
- Phase
- Sound Manager Impl
- HudVictory.cpp
- MonsterDef
- RefuseReason
- Connection Send
- createPlayer
- SlotTable
- DisconnectReason
- Column
- GameStatus
- GameView.cpp
- OwnState
- game/App.cpp
- Systems.hpp
- .operator()
- Track
- CLAUDE.md
- findPlayerBySlot
- LevelDef
- App
- UdpSocket::Impl
- map1
- Archetype
- ObstacleStep
- Audio.cpp
- App::run
- vcpkg.json
- GateSpec
- Gameplay
- Datagram
- Hud.cpp
- bossHazardSystem
- PlayerInput
- ProjectileDef
- Streak
- Vector2f
- Replica.hpp
- FixedTimestep
- Obstacles.hpp
- scatter
- ObstacleKind
- LeviathanMove
- Client::send
- Star
- Behaviours
- Background::drawAura
- size_t
- Scene
- Vec2
- LaserCycle
- PendingSpawn
- Connection::handle
- R-Type game logic
- BulletLook
- TurretFire
- ObstacleMotion
- Window::Window
- Use the graph

## God Nodes (most connected - your core abstractions)
1. `TEST()` - 194 edges
2. `Registry` - 158 edges
3. `Entity` - 126 edges
4. `Game` - 114 edges
5. `Replica` - 61 edges
6. `TEST()` - 54 edges
7. `LevelDef` - 49 edges
8. `SpriteId` - 48 edges
9. `Hud` - 44 edges
10. `Server` - 43 edges

## Surprising Connections (you probably didn't know these)
- `The ECS rules to remember` --references--> `each()`  [INFERRED]
  src/client/game/Rtype/doc/README.md → engine/core/include/engine/core/Registry.hpp
- `Use the graph` --references--> `Entity`  [INFERRED]
  docs/graphify.md → engine/core/include/engine/core/Entity.hpp
- `Use the graph` --references--> `Connection`  [INFERRED]
  docs/graphify.md → engine/network/include/engine/network/Connection.hpp
- `The first map: 20 levels` --references--> `map1()`  [INFERRED]
  src/client/game/Rtype/doc/README.md → src/client/game/Rtype/src/maps/map1.cpp
- `Add a system` --references--> `each()`  [INFERRED]
  src/client/game/Rtype/doc/README.md → engine/core/include/engine/core/Registry.hpp

## Import Cycles
- None detected.

## Communities (166 total, 48 thin omitted)

### Community 1 - "IntRect"
Cohesion: 0.15
Nodes (13): Animation, firstFrame, frameCount, frameDuration, step, IntRect, height, left (+5 more)

### Community 2 - "string"
Cohesion: 0.12
Nodes (6): Window, _impl, handle(), App::listening(), App::loop(), App::run()

### Community 4 - "Background.cpp"
Cohesion: 0.19
Nodes (14): Renderer, Background::drawBand(), Background::drawBars(), Background::drawFade(), Background::drawNebula(), Background::drawSky(), Background::drawStar(), Background::drawStars() (+6 more)

### Community 5 - "Client Connection"
Cohesion: 0.05
Nodes (17): Client, _attempts, _connection, _events, _handshakeSequence, _lastAttempt, _server, _socket (+9 more)

### Community 6 - "vector"
Cohesion: 0.08
Nodes (8): SoundManager::Impl, buffers, ids, SoundManager::load(), TextureManager::Impl, ids, textures, TextureManager::load()

### Community 7 - "Component.hpp"
Cohesion: 0.16
Nodes (7): add(), create(), remove(), ComponentRegistry, idOf(), infoOf(), swapRemove()

### Community 8 - "Delivery and Sessions"
Cohesion: 0.12
Nodes (16): Delivery, Reliable, Unreliable, Server::admit(), Server::broadcast(), Server::checkSessions(), Server::clientCount(), Server::close() (+8 more)

### Community 9 - "Build and Protocol Docs"
Cohesion: 0.16
Nodes (17): engine_audio library, SFML::Audio, engine build root, engine_core library, Handshake and connection, Network module (engine::network), ReliableChannel, UDP wire format (+9 more)

### Community 10 - "Server Events"
Cohesion: 0.06
Nodes (11): Server, _events, _maxClients, _nextClientId, _sessions, _socket, _version, Server::handleConnect() (+3 more)

### Community 14 - "Entity"
Cohesion: 0.08
Nodes (20): Entity, id, hash<Engine::Entity>, EntityManager, _freeList, _generations, eachRow(), has() (+12 more)

### Community 15 - "Connection Channel"
Cohesion: 0.08
Nodes (10): Connection, _channel, _lastPing, _lastReceived, _outbox, _pingId, _pingSentAt, _received (+2 more)

### Community 16 - "Game"
Cohesion: 0.05
Nodes (25): What the server reads, countBefore(), Game, _alive, _augments, _coinCounter, _deaths, _effectLevel (+17 more)

### Community 17 - "Address"
Cohesion: 0.14
Nodes (12): Address, ip, port, toAddress(), toEndpoint(), UdpSocket::Impl::Impl(), UdpSocket::Impl::store(), UdpSocket::port() (+4 more)

### Community 18 - ".updatePlaying"
Cohesion: 0.06
Nodes (18): Replica, CLOCK_CORRECTION_RATE, _cues, _entities, INTERPOLATION_DELAY, _latestTick, MAX_CLOCK_ERROR, _prediction (+10 more)

### Community 19 - "Hud"
Cohesion: 0.08
Nodes (18): Hud, BAR_HEIGHT, BAR_WIDTH, BAR_X, BOSS_BAR_WIDTH, _font, _height, ICON_GAP (+10 more)

### Community 20 - "Entity State Payload"
Cohesion: 0.19
Nodes (19): decodeDestroy(), decodeInput(), decodeLeaderboard(), decodeOwnState(), decodePause(), decodeSelectMap(), decodeSpawn(), decodeStates() (+11 more)

### Community 21 - "Prediction"
Cohesion: 0.07
Nodes (16): Pending, dt, input, sequence, Prediction, _box, _input, MAX_PENDING (+8 more)

### Community 22 - "Binary Writer"
Cohesion: 0.15
Nodes (10): BinaryWriter, _data, MAX_VARINT, BinaryWriter::writeFloat(), BinaryWriter::writeU32(), Connection::sendId(), Packet::serialize(), Server::sendAccept() (+2 more)

### Community 23 - "Packet"
Cohesion: 0.10
Nodes (11): Packet, _messages, Client::checkConnection(), Client::flushConnection(), Client::handleHandshake(), Client::handleSession(), Client::tryConnect(), Connection::hasHandshakeMessage() (+3 more)

### Community 24 - "Pending Messages"
Cohesion: 0.11
Nodes (8): Pending, message, sentAt, ReliableChannel, _early, _expected, _nextId, _pending

### Community 25 - "Replica"
Cohesion: 0.09
Nodes (17): GameView, ANIMATION_PHASE, _background, _hud, HURT_DURATION, HURT_SHAKE, _hurtUntil, _lastBossHp (+9 more)

### Community 26 - "BinaryReader"
Cohesion: 0.23
Nodes (8): BinaryReader::BinaryReader(), BinaryReader::readBytes(), BinaryReader::readFloat(), BinaryReader::readU16(), BinaryReader::readU32(), BinaryReader::readU8(), BinaryReader::readVarint(), BinaryReader::remaining()

### Community 27 - "DeathRuns.hpp"
Cohesion: 0.17
Nodes (19): addCourse(), addCoursePortal(), checkPattern(), Course, after, gapFrom, gapTo, motion (+11 more)

### Community 28 - "GameView"
Cohesion: 0.09
Nodes (15): App, _assets, _connectedSince, _controls, FRAMERATE, _last, LISTEN_DELAY, _network (+7 more)

### Community 29 - "Layers"
Cohesion: 0.17
Nodes (19): Layers, isShooter(), applyContact(), canHit(), collectProjectiles(), collectTargets(), Collider, box (+11 more)

### Community 30 - "Client.cpp"
Cohesion: 0.22
Nodes (4): Client::flush(), Client::latency(), Client::pollEvent(), Client::sendDisconnect()

### Community 31 - "Leviathan.hpp"
Cohesion: 0.12
Nodes (32): LeviathanState, move, step, swing, timer, StrikeSpec, active, height (+24 more)

### Community 32 - "Audio Player Impl"
Cohesion: 0.18
Nodes (4): AudioPlayer::play(), AudioPlayer::playMusic(), AudioPlayer::setMusicVolume(), toSfmlVolume()

### Community 33 - "Collectibles.hpp"
Cohesion: 0.18
Nodes (9): Level skip, addToPurse(), coinPickupSystem(), collectCoins(), isSummoned(), spawnCoin(), touchesAny(), createCoin() (+1 more)

### Community 34 - "Registry"
Cohesion: 0.07
Nodes (35): CommandBuffer, _commands, Registry, _archetypes, EMPTY_ARCHETYPE, _entities, _iterating, NO_ARCHETYPE (+27 more)

### Community 35 - "BackdropId"
Cohesion: 0.13
Nodes (15): BackdropId, Count, Girders, NebulaBlue, NebulaGreen, NebulaOrange, NebulaPurple, NebulaRed (+7 more)

### Community 36 - "Design Choices Docs"
Cohesion: 0.17
Nodes (16): Accessibility doc, Audio/visual accessibility (health bars, per-slot sprites; palette, contrast proposed), Cognitive accessibility (simple rules, short levels, clear feedback), rtype::config (Config.hpp constants), Motor accessibility (keyboard only done; remap, toggle fire, gamepad proposed), Choice: per-message reliability, Choice: event-based replication with NetId, Choice: sessions with token, ping, 5s timeout (+8 more)

### Community 38 - "Connection Impl"
Cohesion: 0.27
Nodes (6): Connection::Connection(), Connection::latency(), Connection::peerDisconnect(), Connection::resendDelay(), Connection::token(), duration

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
Cohesion: 0.07
Nodes (24): addDamage(), addHp(), addSpeed(), applyAugment(), AugmentDef, apply, id, price (+16 more)

### Community 44 - "Zone"
Cohesion: 0.18
Nodes (12): Variety by zone, stormInterval(), Zone, Arena, Asteroids, Core, Count, DeepSpace (+4 more)

### Community 45 - "Phase"
Cohesion: 0.14
Nodes (9): Audio, _dir, _enabled, _ids, _phase, _player, SOUND_COUNT, _sounds (+1 more)

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
Cohesion: 0.11
Nodes (21): appendLevel(), buildMap(), buildRun(), createCoinCounter(), firstLevel(), isDeathRun(), lastSpawnPoint(), levelAt() (+13 more)

### Community 51 - "Key"
Cohesion: 0.17
Nodes (12): Key, Backspace, Down, Enter, Escape, Left, Right, Space (+4 more)

### Community 52 - "addGates"
Cohesion: 0.11
Nodes (29): Obstacles, PlayerEffects, inverted, lastPortal, speedFactor, portal(), addCave(), addGates() (+21 more)

### Community 53 - "MonsterKind"
Cohesion: 0.15
Nodes (13): MonsterType, kind, MonsterKind, Basic, Bomber, Count, Elite, Kamikaze (+5 more)

### Community 54 - "map3.cpp"
Cohesion: 0.54
Nodes (6): gauntlet(), map3(), minefield(), pinch(), rhythm(), run()

### Community 55 - "Components"
Cohesion: 0.10
Nodes (24): Architecture, Collisions, Components, Data oriented design, Shooting, Systems, Damage, value (+16 more)

### Community 56 - "SpriteId"
Cohesion: 0.14
Nodes (7): KeyPressed, key, Controls, _input, _pause, _respawn, Controls::handle()

### Community 57 - "Window.cpp"
Cohesion: 0.14
Nodes (10): FocusGained, FocusLost, Resized, height, width, convert(), toKey(), translate() (+2 more)

### Community 58 - "SpriteBank"
Cohesion: 0.12
Nodes (10): explosionCue(), Replica::applyPrediction(), Replica::bossSprite(), Replica::localPlayer(), Replica::mapCursor(), Replica::predict(), Replica::record(), Replica::recordSpawnCue() (+2 more)

### Community 59 - "GameTests.cpp"
Cohesion: 0.14
Nodes (9): bossSystem(), colliderOf(), kill(), killBoss(), playerOf(), run(), runBoss(), skipToBoss() (+1 more)

### Community 60 - "src/App.cpp"
Cohesion: 0.24
Nodes (6): AudioPlayer::Impl, music, musicVolume, next, soundVolume, voices

### Community 61 - "Background"
Cohesion: 0.09
Nodes (17): Background, _aura, _auraLevel, _boosted, _fade, _height, _last, LAYERS (+9 more)

### Community 62 - "Message Types"
Cohesion: 0.22
Nodes (5): Message, id, payload, reliable, type

### Community 63 - "EntitySpawned"
Cohesion: 0.15
Nodes (10): EntityDestroyed, id, EntitySpawned, box, id, pos, slot, sprite (+2 more)

### Community 64 - "BossKind"
Cohesion: 0.20
Nodes (10): BossType, kind, BossKind, Count, Dasher, Fortress, Standard, Summoner (+2 more)

### Community 66 - "describeSpawn"
Cohesion: 0.15
Nodes (9): Packet::add(), Packet::isHandshake(), Packet::isValid(), Packet::Packet(), Packet::parseMessage(), Packet::sequence(), Packet::size(), Packet::sizeOf() (+1 more)

### Community 67 - "SpriteId"
Cohesion: 0.06
Nodes (35): isBossSprite(), Renderable, id, SpriteId, AlienShard, BeamOn, BeamWarn, Bomb (+27 more)

### Community 68 - "time_point"
Cohesion: 0.25
Nodes (3): Connection::receive(), Connection::timedOut(), Connection::update()

### Community 69 - "Connection::readId"
Cohesion: 0.20
Nodes (6): Connection::handlePong(), Connection::handleUser(), Connection::nextSequence(), Connection::readId(), Connection::sendPing(), Connection::takeMessages()

### Community 70 - "Session Info"
Cohesion: 0.13
Nodes (13): DisconnectReason, Kicked, Quit, Shutdown, Timeout, Session, address, connection (+5 more)

### Community 71 - "Renderer.cpp"
Cohesion: 0.09
Nodes (19): Color, a, b, g, r, Font, GLYPH_HEIGHT, GLYPH_WIDTH (+11 more)

### Community 72 - "SpriteBank"
Cohesion: 0.07
Nodes (19): TextureManager, _impl, SpriteBank, _assets, _backdrops, _clock, _sprites, GameView::GameView() (+11 more)

### Community 73 - "EntityState"
Cohesion: 0.17
Nodes (12): EntityState, id, pos, encodeInput(), encodeStateChunk(), encodeStates(), InputMessage, input (+4 more)

### Community 74 - "WaveStep"
Cohesion: 0.11
Nodes (21): fittedBase(), fittedSpacing(), formationDelay(), formationSlot(), Slot, delay, y, canTakeFormation() (+13 more)

### Community 75 - "Layers"
Cohesion: 0.15
Nodes (8): BinaryReader, _cursor, PlayerStatus, alive, hp, slot, readPlayers(), readPlayerStatus()

### Community 77 - "BinaryWriter.cpp"
Cohesion: 0.27
Nodes (4): BinaryWriter::writeBytes(), BinaryWriter::writeU16(), BinaryWriter::writeU8(), BinaryWriter::writeVarint()

### Community 80 - "stdexcept"
Cohesion: 0.10
Nodes (7): main(), parsePort(), parsePort(), Case, name, run, Registrar

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
Cohesion: 0.20
Nodes (10): MonsterDef, damage, fireCooldown, height, hp, projectile, speed, sprite (+2 more)

### Community 85 - "RefuseReason"
Cohesion: 0.40
Nodes (5): Refused, reason, RefuseReason, Full, Version

### Community 86 - "Connection Send"
Cohesion: 0.38
Nodes (3): Connection::flush(), Connection::send(), Connection::sendEngine()

### Community 87 - "createPlayer"
Cohesion: 0.29
Nodes (6): Game rules, CoinCounter, quantity, total, coins, value

### Community 88 - "SlotTable"
Cohesion: 0.22
Nodes (9): Sound, BossAlert, Coin, Count, Defeat, Explosion, Hit, Shoot (+1 more)

### Community 89 - "DisconnectReason"
Cohesion: 0.17
Nodes (9): Cue, BossAppears, BossExplosion, CoinPickup, Explosion, Hit, PlayerShot, Audio::play() (+1 more)

### Community 90 - "Column"
Cohesion: 0.10
Nodes (13): Column, _data, _info, ComponentId, id, ComponentInfo, getId(), ComponentRegistry::getInfo() (+5 more)

### Community 91 - "GameStatus"
Cohesion: 0.12
Nodes (13): GameStatus, bossHp, bossMaxHp, coins, deaths, elapsedMs, level, map (+5 more)

### Community 92 - "GameView.cpp"
Cohesion: 0.11
Nodes (14): Aura, Dasher, Fortress, None, Standard, Summoner, GameView::auraOf(), GameView::drawLayer() (+6 more)

### Community 93 - "OwnState"
Cohesion: 0.22
Nodes (8): Prediction::reconcile(), encodeKeys(), OwnState, inverted, lastInput, pos, speedPercent, phaseByte()

### Community 95 - "Systems.hpp"
Cohesion: 0.15
Nodes (19): The tick: `Game::update`, isFarOutOfBounds(), bombVelocity(), collisionSystem(), contactSystem(), deathSystem(), deleteUnitsOutOfBounds(), destroyFarEntities() (+11 more)

### Community 96 - ".operator()"
Cohesion: 0.50
Nodes (3): Install, Keep it up to date: the git hook, Knowledge graph (graphify)

### Community 97 - "Track"
Cohesion: 0.14
Nodes (13): Track, BossDasher, BossFortress, BossStandard, BossSummoner, Core, Count, DeathRun (+5 more)

### Community 98 - "CLAUDE.md"
Cohesion: 0.17
Nodes (10): isValid(), StatusHeader, bossHp, bossMaxHp, coins, deaths, elapsedMs, level (+2 more)

### Community 100 - "LevelDef"
Cohesion: 0.18
Nodes (30): LevelDef, bosses, deathRun, length, obstacles, waves, wave(), closeWith() (+22 more)

### Community 102 - "App"
Cohesion: 0.13
Nodes (11): App, _game, _lastInputs, _lastPhase, _leaderboard, LEADERBOARD_FILE, _server, _slots (+3 more)

### Community 103 - "UdpSocket::Impl"
Cohesion: 0.10
Nodes (12): UdpSocket::Impl::startReceive(), UdpSocket::Impl, buffer, BUFFER_SIZE, io, localPort, MAX_QUEUED, mutex (+4 more)

### Community 105 - "Archetype"
Cohesion: 0.14
Nodes (9): Archetype, _columns, _columnsIndex, _entities, NO_COLUMN, _signature, Signature, WORD_BITS (+1 more)

### Community 106 - "ObstacleStep"
Cohesion: 0.12
Nodes (19): addGate(), gateWall(), shifted(), wall(), withMotion(), asteroid(), placeScattered(), ObstacleStep (+11 more)

### Community 107 - "Audio.cpp"
Cohesion: 0.15
Nodes (7): Audio::Audio(), Audio::follow(), Audio::onPhase(), Audio::playCue(), Audio::playMusic(), Audio::playSound(), Audio::trackOfZone()

### Community 110 - "GateSpec"
Cohesion: 0.12
Nodes (17): GateSpec, at, gapHeight, gapY, kind, motion, pinch, speed (+9 more)

### Community 111 - "Gameplay"
Cohesion: 0.14
Nodes (13): Bosses, Coins, time and level skip, Controls, Enemies, Gameplay, Goal, Leaderboard, Lobby and maps (+5 more)

### Community 113 - "Datagram"
Cohesion: 0.17
Nodes (7): _random, Datagram, bytes, from, Client::handleDatagram(), Server::handleDatagram(), Server::makeToken()

### Community 115 - "Hud.cpp"
Cohesion: 0.13
Nodes (12): formatTime(), Hud::drawBar(), Hud::drawCoins(), Hud::drawLevel(), Hud::drawMapList(), Hud::drawMenuHint(), Hud::drawPauseMenu(), Hud::drawRecords() (+4 more)

### Community 116 - "bossHazardSystem"
Cohesion: 0.33
Nodes (8): bossHazardSystem(), HazardClock, laser, meteor, time, hazardInterval(), spawnBossLaser(), spawnMeteor()

### Community 118 - "PlayerInput"
Cohesion: 0.09
Nodes (14): Position, velocity, speed, direction, PlayerInput, down, fire, left, right, up, calculatePos() (+6 more)

### Community 119 - "ProjectileDef"
Cohesion: 0.20
Nodes (9): volley(), ProjectileDef, drop, height, shots, speed, spread, sprite (+1 more)

### Community 121 - "Streak"
Cohesion: 0.33
Nodes (6): Streak, length, speed, x, y, Background::Background()

### Community 122 - "Vector2f"
Cohesion: 0.10
Nodes (19): Sprite, animation, texture, Vector2f, x, y, Renderer::draw(), Renderer::drawRect() (+11 more)

### Community 124 - "FixedTimestep"
Cohesion: 0.25
Nodes (3): FixedTimestep, _accumulator, _maxAccumulator

### Community 125 - "Obstacles.hpp"
Cohesion: 0.11
Nodes (26): absorbProjectiles(), applyPortal(), beamKind(), crossPortals(), deadlyColliders(), destroyFarObstacles(), fireTurret(), isDeadly() (+18 more)

### Community 126 - "scatter"
Cohesion: 0.24
Nodes (7): crowded(), scatter(), scatterLasers(), scatterLimit(), shapesOf(), shapeWaves(), zoneOf()

### Community 127 - "ObstacleKind"
Cohesion: 0.11
Nodes (19): ObstacleTag, kind, isPortal(), ObstacleKind, Beam, BeamWarn, Count, FastPortal (+11 more)

### Community 128 - "LeviathanMove"
Cohesion: 0.25
Nodes (8): LeviathanMove, Aim, Columns, Dash, Rest, Return, Sweep, Windup

### Community 130 - "Star"
Cohesion: 0.50
Nodes (4): Star, layer, x, y

### Community 131 - "Behaviours"
Cohesion: 0.12
Nodes (16): Behaviours, dasherTarget(), DashTarget, pos, velocity, roamBoss(), MonsterState, age (+8 more)

### Community 134 - "size_t"
Cohesion: 0.40
Nodes (4): EntityRecord, archetype, row, IterationGuard

### Community 138 - "Scene"
Cohesion: 0.33
Nodes (5): Scene, aura, boosted, zone, Background::follow()

### Community 141 - "Vec2"
Cohesion: 0.21
Nodes (17): The ECS rules to remember, Area, box, pos, bulletStartPos(), createBullet(), GameView::drawMeteor(), length() (+9 more)

### Community 144 - "LaserCycle"
Cohesion: 0.50
Nodes (4): LaserCycle, age, beam, hasBeam

### Community 148 - "PendingSpawn"
Cohesion: 0.50
Nodes (4): PendingSpawn, delay, kind, y

### Community 150 - "R-Type game logic"
Cohesion: 0.12
Nodes (16): each(), Add a component, Add a map or a level, Add a player action (a new key), Add a system, Coding conventions, Configuration, Current status (+8 more)

### Community 151 - "BulletLook"
Cohesion: 0.22
Nodes (9): BulletLook, height, sprite, width, Team, id, TeamId, Monster (+1 more)

### Community 152 - "TurretFire"
Cohesion: 0.67
Nodes (3): TurretFire, interval, timer

### Community 153 - "ObstacleMotion"
Cohesion: 0.40
Nodes (5): ObstacleMotion, age, amplitude, baseY, omega

## Knowledge Gaps
- **642 isolated node(s):** `Shoot`, `Hit`, `Explosion`, `BossAlert`, `Victory` (+637 more)
  These have ≤1 connection - possible missing edges or undocumented components. (Counts symbols only; 1244 node(s) total have ≤1 connection when file, concept and rationale nodes are included.)
- **48 thin communities (<3 nodes) omitted from report** — run `graphify query` to explore isolated nodes.

## Suggested Questions
_Questions this graph is uniquely positioned to answer:_

- **Why does `TEST()` connect `Game Tests` to `Behaviours`, `Vec2`, `Entity`, `Game`, `Layers`, `Leviathan.hpp`, `Collectibles.hpp`, `Registry`, `AugmentId`, `Zone`, `Public API: the `Game` class`, `TEST`, `Segments`, `GameTests.cpp`, `Phase`, `Systems.hpp`, `findPlayerBySlot`, `LevelDef`, `ObstacleStep`, `bossHazardSystem`, `Obstacles.hpp`?**
  _High betweenness centrality (0.152) - this node is a cross-community bridge._
- **Are the 75 inferred relationships involving `TEST()` (e.g. with `Registry` and `.alive()`) actually correct?**
  _`TEST()` has 75 INFERRED edges - model-reasoned connections that need verification._
- **What connects `Shoot`, `Hit`, `Explosion` to the rest of the system?**
  _642 weakly-connected nodes found - possible documentation gaps or missing edges._
- **Should `src/App.cpp` be split into smaller, more focused modules?**
  _Cohesion score 0.10574712643678161 - nodes in this community are weakly interconnected._
- **Why does `Registry` connect `Registry` to `IntRect`, `Behaviours`, `size_t`, `Component.hpp`, `Game Tests`, `Vec2`, `Entity`, `Game`, `.updatePlaying`, `BulletLook`, `Replica`, `Layers`, `Leviathan.hpp`, `Collectibles.hpp`, `AugmentId`, `Segments`, `GameTests.cpp`, `GameView.cpp`, `Systems.hpp`, `findPlayerBySlot`, `Archetype`, `bossHazardSystem`, `Obstacles.hpp`?**
  _High betweenness centrality (0.092) - this node is a cross-community bridge._
- **Are the 2 inferred relationships involving `Registry` (e.g. with `TEST()` and `TEST()`) actually correct?**
  _`Registry` has 2 INFERRED edges - model-reasoned connections that need verification._
- **Should `string` be split into smaller, more focused modules?**
  _Cohesion score 0.12280701754385964 - nodes in this community are weakly interconnected._