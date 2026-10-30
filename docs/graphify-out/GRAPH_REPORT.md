# Graph Report - RTYPE  (2026-10-30)

## Corpus Check
- 123 files · ~58,057 words
- Verdict: corpus is large enough that graph structure adds value.
- Unclassified: 5 file(s) not represented in the graph (top: (none) 5)

## Summary
- 1745 nodes · 3445 edges · 116 communities (91 shown, 25 thin omitted)
- Extraction: 88% EXTRACTED · 12% INFERRED · 0% AMBIGUOUS · INFERRED: 400 edges (avg confidence: 0.88)
- Token cost: 0 input · 0 output

## Graph Freshness
- Built from commit: `e228a48e`
- Run `git rev-parse HEAD` and compare to check if the graph is stale.
- Run `graphify update .` after code changes (no API cost).

## Community Hubs (Navigation)
- ECS Column Storage
- IntRect
- Window
- Game.hpp
- Registry
- Client Connection
- Sound Manager
- make_sounds.py
- Delivery and Sessions
- Build and Protocol Docs
- Server Events
- Layers
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
- BinaryReader.cpp
- Systems.hpp
- App
- Collider
- Disconnect Handling
- Client Main
- Audio Player Impl
- src/App.cpp
- Command Buffer
- Registry.hpp
- Design Choices Docs
- Audio Player
- Connection Impl
- Replica Events
- Build Tooling Docs
- App
- Reliable Channel
- AugmentId
- Renderer
- Audio
- Network protocol doc
- Architecture Targets
- EntitySpawned
- Status Tests
- Background
- Keys
- Prediction.cpp
- OwnState
- R-Type game logic
- Components.hpp
- Controls
- Window::Window
- Replica.cpp
- Queries.hpp
- Audio Impl
- findPlayerBySlot
- Message Types
- UdpSocket::Impl
- SpriteId
- Window.cpp
- Packet.cpp
- Factories.hpp
- time_point
- Connection::handle
- Session Info
- Renderer Draw
- TextureManager
- Texture Manager Impl
- WaveStep
- BinaryReader
- Window Impl Header
- TEST
- GameStatus
- Controls.cpp
- Test Harness
- Audio.cpp
- Sound Manager Impl
- Entity Manager
- .update
- Flush and Serialize
- Connection Send
- Font.cpp
- Sound
- Cue
- Sample
- Team
- Font
- Sprite
- window/Event.hpp
- Phase
- Knowledge graph (graphify)
- Track
- StatusHeader
- vector
- .operator()
- App::run
- pull_request_template.md
- .load
- PendingSpawn
- CLAUDE.md
- vcpkg.json

## God Nodes (most connected - your core abstractions)
1. `Registry` - 94 edges
2. `Entity` - 82 edges
3. `Game` - 68 edges
4. `TEST()` - 63 edges
5. `Server` - 43 edges
6. `Connection` - 43 edges
7. `Replica` - 43 edges
8. `Client` - 38 edges
9. `Hud` - 32 edges
10. `Audio` - 28 edges

## Surprising Connections (you probably didn't know these)
- `The ECS rules to remember` --references--> `each()`  [INFERRED]
  src/client/game/Rtype/doc/README.md → engine/core/include/engine/core/Registry.hpp
- `Use the graph` --references--> `Entity`  [INFERRED]
  docs/graphify.md → engine/core/include/engine/core/Entity.hpp
- `Use the graph` --references--> `Connection`  [INFERRED]
  docs/graphify.md → engine/network/include/engine/network/Connection.hpp
- `Position, velocity, speed, direction` --references--> `calculatePos()`  [INFERRED]
  src/client/game/Rtype/doc/README.md → src/client/game/Rtype/include/logic/Movements.hpp
- `Add a system` --references--> `each()`  [INFERRED]
  src/client/game/Rtype/doc/README.md → engine/core/include/engine/core/Registry.hpp

## Import Cycles
- None detected.

## Hyperedges (group relationships)
- **Reliable game messages (SPAWN, DESTROY, YOU, RESPAWN)** — docs_protocol_spawn, docs_protocol_destroy, docs_protocol_you, docs_protocol_respawn [EXTRACTED 1.00]
- **Server glue: network + game + core via App** — docs_architecture_server_app_glue, docs_architecture_engine_network, docs_architecture_rtype_game [EXTRACTED 1.00]
- **Unreliable game messages (INPUT, STATES, STATUS)** — docs_protocol_input, docs_protocol_states, docs_protocol_status [EXTRACTED 1.00]

## Communities (116 total, 25 thin omitted)

### Community 0 - "ECS Column Storage"
Cohesion: 0.06
Nodes (24): Archetype, _columns, _columnsIndex, _entities, NO_COLUMN, _signature, Column, _data (+16 more)

### Community 1 - "IntRect"
Cohesion: 0.21
Nodes (11): Animation, firstFrame, frameCount, frameDuration, step, IntRect, height, left (+3 more)

### Community 2 - "Window"
Cohesion: 0.15
Nodes (5): Window, _impl, handle(), App::listening(), App::loop()

### Community 3 - "Game.hpp"
Cohesion: 0.19
Nodes (6): Star, layer, x, y, Background::Background(), Background::drawStar()

### Community 4 - "Registry"
Cohesion: 0.12
Nodes (18): eachRow(), EntityRecord, archetype, row, IterationGuard, Registry, _archetypes, EMPTY_ARCHETYPE (+10 more)

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
Cohesion: 0.14
Nodes (13): Server::admit(), Server::broadcast(), Server::checkSessions(), Server::clientCount(), Server::close(), Server::disconnect(), Server::find(), Server::kick() (+5 more)

### Community 9 - "Build and Protocol Docs"
Cohesion: 0.14
Nodes (20): engine_audio library, SFML::Audio, engine build root, engine_core library, Game protocol, Handshake and connection, Network module (engine::network), ReliableChannel (+12 more)

### Community 10 - "Server Events"
Cohesion: 0.06
Nodes (10): Server, _events, _maxClients, _nextClientId, _sessions, _socket, _version, Server::handleConnect() (+2 more)

### Community 11 - "Layers"
Cohesion: 0.16
Nodes (22): Architecture, Data oriented design, Layers, The ECS rules to remember, bossFan(), bulletDirection(), bulletStartPos(), isShooter() (+14 more)

### Community 14 - "Entity Registry"
Cohesion: 0.17
Nodes (9): Entity, id, has(), remove(), tryGet(), createBoss(), createMonster(), spawnBoss() (+1 more)

### Community 15 - "Connection Channel"
Cohesion: 0.08
Nodes (10): Connection, _channel, _lastPing, _lastReceived, _outbox, _pingId, _pingSentAt, _received (+2 more)

### Community 16 - "Game"
Cohesion: 0.09
Nodes (15): Game, _augments, _bossSpawned, _coinCounter, _elapsed, _events, _nextNetId, _nextWave (+7 more)

### Community 17 - "UdpSocket::Impl"
Cohesion: 0.11
Nodes (15): Address, ip, port, Datagram, bytes, from, toAddress(), toEndpoint() (+7 more)

### Community 18 - "Replica"
Cohesion: 0.08
Nodes (12): Replica, CLOCK_CORRECTION_RATE, _cues, _entities, INTERPOLATION_DELAY, _latestTick, MAX_CLOCK_ERROR, _prediction (+4 more)

### Community 19 - "Hud"
Cohesion: 0.10
Nodes (17): Hud, BAR_HEIGHT, BAR_WIDTH, BAR_X, BOSS_BAR_WIDTH, _font, _height, ICON_GAP (+9 more)

### Community 20 - "Entity State Payload"
Cohesion: 0.23
Nodes (16): decodeDestroy(), decodeInput(), decodeOwnState(), decodeSpawn(), decodeStates(), decodeStatus(), decodeYou(), encodeInput() (+8 more)

### Community 21 - "Prediction"
Cohesion: 0.09
Nodes (13): Pending, dt, input, sequence, Prediction, _input, MAX_PENDING, _nextSequence (+5 more)

### Community 22 - "Binary Writer"
Cohesion: 0.11
Nodes (15): BinaryWriter, _data, MAX_VARINT, BinaryWriter::writeBytes(), BinaryWriter::writeFloat(), BinaryWriter::writeU16(), BinaryWriter::writeU32(), BinaryWriter::writeU8() (+7 more)

### Community 23 - "Handshake Sequencing"
Cohesion: 0.11
Nodes (10): Packet, _messages, Client::checkConnection(), Client::handleDatagram(), Client::handleHandshake(), Client::handleSession(), Client::tryConnect(), Connection::hasHandshakeMessage() (+2 more)

### Community 24 - "Pending Messages"
Cohesion: 0.11
Nodes (8): Pending, message, sentAt, ReliableChannel, _early, _expected, _nextId, _pending

### Community 25 - "GameView"
Cohesion: 0.12
Nodes (9): GameView, ANIMATION_PHASE, _background, _hud, PLAYER_FRAME_DOWN, PLAYER_FRAME_IDLE, PLAYER_FRAME_UP, _worldToScreen (+1 more)

### Community 26 - "BinaryReader.cpp"
Cohesion: 0.20
Nodes (8): BinaryReader::BinaryReader(), BinaryReader::readBytes(), BinaryReader::readFloat(), BinaryReader::readU16(), BinaryReader::readU32(), BinaryReader::readU8(), BinaryReader::readVarint(), BinaryReader::remaining()

### Community 27 - "Systems.hpp"
Cohesion: 0.22
Nodes (12): The tick: `Game::update`, bossSystem(), explosionSize(), deathSystem(), isReadyToShoot(), lifetimeSystem(), movementSystem(), playerControlSystem() (+4 more)

### Community 28 - "App"
Cohesion: 0.11
Nodes (13): App, _assets, _connectedSince, _controls, FRAMERATE, _last, LISTEN_DELAY, _network (+5 more)

### Community 29 - "Collider"
Cohesion: 0.22
Nodes (16): applyContact(), canHit(), collectProjectiles(), collectTargets(), Collider, box, entity, pos (+8 more)

### Community 30 - "Disconnect Handling"
Cohesion: 0.08
Nodes (16): Delivery, Reliable, Unreliable, DisconnectReason, Kicked, Quit, Shutdown, Timeout (+8 more)

### Community 31 - "Client Main"
Cohesion: 0.13
Nodes (3): main(), parsePort(), parsePort()

### Community 32 - "Audio Player Impl"
Cohesion: 0.18
Nodes (4): AudioPlayer::play(), AudioPlayer::playMusic(), AudioPlayer::setMusicVolume(), toSfmlVolume()

### Community 34 - "Command Buffer"
Cohesion: 0.14
Nodes (5): CommandBuffer, _commands, Scheduler, _commands, _systems

### Community 35 - "Registry.hpp"
Cohesion: 0.21
Nodes (5): add(), create(), remove(), infoOf(), swapRemove()

### Community 36 - "Design Choices Docs"
Cohesion: 0.20
Nodes (14): Audio/visual accessibility (health bars, per-slot sprites; palette, contrast proposed), rtype::config (Config.hpp constants), Motor accessibility (keyboard only done; remap, toggle fire, gamepad proposed), Choice: per-message reliability, Choice: event-based replication with NetId, Choice: sessions with token, ping, 5s timeout, DESTROY 0x21 (S->C, reliable, entity id), Connection lifecycle / Handshake (Connect, Accept(token), Refuse, YOU, SPAWN, Ping, timeout 5s) (+6 more)

### Community 38 - "Connection Impl"
Cohesion: 0.18
Nodes (9): Connection::Connection(), Connection::handlePong(), Connection::latency(), Connection::nextSequence(), Connection::peerDisconnect(), Connection::readId(), Connection::resendDelay(), Connection::token() (+1 more)

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
Cohesion: 0.10
Nodes (22): addDamage(), addHp(), addSpeed(), applyAugment(), AugmentDef, apply, id, price (+14 more)

### Community 44 - "Renderer"
Cohesion: 0.20
Nodes (6): Renderer, SpriteBank, _clock, _sprites, GameView::GameView(), Hud::Hud()

### Community 45 - "Audio"
Cohesion: 0.12
Nodes (10): Audio, _dir, _enabled, _ids, _phase, _player, SOUND_COUNT, _sounds (+2 more)

### Community 46 - "Network protocol doc"
Cohesion: 0.17
Nodes (20): Accessibility doc, Cognitive accessibility (simple rules, short levels, clear feedback), Architecture doc, Build and run doc, CMake presets linux-debug / windows-debug (Ninja on Linux), Build troubleshooting table, Contributing doc, Branch prefixes and commit types (+12 more)

### Community 47 - "Architecture Targets"
Cohesion: 0.16
Nodes (17): r-type_client (display executable), Client replica of the world (entity kind + position), engine::core (Registry, Scheduler, CommandBuffer, FixedTimestep), engine::network (UDP transport, sessions, ping, reliability, wraps Asio), engine::window (rendering, wraps SFML), Three decoupled layers: rendering, networking, game logic, rtype::common (shared headers: Vec2, return codes), rtype::game (header-only game logic) (+9 more)

### Community 48 - "EntitySpawned"
Cohesion: 0.11
Nodes (18): EntityDestroyed, id, EntitySpawned, box, id, pos, slot, sprite (+10 more)

### Community 50 - "Background"
Cohesion: 0.20
Nodes (7): Background, LAYERS, _stars, STARS_PER_LAYER, _width, _worldToScreen, Background::draw()

### Community 51 - "Keys"
Cohesion: 0.18
Nodes (11): Key, Down, Enter, Escape, Left, Right, Space, Unknown (+3 more)

### Community 52 - "Prediction.cpp"
Cohesion: 0.13
Nodes (11): PlayerInput, down, fire, left, right, up, Prediction::position(), Prediction::reconcile() (+3 more)

### Community 53 - "OwnState"
Cohesion: 0.18
Nodes (11): Replica::apply(), encodeStates(), InputMessage, input, sequence, OwnState, lastInput, pos (+3 more)

### Community 54 - "R-Type game logic"
Cohesion: 0.17
Nodes (12): each(), Add a component, Add a new kind of monster, Add a player action (a new key), Add a system, Coding conventions, Configuration, Current status (+4 more)

### Community 55 - "Components.hpp"
Cohesion: 0.10
Nodes (27): Components, Position, velocity, speed, direction, Shooting, Systems, BossTag, age, CoinCounter, quantity (+19 more)

### Community 56 - "Controls"
Cohesion: 0.33
Nodes (3): Controls, _input, _respawn

### Community 58 - "Replica.cpp"
Cohesion: 0.18
Nodes (7): explosionCue(), Replica::applyPrediction(), Replica::localPlayer(), Replica::predict(), Replica::record(), Replica::recordSpawnCue(), Replica::slot()

### Community 59 - "Queries.hpp"
Cohesion: 0.21
Nodes (12): collectColliders(), collectMonsters(), collectPlayers(), collectProjectileEntities(), collectWith(), findFreeSlot(), findPlayerBySlot(), getCollider() (+4 more)

### Community 60 - "Audio Impl"
Cohesion: 0.24
Nodes (6): AudioPlayer::Impl, music, musicVolume, next, soundVolume, voices

### Community 62 - "Message Types"
Cohesion: 0.22
Nodes (5): Message, id, payload, reliable, type

### Community 63 - "UdpSocket::Impl"
Cohesion: 0.10
Nodes (12): UdpSocket::Impl::startReceive(), UdpSocket::Impl, buffer, BUFFER_SIZE, io, localPort, MAX_QUEUED, mutex (+4 more)

### Community 64 - "SpriteId"
Cohesion: 0.18
Nodes (11): Renderable, id, SpriteId, Boss, Coin, Count, Explosion, Monster (+3 more)

### Community 65 - "Window.cpp"
Cohesion: 0.22
Nodes (5): convert(), toKey(), translate(), Window::clear(), Window::pollEvent()

### Community 66 - "Packet.cpp"
Cohesion: 0.13
Nodes (10): Connection::flush(), Packet::add(), Packet::isHandshake(), Packet::isValid(), Packet::Packet(), Packet::parseMessage(), Packet::sequence(), Packet::size() (+2 more)

### Community 67 - "Factories.hpp"
Cohesion: 0.17
Nodes (9): addToPurse(), coinPickupSystem(), collectCoins(), spawnCoin(), touchesAny(), createCoin(), createCoinCounter(), createExplosion() (+1 more)

### Community 68 - "time_point"
Cohesion: 0.25
Nodes (3): Connection::receive(), Connection::timedOut(), Connection::update()

### Community 69 - "Connection::handle"
Cohesion: 0.22
Nodes (3): Connection::handle(), Connection::handleUser(), Connection::sendPing()

### Community 70 - "Session Info"
Cohesion: 0.15
Nodes (9): _random, Session, address, connection, id, Server::findByAddress(), Server::findByToken(), Server::handleSession() (+1 more)

### Community 71 - "Renderer Draw"
Cohesion: 0.21
Nodes (8): Vector2f, x, y, Renderer::draw(), Renderer::drawRect(), Renderer::drawResized(), Renderer::Renderer(), Hud::drawBar()

### Community 72 - "TextureManager"
Cohesion: 0.20
Nodes (5): TextureManager, _impl, SpriteBank::loadIcons(), SpriteBank::set(), SpriteBank::SpriteBank()

### Community 73 - "Texture Manager Impl"
Cohesion: 0.25
Nodes (3): TextureManager::Impl, ids, textures

### Community 74 - "WaveStep"
Cohesion: 0.33
Nodes (6): WaveStep, at, count, dy, interval, y

### Community 75 - "BinaryReader"
Cohesion: 0.15
Nodes (8): BinaryReader, _cursor, PlayerStatus, alive, hp, slot, readPlayers(), readPlayerStatus()

### Community 78 - "GameStatus"
Cohesion: 0.13
Nodes (15): GameStatus, bossHp, bossMaxHp, coins, elapsedMs, phase, players, formatTime() (+7 more)

### Community 79 - "Controls.cpp"
Cohesion: 0.20
Nodes (5): KeyReleased, key, Controls::applyKey(), Controls::handle(), Controls::poll()

### Community 80 - "Test Harness"
Cohesion: 0.33
Nodes (4): Case, name, run, Registrar

### Community 81 - "Audio.cpp"
Cohesion: 0.40
Nodes (3): Audio::follow(), Audio::onPhase(), Audio::playCue()

### Community 82 - "Sound Manager Impl"
Cohesion: 0.25
Nodes (3): SoundManager, _impl, AudioPlayer::AudioPlayer()

### Community 83 - "Entity Manager"
Cohesion: 0.29
Nodes (3): EntityManager, _freeList, _generations

### Community 84 - ".update"
Cohesion: 0.24
Nodes (5): kill(), killBoss(), playerOf(), run(), skipToBoss()

### Community 85 - "Flush and Serialize"
Cohesion: 0.22
Nodes (8): Refused, reason, RefuseReason, Full, Version, Client::flushConnection(), Server::flush(), Server::sendRefuse()

### Community 86 - "Connection Send"
Cohesion: 0.47
Nodes (3): Connection::send(), Connection::sendEngine(), Connection::takeMessages()

### Community 87 - "Font.cpp"
Cohesion: 0.16
Nodes (10): Color, a, b, g, r, Font::draw(), Font::drawCentered(), Font::drawRow() (+2 more)

### Community 88 - "Sound"
Cohesion: 0.20
Nodes (10): Sound, BossAlert, Coin, Count, Defeat, Explosion, Hit, Shoot (+2 more)

### Community 89 - "Cue"
Cohesion: 0.17
Nodes (9): Cue, BossAppears, BossExplosion, CoinPickup, Explosion, Hit, PlayerShot, Audio::play() (+1 more)

### Community 90 - "Sample"
Cohesion: 0.22
Nodes (5): Sample, pos, tick, Replica::interpolate(), Replica::update()

### Community 91 - "Team"
Cohesion: 0.17
Nodes (12): Collisions, BulletLook, height, sprite, width, Health, hp, Team (+4 more)

### Community 92 - "Font"
Cohesion: 0.15
Nodes (9): Font, GLYPH_HEIGHT, GLYPH_WIDTH, find(), Font::drawGlyph(), Glyph, rows, symbol (+1 more)

### Community 93 - "Sprite"
Cohesion: 0.29
Nodes (4): Sprite, animation, texture, Hud::drawIcon()

### Community 94 - "window/Event.hpp"
Cohesion: 0.25
Nodes (6): Closed, FocusGained, FocusLost, Resized, height, width

### Community 95 - "Phase"
Cohesion: 0.33
Nodes (5): Phase, Defeat, Playing, Victory, Hud::trackPhase()

### Community 96 - "Knowledge graph (graphify)"
Cohesion: 0.40
Nodes (5): Install, Keep it up to date: the git hook, Knowledge graph (graphify), Use the graph, With an AI agent (Claude Code)

### Community 97 - "Track"
Cohesion: 0.40
Nodes (5): Track, Boss, Level, None, Audio::playMusic()

### Community 98 - "StatusHeader"
Cohesion: 0.25
Nodes (7): StatusHeader, bossHp, bossMaxHp, coins, count, elapsedMs, phase

### Community 107 - "PendingSpawn"
Cohesion: 0.67
Nodes (3): PendingSpawn, delay, y

## Knowledge Gaps
- **381 isolated node(s):** `BAR_X`, `BAR_WIDTH`, `BAR_HEIGHT`, `ROW_STEP`, `BOSS_BAR_WIDTH` (+376 more)
  These have ≤1 connection - possible missing edges or undocumented components. (Counts symbols only; 781 node(s) total have ≤1 connection when file, concept and rationale nodes are included.)
- **25 thin communities (<3 nodes) omitted from report** — run `graphify query` to explore isolated nodes.

## Suggested Questions
_Questions this graph is uniquely positioned to answer:_

- **Why does `Game` connect `Game` to `Game.hpp`, `vector`, `Registry`, `Systems.hpp`, `App`, `AugmentId`, `PendingSpawn`, `TEST`, `Entity Registry`, `Game Tests`, `EntitySpawned`, `.update`, `R-Type game logic`, `Queries.hpp`, `findPlayerBySlot`, `Phase`?**
  _High betweenness centrality (0.089) - this node is a cross-community bridge._
- **Are the 2 inferred relationships involving `Registry` (e.g. with `TEST()` and `TEST()`) actually correct?**
  _`Registry` has 2 INFERRED edges - model-reasoned connections that need verification._
- **What connects `BAR_X`, `BAR_WIDTH`, `BAR_HEIGHT` to the rest of the system?**
  _381 weakly-connected nodes found - possible documentation gaps or missing edges._
- **Should `ECS Column Storage` be split into smaller, more focused modules?**
  _Cohesion score 0.05827067669172932 - nodes in this community are weakly interconnected._
- **Why does `Registry` connect `Registry` to `ECS Column Storage`, `Command Buffer`, `Registry.hpp`, `Factories.hpp`, `Queries.hpp`, `Team`, `AugmentId`, `Layers`, `TEST`, `Entity Registry`, `Game Tests`, `Game`, `EntitySpawned`, `Replica`, `Entity Manager`, `Systems.hpp`, `Collider`?**
  _High betweenness centrality (0.082) - this node is a cross-community bridge._
- **Are the 5 inferred relationships involving `Entity` (e.g. with `Use the graph` and `coinPickupSystem()`) actually correct?**
  _`Entity` has 5 INFERRED edges - model-reasoned connections that need verification._
- **Should `Registry` be split into smaller, more focused modules?**
  _Cohesion score 0.11594202898550725 - nodes in this community are weakly interconnected._