# Graph Report - RTYPE  (2026-11-03)

## Corpus Check
- 142 files · ~139,670 words
- Verdict: corpus is large enough that graph structure adds value.
- Unclassified: 5 file(s) not represented in the graph (top: (none) 5)

## Summary
- 2387 nodes · 4782 edges · 145 communities (111 shown, 34 thin omitted)
- Extraction: 87% EXTRACTED · 13% INFERRED · 0% AMBIGUOUS · INFERRED: 630 edges (avg confidence: 0.88)
- Token cost: 0 input · 0 output

## Graph Freshness
- Built from commit: `f835fba7`
- Run `git rev-parse HEAD` and compare to check if the graph is stale.
- Run `graphify update .` after code changes (no API cost).

## Community Hubs (Navigation)
- Signature
- IntRect
- string
- rtype/Protocol.hpp
- Background.cpp
- Client Connection
- vector
- UdpSocket::Impl
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
- RegistryData
- GameView
- App
- Client.cpp
- Registry.hpp
- Audio Player Impl
- GateSpec
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
- Public API: the `Game` class
- TEST
- Segments
- Key
- addGates
- MonsterKind
- Run
- Components
- SpriteId
- Window.cpp
- SpriteBank
- Systems.hpp
- src/App.cpp
- Background
- Message Types
- EntitySpawned
- BossKind
- window/Event.hpp
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
- Replication.hpp
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
- App
- Aura
- .operator()
- Track
- CLAUDE.md
- GameTests.cpp
- LevelDef
- ComponentRegistry.cpp
- src/App.cpp
- map1
- Archetype
- ObstacleStep
- Audio.cpp
- App::run
- vcpkg.json
- Gameplay
- Audio::play
- Hud.cpp
- ComponentId
- Scene
- Network module (engine::network)
- Vec2
- Streak
- Vector2f
- TextureManager::Impl
- ObstacleMotion
- collectPlayers
- Column
- ObstacleKind
- Hud::drawTimer
- Message
- Star
- BulletLook
- decodeLeaderboard
- Roadmap
- isNewer

## God Nodes (most connected - your core abstractions)
1. `TEST()` - 159 edges
2. `Registry` - 134 edges
3. `Entity` - 110 edges
4. `Game` - 93 edges
5. `Replica` - 57 edges
6. `Server` - 43 edges
7. `Connection` - 43 edges
8. `Layers` - 42 edges
9. `LevelDef` - 42 edges
10. `Background` - 40 edges

## Surprising Connections (you probably didn't know these)
- `Use the graph` --references--> `Entity`  [INFERRED]
  docs/graphify.md → engine/core/include/engine/core/Entity.hpp
- `Use the graph` --references--> `Connection`  [INFERRED]
  docs/graphify.md → engine/network/include/engine/network/Connection.hpp
- `The ECS rules to remember` --references--> `each()`  [INFERRED]
  src/client/game/Rtype/doc/README.md → engine/core/include/engine/core/Registry.hpp
- `The first map: 20 levels` --references--> `map1()`  [INFERRED]
  src/client/game/Rtype/doc/README.md → src/client/game/Rtype/src/maps/map1.cpp
- `Add a system` --references--> `each()`  [INFERRED]
  src/client/game/Rtype/doc/README.md → engine/core/include/engine/core/Registry.hpp

## Import Cycles
- None detected.

## Communities (145 total, 34 thin omitted)

### Community 0 - "Signature"
Cohesion: 0.30
Nodes (3): Signature, WORD_BITS, _words

### Community 1 - "IntRect"
Cohesion: 0.16
Nodes (14): Animation, firstFrame, frameCount, frameDuration, step, IntRect, height, left (+6 more)

### Community 2 - "string"
Cohesion: 0.19
Nodes (3): Window, _impl, App::loop()

### Community 3 - "rtype/Protocol.hpp"
Cohesion: 0.06
Nodes (21): add(), create(), remove(), infoOf(), swapRemove(), BossTag, age, summonTimer (+13 more)

### Community 4 - "Background.cpp"
Cohesion: 0.19
Nodes (15): Renderer, Background::drawBand(), Background::drawBars(), Background::drawFade(), Background::drawNebula(), Background::drawRings(), Background::drawSky(), Background::drawStar() (+7 more)

### Community 5 - "Client Connection"
Cohesion: 0.05
Nodes (17): Client, _attempts, _connection, _events, _handshakeSequence, _lastAttempt, _server, _socket (+9 more)

### Community 6 - "vector"
Cohesion: 0.12
Nodes (5): SoundManager::Impl, buffers, ids, SoundManager::load(), TextureManager::load()

### Community 7 - "UdpSocket::Impl"
Cohesion: 0.11
Nodes (11): UdpSocket::Impl::startReceive(), UdpSocket::Impl, buffer, BUFFER_SIZE, io, localPort, MAX_QUEUED, mutex (+3 more)

### Community 8 - "Delivery and Sessions"
Cohesion: 0.11
Nodes (17): Delivery, Reliable, Unreliable, Server::admit(), Server::broadcast(), Server::checkSessions(), Server::clientCount(), Server::close() (+9 more)

### Community 9 - "Build and Protocol Docs"
Cohesion: 0.23
Nodes (13): engine_audio library, SFML::Audio, engine build root, engine_core library, asio::asio, engine_network library, engine_window library, SFML::Graphics (+5 more)

### Community 10 - "Server Events"
Cohesion: 0.06
Nodes (9): Server, _events, _maxClients, _nextClientId, _sessions, _socket, _version, Server::handleConnect() (+1 more)

### Community 14 - "Registry"
Cohesion: 0.06
Nodes (39): Entity, id, hash<Engine::Entity>, EntityManager, _freeList, _generations, eachRow(), EntityRecord (+31 more)

### Community 15 - "Connection Channel"
Cohesion: 0.08
Nodes (10): Connection, _channel, _lastPing, _lastReceived, _outbox, _pingId, _pingSentAt, _received (+2 more)

### Community 16 - "Game"
Cohesion: 0.06
Nodes (21): Maps and lobby, Game, _augments, _coinCounter, _effectLevel, _elapsed, _events, _lobby (+13 more)

### Community 17 - "UdpSocket::Impl"
Cohesion: 0.11
Nodes (15): Address, ip, port, Datagram, bytes, from, toAddress(), toEndpoint() (+7 more)

### Community 18 - ".updatePlaying"
Cohesion: 0.07
Nodes (13): Replica, CLOCK_CORRECTION_RATE, _cues, _entities, INTERPOLATION_DELAY, _latestTick, MAX_CLOCK_ERROR, _prediction (+5 more)

### Community 19 - "Hud"
Cohesion: 0.09
Nodes (17): Hud, BAR_HEIGHT, BAR_WIDTH, BAR_X, BOSS_BAR_WIDTH, _font, _height, ICON_GAP (+9 more)

### Community 20 - "Entity State Payload"
Cohesion: 0.18
Nodes (16): decodeInput(), decodeOwnState(), decodePause(), decodeSelectMap(), decodeSpawn(), decodeStates(), decodeYou(), encodePause() (+8 more)

### Community 21 - "Prediction"
Cohesion: 0.07
Nodes (16): Pending, dt, input, sequence, Prediction, _box, _input, MAX_PENDING (+8 more)

### Community 22 - "Binary Writer"
Cohesion: 0.20
Nodes (9): BinaryWriter, _data, MAX_VARINT, Packet::serialize(), Server::sendAccept(), encode(), encodeInput(), encodeStateChunk() (+1 more)

### Community 23 - "Packet"
Cohesion: 0.10
Nodes (10): Packet, _messages, Client::checkConnection(), Client::handleDatagram(), Client::handleHandshake(), Client::handleSession(), Client::tryConnect(), Connection::hasHandshakeMessage() (+2 more)

### Community 24 - "Pending Messages"
Cohesion: 0.11
Nodes (8): Pending, message, sentAt, ReliableChannel, _early, _expected, _nextId, _pending

### Community 25 - "Replica"
Cohesion: 0.11
Nodes (13): GameView, ANIMATION_PHASE, _background, _hud, PLAYER_FRAME_DOWN, PLAYER_FRAME_IDLE, PLAYER_FRAME_UP, _screenHeight (+5 more)

### Community 26 - "BinaryReader"
Cohesion: 0.20
Nodes (8): BinaryReader::BinaryReader(), BinaryReader::readBytes(), BinaryReader::readFloat(), BinaryReader::readU16(), BinaryReader::readU32(), BinaryReader::readU8(), BinaryReader::readVarint(), BinaryReader::remaining()

### Community 27 - "RegistryData"
Cohesion: 0.29
Nodes (4): RegistryData, ids, infos, mutex

### Community 28 - "GameView"
Cohesion: 0.10
Nodes (14): App, _assets, _connectedSince, _controls, FRAMERATE, _last, LISTEN_DELAY, _network (+6 more)

### Community 29 - "App"
Cohesion: 0.11
Nodes (29): Layers, bulletStartPos(), isShooter(), applyContact(), canHit(), collectProjectiles(), collectTargets(), Collider (+21 more)

### Community 30 - "Client.cpp"
Cohesion: 0.09
Nodes (13): DisconnectReason, Kicked, Quit, Shutdown, Timeout, Client::Client(), Client::close(), Client::flush() (+5 more)

### Community 31 - "Registry.hpp"
Cohesion: 0.20
Nodes (3): main(), parsePort(), parsePort()

### Community 32 - "Audio Player Impl"
Cohesion: 0.15
Nodes (5): AudioPlayer::AudioPlayer(), AudioPlayer::play(), AudioPlayer::playMusic(), AudioPlayer::setMusicVolume(), toSfmlVolume()

### Community 33 - "GateSpec"
Cohesion: 0.12
Nodes (17): GateSpec, at, gapHeight, gapY, kind, motion, pinch, speed (+9 more)

### Community 34 - "Command Buffer"
Cohesion: 0.14
Nodes (5): CommandBuffer, _commands, Scheduler, _commands, _systems

### Community 35 - "BackdropId"
Cohesion: 0.12
Nodes (16): BackdropId, Count, Girders, NebulaBlue, NebulaGreen, NebulaOrange, NebulaPurple, NebulaRed (+8 more)

### Community 36 - "Design Choices Docs"
Cohesion: 0.20
Nodes (14): Audio/visual accessibility (health bars, per-slot sprites; palette, contrast proposed), rtype::config (Config.hpp constants), Motor accessibility (keyboard only done; remap, toggle fire, gamepad proposed), Choice: per-message reliability, Choice: event-based replication with NetId, Choice: sessions with token, ping, 5s timeout, DESTROY 0x21 (S->C, reliable, entity id), Connection lifecycle / Handshake (Connect, Accept(token), Refuse, YOU, SPAWN, Ping, timeout 5s) (+6 more)

### Community 38 - "Connection Impl"
Cohesion: 0.18
Nodes (9): Connection::Connection(), Connection::handlePong(), Connection::latency(), Connection::nextSequence(), Connection::peerDisconnect(), Connection::readId(), Connection::resendDelay(), Connection::token() (+1 more)

### Community 39 - "Replica Events"
Cohesion: 0.13
Nodes (6): Unreachable, applyDecoded(), Network, _client, Network::Network(), _open

### Community 40 - "Build Tooling Docs"
Cohesion: 0.10
Nodes (22): Dependency rules, engine::audio (sounds and music, wraps SFML), engine::window (rendering, wraps SFML), Game::setInput / Game::update(dt), Server App class (only place including both network and game), Server threads: network thread and game thread, vcpkg submodule (external/vcpkg, manifest vcpkg.json, Asio + SFML 3), Technical and comparative study doc (+14 more)

### Community 41 - "Message"
Cohesion: 0.15
Nodes (9): Connected, client, Disconnected, client, reason, Message, client, payload (+1 more)

### Community 42 - "Reliable Channel"
Cohesion: 0.23
Nodes (5): ReliableChannel::acknowledge(), ReliableChannel::receive(), ReliableChannel::resend(), ReliableChannel::send(), ReliableChannel::takeReady()

### Community 43 - "AugmentId"
Cohesion: 0.07
Nodes (23): addDamage(), addHp(), addSpeed(), applyAugment(), AugmentDef, apply, id, price (+15 more)

### Community 44 - "Zone"
Cohesion: 0.15
Nodes (11): Zone, Arena, Asteroids, Core, Count, DeepSpace, Nebula, Warp (+3 more)

### Community 45 - "Phase"
Cohesion: 0.14
Nodes (9): Audio, _dir, _enabled, _ids, _phase, _player, SOUND_COUNT, _sounds (+1 more)

### Community 46 - "Network protocol doc"
Cohesion: 0.14
Nodes (23): Accessibility doc, Cognitive accessibility (simple rules, short levels, clear feedback), Architecture doc, Three decoupled layers: rendering, networking, game logic, Build and run doc, CMake presets linux-debug / windows-debug (Ninja on Linux), Build troubleshooting table, Security risks and mitigations (+15 more)

### Community 47 - "Architecture Targets"
Cohesion: 0.15
Nodes (17): r-type_client (display executable), Client replica of the world (entity kind + position), engine::core (Registry, Scheduler, CommandBuffer, FixedTimestep), engine::network (UDP transport, sessions, ping, reliability, wraps Asio), rtype::common (shared headers: Vec2, return codes), rtype::game (header-only game logic), rtype::protocol (Protocol.hpp message encoding), r-type_server (authoritative server executable) (+9 more)

### Community 49 - "TEST"
Cohesion: 0.06
Nodes (7): Leaderboard, _records, RunRecord, players, timeMs, sampleStatus(), TEST()

### Community 50 - "Segments"
Cohesion: 0.10
Nodes (18): appendLevel(), buildMap(), buildRun(), createCoinCounter(), firstLevel(), lastSpawnPoint(), levelAt(), levelSpan() (+10 more)

### Community 51 - "Key"
Cohesion: 0.12
Nodes (15): Key, Backspace, Down, Enter, Escape, Left, Right, Space (+7 more)

### Community 52 - "addGates"
Cohesion: 0.13
Nodes (25): Obstacles, portal(), addCave(), addGates(), addRun(), addSweepers(), CavePiece, bottom (+17 more)

### Community 53 - "MonsterKind"
Cohesion: 0.14
Nodes (14): MonsterType, kind, PendingSpawn, delay, kind, y, MonsterKind, Basic (+6 more)

### Community 54 - "Run"
Cohesion: 0.28
Nodes (11): build(), chain(), gauntlet(), map3(), pinch(), Run, gaps, portals (+3 more)

### Community 55 - "Components"
Cohesion: 0.05
Nodes (52): each(), Add a component, Add a player action (a new key), Add a system, Architecture, Coding conventions, Collisions, Components (+44 more)

### Community 56 - "SpriteId"
Cohesion: 0.22
Nodes (4): Controls, _input, _pause, _respawn

### Community 57 - "Window.cpp"
Cohesion: 0.17
Nodes (6): convert(), toKey(), translate(), Window::clear(), Window::pollEvent(), Window::Window()

### Community 58 - "SpriteBank"
Cohesion: 0.13
Nodes (9): explosionCue(), Replica::applyPrediction(), Replica::bossSprite(), Replica::localPlayer(), Replica::mapCursor(), Replica::predict(), Replica::recordSpawnCue(), Replica::refreshSolids() (+1 more)

### Community 59 - "Systems.hpp"
Cohesion: 0.15
Nodes (14): Position, velocity, speed, direction, The tick: `Game::update`, bossSystem(), calculatePos(), inputDirection(), obstacleMotionSystem(), contactSystem(), lifetimeSystem() (+6 more)

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
Cohesion: 0.18
Nodes (10): BossType, kind, BossKind, Count, Dasher, Fortress, Standard, Summoner (+2 more)

### Community 65 - "window/Event.hpp"
Cohesion: 0.12
Nodes (9): Closed, FocusGained, FocusLost, Resized, height, width, handle(), Controls::applyKey() (+1 more)

### Community 66 - "describeSpawn"
Cohesion: 0.15
Nodes (9): Packet::add(), Packet::isHandshake(), Packet::isValid(), Packet::Packet(), Packet::parseMessage(), Packet::sequence(), Packet::size(), Packet::sizeOf() (+1 more)

### Community 67 - "SpriteId"
Cohesion: 0.07
Nodes (28): isBossSprite(), Renderable, id, SpriteId, AlienShard, Bomb, Bomber, Boss (+20 more)

### Community 68 - "time_point"
Cohesion: 0.25
Nodes (3): Connection::receive(), Connection::timedOut(), Connection::update()

### Community 70 - "Session Info"
Cohesion: 0.15
Nodes (9): _random, Session, address, connection, id, Server::findByAddress(), Server::findByToken(), Server::handleSession() (+1 more)

### Community 71 - "Renderer.cpp"
Cohesion: 0.09
Nodes (19): Color, a, b, g, r, Font, GLYPH_HEIGHT, GLYPH_WIDTH (+11 more)

### Community 72 - "SpriteBank"
Cohesion: 0.07
Nodes (21): TextureManager, _impl, SpriteBank, _assets, _backdrops, _clock, _sprites, Background::Background() (+13 more)

### Community 73 - "EntityState"
Cohesion: 0.24
Nodes (8): EntityState, id, pos, Replica::record(), encodeStates(), StatesSnapshot, states, tick

### Community 74 - "WaveStep"
Cohesion: 0.22
Nodes (8): shifted(), WaveStep, at, count, dy, interval, kind, y

### Community 75 - "Layers"
Cohesion: 0.14
Nodes (11): BinaryReader, _cursor, PlayerStatus, alive, hp, slot, decodeStatus(), isValid() (+3 more)

### Community 77 - "BinaryWriter.cpp"
Cohesion: 0.22
Nodes (6): BinaryWriter::writeBytes(), BinaryWriter::writeFloat(), BinaryWriter::writeU16(), BinaryWriter::writeU32(), BinaryWriter::writeU8(), BinaryWriter::writeVarint()

### Community 80 - "client/main.cpp"
Cohesion: 0.20
Nodes (4): Case, name, run, Registrar

### Community 81 - "Phase"
Cohesion: 0.15
Nodes (9): collectMonsters(), collectProjectileEntities(), Phase, Defeat, Lobby, Playing, Victory, Hud::trackPhase() (+1 more)

### Community 83 - "Replication.hpp"
Cohesion: 0.27
Nodes (6): collectSpawns(), collectStates(), collectUntracked(), describeSpawn(), isReplicable(), slotOf()

### Community 84 - "MonsterDef"
Cohesion: 0.14
Nodes (15): Add a new kind of monster, addMonsterStats(), createBoss(), createMonster(), setBossStats(), flyStraight(), MonsterDef, damage (+7 more)

### Community 85 - "RefuseReason"
Cohesion: 0.22
Nodes (8): Refused, reason, RefuseReason, Full, Version, Client::flushConnection(), Server::flush(), Server::sendRefuse()

### Community 86 - "Connection Send"
Cohesion: 0.32
Nodes (4): Connection::flush(), Connection::send(), Connection::sendEngine(), Connection::takeMessages()

### Community 87 - "createPlayer"
Cohesion: 0.15
Nodes (7): Game rules, CoinCounter, quantity, total, coins, value, createPlayer()

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
Cohesion: 0.12
Nodes (13): GameStatus, bossHp, bossMaxHp, coins, elapsedMs, level, map, paused (+5 more)

### Community 92 - "GameView.cpp"
Cohesion: 0.18
Nodes (7): GameView::drawLayer(), GameView::drawWorld(), GameView::GameView(), GameView::isBlock(), GameView::isPortalSprite(), GameView::sceneOf(), GameView::tiltFrame()

### Community 93 - "OwnState"
Cohesion: 0.20
Nodes (9): Prediction::reconcile(), encodeKeys(), isValidSpawn(), OwnState, inverted, lastInput, pos, speedPercent (+1 more)

### Community 94 - "App"
Cohesion: 0.07
Nodes (15): FixedTimestep, _accumulator, _maxAccumulator, App, App::App(), _game, _lastInputs, _lastPhase (+7 more)

### Community 95 - "Aura"
Cohesion: 0.29
Nodes (7): Aura, Dasher, Fortress, None, Standard, Summoner, GameView::auraOf()

### Community 96 - ".operator()"
Cohesion: 0.33
Nodes (5): Install, Keep it up to date: the git hook, Knowledge graph (graphify), Use the graph, With an AI agent (Claude Code)

### Community 97 - "Track"
Cohesion: 0.22
Nodes (9): Track, BossDasher, BossFortress, BossStandard, BossSummoner, Count, DeathRun, Level (+1 more)

### Community 98 - "CLAUDE.md"
Cohesion: 0.20
Nodes (9): StatusHeader, bossHp, bossMaxHp, coins, count, elapsedMs, level, map (+1 more)

### Community 99 - "GameTests.cpp"
Cohesion: 0.17
Nodes (11): addObstacleMotion(), createObstacle(), obstacleSprite(), spawnMonster(), blockOn(), colliderOf(), kill(), killBoss() (+3 more)

### Community 100 - "LevelDef"
Cohesion: 0.18
Nodes (28): LevelDef, bosses, length, obstacles, waves, wave(), level1(), level10() (+20 more)

### Community 103 - "src/App.cpp"
Cohesion: 0.11
Nodes (4): App::App(), App::listening(), App::playSounds(), App::run()

### Community 105 - "Archetype"
Cohesion: 0.27
Nodes (6): Archetype, _columns, _columnsIndex, _entities, NO_COLUMN, _signature

### Community 106 - "ObstacleStep"
Cohesion: 0.14
Nodes (16): addGate(), gateWall(), wall(), withMotion(), ObstacleStep, amplitude, at, height (+8 more)

### Community 107 - "Audio.cpp"
Cohesion: 0.20
Nodes (6): Audio::follow(), Audio::onPhase(), Audio::playCue(), Audio::playMusic(), Audio::playSound(), Audio::trackFor()

### Community 111 - "Gameplay"
Cohesion: 0.17
Nodes (12): Bosses, Coins, time and level skip, Controls, Enemies, Gameplay, Goal, Leaderboard, Lobby and maps (+4 more)

### Community 115 - "Hud.cpp"
Cohesion: 0.18
Nodes (8): formatTime(), Hud::drawEndScreen(), Hud::drawLobby(), Hud::drawMapList(), Hud::drawMenuHint(), Hud::drawPauseMenu(), Hud::drawRecords(), twoDigits()

### Community 116 - "ComponentId"
Cohesion: 0.22
Nodes (5): ComponentId, id, ComponentRegistry, getId(), idOf()

### Community 117 - "Scene"
Cohesion: 0.33
Nodes (5): Scene, aura, boosted, zone, Background::follow()

### Community 118 - "Network module (engine::network)"
Cohesion: 0.40
Nodes (5): Game protocol, Handshake and connection, Network module (engine::network), ReliableChannel, UDP wire format

### Community 119 - "Vec2"
Cohesion: 0.08
Nodes (31): Add a map or a level, Behaviours, Kinds of monsters and bosses, dasherTarget(), DashTarget, pos, velocity, roamBoss() (+23 more)

### Community 121 - "Streak"
Cohesion: 0.40
Nodes (5): Streak, length, speed, x, y

### Community 122 - "Vector2f"
Cohesion: 0.11
Nodes (17): Sprite, animation, texture, Vector2f, x, y, Renderer::draw(), Renderer::drawRect() (+9 more)

### Community 123 - "TextureManager::Impl"
Cohesion: 0.25
Nodes (3): TextureManager::Impl, ids, textures

### Community 124 - "ObstacleMotion"
Cohesion: 0.40
Nodes (5): ObstacleMotion, age, amplitude, baseY, omega

### Community 125 - "collectPlayers"
Cohesion: 0.14
Nodes (17): addToPurse(), coinPickupSystem(), collectCoins(), touchesAny(), createExplosion(), createExplosionOn(), absorbProjectiles(), crossPortals() (+9 more)

### Community 126 - "Column"
Cohesion: 0.31
Nodes (4): Column, _data, _info, ComponentInfo

### Community 127 - "ObstacleKind"
Cohesion: 0.11
Nodes (19): ObstacleTag, kind, PlayerEffects, inverted, lastPortal, speedFactor, isPortal(), ObstacleKind (+11 more)

### Community 128 - "Hud::drawTimer"
Cohesion: 0.47
Nodes (3): Hud::drawCoins(), Hud::drawLevel(), Hud::drawTimer()

### Community 130 - "Star"
Cohesion: 0.50
Nodes (4): Star, layer, x, y

### Community 131 - "BulletLook"
Cohesion: 0.50
Nodes (4): BulletLook, height, sprite, width

## Knowledge Gaps
- **579 isolated node(s):** `Unknown`, `Up`, `Down`, `Left`, `Right` (+574 more)
  These have ≤1 connection - possible missing edges or undocumented components. (Counts symbols only; 1136 node(s) total have ≤1 connection when file, concept and rationale nodes are included.)
- **34 thin communities (<3 nodes) omitted from report** — run `graphify query` to explore isolated nodes.

## Suggested Questions
_Questions this graph is uniquely positioned to answer:_

- **Why does `TEST()` connect `Game Tests` to `BossKind`, `GameTests.cpp`, `ObstacleStep`, `AugmentId`, `Registry`, `Game`, `Phase`, `Segments`, `Public API: the `Game` class`, `MonsterDef`, `TEST`, `Vec2`, `createPlayer`, `collectPlayers`, `Components`, `Systems.hpp`, `App`?**
  _High betweenness centrality (0.101) - this node is a cross-community bridge._
- **Are the 62 inferred relationships involving `TEST()` (e.g. with `Registry` and `.alive()`) actually correct?**
  _`TEST()` has 62 INFERRED edges - model-reasoned connections that need verification._
- **What connects `Unknown`, `Up`, `Down` to the rest of the system?**
  _579 weakly-connected nodes found - possible documentation gaps or missing edges._
- **Should `rtype/Protocol.hpp` be split into smaller, more focused modules?**
  _Cohesion score 0.061893845882560396 - nodes in this community are weakly interconnected._
- **Why does `Registry` connect `Registry` to `IntRect`, `rtype/Protocol.hpp`, `BulletLook`, `Game Tests`, `Game`, `.updatePlaying`, `Replica`, `App`, `Command Buffer`, `AugmentId`, `Segments`, `Components`, `Systems.hpp`, `BossKind`, `Phase`, `Replication.hpp`, `MonsterDef`, `createPlayer`, `GameView.cpp`, `GameTests.cpp`, `Archetype`, `Vec2`, `collectPlayers`?**
  _High betweenness centrality (0.097) - this node is a cross-community bridge._
- **Are the 2 inferred relationships involving `Registry` (e.g. with `TEST()` and `TEST()`) actually correct?**
  _`Registry` has 2 INFERRED edges - model-reasoned connections that need verification._
- **Should `Client Connection` be split into smaller, more focused modules?**
  _Cohesion score 0.046511627906976744 - nodes in this community are weakly interconnected._