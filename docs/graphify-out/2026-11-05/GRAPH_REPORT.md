# Graph Report - RTYPE  (2026-11-05)

## Corpus Check
- 123 files · ~57,512 words
- Verdict: corpus is large enough that graph structure adds value.
- Unclassified: 5 file(s) not represented in the graph (top: (none) 5)

## Summary
- 1706 nodes · 3392 edges · 111 communities (89 shown, 22 thin omitted)
- Extraction: 88% EXTRACTED · 12% INFERRED · 0% AMBIGUOUS · INFERRED: 395 edges (avg confidence: 0.88)
- Token cost: 0 input · 0 output

## Graph Freshness
- Built from commit: `8a5948a0`
- Run `git rev-parse HEAD` and compare to check if the graph is stale.
- Run `graphify update .` after code changes (no API cost).

## Community Hubs (Navigation)
- ECS Column Storage
- IntRect
- Window
- Registry
- Client Connection
- Sound Manager
- make_sounds.py
- Delivery and Sessions
- Build and Protocol Docs
- Server Events
- Vectors.hpp
- Game Tests
- Network Includes
- Entity Registry
- Connection Channel
- Game
- UdpSocket::Impl
- Replica
- Hud
- Entity State Payload
- Prediction
- Binary Writer
- Handshake Sequencing
- Pending Messages
- GameView
- BinaryReader
- Systems.hpp
- App
- Collider
- Disconnect Handling
- Client Main
- Audio Player Impl
- Replica.hpp
- Command Buffer
- Design Choices Docs
- Audio Player
- Connection Impl
- Replica Events
- Build Tooling Docs
- App
- Reliable Channel
- AugmentId
- SpriteBank
- Audio
- Network protocol doc
- Architecture Targets
- EntitySpawned
- Status Tests
- Background
- Keys
- Prediction.cpp
- EntityState
- PlayerInput
- shotVelocities
- Controls
- Window::Window
- Replica.cpp
- coinPickupSystem
- Audio Impl
- findPlayerBySlot
- Message Types
- Session Admission
- SpriteId
- Window.cpp
- R-Type game logic
- Layers
- Connection::receive
- Connection::handle
- Session Info
- Renderer Draw
- Renderer
- Texture Manager Impl
- WaveStep
- describeSpawn
- Window Impl Header
- TEST
- GameStatus
- Components
- Test Harness
- Audio.cpp
- Sound Manager Impl
- Entity Manager
- Entity Records
- Flush and Serialize
- Connection Send
- Font.cpp
- Sound
- Cue
- Sample
- Team
- Font
- CoinCounter
- duration
- Phase
- Knowledge graph (graphify)
- Track
- Rarity
- .addPlayer
- App::App
- .operator()
- Connection::update
- App::run
- pull_request_template.md
- .load
- Audio::play
- PendingSpawn
- CLAUDE.md
- vcpkg.json

## God Nodes (most connected - your core abstractions)
1. `Registry` - 94 edges
2. `Entity` - 82 edges
3. `Game` - 67 edges
4. `TEST()` - 60 edges
5. `Connection` - 43 edges
6. `Server` - 43 edges
7. `Replica` - 43 edges
8. `Client` - 38 edges
9. `Audio` - 28 edges
10. `Hud` - 28 edges

## Surprising Connections (you probably didn't know these)
- `Use the graph` --references--> `Entity`  [INFERRED]
  docs/graphify.md → engine/core/include/engine/core/Entity.hpp
- `The ECS rules to remember` --references--> `each()`  [INFERRED]
  src/client/game/Rtype/doc/README.md → engine/core/include/engine/core/Registry.hpp
- `Use the graph` --references--> `Connection`  [INFERRED]
  docs/graphify.md → engine/network/include/engine/network/Connection.hpp
- `Add a system` --references--> `each()`  [INFERRED]
  src/client/game/Rtype/doc/README.md → engine/core/include/engine/core/Registry.hpp
- `Add a new kind of monster` --references--> `createMonster()`  [INFERRED]
  src/client/game/Rtype/doc/README.md → src/client/game/Rtype/include/logic/Factories.hpp

## Import Cycles
- None detected.

## Hyperedges (group relationships)
- **Reliable game messages (SPAWN, DESTROY, YOU, RESPAWN)** — docs_protocol_spawn, docs_protocol_destroy, docs_protocol_you, docs_protocol_respawn [EXTRACTED 1.00]
- **Server glue: network + game + core via App** — docs_architecture_server_app_glue, docs_architecture_engine_network, docs_architecture_rtype_game [EXTRACTED 1.00]
- **Unreliable game messages (INPUT, STATES, STATUS)** — docs_protocol_input, docs_protocol_states, docs_protocol_status [EXTRACTED 1.00]

## Communities (111 total, 22 thin omitted)

### Community 0 - "ECS Column Storage"
Cohesion: 0.05
Nodes (28): Archetype, _columns, _columnsIndex, _entities, NO_COLUMN, _signature, Column, _data (+20 more)

### Community 1 - "IntRect"
Cohesion: 0.16
Nodes (14): Animation, firstFrame, frameCount, frameDuration, step, IntRect, height, left (+6 more)

### Community 2 - "Window"
Cohesion: 0.14
Nodes (5): Window, _impl, handle(), App::listening(), App::loop()

### Community 4 - "Registry"
Cohesion: 0.11
Nodes (19): eachRow(), Registry, _archetypes, EMPTY_ARCHETYPE, _entities, _iterating, NO_ARCHETYPE, _records (+11 more)

### Community 5 - "Client Connection"
Cohesion: 0.05
Nodes (17): Client, _attempts, _connection, _events, _handshakeSequence, _lastAttempt, _server, _socket (+9 more)

### Community 6 - "Sound Manager"
Cohesion: 0.11
Nodes (5): SoundManager::Impl, buffers, ids, SoundManager::load(), TextureManager::load()

### Community 7 - "make_sounds.py"
Cohesion: 0.23
Nodes (21): boss_alert(), decay(), defeat(), drums(), encode_ogg(), explosion(), fade(), hit() (+13 more)

### Community 8 - "Delivery and Sessions"
Cohesion: 0.11
Nodes (17): Delivery, Reliable, Unreliable, Server::admit(), Server::broadcast(), Server::checkSessions(), Server::clientCount(), Server::close() (+9 more)

### Community 9 - "Build and Protocol Docs"
Cohesion: 0.14
Nodes (20): engine_audio library, SFML::Audio, engine build root, engine_core library, Game protocol, Handshake and connection, Network module (engine::network), ReliableChannel (+12 more)

### Community 10 - "Server Events"
Cohesion: 0.07
Nodes (9): Server, _events, _maxClients, _nextClientId, _random, _sessions, _socket, _version (+1 more)

### Community 11 - "Vectors.hpp"
Cohesion: 0.25
Nodes (13): The ECS rules to remember, GameView::drawEntity(), Prediction::position(), length(), normalize(), operator+(), Vec2, x (+5 more)

### Community 12 - "Game Tests"
Cohesion: 0.07
Nodes (7): bossSystem(), kill(), killBoss(), playerOf(), run(), skipToBoss(), TEST()

### Community 14 - "Entity Registry"
Cohesion: 0.14
Nodes (12): add(), create(), remove(), Entity, id, has(), remove(), tryGet() (+4 more)

### Community 15 - "Connection Channel"
Cohesion: 0.08
Nodes (10): Connection, _channel, _lastPing, _lastReceived, _outbox, _pingId, _pingSentAt, _received (+2 more)

### Community 16 - "Game"
Cohesion: 0.09
Nodes (14): Game, _augments, _bossSpawned, _coinCounter, _events, _nextNetId, _nextWave, _pending (+6 more)

### Community 17 - "UdpSocket::Impl"
Cohesion: 0.05
Nodes (27): Address, ip, port, Datagram, bytes, from, toAddress(), toEndpoint() (+19 more)

### Community 18 - "Replica"
Cohesion: 0.08
Nodes (12): Replica, CLOCK_CORRECTION_RATE, _cues, _entities, INTERPOLATION_DELAY, _latestTick, MAX_CLOCK_ERROR, _prediction (+4 more)

### Community 19 - "Hud"
Cohesion: 0.11
Nodes (16): Hud, BAR_HEIGHT, BAR_WIDTH, BAR_X, BOSS_BAR_WIDTH, COIN_ICON_GAP, COIN_ICON_SIZE, COIN_MARGIN (+8 more)

### Community 20 - "Entity State Payload"
Cohesion: 0.27
Nodes (14): decodeDestroy(), decodeInput(), decodeOwnState(), decodeSpawn(), decodeStates(), decodeStatus(), decodeYou(), encodeKeys() (+6 more)

### Community 21 - "Prediction"
Cohesion: 0.09
Nodes (13): Pending, dt, input, sequence, Prediction, _input, MAX_PENDING, _nextSequence (+5 more)

### Community 22 - "Binary Writer"
Cohesion: 0.12
Nodes (14): BinaryWriter, _data, MAX_VARINT, BinaryWriter::writeBytes(), BinaryWriter::writeFloat(), BinaryWriter::writeU16(), BinaryWriter::writeU32(), BinaryWriter::writeU8() (+6 more)

### Community 23 - "Handshake Sequencing"
Cohesion: 0.11
Nodes (9): Packet, _messages, Client::checkConnection(), Client::handleDatagram(), Client::handleHandshake(), Client::handleSession(), Connection::hasHandshakeMessage(), Packet::parse() (+1 more)

### Community 24 - "Pending Messages"
Cohesion: 0.11
Nodes (8): Pending, message, sentAt, ReliableChannel, _early, _expected, _nextId, _pending

### Community 25 - "GameView"
Cohesion: 0.11
Nodes (14): GameView, ANIMATION_PHASE, _background, _hud, PLAYER_FRAME_DOWN, PLAYER_FRAME_IDLE, PLAYER_FRAME_UP, _worldToScreen (+6 more)

### Community 26 - "BinaryReader"
Cohesion: 0.07
Nodes (19): BinaryReader, _cursor, BinaryReader::BinaryReader(), BinaryReader::readBytes(), BinaryReader::readFloat(), BinaryReader::readU16(), BinaryReader::readU32(), BinaryReader::readU8() (+11 more)

### Community 27 - "Systems.hpp"
Cohesion: 0.21
Nodes (14): The tick: `Game::update`, collectMonsters(), collectProjectileEntities(), contactSystem(), deleteUnitsOutOfBounds(), destroyFarEntities(), isReadyToShoot(), lifetimeSystem() (+6 more)

### Community 28 - "App"
Cohesion: 0.11
Nodes (14): App, _assets, _connectedSince, _controls, FRAMERATE, _last, LISTEN_DELAY, _network (+6 more)

### Community 29 - "Collider"
Cohesion: 0.23
Nodes (16): applyContact(), canHit(), collectProjectiles(), collectTargets(), Collider, box, entity, pos (+8 more)

### Community 30 - "Disconnect Handling"
Cohesion: 0.09
Nodes (13): DisconnectReason, Kicked, Quit, Shutdown, Timeout, Client::Client(), Client::close(), Client::flush() (+5 more)

### Community 31 - "Client Main"
Cohesion: 0.10
Nodes (3): main(), parsePort(), parsePort()

### Community 32 - "Audio Player Impl"
Cohesion: 0.15
Nodes (5): AudioPlayer::AudioPlayer(), AudioPlayer::play(), AudioPlayer::playMusic(), AudioPlayer::setMusicVolume(), toSfmlVolume()

### Community 34 - "Command Buffer"
Cohesion: 0.14
Nodes (5): CommandBuffer, _commands, Scheduler, _commands, _systems

### Community 36 - "Design Choices Docs"
Cohesion: 0.20
Nodes (14): Audio/visual accessibility (health bars, per-slot sprites; palette, contrast proposed), rtype::config (Config.hpp constants), Motor accessibility (keyboard only done; remap, toggle fire, gamepad proposed), Choice: per-message reliability, Choice: event-based replication with NetId, Choice: sessions with token, ping, 5s timeout, DESTROY 0x21 (S->C, reliable, entity id), Connection lifecycle / Handshake (Connect, Accept(token), Refuse, YOU, SPAWN, Ping, timeout 5s) (+6 more)

### Community 38 - "Connection Impl"
Cohesion: 0.21
Nodes (8): Connection::Connection(), Connection::handlePong(), Connection::handleUser(), Connection::nextSequence(), Connection::readId(), Connection::sendPing(), Connection::timedOut(), Connection::token()

### Community 39 - "Replica Events"
Cohesion: 0.15
Nodes (6): Unreachable, applyDecoded(), Network, _client, Network::Network(), _open

### Community 40 - "Build Tooling Docs"
Cohesion: 0.10
Nodes (23): Dependency rules, engine::audio (sounds and music, wraps SFML), Game::setInput / Game::update(dt), Server App class (only place including both network and game), Server threads: network thread and game thread, vcpkg submodule (external/vcpkg, manifest vcpkg.json, Asio + SFML 3), Technical and comparative study doc, Choice: AABB collision (+15 more)

### Community 41 - "App"
Cohesion: 0.05
Nodes (21): FixedTimestep, _accumulator, _maxAccumulator, Connected, client, Disconnected, client, reason (+13 more)

### Community 42 - "Reliable Channel"
Cohesion: 0.23
Nodes (5): ReliableChannel::acknowledge(), ReliableChannel::receive(), ReliableChannel::resend(), ReliableChannel::send(), ReliableChannel::takeReady()

### Community 43 - "AugmentId"
Cohesion: 0.17
Nodes (13): applyAugment(), AugmentDef, apply, id, price, rarity, AugmentId, Count (+5 more)

### Community 44 - "SpriteBank"
Cohesion: 0.13
Nodes (8): Sprite, animation, texture, SpriteBank, _sprites, Hud::Hud(), SpriteBank::set(), SpriteBank::SpriteBank()

### Community 45 - "Audio"
Cohesion: 0.13
Nodes (9): Audio, _dir, _enabled, _ids, _phase, _player, SOUND_COUNT, _sounds (+1 more)

### Community 46 - "Network protocol doc"
Cohesion: 0.17
Nodes (20): Accessibility doc, Cognitive accessibility (simple rules, short levels, clear feedback), Architecture doc, Build and run doc, CMake presets linux-debug / windows-debug (Ninja on Linux), Build troubleshooting table, Contributing doc, Branch prefixes and commit types (+12 more)

### Community 47 - "Architecture Targets"
Cohesion: 0.16
Nodes (17): r-type_client (display executable), Client replica of the world (entity kind + position), engine::core (Registry, Scheduler, CommandBuffer, FixedTimestep), engine::network (UDP transport, sessions, ping, reliability, wraps Asio), engine::window (rendering, wraps SFML), Three decoupled layers: rendering, networking, game logic, rtype::common (shared headers: Vec2, return codes), rtype::game (header-only game logic) (+9 more)

### Community 48 - "EntitySpawned"
Cohesion: 0.14
Nodes (13): EntityDestroyed, id, EntitySpawned, box, id, pos, slot, sprite (+5 more)

### Community 49 - "Status Tests"
Cohesion: 0.14
Nodes (4): InputMessage, input, sequence, TEST()

### Community 50 - "Background"
Cohesion: 0.14
Nodes (12): Background, LAYERS, _stars, STARS_PER_LAYER, _width, _worldToScreen, Star, layer (+4 more)

### Community 51 - "Keys"
Cohesion: 0.15
Nodes (11): Key, Down, Enter, Escape, Left, Right, Space, Unknown (+3 more)

### Community 52 - "Prediction.cpp"
Cohesion: 0.17
Nodes (7): Position, velocity, speed, direction, calculatePos(), inputDirection(), Prediction::reconcile(), Prediction::record(), Prediction::step(), Prediction::velocity()

### Community 53 - "EntityState"
Cohesion: 0.23
Nodes (9): EntityState, id, pos, Replica::record(), encodeStateChunk(), encodeStates(), StatesSnapshot, states (+1 more)

### Community 54 - "PlayerInput"
Cohesion: 0.18
Nodes (11): Add a component, Add a new kind of monster, Add a player action (a new key), How-to, Public API: the `Game` class, PlayerInput, down, fire (+3 more)

### Community 55 - "shotVelocities"
Cohesion: 0.18
Nodes (11): Shooting, Systems, bossFan(), Damage, value, Speed, value, Weapon (+3 more)

### Community 56 - "Controls"
Cohesion: 0.15
Nodes (8): KeyPressed, key, KeyReleased, key, Controls, _input, _respawn, Controls::handle()

### Community 58 - "Replica.cpp"
Cohesion: 0.20
Nodes (6): explosionCue(), Replica::applyPrediction(), Replica::localPlayer(), Replica::predict(), Replica::recordSpawnCue(), Replica::slot()

### Community 59 - "coinPickupSystem"
Cohesion: 0.21
Nodes (9): addToPurse(), coinPickupSystem(), collectCoins(), touchesAny(), collectColliders(), collectPlayers(), collectWith(), getCollider() (+1 more)

### Community 60 - "Audio Impl"
Cohesion: 0.25
Nodes (6): AudioPlayer::Impl, music, musicVolume, next, soundVolume, voices

### Community 61 - "findPlayerBySlot"
Cohesion: 0.24
Nodes (3): Game rules, What the server reads, findPlayerBySlot()

### Community 62 - "Message Types"
Cohesion: 0.22
Nodes (5): Message, id, payload, reliable, type

### Community 63 - "Session Admission"
Cohesion: 0.25
Nodes (3): Server::handleConnect(), Server::handleSession(), Server::open()

### Community 64 - "SpriteId"
Cohesion: 0.18
Nodes (11): Renderable, id, SpriteId, Boss, Coin, Count, Explosion, Monster (+3 more)

### Community 65 - "Window.cpp"
Cohesion: 0.14
Nodes (10): FocusGained, FocusLost, Resized, height, width, convert(), toKey(), translate() (+2 more)

### Community 66 - "R-Type game logic"
Cohesion: 0.20
Nodes (10): Architecture, Coding conventions, Configuration, Current status, Data oriented design, Overview, R-Type game logic, PlayerTag (+2 more)

### Community 67 - "Layers"
Cohesion: 0.31
Nodes (9): Layers, bulletDirection(), BulletLook, height, sprite, width, bulletStartPos(), isShooter() (+1 more)

### Community 70 - "Session Info"
Cohesion: 0.25
Nodes (6): Session, address, connection, id, Server::findByAddress(), Server::findByToken()

### Community 71 - "Renderer Draw"
Cohesion: 0.29
Nodes (4): Renderer::draw(), Renderer::drawRect(), Renderer::drawResized(), Renderer::Renderer()

### Community 72 - "Renderer"
Cohesion: 0.17
Nodes (5): Renderer, TextureManager, _impl, Background::draw(), Font::Font()

### Community 73 - "Texture Manager Impl"
Cohesion: 0.25
Nodes (3): TextureManager::Impl, ids, textures

### Community 74 - "WaveStep"
Cohesion: 0.20
Nodes (6): WaveStep, at, count, dy, interval, y

### Community 75 - "describeSpawn"
Cohesion: 0.24
Nodes (6): collectSpawns(), collectStates(), collectUntracked(), describeSpawn(), isReplicable(), slotOf()

### Community 78 - "GameStatus"
Cohesion: 0.11
Nodes (15): GameStatus, bossHp, bossMaxHp, coins, phase, players, PlayerStatus, alive (+7 more)

### Community 79 - "Components"
Cohesion: 0.22
Nodes (8): Components, BossTag, age, MonsterTag, slot, NetId, value, ProjectileTag

### Community 80 - "Test Harness"
Cohesion: 0.33
Nodes (4): Case, name, run, Registrar

### Community 81 - "Audio.cpp"
Cohesion: 0.31
Nodes (4): Audio::follow(), Audio::onPhase(), Audio::playCue(), Audio::playSound()

### Community 83 - "Entity Manager"
Cohesion: 0.29
Nodes (3): EntityManager, _freeList, _generations

### Community 84 - "Entity Records"
Cohesion: 0.40
Nodes (4): EntityRecord, archetype, row, IterationGuard

### Community 85 - "Flush and Serialize"
Cohesion: 0.20
Nodes (9): Refused, reason, RefuseReason, Full, Version, Client::flushConnection(), Client::tryConnect(), Server::flush() (+1 more)

### Community 86 - "Connection Send"
Cohesion: 0.28
Nodes (4): Connection::flush(), Connection::send(), Connection::sendEngine(), Connection::takeMessages()

### Community 87 - "Font.cpp"
Cohesion: 0.11
Nodes (19): Color, a, b, g, r, Vector2f, x, y (+11 more)

### Community 88 - "Sound"
Cohesion: 0.22
Nodes (9): Sound, BossAlert, Coin, Count, Defeat, Explosion, Hit, Shoot (+1 more)

### Community 89 - "Cue"
Cohesion: 0.22
Nodes (8): Cue, BossAppears, BossExplosion, CoinPickup, Explosion, Hit, PlayerShot, Replica::takeCues()

### Community 90 - "Sample"
Cohesion: 0.22
Nodes (5): Sample, pos, tick, Replica::interpolate(), Replica::update()

### Community 91 - "Team"
Cohesion: 0.25
Nodes (8): Collisions, Health, hp, Team, id, TeamId, Monster, Player

### Community 92 - "Font"
Cohesion: 0.29
Nodes (3): Font, GLYPH_WIDTH, Hud::drawCoins()

### Community 93 - "CoinCounter"
Cohesion: 0.33
Nodes (5): CoinCounter, quantity, total, coins, value

### Community 94 - "duration"
Cohesion: 0.40
Nodes (4): Connection::latency(), Connection::peerDisconnect(), Connection::resendDelay(), duration

### Community 95 - "Phase"
Cohesion: 0.33
Nodes (5): Phase, Defeat, Playing, Victory, Hud::trackPhase()

### Community 96 - "Knowledge graph (graphify)"
Cohesion: 0.40
Nodes (5): Install, Keep it up to date: the git hook, Knowledge graph (graphify), Use the graph, With an AI agent (Claude Code)

### Community 97 - "Track"
Cohesion: 0.40
Nodes (5): Track, Boss, Level, None, Audio::playMusic()

### Community 98 - "Rarity"
Cohesion: 0.40
Nodes (5): Rarity, Common, Epic, Legendary, Rare

### Community 107 - "PendingSpawn"
Cohesion: 0.67
Nodes (3): PendingSpawn, delay, y

## Knowledge Gaps
- **371 isolated node(s):** `_impl`, `voices`, `next`, `music`, `soundVolume` (+366 more)
  These have ≤1 connection - possible missing edges or undocumented components. (Counts symbols only; 762 node(s) total have ≤1 connection when file, concept and rationale nodes are included.)
- **22 thin communities (<3 nodes) omitted from report** — run `graphify query` to explore isolated nodes.

## Suggested Questions
_Questions this graph is uniquely positioned to answer:_

- **Why does `Registry` connect `Registry` to `ECS Column Storage`, `IntRect`, `Vectors.hpp`, `Game Tests`, `Entity Registry`, `Game`, `Replica`, `GameView`, `Systems.hpp`, `Collider`, `Command Buffer`, `Registry.hpp`, `AugmentId`, `shotVelocities`, `coinPickupSystem`, `findPlayerBySlot`, `R-Type game logic`, `Layers`, `describeSpawn`, `TEST`, `Entity Manager`, `Entity Records`?**
  _High betweenness centrality (0.100) - this node is a cross-community bridge._
- **Are the 2 inferred relationships involving `Registry` (e.g. with `TEST()` and `TEST()`) actually correct?**
  _`Registry` has 2 INFERRED edges - model-reasoned connections that need verification._
- **What connects `_impl`, `voices`, `next` to the rest of the system?**
  _371 weakly-connected nodes found - possible documentation gaps or missing edges._
- **Should `ECS Column Storage` be split into smaller, more focused modules?**
  _Cohesion score 0.05069124423963134 - nodes in this community are weakly interconnected._
- **Why does `Game` connect `Game` to `.addPlayer`, `Components.hpp`, `Registry`, `App`, `WaveStep`, `AugmentId`, `Game Tests`, `describeSpawn`, `Entity Registry`, `PendingSpawn`, `PlayerInput`, `Systems.hpp`, `findPlayerBySlot`, `Phase`?**
  _High betweenness centrality (0.086) - this node is a cross-community bridge._
- **Are the 5 inferred relationships involving `Entity` (e.g. with `Use the graph` and `coinPickupSystem()`) actually correct?**
  _`Entity` has 5 INFERRED edges - model-reasoned connections that need verification._
- **Should `Window` be split into smaller, more focused modules?**
  _Cohesion score 0.13970588235294118 - nodes in this community are weakly interconnected._