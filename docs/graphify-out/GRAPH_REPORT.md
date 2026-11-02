# Graph Report - RTYPE  (2026-11-02)

## Corpus Check
- 133 files · ~60,207 words
- Verdict: corpus is large enough that graph structure adds value.
- Unclassified: 5 file(s) not represented in the graph (top: (none) 5)

## Summary
- 1809 nodes · 3590 edges · 109 communities (87 shown, 22 thin omitted)
- Extraction: 88% EXTRACTED · 12% INFERRED · 0% AMBIGUOUS · INFERRED: 430 edges (avg confidence: 0.88)
- Token cost: 0 input · 0 output

## Graph Freshness
- Built from commit: `cdb1bef3`
- Run `git rev-parse HEAD` and compare to check if the graph is stale.
- Run `graphify update .` after code changes (no API cost).

## Community Hubs (Navigation)
- ECS Column Storage
- IntRect
- string
- make_sounds.py
- Registry
- Client Connection
- vector
- R-Type game logic
- Delivery and Sessions
- Build and Protocol Docs
- Server Events
- Game Tests
- Components.hpp
- size_t
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
- Renderer
- GameView
- App
- Client.cpp
- Registry.hpp
- Audio Player Impl
- client/App.hpp
- Command Buffer
- OwnState
- Design Choices Docs
- Audio Player
- Connection Impl
- Replica Events
- Build Tooling Docs
- App
- Reliable Channel
- AugmentId
- Audio
- Phase
- Network protocol doc
- Architecture Targets
- EntitySpawned
- Status Tests
- Segments
- Key
- Window
- MonsterKind
- Window.cpp
- Replica.cpp
- SpriteId
- PlayerInput
- SpriteBank
- Packet.cpp
- Binary Writer Impl
- Message Types
- Delivery
- MonsterKind
- Sound
- describeSpawn
- Cue
- time_point
- Connection::handle
- Session Info
- Renderer.cpp
- TextureManager
- Texture Manager Impl
- Sample
- Layers
- Window Impl Header
- AudioPlayer::Impl
- GameStatus
- client/main.cpp
- EntityManager
- Sound Manager Impl
- Registry
- MonsterDef
- RefuseReason
- Connection Send
- Font.cpp
- DisconnectReason
- Knowledge graph (graphify)
- Track
- Font
- Controls
- .operator()
- pull_request_template.md
- CLAUDE.md
- vcpkg.json
- Window::Window
- Case
- CoinCounter
- Resized
- App::run

## God Nodes (most connected - your core abstractions)
1. `Registry` - 96 edges
2. `Entity` - 84 edges
3. `Game` - 73 edges
4. `TEST()` - 72 edges
5. `Server` - 43 edges
6. `Connection` - 43 edges
7. `Replica` - 43 edges
8. `Client` - 38 edges
9. `Layers` - 32 edges
10. `Hud` - 28 edges

## Surprising Connections (you probably didn't know these)
- `Les niveaux` --references--> `Segments`  [INFERRED]
  roadmap.md → src/client/game/Rtype/include/logic/Levels.hpp
- `Les types de monstres (`kind`)` --references--> `MonsterKind`  [INFERRED]
  roadmap.md → src/client/game/Rtype/include/logic/Kinds.hpp
- `Les étapes` --references--> `bossSystem()`  [INFERRED]
  roadmap.md → src/client/game/Rtype/include/logic/Boss.hpp
- `Les étapes` --references--> `SpriteId`  [INFERRED]
  roadmap.md → src/client/game/Rtype/include/logic/Components.hpp
- `Les étapes` --references--> `MonsterKind`  [INFERRED]
  roadmap.md → src/client/game/Rtype/include/logic/Kinds.hpp

## Import Cycles
- None detected.

## Hyperedges (group relationships)
- **Reliable game messages (SPAWN, DESTROY, YOU, RESPAWN)** — docs_protocol_spawn, docs_protocol_destroy, docs_protocol_you, docs_protocol_respawn [EXTRACTED 1.00]
- **Server glue: network + game + core via App** — docs_architecture_server_app_glue, docs_architecture_engine_network, docs_architecture_rtype_game [EXTRACTED 1.00]
- **Unreliable game messages (INPUT, STATES, STATUS)** — docs_protocol_input, docs_protocol_states, docs_protocol_status [EXTRACTED 1.00]

## Communities (109 total, 22 thin omitted)

### Community 0 - "ECS Column Storage"
Cohesion: 0.06
Nodes (24): Archetype, _columns, _columnsIndex, _entities, NO_COLUMN, _signature, Column, _data (+16 more)

### Community 1 - "IntRect"
Cohesion: 0.19
Nodes (11): Animation, firstFrame, frameCount, frameDuration, step, IntRect, height, left (+3 more)

### Community 2 - "string"
Cohesion: 0.11
Nodes (5): SoundManager::Impl, buffers, ids, SoundManager::load(), TextureManager::load()

### Community 3 - "make_sounds.py"
Cohesion: 0.23
Nodes (21): boss_alert(), decay(), defeat(), drums(), encode_ogg(), explosion(), fade(), hit() (+13 more)

### Community 4 - "Registry"
Cohesion: 0.11
Nodes (32): Registry, _archetypes, EMPTY_ARCHETYPE, _entities, _iterating, NO_ARCHETYPE, _records, The tick: `Game::update` (+24 more)

### Community 5 - "Client Connection"
Cohesion: 0.05
Nodes (17): Client, _attempts, _connection, _events, _handshakeSequence, _lastAttempt, _server, _socket (+9 more)

### Community 7 - "R-Type game logic"
Cohesion: 0.20
Nodes (10): each(), Add a component, Add a new kind of monster, Add a system, Coding conventions, Configuration, Current status, How-to (+2 more)

### Community 8 - "Delivery and Sessions"
Cohesion: 0.14
Nodes (13): Server::admit(), Server::broadcast(), Server::checkSessions(), Server::clientCount(), Server::close(), Server::disconnect(), Server::find(), Server::kick() (+5 more)

### Community 9 - "Build and Protocol Docs"
Cohesion: 0.14
Nodes (20): engine_audio library, SFML::Audio, engine build root, engine_core library, Game protocol, Handshake and connection, Network module (engine::network), ReliableChannel (+12 more)

### Community 10 - "Server Events"
Cohesion: 0.06
Nodes (10): Server, _events, _maxClients, _nextClientId, _sessions, _socket, _version, Server::handleConnect() (+2 more)

### Community 12 - "Game Tests"
Cohesion: 0.05
Nodes (5): kill(), killBoss(), playerOf(), skipToBoss(), TEST()

### Community 13 - "Components.hpp"
Cohesion: 0.09
Nodes (32): Les étapes, Collisions, Components, Shooting, Systems, BossTag, age, Damage (+24 more)

### Community 14 - "size_t"
Cohesion: 0.40
Nodes (4): EntityRecord, archetype, row, IterationGuard

### Community 15 - "Connection Channel"
Cohesion: 0.08
Nodes (10): Connection, _channel, _lastPing, _lastReceived, _outbox, _pingId, _pingSentAt, _received (+2 more)

### Community 16 - "Game"
Cohesion: 0.06
Nodes (21): Game rules, What the server reads, createPlayer(), Game, _augments, _coinCounter, _events, _nextBoss (+13 more)

### Community 17 - "UdpSocket::Impl"
Cohesion: 0.05
Nodes (27): Address, ip, port, Datagram, bytes, from, toAddress(), toEndpoint() (+19 more)

### Community 18 - ".updatePlaying"
Cohesion: 0.15
Nodes (20): applyContact(), canHit(), collectProjectiles(), collectTargets(), Collider, box, entity, pos (+12 more)

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
Cohesion: 0.17
Nodes (11): BinaryWriter, _data, MAX_VARINT, BinaryWriter::writeFloat(), Connection::sendId(), Packet::serialize(), Server::sendAccept(), encode() (+3 more)

### Community 23 - "Packet"
Cohesion: 0.15
Nodes (7): Packet, _messages, Client::handleHandshake(), Client::handleSession(), Connection::hasHandshakeMessage(), Server::handleDatagram(), Server::handleSession()

### Community 24 - "Pending Messages"
Cohesion: 0.11
Nodes (8): Pending, message, sentAt, ReliableChannel, _early, _expected, _nextId, _pending

### Community 25 - "Replica"
Cohesion: 0.08
Nodes (12): Replica, CLOCK_CORRECTION_RATE, _cues, _entities, INTERPOLATION_DELAY, _latestTick, MAX_CLOCK_ERROR, _prediction (+4 more)

### Community 26 - "BinaryReader"
Cohesion: 0.12
Nodes (10): BinaryReader, _cursor, BinaryReader::BinaryReader(), BinaryReader::readBytes(), BinaryReader::readFloat(), BinaryReader::readU16(), BinaryReader::readU32(), BinaryReader::readU8() (+2 more)

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
Cohesion: 0.12
Nodes (10): Client::checkConnection(), Client::flush(), Client::flushConnection(), Client::handleDatagram(), Client::latency(), Client::pollEvent(), Client::sendDisconnect(), Client::tryConnect() (+2 more)

### Community 31 - "Registry.hpp"
Cohesion: 0.21
Nodes (5): add(), create(), remove(), infoOf(), swapRemove()

### Community 32 - "Audio Player Impl"
Cohesion: 0.18
Nodes (4): AudioPlayer::play(), AudioPlayer::playMusic(), AudioPlayer::setMusicVolume(), toSfmlVolume()

### Community 34 - "Command Buffer"
Cohesion: 0.14
Nodes (5): CommandBuffer, _commands, Scheduler, _commands, _systems

### Community 35 - "OwnState"
Cohesion: 0.18
Nodes (11): Replica::apply(), encodeStates(), InputMessage, input, sequence, OwnState, lastInput, pos (+3 more)

### Community 36 - "Design Choices Docs"
Cohesion: 0.20
Nodes (14): Audio/visual accessibility (health bars, per-slot sprites; palette, contrast proposed), rtype::config (Config.hpp constants), Motor accessibility (keyboard only done; remap, toggle fire, gamepad proposed), Choice: per-message reliability, Choice: event-based replication with NetId, Choice: sessions with token, ping, 5s timeout, DESTROY 0x21 (S->C, reliable, entity id), Connection lifecycle / Handshake (Connect, Accept(token), Refuse, YOU, SPAWN, Ping, timeout 5s) (+6 more)

### Community 38 - "Connection Impl"
Cohesion: 0.14
Nodes (12): Connection::Connection(), Connection::handlePong(), Connection::handleUser(), Connection::latency(), Connection::nextSequence(), Connection::peerDisconnect(), Connection::readId(), Connection::resendDelay() (+4 more)

### Community 39 - "Replica Events"
Cohesion: 0.15
Nodes (5): Unreachable, applyDecoded(), Network, _client, _open

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
Cohesion: 0.08
Nodes (19): applyAugment(), AugmentDef, apply, id, price, rarity, AugmentId, Count (+11 more)

### Community 44 - "Audio"
Cohesion: 0.12
Nodes (10): Audio, _dir, _enabled, _ids, _phase, _player, SOUND_COUNT, _sounds (+2 more)

### Community 45 - "Phase"
Cohesion: 0.18
Nodes (8): Phase, Defeat, Playing, Victory, Audio::follow(), Audio::onPhase(), Audio::playCue(), Hud::trackPhase()

### Community 46 - "Network protocol doc"
Cohesion: 0.17
Nodes (20): Accessibility doc, Cognitive accessibility (simple rules, short levels, clear feedback), Architecture doc, Build and run doc, CMake presets linux-debug / windows-debug (Ninja on Linux), Build troubleshooting table, Contributing doc, Branch prefixes and commit types (+12 more)

### Community 47 - "Architecture Targets"
Cohesion: 0.16
Nodes (17): r-type_client (display executable), Client replica of the world (entity kind + position), engine::core (Registry, Scheduler, CommandBuffer, FixedTimestep), engine::network (UDP transport, sessions, ping, reliability, wraps Asio), engine::window (rendering, wraps SFML), Three decoupled layers: rendering, networking, game logic, rtype::common (shared headers: Vec2, return codes), rtype::game (header-only game logic) (+9 more)

### Community 48 - "EntitySpawned"
Cohesion: 0.16
Nodes (13): Public API: the `Game` class, EntityDestroyed, id, EntitySpawned, box, id, pos, slot (+5 more)

### Community 50 - "Segments"
Cohesion: 0.10
Nodes (18): Ajouter une map ou un niveau, Ce qui est en place, Maps : une map est une campagne complète (idée, à faire plus tard), appendLevel(), buildMap(), buildRun(), LevelDef, bosses (+10 more)

### Community 51 - "Key"
Cohesion: 0.11
Nodes (16): Key, Down, Enter, Escape, Left, Right, Space, Unknown (+8 more)

### Community 52 - "Window"
Cohesion: 0.16
Nodes (4): Window, _impl, handle(), App::loop()

### Community 53 - "MonsterKind"
Cohesion: 0.08
Nodes (23): MonsterType, kind, PendingSpawn, delay, kind, y, MonsterKind, Basic (+15 more)

### Community 54 - "Window.cpp"
Cohesion: 0.18
Nodes (7): FocusGained, FocusLost, convert(), toKey(), translate(), Window::clear(), Window::pollEvent()

### Community 55 - "Replica.cpp"
Cohesion: 0.18
Nodes (7): explosionCue(), Replica::applyPrediction(), Replica::localPlayer(), Replica::predict(), Replica::record(), Replica::recordSpawnCue(), Replica::slot()

### Community 56 - "SpriteId"
Cohesion: 0.15
Nodes (13): BulletLook, height, sprite, width, SpriteId, Boss, Coin, Count (+5 more)

### Community 57 - "PlayerInput"
Cohesion: 0.12
Nodes (12): Add a player action (a new key), PlayerInput, down, fire, left, right, up, Prediction::position() (+4 more)

### Community 58 - "SpriteBank"
Cohesion: 0.20
Nodes (5): Sprite, animation, texture, SpriteBank, _sprites

### Community 59 - "Packet.cpp"
Cohesion: 0.12
Nodes (10): Packet::add(), Packet::isHandshake(), Packet::isValid(), Packet::Packet(), Packet::parse(), Packet::parseMessage(), Packet::sequence(), Packet::size() (+2 more)

### Community 61 - "Binary Writer Impl"
Cohesion: 0.21
Nodes (5): BinaryWriter::writeBytes(), BinaryWriter::writeU16(), BinaryWriter::writeU32(), BinaryWriter::writeU8(), BinaryWriter::writeVarint()

### Community 62 - "Message Types"
Cohesion: 0.22
Nodes (5): Message, id, payload, reliable, type

### Community 63 - "Delivery"
Cohesion: 0.20
Nodes (5): Delivery, Reliable, Unreliable, Client::Client(), Client::send()

### Community 64 - "MonsterKind"
Cohesion: 0.12
Nodes (16): Décision sur le gain de temps par kill, Les niveaux, Les types de boss (`BossKind`), Les types de monstres (`kind`), Niveaux, types de monstres et de boss (design retenu), Ordre, Roadmap : les niveaux, À trancher avant de démarrer (+8 more)

### Community 65 - "Sound"
Cohesion: 0.20
Nodes (10): Sound, BossAlert, Coin, Count, Defeat, Explosion, Hit, Shoot (+2 more)

### Community 66 - "describeSpawn"
Cohesion: 0.27
Nodes (6): collectSpawns(), collectStates(), collectUntracked(), describeSpawn(), isReplicable(), slotOf()

### Community 67 - "Cue"
Cohesion: 0.17
Nodes (9): Cue, BossAppears, BossExplosion, CoinPickup, Explosion, Hit, PlayerShot, Audio::play() (+1 more)

### Community 68 - "time_point"
Cohesion: 0.25
Nodes (3): Connection::receive(), Connection::timedOut(), Connection::update()

### Community 70 - "Session Info"
Cohesion: 0.18
Nodes (8): _random, Session, address, connection, id, Server::findByAddress(), Server::findByToken(), Server::makeToken()

### Community 71 - "Renderer.cpp"
Cohesion: 0.29
Nodes (4): Renderer::draw(), Renderer::drawRect(), Renderer::drawResized(), Renderer::Renderer()

### Community 72 - "TextureManager"
Cohesion: 0.15
Nodes (5): TextureManager, _impl, Audio::Audio(), SpriteBank::set(), SpriteBank::SpriteBank()

### Community 73 - "Texture Manager Impl"
Cohesion: 0.25
Nodes (3): TextureManager::Impl, ids, textures

### Community 74 - "Sample"
Cohesion: 0.22
Nodes (5): Sample, pos, tick, Replica::interpolate(), Replica::update()

### Community 75 - "Layers"
Cohesion: 0.16
Nodes (23): Architecture, Data oriented design, Layers, Position, velocity, speed, direction, The ECS rules to remember, bossFan(), bulletDirection(), bulletStartPos() (+15 more)

### Community 77 - "AudioPlayer::Impl"
Cohesion: 0.24
Nodes (6): AudioPlayer::Impl, music, musicVolume, next, soundVolume, voices

### Community 78 - "GameStatus"
Cohesion: 0.11
Nodes (15): GameStatus, bossHp, bossMaxHp, coins, phase, players, PlayerStatus, alive (+7 more)

### Community 80 - "client/main.cpp"
Cohesion: 0.10
Nodes (4): App::App(), main(), parsePort(), parsePort()

### Community 81 - "EntityManager"
Cohesion: 0.29
Nodes (3): EntityManager, _freeList, _generations

### Community 82 - "Sound Manager Impl"
Cohesion: 0.25
Nodes (3): SoundManager, _impl, AudioPlayer::AudioPlayer()

### Community 83 - "Registry"
Cohesion: 0.09
Nodes (20): Entity, id, eachRow(), has(), remove(), tryGet(), addDamage(), addHp() (+12 more)

### Community 84 - "MonsterDef"
Cohesion: 0.25
Nodes (8): MonsterDef, damage, fireCooldown, height, hp, speed, sprite, width

### Community 85 - "RefuseReason"
Cohesion: 0.40
Nodes (5): Refused, reason, RefuseReason, Full, Version

### Community 86 - "Connection Send"
Cohesion: 0.38
Nodes (3): Connection::flush(), Connection::send(), Connection::sendEngine()

### Community 87 - "Font.cpp"
Cohesion: 0.10
Nodes (20): Color, a, b, g, r, Vector2f, x, y (+12 more)

### Community 89 - "DisconnectReason"
Cohesion: 0.29
Nodes (7): DisconnectReason, Kicked, Quit, Shutdown, Timeout, Client::close(), Server::sendDisconnect()

### Community 90 - "Knowledge graph (graphify)"
Cohesion: 0.40
Nodes (5): Install, Keep it up to date: the git hook, Knowledge graph (graphify), Use the graph, With an AI agent (Claude Code)

### Community 91 - "Track"
Cohesion: 0.40
Nodes (5): Track, Boss, Level, None, Audio::playMusic()

### Community 93 - "Font"
Cohesion: 0.33
Nodes (3): Font, GLYPH_WIDTH, Hud::drawCoins()

### Community 95 - "Controls"
Cohesion: 0.33
Nodes (3): Controls, _input, _respawn

### Community 102 - "Case"
Cohesion: 0.22
Nodes (4): Case, name, run, Registrar

### Community 103 - "CoinCounter"
Cohesion: 0.33
Nodes (5): CoinCounter, quantity, total, coins, value

### Community 106 - "Resized"
Cohesion: 0.50
Nodes (3): Resized, height, width

## Knowledge Gaps
- **401 isolated node(s):** `SLOTS`, `delay`, `y`, `kind`, `_reg` (+396 more)
  These have ≤1 connection - possible missing edges or undocumented components. (Counts symbols only; 820 node(s) total have ≤1 connection when file, concept and rationale nodes are included.)
- **22 thin communities (<3 nodes) omitted from report** — run `graphify query` to explore isolated nodes.

## Suggested Questions
_Questions this graph is uniquely positioned to answer:_

- **Why does `Game` connect `Game` to `describeSpawn`, `Registry`, `App`, `AugmentId`, `cstddef`, `Phase`, `Game Tests`, `Segments`, `Registry`, `MonsterKind`, `PlayerInput`?**
  _High betweenness centrality (0.102) - this node is a cross-community bridge._
- **Are the 2 inferred relationships involving `Registry` (e.g. with `TEST()` and `TEST()`) actually correct?**
  _`Registry` has 2 INFERRED edges - model-reasoned connections that need verification._
- **What connects `SLOTS`, `delay`, `y` to the rest of the system?**
  _401 weakly-connected nodes found - possible documentation gaps or missing edges._
- **Should `ECS Column Storage` be split into smaller, more focused modules?**
  _Cohesion score 0.05827067669172932 - nodes in this community are weakly interconnected._
- **Why does `R-Type game logic` connect `R-Type game logic` to `Registry`, `Layers`, `Components.hpp`, `Network protocol doc`, `Game`, `EntitySpawned`?**
  _High betweenness centrality (0.091) - this node is a cross-community bridge._
- **Are the 5 inferred relationships involving `Entity` (e.g. with `Use the graph` and `coinPickupSystem()`) actually correct?**
  _`Entity` has 5 INFERRED edges - model-reasoned connections that need verification._
- **Should `string` be split into smaller, more focused modules?**
  _Cohesion score 0.1111111111111111 - nodes in this community are weakly interconnected._