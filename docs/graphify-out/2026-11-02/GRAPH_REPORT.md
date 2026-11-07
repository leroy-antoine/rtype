# Graph Report - RTYPE  (2026-11-02)

## Corpus Check
- 133 files · ~60,758 words
- Verdict: corpus is large enough that graph structure adds value.
- Unclassified: 5 file(s) not represented in the graph (top: (none) 5)

## Summary
- 1844 nodes · 3644 edges · 115 communities (89 shown, 26 thin omitted)
- Extraction: 88% EXTRACTED · 12% INFERRED · 0% AMBIGUOUS · INFERRED: 435 edges (avg confidence: 0.88)
- Token cost: 0 input · 0 output

## Graph Freshness
- Built from commit: `72a6b5d9`
- Run `git rev-parse HEAD` and compare to check if the graph is stale.
- Run `graphify update .` after code changes (no API cost).

## Community Hubs (Navigation)
- ECS Column Storage
- IntRect
- string
- cstddef
- size_t
- Client Connection
- vector
- R-Type game logic
- Delivery and Sessions
- Build and Protocol Docs
- Server Events
- Components
- Game Tests
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
- Column
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
- Replication.hpp
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
- Registry
- src/App.cpp
- Renderer
- Message Types
- EntitySpawned
- MonsterKind
- Sound
- describeSpawn
- SpriteId
- time_point
- Connection::handle
- Session Info
- Renderer.cpp
- TextureManager
- EntityState
- WaveStep
- Layers
- Window Impl Header
- BinaryWriter.cpp
- Delivery
- client/main.cpp
- Phase
- Sound Manager Impl
- Registry
- MonsterDef
- RefuseReason
- Connection Send
- DisconnectReason
- SlotTable
- DisconnectReason
- Knowledge graph (graphify)
- Layers
- TrackedTable
- CoinCounter
- Resized
- isNewer
- .operator()
- pull_request_template.md
- CLAUDE.md
- Replica.hpp
- CoinCounter
- map1
- mt19937
- App::run
- vcpkg.json

## God Nodes (most connected - your core abstractions)
1. `Registry` - 96 edges
2. `Entity` - 84 edges
3. `TEST()` - 75 edges
4. `Game` - 74 edges
5. `Server` - 43 edges
6. `Connection` - 43 edges
7. `Replica` - 43 edges
8. `Client` - 38 edges
9. `Layers` - 32 edges
10. `Hud` - 32 edges

## Surprising Connections (you probably didn't know these)
- `Les niveaux` --references--> `Segments`  [INFERRED]
  roadmap.md → src/client/game/Rtype/include/logic/Levels.hpp
- `Les types de monstres (`kind`)` --references--> `MonsterKind`  [INFERRED]
  roadmap.md → src/client/game/Rtype/include/logic/Kinds.hpp
- `The ECS rules to remember` --references--> `each()`  [INFERRED]
  src/client/game/Rtype/doc/README.md → engine/core/include/engine/core/Registry.hpp
- `Maps : une map est une campagne complète (idée, à faire plus tard)` --references--> `Phase`  [INFERRED]
  roadmap.md → src/client/game/Rtype/include/logic/Status.hpp
- `Use the graph` --references--> `Connection`  [INFERRED]
  docs/graphify.md → engine/network/include/engine/network/Connection.hpp

## Import Cycles
- None detected.

## Communities (115 total, 26 thin omitted)

### Community 0 - "ECS Column Storage"
Cohesion: 0.14
Nodes (9): Archetype, _columns, _columnsIndex, _entities, NO_COLUMN, _signature, Signature, WORD_BITS (+1 more)

### Community 1 - "IntRect"
Cohesion: 0.19
Nodes (11): Animation, firstFrame, frameCount, frameDuration, step, IntRect, height, left (+3 more)

### Community 2 - "string"
Cohesion: 0.16
Nodes (4): Window, _impl, handle(), App::loop()

### Community 4 - "size_t"
Cohesion: 0.40
Nodes (4): EntityRecord, archetype, row, IterationGuard

### Community 5 - "Client Connection"
Cohesion: 0.05
Nodes (17): Client, _attempts, _connection, _events, _handshakeSequence, _lastAttempt, _server, _socket (+9 more)

### Community 6 - "vector"
Cohesion: 0.09
Nodes (8): SoundManager::Impl, buffers, ids, SoundManager::load(), TextureManager::Impl, ids, textures, TextureManager::load()

### Community 7 - "R-Type game logic"
Cohesion: 0.23
Nodes (21): boss_alert(), decay(), defeat(), drums(), encode_ogg(), explosion(), fade(), hit() (+13 more)

### Community 8 - "Delivery and Sessions"
Cohesion: 0.14
Nodes (13): Server::admit(), Server::broadcast(), Server::checkSessions(), Server::clientCount(), Server::close(), Server::disconnect(), Server::find(), Server::kick() (+5 more)

### Community 9 - "Build and Protocol Docs"
Cohesion: 0.14
Nodes (20): engine_audio library, SFML::Audio, engine build root, engine_core library, Game protocol, Handshake and connection, Network module (engine::network), ReliableChannel (+12 more)

### Community 10 - "Server Events"
Cohesion: 0.06
Nodes (10): Server, _events, _maxClients, _nextClientId, _sessions, _socket, _version, Server::handleConnect() (+2 more)

### Community 11 - "Components"
Cohesion: 0.19
Nodes (17): Components, Position, velocity, speed, direction, The ECS rules to remember, bossFan(), calculatePos(), inputDirection(), GameView::drawEntity(), length() (+9 more)

### Community 14 - "size_t"
Cohesion: 0.09
Nodes (22): Entity, id, eachRow(), has(), remove(), tryGet(), addDamage(), addHp() (+14 more)

### Community 15 - "Connection Channel"
Cohesion: 0.08
Nodes (10): Connection, _channel, _lastPing, _lastReceived, _outbox, _pingId, _pingSentAt, _received (+2 more)

### Community 16 - "Game"
Cohesion: 0.06
Nodes (23): What the server reads, Game, _augments, _coinCounter, _elapsed, _events, _nextBoss, _nextNetId (+15 more)

### Community 17 - "UdpSocket::Impl"
Cohesion: 0.05
Nodes (27): Address, ip, port, Datagram, bytes, from, toAddress(), toEndpoint() (+19 more)

### Community 18 - ".updatePlaying"
Cohesion: 0.08
Nodes (12): Replica, CLOCK_CORRECTION_RATE, _cues, _entities, INTERPOLATION_DELAY, _latestTick, MAX_CLOCK_ERROR, _prediction (+4 more)

### Community 19 - "Hud"
Cohesion: 0.06
Nodes (30): Hud, BAR_HEIGHT, BAR_WIDTH, BAR_X, BOSS_BAR_WIDTH, _font, _height, ICON_GAP (+22 more)

### Community 20 - "Entity State Payload"
Cohesion: 0.24
Nodes (15): decodeDestroy(), decodeInput(), decodeOwnState(), decodeSpawn(), decodeStates(), decodeStatus(), decodeYou(), encodeKeys() (+7 more)

### Community 21 - "Prediction"
Cohesion: 0.09
Nodes (13): Pending, dt, input, sequence, Prediction, _input, MAX_PENDING, _nextSequence (+5 more)

### Community 22 - "Binary Writer"
Cohesion: 0.18
Nodes (10): BinaryWriter, _data, MAX_VARINT, Connection::sendId(), Packet::serialize(), Server::sendAccept(), encode(), encodeInput() (+2 more)

### Community 23 - "Packet"
Cohesion: 0.15
Nodes (7): Packet, _messages, Client::handleHandshake(), Client::handleSession(), Connection::hasHandshakeMessage(), Server::handleDatagram(), Server::handleSession()

### Community 24 - "Pending Messages"
Cohesion: 0.11
Nodes (8): Pending, message, sentAt, ReliableChannel, _early, _expected, _nextId, _pending

### Community 25 - "Replica"
Cohesion: 0.11
Nodes (14): GameView, ANIMATION_PHASE, _background, _hud, PLAYER_FRAME_DOWN, PLAYER_FRAME_IDLE, PLAYER_FRAME_UP, _worldToScreen (+6 more)

### Community 26 - "BinaryReader"
Cohesion: 0.18
Nodes (8): BinaryReader::BinaryReader(), BinaryReader::readBytes(), BinaryReader::readFloat(), BinaryReader::readU16(), BinaryReader::readU32(), BinaryReader::readU8(), BinaryReader::readVarint(), BinaryReader::remaining()

### Community 27 - "Column"
Cohesion: 0.10
Nodes (13): Column, _data, _info, ComponentId, id, ComponentInfo, getId(), ComponentRegistry::getInfo() (+5 more)

### Community 28 - "GameView"
Cohesion: 0.11
Nodes (14): App, _assets, _connectedSince, _controls, FRAMERATE, _last, LISTEN_DELAY, _network (+6 more)

### Community 29 - "App"
Cohesion: 0.15
Nodes (20): applyContact(), canHit(), collectProjectiles(), collectTargets(), Collider, box, entity, pos (+12 more)

### Community 30 - "Client.cpp"
Cohesion: 0.12
Nodes (10): Client::checkConnection(), Client::flush(), Client::flushConnection(), Client::handleDatagram(), Client::latency(), Client::pollEvent(), Client::sendDisconnect(), Client::tryConnect() (+2 more)

### Community 31 - "Registry.hpp"
Cohesion: 0.12
Nodes (4): main(), parsePort(), Network::Network(), parsePort()

### Community 32 - "Audio Player Impl"
Cohesion: 0.18
Nodes (4): AudioPlayer::play(), AudioPlayer::playMusic(), AudioPlayer::setMusicVolume(), toSfmlVolume()

### Community 34 - "Command Buffer"
Cohesion: 0.14
Nodes (5): CommandBuffer, _commands, Scheduler, _commands, _systems

### Community 35 - "OwnState"
Cohesion: 0.13
Nodes (8): add(), create(), remove(), ComponentRegistry, idOf(), infoOf(), swapRemove(), hash<Engine::Entity>

### Community 36 - "Design Choices Docs"
Cohesion: 0.20
Nodes (14): Audio/visual accessibility (health bars, per-slot sprites; palette, contrast proposed), rtype::config (Config.hpp constants), Motor accessibility (keyboard only done; remap, toggle fire, gamepad proposed), Choice: per-message reliability, Choice: event-based replication with NetId, Choice: sessions with token, ping, 5s timeout, DESTROY 0x21 (S->C, reliable, entity id), Connection lifecycle / Handshake (Connect, Accept(token), Refuse, YOU, SPAWN, Ping, timeout 5s) (+6 more)

### Community 38 - "Connection Impl"
Cohesion: 0.18
Nodes (9): Connection::Connection(), Connection::handlePong(), Connection::latency(), Connection::nextSequence(), Connection::peerDisconnect(), Connection::readId(), Connection::resendDelay(), Connection::token() (+1 more)

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
Cohesion: 0.07
Nodes (20): applyAugment(), AugmentDef, apply, id, price, rarity, AugmentId, Count (+12 more)

### Community 44 - "Audio"
Cohesion: 0.18
Nodes (7): Sprite, animation, texture, SpriteBank, _clock, _sprites, Hud::drawIcon()

### Community 45 - "Phase"
Cohesion: 0.12
Nodes (10): Audio, _dir, _enabled, _ids, _phase, _player, SOUND_COUNT, _sounds (+2 more)

### Community 46 - "Network protocol doc"
Cohesion: 0.17
Nodes (20): Accessibility doc, Cognitive accessibility (simple rules, short levels, clear feedback), Architecture doc, Build and run doc, CMake presets linux-debug / windows-debug (Ninja on Linux), Build troubleshooting table, Contributing doc, Branch prefixes and commit types (+12 more)

### Community 47 - "Architecture Targets"
Cohesion: 0.16
Nodes (17): r-type_client (display executable), Client replica of the world (entity kind + position), engine::core (Registry, Scheduler, CommandBuffer, FixedTimestep), engine::network (UDP transport, sessions, ping, reliability, wraps Asio), engine::window (rendering, wraps SFML), Three decoupled layers: rendering, networking, game logic, rtype::common (shared headers: Vec2, return codes), rtype::game (header-only game logic) (+9 more)

### Community 48 - "Replication.hpp"
Cohesion: 0.22
Nodes (6): collectSpawns(), collectStates(), collectUntracked(), describeSpawn(), isReplicable(), slotOf()

### Community 50 - "Segments"
Cohesion: 0.15
Nodes (7): firstLevel(), levelAt(), Segments, bossSpawn, levels, waves, TEST()

### Community 51 - "Key"
Cohesion: 0.11
Nodes (16): Key, Down, Enter, Escape, Left, Right, Space, Unknown (+8 more)

### Community 52 - "Window"
Cohesion: 0.13
Nodes (11): PlayerInput, down, fire, left, right, up, Prediction::position(), Prediction::reconcile() (+3 more)

### Community 53 - "MonsterKind"
Cohesion: 0.14
Nodes (14): MonsterType, kind, PendingSpawn, delay, kind, y, MonsterKind, Basic (+6 more)

### Community 54 - "Window.cpp"
Cohesion: 0.15
Nodes (13): each(), Add a component, Add a new kind of monster, Add a player action (a new key), Add a system, Coding conventions, Configuration, Current status (+5 more)

### Community 55 - "Replica.cpp"
Cohesion: 0.09
Nodes (26): Collisions, Shooting, Systems, Damage, value, Health, hp, Lifetime (+18 more)

### Community 56 - "SpriteId"
Cohesion: 0.33
Nodes (3): Controls, _input, _respawn

### Community 58 - "SpriteBank"
Cohesion: 0.20
Nodes (6): explosionCue(), Replica::applyPrediction(), Replica::localPlayer(), Replica::predict(), Replica::recordSpawnCue(), Replica::slot()

### Community 59 - "Registry"
Cohesion: 0.11
Nodes (31): Registry, _archetypes, EMPTY_ARCHETYPE, _entities, _iterating, NO_ARCHETYPE, _records, The tick: `Game::update` (+23 more)

### Community 60 - "src/App.cpp"
Cohesion: 0.24
Nodes (6): AudioPlayer::Impl, music, musicVolume, next, soundVolume, voices

### Community 61 - "Renderer"
Cohesion: 0.11
Nodes (15): Renderer, Background, LAYERS, _stars, STARS_PER_LAYER, _width, _worldToScreen, Star (+7 more)

### Community 62 - "Message Types"
Cohesion: 0.22
Nodes (5): Message, id, payload, reliable, type

### Community 63 - "EntitySpawned"
Cohesion: 0.14
Nodes (13): EntityDestroyed, id, EntitySpawned, box, id, pos, slot, sprite (+5 more)

### Community 64 - "MonsterKind"
Cohesion: 0.11
Nodes (18): Ajouter une map ou un niveau, Décision sur le gain de temps par kill, Les niveaux, Les types de boss (`BossKind`), Les types de monstres (`kind`), Maps : une map est une campagne complète (idée, à faire plus tard), Niveaux, types de monstres et de boss (design retenu), Ordre (+10 more)

### Community 65 - "Sound"
Cohesion: 0.18
Nodes (7): FocusGained, FocusLost, convert(), toKey(), translate(), Window::clear(), Window::pollEvent()

### Community 66 - "describeSpawn"
Cohesion: 0.12
Nodes (10): Packet::add(), Packet::isHandshake(), Packet::isValid(), Packet::Packet(), Packet::parse(), Packet::parseMessage(), Packet::sequence(), Packet::size() (+2 more)

### Community 67 - "SpriteId"
Cohesion: 0.14
Nodes (14): Les étapes, BossTag, age, Renderable, id, SpriteId, Boss, Coin (+6 more)

### Community 68 - "time_point"
Cohesion: 0.25
Nodes (3): Connection::receive(), Connection::timedOut(), Connection::update()

### Community 69 - "Connection::handle"
Cohesion: 0.25
Nodes (3): Connection::handle(), Connection::handleUser(), Connection::sendPing()

### Community 70 - "Session Info"
Cohesion: 0.18
Nodes (8): _random, Session, address, connection, id, Server::findByAddress(), Server::findByToken(), Server::makeToken()

### Community 71 - "Renderer.cpp"
Cohesion: 0.13
Nodes (14): Color, a, b, g, r, Vector2f, x, y (+6 more)

### Community 72 - "TextureManager"
Cohesion: 0.20
Nodes (5): TextureManager, _impl, SpriteBank::loadIcons(), SpriteBank::set(), SpriteBank::SpriteBank()

### Community 73 - "EntityState"
Cohesion: 0.18
Nodes (11): EntityState, id, pos, Replica::record(), encodeStates(), InputMessage, input, sequence (+3 more)

### Community 74 - "WaveStep"
Cohesion: 0.12
Nodes (18): Ce qui est en place, appendLevel(), buildMap(), buildRun(), LevelDef, bosses, length, waves (+10 more)

### Community 75 - "Layers"
Cohesion: 0.15
Nodes (8): BinaryReader, _cursor, PlayerStatus, alive, hp, slot, readPlayers(), readPlayerStatus()

### Community 77 - "BinaryWriter.cpp"
Cohesion: 0.22
Nodes (6): BinaryWriter::writeBytes(), BinaryWriter::writeFloat(), BinaryWriter::writeU16(), BinaryWriter::writeU32(), BinaryWriter::writeU8(), BinaryWriter::writeVarint()

### Community 78 - "Delivery"
Cohesion: 0.20
Nodes (5): Delivery, Reliable, Unreliable, Client::Client(), Client::send()

### Community 80 - "client/main.cpp"
Cohesion: 0.24
Nodes (4): Case, name, run, Registrar

### Community 81 - "Phase"
Cohesion: 0.18
Nodes (8): Phase, Defeat, Playing, Victory, Audio::follow(), Audio::onPhase(), Audio::playCue(), Hud::trackPhase()

### Community 82 - "Sound Manager Impl"
Cohesion: 0.25
Nodes (3): SoundManager, _impl, AudioPlayer::AudioPlayer()

### Community 83 - "Registry"
Cohesion: 0.29
Nodes (3): EntityManager, _freeList, _generations

### Community 84 - "MonsterDef"
Cohesion: 0.25
Nodes (8): MonsterDef, damage, fireCooldown, height, hp, speed, sprite, width

### Community 85 - "RefuseReason"
Cohesion: 0.40
Nodes (5): Refused, reason, RefuseReason, Full, Version

### Community 86 - "Connection Send"
Cohesion: 0.32
Nodes (4): Connection::flush(), Connection::send(), Connection::sendEngine(), Connection::takeMessages()

### Community 87 - "DisconnectReason"
Cohesion: 0.29
Nodes (7): DisconnectReason, Kicked, Quit, Shutdown, Timeout, Client::close(), Server::sendDisconnect()

### Community 88 - "SlotTable"
Cohesion: 0.20
Nodes (10): Sound, BossAlert, Coin, Count, Defeat, Explosion, Hit, Shoot (+2 more)

### Community 89 - "DisconnectReason"
Cohesion: 0.17
Nodes (9): Cue, BossAppears, BossExplosion, CoinPickup, Explosion, Hit, PlayerShot, Audio::play() (+1 more)

### Community 90 - "Knowledge graph (graphify)"
Cohesion: 0.22
Nodes (5): Sample, pos, tick, Replica::interpolate(), Replica::update()

### Community 91 - "Layers"
Cohesion: 0.26
Nodes (11): Architecture, Data oriented design, Layers, bulletDirection(), BulletLook, height, sprite, width (+3 more)

### Community 92 - "TrackedTable"
Cohesion: 0.11
Nodes (13): Font, GLYPH_HEIGHT, GLYPH_WIDTH, find(), Font::draw(), Font::drawCentered(), Font::drawGlyph(), Font::Font() (+5 more)

### Community 93 - "CoinCounter"
Cohesion: 0.33
Nodes (5): CoinCounter, quantity, total, coins, value

### Community 94 - "Resized"
Cohesion: 0.50
Nodes (3): Resized, height, width

### Community 96 - ".operator()"
Cohesion: 0.40
Nodes (5): Install, Keep it up to date: the git hook, Knowledge graph (graphify), Use the graph, With an AI agent (Claude Code)

### Community 97 - "pull_request_template.md"
Cohesion: 0.40
Nodes (5): Track, Boss, Level, None, Audio::playMusic()

### Community 98 - "CLAUDE.md"
Cohesion: 0.25
Nodes (7): StatusHeader, bossHp, bossMaxHp, coins, count, elapsedMs, phase

## Knowledge Gaps
- **411 isolated node(s):** `BAR_X`, `BAR_WIDTH`, `BAR_HEIGHT`, `ROW_STEP`, `BOSS_BAR_WIDTH` (+406 more)
  These have ≤1 connection - possible missing edges or undocumented components. (Counts symbols only; 836 node(s) total have ≤1 connection when file, concept and rationale nodes are included.)
- **26 thin communities (<3 nodes) omitted from report** — run `graphify query` to explore isolated nodes.

## Suggested Questions
_Questions this graph is uniquely positioned to answer:_

- **Why does `Game` connect `Game` to `cstddef`, `App`, `AugmentId`, `Game Tests`, `size_t`, `Replication.hpp`, `Phase`, `Segments`, `MonsterKind`, `Window.cpp`, `Registry`?**
  _High betweenness centrality (0.085) - this node is a cross-community bridge._
- **Are the 2 inferred relationships involving `Registry` (e.g. with `TEST()` and `TEST()`) actually correct?**
  _`Registry` has 2 INFERRED edges - model-reasoned connections that need verification._
- **What connects `BAR_X`, `BAR_WIDTH`, `BAR_HEIGHT` to the rest of the system?**
  _411 weakly-connected nodes found - possible documentation gaps or missing edges._
- **Should `ECS Column Storage` be split into smaller, more focused modules?**
  _Cohesion score 0.14039408866995073 - nodes in this community are weakly interconnected._
- **Why does `R-Type game logic` connect `Window.cpp` to `Layers`, `Components`, `Network protocol doc`, `Replica.cpp`, `Registry`?**
  _High betweenness centrality (0.075) - this node is a cross-community bridge._
- **Are the 5 inferred relationships involving `Entity` (e.g. with `Use the graph` and `coinPickupSystem()`) actually correct?**
  _`Entity` has 5 INFERRED edges - model-reasoned connections that need verification._
- **Should `Client Connection` be split into smaller, more focused modules?**
  _Cohesion score 0.046511627906976744 - nodes in this community are weakly interconnected._