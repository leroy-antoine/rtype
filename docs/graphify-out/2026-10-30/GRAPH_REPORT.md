# Graph Report - RTYPE  (2026-10-25)

## Corpus Check
- Large corpus: 6983 files · ~730,524 words. Semantic extraction will be expensive (many Claude tokens). Consider running on a subfolder.

## Summary
- 1445 nodes · 2845 edges · 99 communities (83 shown, 16 thin omitted)
- Extraction: 91% EXTRACTED · 9% INFERRED · 0% AMBIGUOUS · INFERRED: 253 edges (avg confidence: 0.85)
- Token cost: 0 input · 0 output

## Community Hubs (Navigation)
- ECS Column Storage
- Animation
- Window Backend
- Game Boss Logic
- Registry Queries
- Client Connection
- Sound Manager
- ECS Archetype
- Delivery and Sessions
- Build and Protocol Docs
- Server Events
- Bullets and Boss Fan
- Game Tests
- Network Includes
- Entity Registry
- Connection Channel
- Game Core
- Network Address
- Packet Parsing
- UDP Socket
- Entity State Payload
- Font and Vectors
- Binary Writer
- Handshake Sequencing
- Pending Messages
- HUD
- Binary Reader
- Renderer Background
- Replica
- Collisions
- Disconnect Handling
- Client Main
- Audio Player Impl
- Client App Events
- Command Buffer
- Boss System
- Design Choices Docs
- Audio Player
- Connection Impl
- Replica Events
- Build Tooling Docs
- Connect Events
- Reliable Channel
- Replication
- Client Audio
- Server App
- Contributing and Security
- Architecture Targets
- Entity Spawn Events
- Status Tests
- Network and Protocol Docs
- Keys
- Audio Cues
- Wave Spawning
- Connect Handling
- Controls
- Window Impl
- Player Lifecycle
- Server App State
- Audio Impl
- Binary Writer Impl
- Message Types
- Session Admission
- UDP Socket Impl
- Window Events
- Fixed Timestep
- Binary Reader Basics
- Connection Receive
- Connection Handlers
- Session Info
- Renderer Draw
- Texture Manager
- Texture Manager Impl
- Sound IDs
- Client Send
- Window Impl Header
- Player Input
- Game Status
- Test Harness
- Dependency Rules
- Sound Manager Impl
- Entity Manager
- Entity Records
- Flush and Serialize
- Connection Send
- Color
- Audio Cue Enum
- Player Creation
- Layer Architecture Doc
- Refuse Reasons
- Game Phase
- Client State
- Resend Delay
- Resize Event
- Level Tracks
- Player Status
- Pending Spawns

## God Nodes (most connected - your core abstractions)
1. `Registry` - 82 edges
2. `Entity` - 69 edges
3. `Game` - 59 edges
4. `TEST()` - 50 edges
5. `Server` - 43 edges
6. `Connection` - 42 edges
7. `Client` - 38 edges
8. `Audio` - 28 edges
9. `Replica` - 27 edges
10. `App` - 26 edges

## Surprising Connections (you probably didn't know these)
- `collisionSystem()` --references--> `Entity`  [INFERRED]
  src/games/rtype/include/Systems.hpp → engine/core/include/engine/core/Entity.hpp
- `deathSystem()` --references--> `Entity`  [INFERRED]
  src/games/rtype/include/Systems.hpp → engine/core/include/engine/core/Entity.hpp
- `lifetimeSystem()` --references--> `Entity`  [INFERRED]
  src/games/rtype/include/Systems.hpp → engine/core/include/engine/core/Entity.hpp
- `TEST()` --references--> `Registry`  [INFERRED]
  tests/GameTests.cpp → engine/core/include/engine/core/Registry.hpp
- `encodeStateChunk()` --calls--> `_data`  [INFERRED]
  src/include/rtype/Protocol.hpp → engine/network/include/engine/network/BinaryWriter.hpp

## Import Cycles
- None detected.

## Hyperedges (group relationships)
- **Server executable stack** — src_server_cmakelists_r_type_server, src_games_rtype_cmakelists_rtype_game, engine_network_cmakelists_engine_network [INFERRED 0.85]
- **Server glue: network + game + core via App** — docs_architecture_server_app_glue, docs_architecture_engine_network, docs_architecture_rtype_game [EXTRACTED 1.00]
- **Reliable game messages (SPAWN, DESTROY, YOU, RESPAWN)** — docs_protocol_spawn, docs_protocol_destroy, docs_protocol_you, docs_protocol_respawn [EXTRACTED 1.00]
- **Unreliable game messages (INPUT, STATES, STATUS)** — docs_protocol_input, docs_protocol_states, docs_protocol_status [EXTRACTED 1.00]

## Communities (99 total, 16 thin omitted)

### Community 0 - "ECS Column Storage"
Cohesion: 0.07
Nodes (18): Column, _data, _info, ComponentId, id, ComponentInfo, ComponentRegistry, getId() (+10 more)

### Community 1 - "Animation"
Cohesion: 0.06
Nodes (34): Animation, firstFrame, frameCount, frameDuration, step, Sprite, animation, texture (+26 more)

### Community 2 - "Window Backend"
Cohesion: 0.07
Nodes (17): Window, _impl, App, App::App(), _assets, _connectedSince, _controls, FRAMERATE (+9 more)

### Community 3 - "Game Boss Logic"
Cohesion: 0.10
Nodes (18): BossTag, age, Damage, value, Health, hp, MonsterTag, slot (+10 more)

### Community 4 - "Registry Queries"
Cohesion: 0.11
Nodes (27): eachRow(), Registry, _archetypes, EMPTY_ARCHETYPE, _entities, _iterating, NO_ARCHETYPE, _records (+19 more)

### Community 5 - "Client Connection"
Cohesion: 0.07
Nodes (11): Client, _attempts, _connection, _events, _handshakeSequence, _lastAttempt, _server, _socket (+3 more)

### Community 6 - "Sound Manager"
Cohesion: 0.11
Nodes (5): SoundManager::Impl, buffers, ids, SoundManager::load(), TextureManager::load()

### Community 7 - "ECS Archetype"
Cohesion: 0.13
Nodes (10): Archetype, _columns, _columnsIndex, _entities, NO_COLUMN, _signature, each(), Signature (+2 more)

### Community 8 - "Delivery and Sessions"
Cohesion: 0.11
Nodes (17): Delivery, Reliable, Unreliable, Server::admit(), Server::broadcast(), Server::checkSessions(), Server::clientCount(), Server::close() (+9 more)

### Community 9 - "Build and Protocol Docs"
Cohesion: 0.11
Nodes (27): engine_audio library, SFML::Audio, engine build root, engine_core library, Game protocol, Handshake and connection, Network module (engine::network), ReliableChannel (+19 more)

### Community 10 - "Server Events"
Cohesion: 0.07
Nodes (7): Server, _events, _maxClients, _nextClientId, _sessions, _socket, _version

### Community 11 - "Bullets and Boss Fan"
Cohesion: 0.12
Nodes (25): bossFan(), bulletDirection(), BulletLook, height, sprite, width, bulletStartPos(), isShooter() (+17 more)

### Community 12 - "Game Tests"
Cohesion: 0.09
Nodes (5): kill(), killBoss(), playerOf(), skipToBoss(), TEST()

### Community 14 - "Entity Registry"
Cohesion: 0.14
Nodes (10): add(), create(), remove(), Entity, id, has(), remove(), tryGet() (+2 more)

### Community 15 - "Connection Channel"
Cohesion: 0.08
Nodes (10): Connection, _channel, _lastPing, _lastReceived, _outbox, _pingId, _pingSentAt, _received (+2 more)

### Community 16 - "Game Core"
Cohesion: 0.10
Nodes (12): Game, _bossSpawned, _events, _nextNetId, _nextWave, _pending, _phaseTimer, _reg (+4 more)

### Community 17 - "Network Address"
Cohesion: 0.14
Nodes (12): Address, ip, port, toAddress(), toEndpoint(), UdpSocket::Impl::Impl(), UdpSocket::Impl::store(), UdpSocket::port() (+4 more)

### Community 18 - "Packet Parsing"
Cohesion: 0.13
Nodes (10): Packet::add(), Packet::isHandshake(), Packet::isValid(), Packet::Packet(), Packet::parse(), Packet::parseMessage(), Packet::sequence(), Packet::size() (+2 more)

### Community 19 - "UDP Socket"
Cohesion: 0.10
Nodes (12): UdpSocket::Impl::startReceive(), UdpSocket::Impl, buffer, BUFFER_SIZE, io, localPort, MAX_QUEUED, mutex (+4 more)

### Community 20 - "Entity State Payload"
Cohesion: 0.16
Nodes (16): EntityState, id, pos, decodeInput(), decodeSpawn(), decodeStates(), decodeStatus(), decodeYou() (+8 more)

### Community 21 - "Font and Vectors"
Cohesion: 0.18
Nodes (10): Vector2f, x, y, find(), Font, GLYPH_HEIGHT, GLYPH_WIDTH, Glyph (+2 more)

### Community 22 - "Binary Writer"
Cohesion: 0.15
Nodes (10): BinaryWriter, _data, MAX_VARINT, BinaryWriter::writeFloat(), BinaryWriter::writeU32(), Connection::sendId(), Packet::serialize(), Server::sendAccept() (+2 more)

### Community 23 - "Handshake Sequencing"
Cohesion: 0.13
Nodes (7): Packet, _messages, Client::checkConnection(), Client::handleDatagram(), Client::handleHandshake(), Client::handleSession(), Connection::flush()

### Community 24 - "Pending Messages"
Cohesion: 0.11
Nodes (8): Pending, message, sentAt, ReliableChannel, _early, _expected, _nextId, _pending

### Community 25 - "HUD"
Cohesion: 0.17
Nodes (11): Hud, BAR_HEIGHT, BAR_WIDTH, BAR_X, BOSS_BAR_WIDTH, _font, _height, _phase (+3 more)

### Community 26 - "Binary Reader"
Cohesion: 0.18
Nodes (8): BinaryReader::BinaryReader(), BinaryReader::readBytes(), BinaryReader::readFloat(), BinaryReader::readU16(), BinaryReader::readU32(), BinaryReader::readU8(), BinaryReader::readVarint(), BinaryReader::remaining()

### Community 27 - "Renderer Background"
Cohesion: 0.13
Nodes (14): Renderer, Background, Background::Background(), LAYERS, _stars, STARS_PER_LAYER, _width, _worldToScreen (+6 more)

### Community 28 - "Replica"
Cohesion: 0.15
Nodes (6): explosionCue(), Replica, _cues, _entities, _reg, _status

### Community 29 - "Collisions"
Cohesion: 0.22
Nodes (16): applyContact(), canHit(), collectProjectiles(), collectTargets(), Collider, box, entity, pos (+8 more)

### Community 30 - "Disconnect Handling"
Cohesion: 0.13
Nodes (11): DisconnectReason, Kicked, Quit, Shutdown, Timeout, Client::close(), Client::flush(), Client::latency() (+3 more)

### Community 31 - "Client Main"
Cohesion: 0.14
Nodes (3): main(), parsePort(), parsePort()

### Community 32 - "Audio Player Impl"
Cohesion: 0.15
Nodes (5): AudioPlayer::AudioPlayer(), AudioPlayer::play(), AudioPlayer::playMusic(), AudioPlayer::setMusicVolume(), toSfmlVolume()

### Community 34 - "Command Buffer"
Cohesion: 0.14
Nodes (5): CommandBuffer, _commands, Scheduler, _commands, _systems

### Community 35 - "Boss System"
Cohesion: 0.17
Nodes (8): bossSystem(), calculatePos(), inputDirection(), lifetimeSystem(), movementSystem(), playerControlSystem(), updateWeaponTimers(), run()

### Community 36 - "Design Choices Docs"
Cohesion: 0.20
Nodes (14): Audio/visual accessibility (health bars, per-slot sprites; palette, contrast proposed), rtype::config (Config.hpp constants), Motor accessibility (keyboard only done; remap, toggle fire, gamepad proposed), Choice: per-message reliability, Choice: event-based replication with NetId, Choice: sessions with token, ping, 5s timeout, DESTROY 0x21 (S->C, reliable, entity id), Connection lifecycle / Handshake (Connect, Accept(token), Refuse, YOU, SPAWN, Ping, timeout 5s) (+6 more)

### Community 38 - "Connection Impl"
Cohesion: 0.19
Nodes (8): Connection::Connection(), Connection::handlePong(), Connection::hasHandshakeMessage(), Connection::latency(), Connection::nextSequence(), Connection::peerDisconnect(), Connection::readId(), Connection::token()

### Community 39 - "Replica Events"
Cohesion: 0.18
Nodes (5): Unreachable, applyDecoded(), Network, _client, _open

### Community 40 - "Build Tooling Docs"
Cohesion: 0.18
Nodes (13): vcpkg submodule (external/vcpkg, manifest vcpkg.json, Asio + SFML 3), Technical and comparative study doc, Choice: AABB collision, Choice: hand-written binary big-endian encoding, Choice: CMake with presets, Choice: C++20, Choice: collect first, act after loop (deferred structural changes), Long-term monitoring (pinned vcpkg baseline, TODO CI/sanitizers/fuzzing) (+5 more)

### Community 41 - "Connect Events"
Cohesion: 0.15
Nodes (9): Connected, client, Disconnected, client, reason, Message, client, payload (+1 more)

### Community 42 - "Reliable Channel"
Cohesion: 0.23
Nodes (5): ReliableChannel::acknowledge(), ReliableChannel::receive(), ReliableChannel::resend(), ReliableChannel::send(), ReliableChannel::takeReady()

### Community 43 - "Replication"
Cohesion: 0.24
Nodes (6): collectSpawns(), collectStates(), collectUntracked(), describeSpawn(), isReplicable(), slotOf()

### Community 44 - "Client Audio"
Cohesion: 0.15
Nodes (9): Audio, _dir, _enabled, _ids, _phase, _player, SOUND_COUNT, _sounds (+1 more)

### Community 46 - "Contributing and Security"
Cohesion: 0.20
Nodes (12): Accessibility doc, Cognitive accessibility (simple rules, short levels, clear feedback), Security risks and mitigations, Contributing doc, Branch prefixes and commit types, Coding conventions (80 cols, 25-line functions, 5 params, 2 nested ifs, no ternary, _ private prefix), Part ownership (engine/core, server, client, game logic each have an owner), Network protocol doc (+4 more)

### Community 47 - "Architecture Targets"
Cohesion: 0.20
Nodes (11): r-type_client (display executable), Client replica of the world (entity kind + position), engine::core (Registry, Scheduler, CommandBuffer, FixedTimestep), rtype::common (shared headers: Vec2, return codes), rtype::game (header-only game logic), r-type_server (authoritative server executable), Choice: authoritative server, Choice: archetype-based ECS (+3 more)

### Community 48 - "Entity Spawn Events"
Cohesion: 0.18
Nodes (10): EntityDestroyed, id, EntitySpawned, box, id, pos, slot, sprite (+2 more)

### Community 50 - "Network and Protocol Docs"
Cohesion: 0.20
Nodes (11): engine::network (UDP transport, sessions, ping, reliability, wraps Asio), rtype::protocol (Protocol.hpp message encoding), Build and run doc, CMake presets linux-debug / windows-debug (Ninja on Linux), rtype_tests executable (game logic + protocol encoding tests, ctest), Build troubleshooting table, Choice: Asio, R-Type README (+3 more)

### Community 51 - "Keys"
Cohesion: 0.18
Nodes (11): Key, Down, Enter, Escape, Left, Right, Space, Unknown (+3 more)

### Community 53 - "Wave Spawning"
Cohesion: 0.18
Nodes (7): spawnBoss(), WaveStep, at, count, dy, interval, y

### Community 54 - "Connect Handling"
Cohesion: 0.20
Nodes (6): _random, Datagram, bytes, from, Server::handleDatagram(), Server::makeToken()

### Community 56 - "Controls"
Cohesion: 0.22
Nodes (5): KeyReleased, key, Controls, _input, _respawn

### Community 59 - "Server App State"
Cohesion: 0.20
Nodes (7): App, _game, _server, _slots, STATUS_EVERY_TICKS, _ticks, _timestep

### Community 60 - "Audio Impl"
Cohesion: 0.25
Nodes (6): AudioPlayer::Impl, music, musicVolume, next, soundVolume, voices

### Community 61 - "Binary Writer Impl"
Cohesion: 0.31
Nodes (4): BinaryWriter::writeBytes(), BinaryWriter::writeU16(), BinaryWriter::writeU8(), BinaryWriter::writeVarint()

### Community 62 - "Message Types"
Cohesion: 0.22
Nodes (5): Message, id, payload, reliable, type

### Community 63 - "Session Admission"
Cohesion: 0.25
Nodes (3): Server::handleConnect(), Server::handleSession(), Server::open()

### Community 65 - "Window Events"
Cohesion: 0.33
Nodes (6): FocusGained, FocusLost, convert(), toKey(), translate(), Window::pollEvent()

### Community 66 - "Fixed Timestep"
Cohesion: 0.25
Nodes (3): FixedTimestep, _accumulator, _maxAccumulator

### Community 68 - "Connection Receive"
Cohesion: 0.25
Nodes (3): Connection::receive(), Connection::timedOut(), Connection::update()

### Community 69 - "Connection Handlers"
Cohesion: 0.25
Nodes (3): Connection::handle(), Connection::handleUser(), Connection::sendPing()

### Community 70 - "Session Info"
Cohesion: 0.25
Nodes (6): Session, address, connection, id, Server::findByAddress(), Server::findByToken()

### Community 71 - "Renderer Draw"
Cohesion: 0.29
Nodes (4): Renderer::draw(), Renderer::drawRect(), Renderer::drawResized(), Renderer::Renderer()

### Community 72 - "Texture Manager"
Cohesion: 0.25
Nodes (3): TextureManager, _impl, SpriteBank::SpriteBank()

### Community 73 - "Texture Manager Impl"
Cohesion: 0.25
Nodes (3): TextureManager::Impl, ids, textures

### Community 74 - "Sound IDs"
Cohesion: 0.25
Nodes (8): Sound, BossAlert, Count, Defeat, Explosion, Hit, Shoot, Victory

### Community 77 - "Player Input"
Cohesion: 0.29
Nodes (6): PlayerInput, down, fire, left, right, up

### Community 78 - "Game Status"
Cohesion: 0.29
Nodes (6): GameStatus, bossHp, bossMaxHp, phase, players, sampleStatus()

### Community 80 - "Test Harness"
Cohesion: 0.33
Nodes (4): Case, name, run, Registrar

### Community 81 - "Dependency Rules"
Cohesion: 0.33
Nodes (6): Dependency rules, Game::setInput / Game::update(dt), Server App class (only place including both network and game), Server threads: network thread and game thread, Choice: mediator in main instead of IGame interface, Choice: network thread + game thread

### Community 83 - "Entity Manager"
Cohesion: 0.33
Nodes (3): EntityManager, _freeList, _generations

### Community 84 - "Entity Records"
Cohesion: 0.40
Nodes (4): EntityRecord, archetype, row, IterationGuard

### Community 85 - "Flush and Serialize"
Cohesion: 0.40
Nodes (4): Client::flushConnection(), Client::tryConnect(), Server::flush(), Server::sendRefuse()

### Community 86 - "Connection Send"
Cohesion: 0.47
Nodes (3): Connection::send(), Connection::sendEngine(), Connection::takeMessages()

### Community 87 - "Color"
Cohesion: 0.33
Nodes (5): Color, a, b, g, r

### Community 88 - "Audio Cue Enum"
Cohesion: 0.33
Nodes (6): Cue, BossAppears, BossExplosion, Explosion, Hit, PlayerShot

### Community 90 - "Layer Architecture Doc"
Cohesion: 0.40
Nodes (5): Architecture doc, engine::audio (sounds and music, wraps SFML), engine::window (rendering, wraps SFML), Three decoupled layers: rendering, networking, game logic, Choice: SFML 3

### Community 91 - "Refuse Reasons"
Cohesion: 0.40
Nodes (5): Refused, reason, RefuseReason, Full, Version

### Community 92 - "Game Phase"
Cohesion: 0.40
Nodes (4): Phase, Defeat, Playing, Victory

### Community 93 - "Client State"
Cohesion: 0.50
Nodes (4): State, Closed, Connected, Connecting

### Community 94 - "Resend Delay"
Cohesion: 0.50
Nodes (4): Connection::resendDelay(), Lifetime, duration, remaining

### Community 95 - "Resize Event"
Cohesion: 0.50
Nodes (3): Resized, height, width

### Community 96 - "Level Tracks"
Cohesion: 0.50
Nodes (4): Track, Boss, Level, None

### Community 97 - "Player Status"
Cohesion: 0.50
Nodes (4): PlayerStatus, alive, hp, slot

### Community 98 - "Pending Spawns"
Cohesion: 0.67
Nodes (3): PendingSpawn, delay, y

## Knowledge Gaps
- **318 isolated node(s):** `_impl`, `voices`, `next`, `music`, `soundVolume` (+313 more)
  These have ≤1 connection - possible missing edges or undocumented components. (Counts symbols only; 631 node(s) total have ≤1 connection when file, concept and rationale nodes are included.)
- **16 thin communities (<3 nodes) omitted from report** — run `graphify query` to explore isolated nodes.

## Suggested Questions
_Questions this graph is uniquely positioned to answer:_

- **Why does `Registry` connect `Registry Queries` to `ECS Column Storage`, `Animation`, `Command Buffer`, `Boss System`, `ECS Archetype`, `Bullets and Boss Fan`, `Replication`, `Game Tests`, `Entity Registry`, `Game Core`, `Entity Manager`, `Entity Records`, `Wave Spawning`, `Player Creation`, `Player Lifecycle`, `Replica`, `Collisions`?**
  _High betweenness centrality (0.090) - this node is a cross-community bridge._
- **Are the 3 inferred relationships involving `Entity` (e.g. with `collisionSystem()` and `deathSystem()`) actually correct?**
  _`Entity` has 3 INFERRED edges - model-reasoned connections that need verification._
- **What connects `_impl`, `voices`, `next` to the rest of the system?**
  _318 weakly-connected nodes found - possible documentation gaps or missing edges._
- **Should `ECS Column Storage` be split into smaller, more focused modules?**
  _Cohesion score 0.06787330316742081 - nodes in this community are weakly interconnected._
- **Why does `Game` connect `Game Core` to `Pending Spawns`, `Game Boss Logic`, `Registry Queries`, `Boss System`, `Replication`, `Game Tests`, `Wave Spawning`, `Player Creation`, `Player Lifecycle`, `Server App State`, `Game Phase`?**
  _High betweenness centrality (0.068) - this node is a cross-community bridge._
- **Are the 25 inferred relationships involving `TEST()` (e.g. with `Registry` and `.alive()`) actually correct?**
  _`TEST()` has 25 INFERRED edges - model-reasoned connections that need verification._
- **Should `Animation` be split into smaller, more focused modules?**
  _Cohesion score 0.05957767722473605 - nodes in this community are weakly interconnected._