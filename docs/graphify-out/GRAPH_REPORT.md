# Graph Report - RTYPE  (2026-11-02)

## Corpus Check
- 139 files · ~70,522 words
- Verdict: corpus is large enough that graph structure adds value.
- Unclassified: 5 file(s) not represented in the graph (top: (none) 5)

## Summary
- 2145 nodes · 4340 edges · 148 communities (113 shown, 35 thin omitted)
- Extraction: 87% EXTRACTED · 13% INFERRED · 0% AMBIGUOUS · INFERRED: 557 edges (avg confidence: 0.88)
- Token cost: 0 input · 0 output

## Graph Freshness
- Built from commit: `d22c5113`
- Run `git rev-parse HEAD` and compare to check if the graph is stale.
- Run `graphify update .` after code changes (no API cost).

## Community Hubs (Navigation)
- Signature
- IntRect
- string
- Components.hpp
- .getIndex
- Client Connection
- vector
- R-Type game logic
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
- Column
- GameView
- App
- Client.cpp
- Registry.hpp
- Audio Player Impl
- Command Buffer
- OwnState
- Design Choices Docs
- Audio Player
- Connection Impl
- Replica Events
- Build Tooling Docs
- Message
- Reliable Channel
- AugmentId
- Sprite
- Phase
- Network protocol doc
- Architecture Targets
- Public API: the `Game` class
- TEST
- Segments
- Key
- addGates
- MonsterKind
- R-Type game logic
- Layers
- SpriteId
- Window.cpp
- SpriteBank
- Systems.hpp
- src/App.cpp
- Renderer
- Message Types
- EntitySpawned
- BossKind
- convert
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
- Registry
- MonsterDef
- RefuseReason
- Connection Send
- .reset
- SlotTable
- DisconnectReason
- Knowledge graph (graphify)
- GameStatus
- TrackedTable
- CoinCounter
- App
- game/App.cpp
- .operator()
- pull_request_template.md
- CLAUDE.md
- LevelDef
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
- ComponentId
- UdpSocket
- ProjectileDef
- Controls.cpp
- FixedTimestep
- Renderer.cpp
- TextureManager::Impl
- monsterSystem
- collectMonsters
- Column
- ObstacleKind
- Hud::drawTimer
- How-to
- MonsterState
- PendingSpawn
- .operator()
- Hud::drawLobby
- BossTag
- TeamId
- map2
- Network::Network
- Renderable

## God Nodes (most connected - your core abstractions)
1. `TEST()` - 139 edges
2. `Registry` - 128 edges
3. `Entity` - 106 edges
4. `Game` - 89 edges
5. `Replica` - 50 edges
6. `Server` - 43 edges
7. `Connection` - 43 edges
8. `Layers` - 39 edges
9. `LevelDef` - 38 edges
10. `Client` - 38 edges

## Surprising Connections (you probably didn't know these)
- `The ECS rules to remember` --references--> `each()`  [INFERRED]
  src/client/game/Rtype/doc/README.md → engine/core/include/engine/core/Registry.hpp
- `Use the graph` --references--> `Entity`  [INFERRED]
  docs/graphify.md → engine/core/include/engine/core/Entity.hpp
- `Use the graph` --references--> `Connection`  [INFERRED]
  docs/graphify.md → engine/network/include/engine/network/Connection.hpp
- `The first map: 20 levels` --references--> `map1()`  [INFERRED]
  src/client/game/Rtype/doc/README.md → src/client/game/Rtype/src/maps/map1.cpp
- `Components` --references--> `ProjectileTag`  [INFERRED]
  src/client/game/Rtype/doc/README.md → src/client/game/Rtype/include/logic/Components.hpp

## Import Cycles
- None detected.

## Communities (148 total, 35 thin omitted)

### Community 0 - "Signature"
Cohesion: 0.24
Nodes (5): each(), Signature, WORD_BITS, _words, Add a system

### Community 1 - "IntRect"
Cohesion: 0.17
Nodes (13): Animation, firstFrame, frameCount, frameDuration, step, IntRect, height, left (+5 more)

### Community 2 - "string"
Cohesion: 0.16
Nodes (4): Window, _impl, handle(), App::loop()

### Community 3 - "Components.hpp"
Cohesion: 0.21
Nodes (5): inBurst(), MonsterTag, slot, ProjectileTag, operator+()

### Community 4 - ".getIndex"
Cohesion: 0.15
Nodes (7): EntityRecord, archetype, row, has(), IterationGuard, remove(), tryGet()

### Community 5 - "Client Connection"
Cohesion: 0.06
Nodes (15): Client, _attempts, _connection, _events, _handshakeSequence, _lastAttempt, _server, _socket (+7 more)

### Community 6 - "vector"
Cohesion: 0.12
Nodes (5): SoundManager::Impl, buffers, ids, SoundManager::load(), TextureManager::load()

### Community 7 - "R-Type game logic"
Cohesion: 0.23
Nodes (21): boss_alert(), decay(), defeat(), drums(), encode_ogg(), explosion(), fade(), hit() (+13 more)

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
Cohesion: 0.07
Nodes (55): Entity, id, eachRow(), Registry, _archetypes, EMPTY_ARCHETYPE, _entities, _iterating (+47 more)

### Community 15 - "Connection Channel"
Cohesion: 0.08
Nodes (10): Connection, _channel, _lastPing, _lastReceived, _outbox, _pingId, _pingSentAt, _received (+2 more)

### Community 16 - "Game"
Cohesion: 0.06
Nodes (24): Game, _augments, _coinCounter, _elapsed, _events, _lobby, _nextBoss, _nextNetId (+16 more)

### Community 17 - "UdpSocket::Impl"
Cohesion: 0.05
Nodes (27): Address, ip, port, Datagram, bytes, from, toAddress(), toEndpoint() (+19 more)

### Community 18 - ".updatePlaying"
Cohesion: 0.08
Nodes (13): Replica, CLOCK_CORRECTION_RATE, _cues, _entities, INTERPOLATION_DELAY, _latestTick, MAX_CLOCK_ERROR, _prediction (+5 more)

### Community 19 - "Hud"
Cohesion: 0.10
Nodes (17): Hud, BAR_HEIGHT, BAR_WIDTH, BAR_X, BOSS_BAR_WIDTH, _font, _height, ICON_GAP (+9 more)

### Community 20 - "Entity State Payload"
Cohesion: 0.19
Nodes (18): decodeDestroy(), decodeInput(), decodeLeaderboard(), decodeOwnState(), decodeSelectMap(), decodeSpawn(), decodeStates(), decodeStatus() (+10 more)

### Community 21 - "Prediction"
Cohesion: 0.05
Nodes (26): Add a player action (a new key), Pending, dt, input, sequence, Prediction, _input, MAX_PENDING (+18 more)

### Community 22 - "Binary Writer"
Cohesion: 0.18
Nodes (10): BinaryWriter, _data, MAX_VARINT, Connection::sendId(), Packet::serialize(), Server::sendAccept(), encode(), encodeInput() (+2 more)

### Community 23 - "Packet"
Cohesion: 0.10
Nodes (12): Packet, _messages, Client::checkConnection(), Client::flushConnection(), Client::handleDatagram(), Client::handleHandshake(), Client::handleSession(), Client::tryConnect() (+4 more)

### Community 24 - "Pending Messages"
Cohesion: 0.11
Nodes (8): Pending, message, sentAt, ReliableChannel, _early, _expected, _nextId, _pending

### Community 25 - "Replica"
Cohesion: 0.10
Nodes (10): GameView, ANIMATION_PHASE, _background, _hud, PLAYER_FRAME_DOWN, PLAYER_FRAME_IDLE, PLAYER_FRAME_UP, _screenWidth (+2 more)

### Community 26 - "BinaryReader"
Cohesion: 0.23
Nodes (8): BinaryReader::BinaryReader(), BinaryReader::readBytes(), BinaryReader::readFloat(), BinaryReader::readU16(), BinaryReader::readU32(), BinaryReader::readU8(), BinaryReader::readVarint(), BinaryReader::remaining()

### Community 27 - "Column"
Cohesion: 0.12
Nodes (9): ComponentInfo, infoOf(), swapRemove(), ComponentRegistry::getInfo(), ComponentRegistry::registerType(), RegistryData, ids, infos (+1 more)

### Community 28 - "GameView"
Cohesion: 0.11
Nodes (14): App, _assets, _connectedSince, _controls, FRAMERATE, _last, LISTEN_DELAY, _network (+6 more)

### Community 29 - "App"
Cohesion: 0.10
Nodes (35): The tick: `Game::update`, applyContact(), canHit(), collectProjectiles(), collectTargets(), Collider, box, entity (+27 more)

### Community 30 - "Client.cpp"
Cohesion: 0.09
Nodes (13): DisconnectReason, Kicked, Quit, Shutdown, Timeout, Client::Client(), Client::close(), Client::flush() (+5 more)

### Community 31 - "Registry.hpp"
Cohesion: 0.15
Nodes (3): main(), parsePort(), parsePort()

### Community 32 - "Audio Player Impl"
Cohesion: 0.18
Nodes (4): AudioPlayer::play(), AudioPlayer::playMusic(), AudioPlayer::setMusicVolume(), toSfmlVolume()

### Community 34 - "Command Buffer"
Cohesion: 0.14
Nodes (5): CommandBuffer, _commands, Scheduler, _commands, _systems

### Community 35 - "OwnState"
Cohesion: 0.26
Nodes (3): add(), create(), remove()

### Community 36 - "Design Choices Docs"
Cohesion: 0.17
Nodes (17): Accessibility doc, Audio/visual accessibility (health bars, per-slot sprites; palette, contrast proposed), Cognitive accessibility (simple rules, short levels, clear feedback), rtype::config (Config.hpp constants), Motor accessibility (keyboard only done; remap, toggle fire, gamepad proposed), Choice: per-message reliability, Choice: event-based replication with NetId, Choice: sessions with token, ping, 5s timeout (+9 more)

### Community 38 - "Connection Impl"
Cohesion: 0.18
Nodes (9): Connection::Connection(), Connection::handlePong(), Connection::latency(), Connection::nextSequence(), Connection::peerDisconnect(), Connection::readId(), Connection::resendDelay(), Connection::token() (+1 more)

### Community 39 - "Replica Events"
Cohesion: 0.17
Nodes (5): Unreachable, applyDecoded(), Network, _client, _open

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

### Community 44 - "Sprite"
Cohesion: 0.33
Nodes (5): Sprite, animation, texture, GameView::drawTile(), Hud::drawIcon()

### Community 45 - "Phase"
Cohesion: 0.12
Nodes (9): Audio, _dir, _enabled, _ids, _phase, _player, SOUND_COUNT, _sounds (+1 more)

### Community 46 - "Network protocol doc"
Cohesion: 0.17
Nodes (21): Architecture doc, Build and run doc, CMake presets linux-debug / windows-debug (Ninja on Linux), Build troubleshooting table, Contributing doc, Branch prefixes and commit types, Coding conventions (80 cols, 25-line functions, 5 params, 2 nested ifs, no ternary, _ private prefix), Part ownership (engine/core, server, client, game logic each have an owner) (+13 more)

### Community 47 - "Architecture Targets"
Cohesion: 0.15
Nodes (18): r-type_client (display executable), Client replica of the world (entity kind + position), engine::audio (sounds and music, wraps SFML), engine::core (Registry, Scheduler, CommandBuffer, FixedTimestep), engine::network (UDP transport, sessions, ping, reliability, wraps Asio), engine::window (rendering, wraps SFML), Three decoupled layers: rendering, networking, game logic, rtype::common (shared headers: Vec2, return codes) (+10 more)

### Community 49 - "TEST"
Cohesion: 0.07
Nodes (8): Leaderboard, _records, RunRecord, players, timeMs, Audio::Audio(), sampleStatus(), TEST()

### Community 50 - "Segments"
Cohesion: 0.13
Nodes (14): firstLevel(), lastSpawnPoint(), levelAt(), levelSpan(), levelStart(), Segments, bossSpawn, levels (+6 more)

### Community 51 - "Key"
Cohesion: 0.22
Nodes (9): Key, Down, Enter, Escape, Left, Right, Space, Unknown (+1 more)

### Community 52 - "addGates"
Cohesion: 0.15
Nodes (16): addCave(), addGates(), CavePiece, bottom, top, Gap, center, height (+8 more)

### Community 53 - "MonsterKind"
Cohesion: 0.20
Nodes (10): MonsterType, kind, MonsterKind, Basic, Bomber, Count, Elite, Kamikaze (+2 more)

### Community 54 - "R-Type game logic"
Cohesion: 0.33
Nodes (6): Coding conventions, Configuration, Current status, Overview, R-Type game logic, The first map: 20 levels

### Community 55 - "Layers"
Cohesion: 0.08
Nodes (43): Architecture, Collisions, Components, Data oriented design, Layers, Shooting, Systems, The ECS rules to remember (+35 more)

### Community 56 - "SpriteId"
Cohesion: 0.29
Nodes (3): Controls, _input, _respawn

### Community 57 - "Window.cpp"
Cohesion: 0.18
Nodes (3): toKey(), Window::clear(), Window::Window()

### Community 58 - "SpriteBank"
Cohesion: 0.15
Nodes (7): explosionCue(), Replica::applyPrediction(), Replica::localPlayer(), Replica::mapCursor(), Replica::record(), Replica::recordSpawnCue(), Replica::slot()

### Community 59 - "Systems.hpp"
Cohesion: 0.15
Nodes (16): Position, velocity, speed, direction, bossSystem(), volley(), calculatePos(), inputDirection(), bombVelocity(), isReadyToShoot(), lifetimeSystem() (+8 more)

### Community 60 - "src/App.cpp"
Cohesion: 0.24
Nodes (6): AudioPlayer::Impl, music, musicVolume, next, soundVolume, voices

### Community 61 - "Renderer"
Cohesion: 0.10
Nodes (17): Renderer, Background, LAYERS, _stars, STARS_PER_LAYER, _width, _worldToScreen, Star (+9 more)

### Community 62 - "Message Types"
Cohesion: 0.22
Nodes (5): Message, id, payload, reliable, type

### Community 63 - "EntitySpawned"
Cohesion: 0.12
Nodes (13): EntityDestroyed, id, EntitySpawned, box, id, pos, slot, sprite (+5 more)

### Community 64 - "BossKind"
Cohesion: 0.25
Nodes (8): BossType, kind, BossKind, Count, Dasher, Fortress, Standard, Summoner

### Community 65 - "convert"
Cohesion: 0.16
Nodes (13): Closed, FocusGained, FocusLost, KeyPressed, key, KeyReleased, key, Resized (+5 more)

### Community 66 - "describeSpawn"
Cohesion: 0.11
Nodes (11): Connection::flush(), Packet::add(), Packet::isHandshake(), Packet::isValid(), Packet::Packet(), Packet::parse(), Packet::parseMessage(), Packet::sequence() (+3 more)

### Community 67 - "SpriteId"
Cohesion: 0.09
Nodes (22): SpriteId, AlienShard, Bomb, Bomber, Boss, Coin, Count, DasherBoss (+14 more)

### Community 68 - "time_point"
Cohesion: 0.25
Nodes (3): Connection::receive(), Connection::timedOut(), Connection::update()

### Community 69 - "Connection::handle"
Cohesion: 0.22
Nodes (3): Connection::handle(), Connection::handleUser(), Connection::sendPing()

### Community 70 - "Session Info"
Cohesion: 0.18
Nodes (8): _random, Session, address, connection, id, Server::findByAddress(), Server::findByToken(), Server::makeToken()

### Community 71 - "Renderer.cpp"
Cohesion: 0.14
Nodes (15): Color, a, b, g, r, Vector2f, x, y (+7 more)

### Community 72 - "SpriteBank"
Cohesion: 0.12
Nodes (7): TextureManager, _impl, SpriteBank, _assets, _clock, _sprites, SpriteBank::SpriteBank()

### Community 73 - "EntityState"
Cohesion: 0.15
Nodes (12): EntityState, id, pos, collectSpawns(), collectStates(), encodeStates(), InputMessage, input (+4 more)

### Community 74 - "WaveStep"
Cohesion: 0.14
Nodes (11): appendLevel(), buildMap(), buildRun(), shifted(), WaveStep, at, count, dy (+3 more)

### Community 75 - "Layers"
Cohesion: 0.15
Nodes (8): BinaryReader, _cursor, PlayerStatus, alive, hp, slot, readPlayers(), readPlayerStatus()

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

### Community 83 - "Registry"
Cohesion: 0.29
Nodes (3): EntityManager, _freeList, _generations

### Community 84 - "MonsterDef"
Cohesion: 0.22
Nodes (9): MonsterDef, damage, fireCooldown, height, hp, projectile, speed, sprite (+1 more)

### Community 85 - "RefuseReason"
Cohesion: 0.40
Nodes (5): Refused, reason, RefuseReason, Full, Version

### Community 86 - "Connection Send"
Cohesion: 0.47
Nodes (3): Connection::send(), Connection::sendEngine(), Connection::takeMessages()

### Community 87 - ".reset"
Cohesion: 0.14
Nodes (3): Game rules, Maps and lobby, findFreeSlot()

### Community 88 - "SlotTable"
Cohesion: 0.22
Nodes (9): Sound, BossAlert, Coin, Count, Defeat, Explosion, Hit, Shoot (+1 more)

### Community 89 - "DisconnectReason"
Cohesion: 0.22
Nodes (8): Cue, BossAppears, BossExplosion, CoinPickup, Explosion, Hit, PlayerShot, Replica::takeCues()

### Community 90 - "Knowledge graph (graphify)"
Cohesion: 0.22
Nodes (5): Sample, pos, tick, Replica::interpolate(), Replica::update()

### Community 91 - "GameStatus"
Cohesion: 0.13
Nodes (12): GameStatus, bossHp, bossMaxHp, coins, elapsedMs, level, map, phase (+4 more)

### Community 92 - "TrackedTable"
Cohesion: 0.17
Nodes (7): Font, GLYPH_HEIGHT, GLYPH_WIDTH, Glyph, rows, symbol, Hud::textTop()

### Community 93 - "CoinCounter"
Cohesion: 0.33
Nodes (5): CoinCounter, quantity, total, coins, value

### Community 94 - "App"
Cohesion: 0.13
Nodes (11): App, _game, _lastInputs, _lastPhase, _leaderboard, LEADERBOARD_FILE, _server, _slots (+3 more)

### Community 96 - ".operator()"
Cohesion: 0.40
Nodes (5): Install, Keep it up to date: the git hook, Knowledge graph (graphify), Use the graph, With an AI agent (Claude Code)

### Community 97 - "pull_request_template.md"
Cohesion: 0.40
Nodes (5): Track, Boss, Level, None, Audio::playMusic()

### Community 98 - "CLAUDE.md"
Cohesion: 0.17
Nodes (10): isValid(), StatusHeader, bossHp, bossMaxHp, coins, count, elapsedMs, level (+2 more)

### Community 100 - "LevelDef"
Cohesion: 0.21
Nodes (27): LevelDef, bosses, length, obstacles, waves, wave(), level1(), level10() (+19 more)

### Community 103 - "src/App.cpp"
Cohesion: 0.12
Nodes (4): App::App(), App::listening(), App::playSounds(), App::run()

### Community 105 - "Archetype"
Cohesion: 0.29
Nodes (6): Archetype, _columns, _columnsIndex, _entities, NO_COLUMN, _signature

### Community 106 - "ObstacleStep"
Cohesion: 0.20
Nodes (12): Obstacles, addGate(), wall(), ObstacleStep, at, height, kind, width (+4 more)

### Community 107 - "Audio.cpp"
Cohesion: 0.18
Nodes (5): Audio::follow(), Audio::onPhase(), Audio::play(), Audio::playCue(), Audio::playSound()

### Community 111 - "Gameplay"
Cohesion: 0.18
Nodes (10): Bosses, Coins, time and level skip, Controls, Enemies, Gameplay, Goal, Leaderboard, Lobby and maps (+2 more)

### Community 113 - "SpriteBank.cpp"
Cohesion: 0.20
Nodes (3): SpriteBank::add(), SpriteBank::loadMisc(), SpriteBank::texture()

### Community 115 - "Hud.cpp"
Cohesion: 0.27
Nodes (5): formatTime(), Hud::drawMapList(), Hud::drawRecords(), Hud::drawRespawnHint(), twoDigits()

### Community 116 - "ComponentId"
Cohesion: 0.20
Nodes (5): ComponentId, id, ComponentRegistry, getId(), idOf()

### Community 119 - "ProjectileDef"
Cohesion: 0.22
Nodes (9): Kinds of monsters and bosses, ProjectileDef, drop, height, shots, speed, spread, sprite (+1 more)

### Community 120 - "Controls.cpp"
Cohesion: 0.25
Nodes (3): Controls::applyKey(), Controls::handle(), Controls::poll()

### Community 121 - "FixedTimestep"
Cohesion: 0.25
Nodes (3): FixedTimestep, _accumulator, _maxAccumulator

### Community 122 - "Renderer.cpp"
Cohesion: 0.29
Nodes (4): Renderer::draw(), Renderer::drawRect(), Renderer::drawResized(), Renderer::Renderer()

### Community 123 - "TextureManager::Impl"
Cohesion: 0.25
Nodes (3): TextureManager::Impl, ids, textures

### Community 124 - "monsterSystem"
Cohesion: 0.25
Nodes (8): Behaviours, dasherTarget(), DashTarget, pos, velocity, roamBoss(), hasEntered(), monsterSystem()

### Community 125 - "collectMonsters"
Cohesion: 0.32
Nodes (5): countMinions(), collectMonsters(), collectObstacles(), collectProjectileEntities(), collectWith()

### Community 126 - "Column"
Cohesion: 0.38
Nodes (3): Column, _data, _info

### Community 127 - "ObstacleKind"
Cohesion: 0.29
Nodes (7): ObstacleTag, kind, obstacleSprite(), ObstacleKind, Count, Hazard, Rock

### Community 128 - "Hud::drawTimer"
Cohesion: 0.47
Nodes (3): Hud::drawCoins(), Hud::drawLevel(), Hud::drawTimer()

### Community 129 - "How-to"
Cohesion: 0.50
Nodes (4): Add a component, Add a map or a level, Add a new kind of monster, How-to

### Community 130 - "MonsterState"
Cohesion: 0.50
Nodes (4): MonsterState, age, locked, summoned

### Community 131 - "PendingSpawn"
Cohesion: 0.50
Nodes (4): PendingSpawn, delay, kind, y

### Community 134 - "BossTag"
Cohesion: 0.67
Nodes (3): BossTag, age, summonTimer

### Community 135 - "TeamId"
Cohesion: 0.67
Nodes (3): TeamId, Monster, Player

## Knowledge Gaps
- **476 isolated node(s):** `WINDOW_WIDTH`, `WINDOW_HEIGHT`, `FRAMERATE`, `WORLD_TO_SCREEN`, `LISTEN_DELAY` (+471 more)
  These have ≤1 connection - possible missing edges or undocumented components. (Counts symbols only; 994 node(s) total have ≤1 connection when file, concept and rationale nodes are included.)
- **35 thin communities (<3 nodes) omitted from report** — run `graphify query` to explore isolated nodes.

## Suggested Questions
_Questions this graph is uniquely positioned to answer:_

- **Why does `TEST()` connect `Game Tests` to `Replica.hpp`, `.getIndex`, `WaveStep`, `AugmentId`, `ObstacleStep`, `Registry`, `Game`, `Public API: the `Game` class`, `Phase`, `TEST`, `Segments`, `App`, `.reset`, `Layers`, `Systems.hpp`, `monsterSystem`, `collectMonsters`?**
  _High betweenness centrality (0.139) - this node is a cross-community bridge._
- **Are the 56 inferred relationships involving `TEST()` (e.g. with `Registry` and `.alive()`) actually correct?**
  _`TEST()` has 56 INFERRED edges - model-reasoned connections that need verification._
- **What connects `WINDOW_WIDTH`, `WINDOW_HEIGHT`, `FRAMERATE` to the rest of the system?**
  _476 weakly-connected nodes found - possible documentation gaps or missing edges._
- **Should `.getIndex` be split into smaller, more focused modules?**
  _Cohesion score 0.14761904761904762 - nodes in this community are weakly interconnected._
- **Why does `Game` connect `Game` to `Components.hpp`, `PendingSpawn`, `AugmentId`, `Game Tests`, `Components.hpp`, `Registry`, `Public API: the `Game` class`, `Phase`, `Segments`, `App`, `Prediction`, `.reset`, `Systems.hpp`, `collectMonsters`, `App`?**
  _High betweenness centrality (0.111) - this node is a cross-community bridge._
- **Are the 2 inferred relationships involving `Registry` (e.g. with `TEST()` and `TEST()`) actually correct?**
  _`Registry` has 2 INFERRED edges - model-reasoned connections that need verification._
- **Should `Client Connection` be split into smaller, more focused modules?**
  _Cohesion score 0.058823529411764705 - nodes in this community are weakly interconnected._