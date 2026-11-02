# Graph Report - RTYPE  (2026-10-30)

## Corpus Check
- 129 files · ~59,339 words
- Verdict: corpus is large enough that graph structure adds value.
- Unclassified: 5 file(s) not represented in the graph (top: (none) 5)

## Summary
- 1775 nodes · 3512 edges · 109 communities (85 shown, 24 thin omitted)
- Extraction: 88% EXTRACTED · 12% INFERRED · 0% AMBIGUOUS · INFERRED: 412 edges (avg confidence: 0.88)
- Token cost: 0 input · 0 output

## Graph Freshness
- Built from commit: `d377daa7`
- Run `git rev-parse HEAD` and compare to check if the graph is stale.
- Run `graphify update .` after code changes (no API cost).

## Community Hubs (Navigation)
- ECS Column Storage
- IntRect
- string
- make_sounds.py
- .updatePlaying
- Client Connection
- vector
- R-Type game logic
- Delivery and Sessions
- Build and Protocol Docs
- Server Events
- Game Tests
- Layers
- size_t
- Connection Channel
- Game
- UdpSocket::Impl
- .updatePlaying
- Hud
- Entity State Payload
- Prediction
- Binary Writer
- Handshake Sequencing
- Pending Messages
- Replica
- BinaryReader
- Renderer
- GameView
- App
- Client.cpp
- Registry.hpp
- Audio Player Impl
- Network::Network
- Command Buffer
- EntityState
- Design Choices Docs
- Audio Player
- Connection Impl
- Replica Events
- Build Tooling Docs
- App
- Reliable Channel
- AugmentId
- Audio
- Audio.cpp
- Network protocol doc
- Architecture Targets
- EntitySpawned
- Status Tests
- Segments
- Key
- Window
- WaveStep
- Window.cpp
- Replica.cpp
- SpriteId
- PlayerInput
- SpriteBank
- .load
- Binary Writer Impl
- Message Types
- findPlayerBySlot
- MonsterKind
- Sound
- describeSpawn
- Cue
- time_point
- Connection::handle
- Session Info
- Renderer Draw
- TextureManager
- Texture Manager Impl
- Sample
- BulletLook
- Window Impl Header
- AudioPlayer::Impl
- GameStatus
- findPlayerBySlot
- client/main.cpp
- Server::handleDatagram
- Sound Manager Impl
- Registry
- RefuseReason
- Connection Send
- Font.cpp
- Phase
- Knowledge graph (graphify)
- Track
- Server::handleConnect
- Controls
- CommandBuffer
- pull_request_template.md
- CLAUDE.md
- vcpkg.json
- vector
- Case
- CoinCounter
- Rarity
- Controls.cpp
- Resized
- App::App
- App::run

## God Nodes (most connected - your core abstractions)
1. `Registry` - 96 edges
2. `Entity` - 84 edges
3. `Game` - 72 edges
4. `TEST()` - 71 edges
5. `Server` - 43 edges
6. `Connection` - 43 edges
7. `Replica` - 43 edges
8. `Client` - 38 edges
9. `Layers` - 28 edges
10. `Hud` - 28 edges

## Surprising Connections (you probably didn't know these)
- `Les niveaux` --references--> `Segments`  [INFERRED]
  roadmap.md → src/client/game/Rtype/include/logic/Levels.hpp
- `The ECS rules to remember` --references--> `each()`  [INFERRED]
  src/client/game/Rtype/doc/README.md → engine/core/include/engine/core/Registry.hpp
- `Use the graph` --references--> `Connection`  [INFERRED]
  docs/graphify.md → engine/network/include/engine/network/Connection.hpp
- `Les types de boss (`BossKind`)` --references--> `BossKind`  [INFERRED]
  roadmap.md → src/client/game/Rtype/include/logic/Kinds.hpp
- `Les types de monstres (`kind`)` --references--> `MonsterKind`  [INFERRED]
  roadmap.md → src/client/game/Rtype/include/logic/Kinds.hpp

## Import Cycles
- None detected.

## Hyperedges (group relationships)
- **Reliable game messages (SPAWN, DESTROY, YOU, RESPAWN)** — docs_protocol_spawn, docs_protocol_destroy, docs_protocol_you, docs_protocol_respawn [EXTRACTED 1.00]
- **Server glue: network + game + core via App** — docs_architecture_server_app_glue, docs_architecture_engine_network, docs_architecture_rtype_game [EXTRACTED 1.00]
- **Unreliable game messages (INPUT, STATES, STATUS)** — docs_protocol_input, docs_protocol_states, docs_protocol_status [EXTRACTED 1.00]

## Communities (109 total, 24 thin omitted)

### Community 0 - "ECS Column Storage"
Cohesion: 0.05
Nodes (26): Archetype, _columns, _columnsIndex, _entities, NO_COLUMN, _signature, Column, _data (+18 more)

### Community 1 - "IntRect"
Cohesion: 0.16
Nodes (14): Animation, firstFrame, frameCount, frameDuration, step, IntRect, height, left (+6 more)

### Community 2 - "string"
Cohesion: 0.12
Nodes (5): SoundManager::Impl, buffers, ids, SoundManager::load(), TextureManager::load()

### Community 3 - "make_sounds.py"
Cohesion: 0.23
Nodes (21): boss_alert(), decay(), defeat(), drums(), encode_ogg(), explosion(), fade(), hit() (+13 more)

### Community 4 - ".updatePlaying"
Cohesion: 0.18
Nodes (14): The tick: `Game::update`, isOutOfBounds(), collectMonsters(), collectPlayers(), collectProjectileEntities(), collectWith(), contactSystem(), deleteUnitsOutOfBounds() (+6 more)

### Community 5 - "Client Connection"
Cohesion: 0.05
Nodes (17): Client, _attempts, _connection, _events, _handshakeSequence, _lastAttempt, _server, _socket (+9 more)

### Community 7 - "R-Type game logic"
Cohesion: 0.20
Nodes (10): each(), Add a component, Add a new kind of monster, Add a player action (a new key), Add a system, Configuration, Current status, How-to (+2 more)

### Community 8 - "Delivery and Sessions"
Cohesion: 0.12
Nodes (15): Delivery, Reliable, Unreliable, Server::broadcast(), Server::checkSessions(), Server::clientCount(), Server::close(), Server::disconnect() (+7 more)

### Community 9 - "Build and Protocol Docs"
Cohesion: 0.14
Nodes (20): engine_audio library, SFML::Audio, engine build root, engine_core library, Game protocol, Handshake and connection, Network module (engine::network), ReliableChannel (+12 more)

### Community 10 - "Server Events"
Cohesion: 0.07
Nodes (7): Server, _events, _maxClients, _nextClientId, _sessions, _socket, _version

### Community 12 - "Game Tests"
Cohesion: 0.06
Nodes (5): kill(), killBoss(), playerOf(), skipToBoss(), TEST()

### Community 13 - "Layers"
Cohesion: 0.08
Nodes (43): Architecture, Collisions, Components, Data oriented design, Layers, Position, velocity, speed, direction, Shooting, Systems (+35 more)

### Community 14 - "size_t"
Cohesion: 0.40
Nodes (4): EntityRecord, archetype, row, IterationGuard

### Community 15 - "Connection Channel"
Cohesion: 0.08
Nodes (10): Connection, _channel, _lastPing, _lastReceived, _outbox, _pingId, _pingSentAt, _received (+2 more)

### Community 16 - "Game"
Cohesion: 0.09
Nodes (16): Game, _augments, _coinCounter, _events, _nextBoss, _nextNetId, _nextWave, _pending (+8 more)

### Community 17 - "UdpSocket::Impl"
Cohesion: 0.05
Nodes (27): Address, ip, port, Datagram, bytes, from, toAddress(), toEndpoint() (+19 more)

### Community 18 - ".updatePlaying"
Cohesion: 0.12
Nodes (23): Coding conventions, applyContact(), canHit(), collectProjectiles(), collectTargets(), Collider, box, entity (+15 more)

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
Cohesion: 0.18
Nodes (10): BinaryWriter, _data, MAX_VARINT, Packet::serialize(), Server::sendAccept(), encode(), encodeInput(), encodeStateChunk() (+2 more)

### Community 23 - "Handshake Sequencing"
Cohesion: 0.11
Nodes (11): Packet, _messages, Client::checkConnection(), Client::flushConnection(), Client::handleHandshake(), Client::handleSession(), Client::tryConnect(), Connection::hasHandshakeMessage() (+3 more)

### Community 24 - "Pending Messages"
Cohesion: 0.11
Nodes (8): Pending, message, sentAt, ReliableChannel, _early, _expected, _nextId, _pending

### Community 25 - "Replica"
Cohesion: 0.08
Nodes (12): Replica, CLOCK_CORRECTION_RATE, _cues, _entities, INTERPOLATION_DELAY, _latestTick, MAX_CLOCK_ERROR, _prediction (+4 more)

### Community 26 - "BinaryReader"
Cohesion: 0.07
Nodes (19): BinaryReader, _cursor, BinaryReader::BinaryReader(), BinaryReader::readBytes(), BinaryReader::readFloat(), BinaryReader::readU16(), BinaryReader::readU32(), BinaryReader::readU8() (+11 more)

### Community 27 - "Renderer"
Cohesion: 0.11
Nodes (15): Renderer, Background, LAYERS, _stars, STARS_PER_LAYER, _width, _worldToScreen, Star (+7 more)

### Community 28 - "GameView"
Cohesion: 0.11
Nodes (14): GameView, ANIMATION_PHASE, _background, _hud, PLAYER_FRAME_DOWN, PLAYER_FRAME_IDLE, PLAYER_FRAME_UP, _worldToScreen (+6 more)

### Community 29 - "App"
Cohesion: 0.11
Nodes (14): App, _assets, _connectedSince, _controls, FRAMERATE, _last, LISTEN_DELAY, _network (+6 more)

### Community 30 - "Client.cpp"
Cohesion: 0.09
Nodes (13): DisconnectReason, Kicked, Quit, Shutdown, Timeout, Client::Client(), Client::close(), Client::flush() (+5 more)

### Community 31 - "Registry.hpp"
Cohesion: 0.26
Nodes (3): add(), create(), remove()

### Community 32 - "Audio Player Impl"
Cohesion: 0.15
Nodes (5): AudioPlayer::AudioPlayer(), AudioPlayer::play(), AudioPlayer::playMusic(), AudioPlayer::setMusicVolume(), toSfmlVolume()

### Community 34 - "Command Buffer"
Cohesion: 0.29
Nodes (3): Scheduler, _commands, _systems

### Community 35 - "EntityState"
Cohesion: 0.25
Nodes (7): EntityState, id, pos, Replica::record(), StatesSnapshot, states, tick

### Community 36 - "Design Choices Docs"
Cohesion: 0.20
Nodes (14): Audio/visual accessibility (health bars, per-slot sprites; palette, contrast proposed), rtype::config (Config.hpp constants), Motor accessibility (keyboard only done; remap, toggle fire, gamepad proposed), Choice: per-message reliability, Choice: event-based replication with NetId, Choice: sessions with token, ping, 5s timeout, DESTROY 0x21 (S->C, reliable, entity id), Connection lifecycle / Handshake (Connect, Accept(token), Refuse, YOU, SPAWN, Ping, timeout 5s) (+6 more)

### Community 38 - "Connection Impl"
Cohesion: 0.18
Nodes (11): Connection::handlePong(), Connection::handleUser(), Connection::latency(), Connection::nextSequence(), Connection::peerDisconnect(), Connection::readId(), Connection::resendDelay(), Connection::sendId() (+3 more)

### Community 39 - "Replica Events"
Cohesion: 0.16
Nodes (7): Refused, reason, Unreachable, applyDecoded(), Network, _client, _open

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
Cohesion: 0.10
Nodes (14): applyAugment(), AugmentDef, apply, id, price, rarity, AugmentId, Count (+6 more)

### Community 44 - "Audio"
Cohesion: 0.13
Nodes (9): Audio, _dir, _enabled, _ids, _phase, _player, SOUND_COUNT, _sounds (+1 more)

### Community 45 - "Audio.cpp"
Cohesion: 0.24
Nodes (4): Audio::follow(), Audio::onPhase(), Audio::play(), Audio::playCue()

### Community 46 - "Network protocol doc"
Cohesion: 0.17
Nodes (20): Accessibility doc, Cognitive accessibility (simple rules, short levels, clear feedback), Architecture doc, Build and run doc, CMake presets linux-debug / windows-debug (Ninja on Linux), Build troubleshooting table, Contributing doc, Branch prefixes and commit types (+12 more)

### Community 47 - "Architecture Targets"
Cohesion: 0.16
Nodes (17): r-type_client (display executable), Client replica of the world (entity kind + position), engine::core (Registry, Scheduler, CommandBuffer, FixedTimestep), engine::network (UDP transport, sessions, ping, reliability, wraps Asio), engine::window (rendering, wraps SFML), Three decoupled layers: rendering, networking, game logic, rtype::common (shared headers: Vec2, return codes), rtype::game (header-only game logic) (+9 more)

### Community 48 - "EntitySpawned"
Cohesion: 0.15
Nodes (13): EntityDestroyed, id, EntitySpawned, box, id, pos, slot, sprite (+5 more)

### Community 49 - "Status Tests"
Cohesion: 0.14
Nodes (4): InputMessage, input, sequence, TEST()

### Community 50 - "Segments"
Cohesion: 0.20
Nodes (6): firstLevel(), levelAt(), Segments, bossSpawn, levels, waves

### Community 51 - "Key"
Cohesion: 0.13
Nodes (16): Closed, FocusGained, FocusLost, Key, Down, Enter, Escape, Left (+8 more)

### Community 52 - "Window"
Cohesion: 0.14
Nodes (5): Window, _impl, handle(), App::listening(), App::loop()

### Community 53 - "WaveStep"
Cohesion: 0.15
Nodes (8): spawnMonster(), WaveStep, at, count, dy, interval, kind, y

### Community 54 - "Window.cpp"
Cohesion: 0.17
Nodes (6): convert(), toKey(), translate(), Window::clear(), Window::pollEvent(), Window::Window()

### Community 55 - "Replica.cpp"
Cohesion: 0.20
Nodes (6): explosionCue(), Replica::applyPrediction(), Replica::localPlayer(), Replica::predict(), Replica::recordSpawnCue(), Replica::slot()

### Community 56 - "SpriteId"
Cohesion: 0.09
Nodes (23): Les étapes, bossSystem(), BossTag, age, Renderable, id, SpriteId, Boss (+15 more)

### Community 57 - "PlayerInput"
Cohesion: 0.13
Nodes (11): PlayerInput, down, fire, left, right, up, Prediction::position(), Prediction::reconcile() (+3 more)

### Community 58 - "SpriteBank"
Cohesion: 0.20
Nodes (5): Sprite, animation, texture, SpriteBank, _sprites

### Community 61 - "Binary Writer Impl"
Cohesion: 0.20
Nodes (6): BinaryWriter::writeBytes(), BinaryWriter::writeFloat(), BinaryWriter::writeU16(), BinaryWriter::writeU32(), BinaryWriter::writeU8(), BinaryWriter::writeVarint()

### Community 62 - "Message Types"
Cohesion: 0.22
Nodes (5): Message, id, payload, reliable, type

### Community 64 - "MonsterKind"
Cohesion: 0.06
Nodes (30): Décision sur le gain de temps par kill, Les niveaux, Les types de boss (`BossKind`), Les types de monstres (`kind`), Niveaux, types de monstres et de boss (design retenu), Ordre, Roadmap : les niveaux, À trancher avant de démarrer (+22 more)

### Community 65 - "Sound"
Cohesion: 0.20
Nodes (10): Sound, BossAlert, Coin, Count, Defeat, Explosion, Hit, Shoot (+2 more)

### Community 66 - "describeSpawn"
Cohesion: 0.24
Nodes (6): collectSpawns(), collectStates(), collectUntracked(), describeSpawn(), isReplicable(), slotOf()

### Community 67 - "Cue"
Cohesion: 0.22
Nodes (8): Cue, BossAppears, BossExplosion, CoinPickup, Explosion, Hit, PlayerShot, Replica::takeCues()

### Community 68 - "time_point"
Cohesion: 0.18
Nodes (5): Connection::Connection(), Connection::receive(), Connection::timedOut(), Connection::token(), Connection::update()

### Community 70 - "Session Info"
Cohesion: 0.25
Nodes (6): Session, address, connection, id, Server::findByAddress(), Server::findByToken()

### Community 71 - "Renderer Draw"
Cohesion: 0.13
Nodes (14): Color, a, b, g, r, Vector2f, x, y (+6 more)

### Community 72 - "TextureManager"
Cohesion: 0.20
Nodes (4): TextureManager, _impl, SpriteBank::set(), SpriteBank::SpriteBank()

### Community 73 - "Texture Manager Impl"
Cohesion: 0.25
Nodes (3): TextureManager::Impl, ids, textures

### Community 74 - "Sample"
Cohesion: 0.22
Nodes (5): Sample, pos, tick, Replica::interpolate(), Replica::update()

### Community 75 - "BulletLook"
Cohesion: 0.12
Nodes (14): bossFan(), bulletDirection(), BulletLook, height, sprite, width, Team, id (+6 more)

### Community 77 - "AudioPlayer::Impl"
Cohesion: 0.25
Nodes (6): AudioPlayer::Impl, music, musicVolume, next, soundVolume, voices

### Community 78 - "GameStatus"
Cohesion: 0.11
Nodes (15): GameStatus, bossHp, bossMaxHp, coins, phase, players, PlayerStatus, alive (+7 more)

### Community 80 - "client/main.cpp"
Cohesion: 0.13
Nodes (3): main(), parsePort(), parsePort()

### Community 81 - "Server::handleDatagram"
Cohesion: 0.22
Nodes (4): _random, Client::handleDatagram(), Server::handleDatagram(), Server::makeToken()

### Community 83 - "Registry"
Cohesion: 0.07
Nodes (33): Entity, id, hash<Engine::Entity>, EntityManager, _freeList, _generations, eachRow(), has() (+25 more)

### Community 85 - "RefuseReason"
Cohesion: 0.29
Nodes (5): RefuseReason, Full, Version, Server::admit(), Server::pollEvent()

### Community 86 - "Connection Send"
Cohesion: 0.32
Nodes (3): Connection::flush(), Connection::send(), Connection::sendEngine()

### Community 87 - "Font.cpp"
Cohesion: 0.11
Nodes (13): Font, GLYPH_HEIGHT, GLYPH_WIDTH, find(), Font::draw(), Font::drawCentered(), Font::drawGlyph(), Font::Font() (+5 more)

### Community 89 - "Phase"
Cohesion: 0.33
Nodes (5): Phase, Defeat, Playing, Victory, Hud::trackPhase()

### Community 90 - "Knowledge graph (graphify)"
Cohesion: 0.40
Nodes (5): Install, Keep it up to date: the git hook, Knowledge graph (graphify), Use the graph, With an AI agent (Claude Code)

### Community 91 - "Track"
Cohesion: 0.40
Nodes (5): Track, Boss, Level, None, Audio::playMusic()

### Community 94 - "Server::handleConnect"
Cohesion: 0.25
Nodes (3): Server::handleConnect(), Server::handleSession(), Server::open()

### Community 95 - "Controls"
Cohesion: 0.22
Nodes (4): Controls, _input, _respawn, Controls::handle()

### Community 102 - "Case"
Cohesion: 0.33
Nodes (4): Case, name, run, Registrar

### Community 103 - "CoinCounter"
Cohesion: 0.33
Nodes (5): CoinCounter, quantity, total, coins, value

### Community 104 - "Rarity"
Cohesion: 0.40
Nodes (5): Rarity, Common, Epic, Legendary, Rare

### Community 106 - "Resized"
Cohesion: 0.50
Nodes (3): Resized, height, width

## Knowledge Gaps
- **398 isolated node(s):** `up`, `down`, `left`, `right`, `fire` (+393 more)
  These have ≤1 connection - possible missing edges or undocumented components. (Counts symbols only; 804 node(s) total have ≤1 connection when file, concept and rationale nodes are included.)
- **24 thin communities (<3 nodes) omitted from report** — run `graphify query` to explore isolated nodes.

## Suggested Questions
_Questions this graph is uniquely positioned to answer:_

- **Why does `Registry` connect `Registry` to `ECS Column Storage`, `IntRect`, `.updatePlaying`, `Game Tests`, `Layers`, `size_t`, `Game`, `.updatePlaying`, `Replica`, `GameView`, `Registry.hpp`, `Command Buffer`, `AugmentId`, `WaveStep`, `SpriteId`, `findPlayerBySlot`, `describeSpawn`, `BulletLook`, `CommandBuffer`?**
  _High betweenness centrality (0.093) - this node is a cross-community bridge._
- **Are the 2 inferred relationships involving `Registry` (e.g. with `TEST()` and `TEST()`) actually correct?**
  _`Registry` has 2 INFERRED edges - model-reasoned connections that need verification._
- **What connects `up`, `down`, `left` to the rest of the system?**
  _398 weakly-connected nodes found - possible documentation gaps or missing edges._
- **Should `ECS Column Storage` be split into smaller, more focused modules?**
  _Cohesion score 0.050595238095238096 - nodes in this community are weakly interconnected._
- **Why does `Game` connect `Game` to `MonsterKind`, `describeSpawn`, `.updatePlaying`, `vector`, `App`, `AugmentId`, `Components.hpp`, `Game Tests`, `findPlayerBySlot`, `Segments`, `Registry`, `WaveStep`, `Phase`, `findPlayerBySlot`?**
  _High betweenness centrality (0.091) - this node is a cross-community bridge._
- **Are the 5 inferred relationships involving `Entity` (e.g. with `Use the graph` and `coinPickupSystem()`) actually correct?**
  _`Entity` has 5 INFERRED edges - model-reasoned connections that need verification._
- **Should `string` be split into smaller, more focused modules?**
  _Cohesion score 0.12307692307692308 - nodes in this community are weakly interconnected._