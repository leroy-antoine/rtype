# Graph Report - RTYPE  (2026-11-06)

## Corpus Check
- 150 files · ~131,830 words
- Verdict: corpus is large enough that graph structure adds value.
- Unclassified: 5 file(s) not represented in the graph (top: (none) 5)

## Summary
- 2847 nodes · 5944 edges · 148 communities (131 shown, 17 thin omitted)
- Extraction: 87% EXTRACTED · 13% INFERRED · 0% AMBIGUOUS · INFERRED: 801 edges (avg confidence: 0.87)
- Token cost: 0 input · 0 output

## Graph Freshness
- Built from commit: `f02aa965`
- Run `git rev-parse HEAD` and compare to check if the graph is stale.
- Run `graphify update .` after code changes (no API cost).

## Community Hubs (Navigation)
- src/App.cpp
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
- ShopPanel.cpp
- Game Tests
- Components.hpp
- Session
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
- Collider
- ShopPanel
- Leviathan.hpp
- Audio Player Impl
- TEST
- Registry
- BackdropId
- Design Choices Docs
- Audio Player
- Phase
- Replica Events
- Build Tooling Docs
- Datagram
- Reliable Channel
- Shop.hpp
- Sample
- Phase
- Network protocol doc
- Architecture Targets
- ShopPanel::handle
- TEST
- Segments
- Key
- addGates
- MonsterKind
- map3.cpp
- Layers
- SpriteId
- Window.cpp
- SpriteBank
- SpriteBank.cpp
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
- BinaryReader
- Window Impl Header
- Renderer.cpp
- TEST
- ShopPanel::drawCard
- stdexcept
- Obstacles.hpp
- Sound Manager Impl
- HudVictory.cpp
- MonsterDef
- Background::drawAura
- Connection Send
- Hud::drawPlayers
- SlotTable
- DisconnectReason
- Connection::readId
- GameStatus
- LaserCycle
- Replica::apply
- game/App.cpp
- PlayerEffects
- .operator()
- Track
- CLAUDE.md
- findPlayerBySlot
- LevelDef
- UdpSocket
- App
- UdpSocket::Impl
- map1
- Archetype
- Zone
- Audio.cpp
- App::run
- vcpkg.json
- GateSpec
- Gameplay
- buildMap
- PendingSpawn
- EntityManager
- formatTime
- bossHazardSystem
- PlayerInput
- ProjectileDef
- TextureManager::Impl
- Streak
- GameView.cpp
- optional
- FixedTimestep
- TEST
- StrikeSpec
- ObstacleKind
- LeviathanMove
- Client::send
- Star
- Behaviours
- CommandBuffer
- Hud::drawLobby
- TeamId
- Case
- Hud::drawTimer
- Hud::drawVictoryStats
- Scene
- Boss.hpp
- Sprite
- Resized
- MonsterState
- Regeneration
- Shield
- How-to
- TurretFire
- ObstacleMotion

## God Nodes (most connected - your core abstractions)
1. `TEST()` - 197 edges
2. `Registry` - 164 edges
3. `Game` - 138 edges
4. `Entity` - 130 edges
5. `Replica` - 76 edges
6. `TEST()` - 59 edges
7. `TEST()` - 54 edges
8. `Hud` - 51 edges
9. `LevelDef` - 49 edges
10. `SpriteId` - 48 edges

## Surprising Connections (you probably didn't know these)
- `Use the graph` --references--> `Entity`  [INFERRED]
  docs/graphify.md → engine/core/include/engine/core/Entity.hpp
- `The ECS rules to remember` --references--> `each()`  [INFERRED]
  src/client/game/Rtype/doc/README.md → engine/core/include/engine/core/Registry.hpp
- `Use the graph` --references--> `Connection`  [INFERRED]
  docs/graphify.md → engine/network/include/engine/network/Connection.hpp
- `Add a system` --references--> `each()`  [INFERRED]
  src/client/game/Rtype/doc/README.md → engine/core/include/engine/core/Registry.hpp
- `Components` --references--> `ProjectileTag`  [INFERRED]
  src/client/game/Rtype/doc/README.md → src/client/game/Rtype/include/logic/Components.hpp

## Import Cycles
- None detected.

## Communities (148 total, 17 thin omitted)

### Community 1 - "IntRect"
Cohesion: 0.15
Nodes (14): Animation, firstFrame, frameCount, frameDuration, step, IntRect, height, left (+6 more)

### Community 2 - "string"
Cohesion: 0.12
Nodes (7): Window, _impl, handle(), App::listening(), App::loop(), App::run(), App::useShop()

### Community 3 - "Components.hpp"
Cohesion: 0.15
Nodes (8): MaxHealth, hp, MonsterTag, slot, MultiShot, bullets, ProjectileTag, Hud::drawMapList()

### Community 4 - "Background.cpp"
Cohesion: 0.17
Nodes (13): Renderer, Background::drawBand(), Background::drawBars(), Background::drawFade(), Background::drawSky(), Background::drawStar(), Background::drawStars(), Background::drawStreaks() (+5 more)

### Community 5 - "Client Connection"
Cohesion: 0.06
Nodes (15): Client, _attempts, _connection, _events, _handshakeSequence, _lastAttempt, _server, _socket (+7 more)

### Community 6 - "vector"
Cohesion: 0.12
Nodes (5): SoundManager::Impl, buffers, ids, SoundManager::load(), TextureManager::load()

### Community 7 - "Component.hpp"
Cohesion: 0.17
Nodes (4): add(), create(), remove(), hash<Engine::Entity>

### Community 8 - "Delivery and Sessions"
Cohesion: 0.12
Nodes (16): Delivery, Reliable, Unreliable, Server::admit(), Server::broadcast(), Server::checkSessions(), Server::clientCount(), Server::close() (+8 more)

### Community 9 - "Build and Protocol Docs"
Cohesion: 0.16
Nodes (17): engine_audio library, SFML::Audio, engine build root, engine_core library, Handshake and connection, Network module (engine::network), ReliableChannel, UDP wire format (+9 more)

### Community 10 - "Server Events"
Cohesion: 0.06
Nodes (10): Server, _events, _maxClients, _nextClientId, _sessions, _socket, _version, Server::handleConnect() (+2 more)

### Community 11 - "ShopPanel.cpp"
Cohesion: 0.18
Nodes (8): Hud::textTop(), ShopPanel::affordable(), ShopPanel::drawPrice(), ShopPanel::hint(), ShopPanel::isOpen(), ShopPanel::selection(), ShopPanel::ShopPanel(), ShopPanel::showMessage()

### Community 13 - "Components.hpp"
Cohesion: 0.23
Nodes (3): isNewer(), Connection::Connection(), Connection::token()

### Community 14 - "Session"
Cohesion: 0.15
Nodes (9): _random, Session, address, connection, id, Server::findByAddress(), Server::findByToken(), Server::handleSession() (+1 more)

### Community 15 - "Connection Channel"
Cohesion: 0.08
Nodes (10): Connection, _channel, _lastPing, _lastReceived, _outbox, _pingId, _pingSentAt, _received (+2 more)

### Community 16 - "Game"
Cohesion: 0.04
Nodes (33): Public API: the `Game` class, What the server reads, Game, _alive, _augments, _coinCounter, _countdown, _deaths (+25 more)

### Community 17 - "Address"
Cohesion: 0.14
Nodes (12): Address, ip, port, toAddress(), toEndpoint(), UdpSocket::Impl::Impl(), UdpSocket::Impl::store(), UdpSocket::port() (+4 more)

### Community 18 - ".updatePlaying"
Cohesion: 0.05
Nodes (18): Replica, _augments, _buyResult, CLOCK_CORRECTION_RATE, _cues, _entities, INTERPOLATION_DELAY, _latestTick (+10 more)

### Community 19 - "Hud"
Cohesion: 0.08
Nodes (21): Hud, AUGMENT_GAP, AUGMENT_PADDING, AUGMENT_TEXT_SCALE, BAR_HEIGHT, BAR_WIDTH, BAR_X, BOSS_BAR_WIDTH (+13 more)

### Community 20 - "Entity State Payload"
Cohesion: 0.17
Nodes (27): decodeAugments(), decodeBuy(), decodeBuyResult(), decodeDestroy(), decodeInput(), decodeOwnState(), decodePause(), decodeReady() (+19 more)

### Community 21 - "Prediction"
Cohesion: 0.07
Nodes (16): Pending, dt, input, sequence, Prediction, _box, _input, MAX_PENDING (+8 more)

### Community 22 - "Binary Writer"
Cohesion: 0.10
Nodes (17): BinaryWriter, _data, MAX_VARINT, BinaryWriter::writeBytes(), BinaryWriter::writeFloat(), BinaryWriter::writeU16(), BinaryWriter::writeU32(), BinaryWriter::writeU8() (+9 more)

### Community 23 - "Packet"
Cohesion: 0.08
Nodes (16): Packet, _messages, RefuseReason, Full, Version, Client::checkConnection(), Client::flushConnection(), Client::handleHandshake() (+8 more)

### Community 24 - "Pending Messages"
Cohesion: 0.11
Nodes (8): Pending, message, sentAt, ReliableChannel, _early, _expected, _nextId, _pending

### Community 25 - "Replica"
Cohesion: 0.08
Nodes (17): GameView, ANIMATION_PHASE, _background, _hud, HURT_DURATION, HURT_SHAKE, _hurtUntil, _lastBossHp (+9 more)

### Community 26 - "BinaryReader"
Cohesion: 0.20
Nodes (8): BinaryReader::BinaryReader(), BinaryReader::readBytes(), BinaryReader::readFloat(), BinaryReader::readU16(), BinaryReader::readU32(), BinaryReader::readU8(), BinaryReader::readVarint(), BinaryReader::remaining()

### Community 27 - "DeathRuns.hpp"
Cohesion: 0.17
Nodes (19): addCourse(), addCoursePortal(), checkPattern(), Course, after, gapFrom, gapTo, motion (+11 more)

### Community 28 - "GameView"
Cohesion: 0.08
Nodes (16): App, _assets, _connectedSince, _controls, FRAMERATE, _last, LISTEN_DELAY, _network (+8 more)

### Community 29 - "Collider"
Cohesion: 0.11
Nodes (25): applyContact(), canHit(), collectProjectiles(), collectTargets(), Collider, box, entity, pos (+17 more)

### Community 30 - "ShopPanel"
Cohesion: 0.07
Nodes (20): ShopPanel, BORDER, CARD_GAP, CARD_HEIGHT, CARD_TOP, CARD_WIDTH, COIN_GAP, COIN_SIZE (+12 more)

### Community 31 - "Leviathan.hpp"
Cohesion: 0.17
Nodes (24): LeviathanState, move, step, swing, timer, aimVelocity(), armStrike(), bossStopX() (+16 more)

### Community 32 - "Audio Player Impl"
Cohesion: 0.18
Nodes (4): AudioPlayer::play(), AudioPlayer::playMusic(), AudioPlayer::setMusicVolume(), toSfmlVolume()

### Community 33 - "TEST"
Cohesion: 0.07
Nodes (18): bossSystem(), fan(), volley(), obstacleMotionSystem(), bombVelocity(), healPlayers(), isReadyToShoot(), lifetimeSystem() (+10 more)

### Community 34 - "Registry"
Cohesion: 0.06
Nodes (55): Entity, id, eachRow(), EntityRecord, archetype, row, has(), IterationGuard (+47 more)

### Community 35 - "BackdropId"
Cohesion: 0.13
Nodes (15): BackdropId, Count, Girders, NebulaBlue, NebulaGreen, NebulaOrange, NebulaPurple, NebulaRed (+7 more)

### Community 36 - "Design Choices Docs"
Cohesion: 0.17
Nodes (16): Accessibility doc, Audio/visual accessibility (health bars, per-slot sprites; palette, contrast proposed), Cognitive accessibility (simple rules, short levels, clear feedback), rtype::config (Config.hpp constants), Motor accessibility (keyboard only done; remap, toggle fire, gamepad proposed), Choice: per-message reliability, Choice: event-based replication with NetId, Choice: sessions with token, ping, 5s timeout (+8 more)

### Community 38 - "Phase"
Cohesion: 0.17
Nodes (9): Phase, Defeat, Lobby, Playing, Shop, Victory, Audio::onPhase(), Audio::playCue() (+1 more)

### Community 39 - "Replica Events"
Cohesion: 0.07
Nodes (17): Connected, client, Disconnected, client, reason, Message, client, payload (+9 more)

### Community 40 - "Build Tooling Docs"
Cohesion: 0.12
Nodes (20): Dependency rules, Game::setInput / Game::update(dt), Server App class (only place including both network and game), Server threads: network thread and game thread, Technical and comparative study doc, Choice: AABB collision, Choice: hand-written binary big-endian encoding, Choice: CMake with presets (+12 more)

### Community 41 - "Datagram"
Cohesion: 0.22
Nodes (5): Datagram, bytes, from, Client::handleDatagram(), Server::handleDatagram()

### Community 42 - "Reliable Channel"
Cohesion: 0.15
Nodes (9): Connection::latency(), Connection::peerDisconnect(), Connection::resendDelay(), ReliableChannel::acknowledge(), ReliableChannel::receive(), ReliableChannel::resend(), ReliableChannel::send(), ReliableChannel::takeReady() (+1 more)

### Community 43 - "Shop.hpp"
Cohesion: 0.07
Nodes (36): applyAugment(), AugmentDef, apply, id, price, rarity, AugmentId, Count (+28 more)

### Community 44 - "Sample"
Cohesion: 0.22
Nodes (5): Sample, pos, tick, Replica::interpolate(), Replica::update()

### Community 45 - "Phase"
Cohesion: 0.10
Nodes (12): Audio, _dir, _enabled, _ids, _phase, _player, SHOP_SOUND_COUNT, _shopIds (+4 more)

### Community 46 - "Network protocol doc"
Cohesion: 0.16
Nodes (22): Architecture doc, Build and run doc, CMake presets linux-debug / windows-debug (Ninja on Linux), Build troubleshooting table, vcpkg submodule (external/vcpkg, manifest vcpkg.json, Asio + SFML 3), Contributing doc, Branch prefixes and commit types, Coding conventions (80 cols, 25-line functions, 5 params, 2 nested ifs, no ternary, _ private prefix) (+14 more)

### Community 47 - "Architecture Targets"
Cohesion: 0.14
Nodes (19): r-type_client (display executable), Client replica of the world (entity kind + position), engine::audio (sounds and music, wraps SFML), engine::core (Registry, Scheduler, CommandBuffer, FixedTimestep), engine::network (UDP transport, sessions, ping, reliability, wraps Asio), engine::window (rendering, wraps SFML), Three decoupled layers: rendering, networking, game logic, rtype::common (shared headers: Vec2, return codes) (+11 more)

### Community 48 - "ShopPanel::handle"
Cohesion: 0.25
Nodes (3): ShopPanel::confirm(), ShopPanel::handle(), ShopPanel::showResult()

### Community 49 - "TEST"
Cohesion: 0.05
Nodes (6): Leaderboard, _records, RunRecord, players, timeMs, TEST()

### Community 50 - "Segments"
Cohesion: 0.11
Nodes (17): firstLevel(), isDeathRun(), lastSpawnPoint(), levelAt(), levelSpan(), levelStart(), levelStartOf(), Segments (+9 more)

### Community 51 - "Key"
Cohesion: 0.10
Nodes (21): Key, A, Backspace, C, D, Down, E, Enter (+13 more)

### Community 52 - "addGates"
Cohesion: 0.10
Nodes (32): Obstacles, portal(), wall(), addCave(), addGates(), addRun(), addSweepers(), CavePiece (+24 more)

### Community 53 - "MonsterKind"
Cohesion: 0.15
Nodes (13): MonsterType, kind, MonsterKind, Basic, Bomber, Count, Elite, Kamikaze (+5 more)

### Community 54 - "map3.cpp"
Cohesion: 0.54
Nodes (6): gauntlet(), map3(), minefield(), pinch(), rhythm(), run()

### Community 55 - "Layers"
Cohesion: 0.07
Nodes (53): Architecture, Coding conventions, Collisions, Components, Configuration, Current status, Data oriented design, Layers (+45 more)

### Community 56 - "SpriteId"
Cohesion: 0.17
Nodes (6): Controls, _held, _input, _pause, _pressed, _respawn

### Community 57 - "Window.cpp"
Cohesion: 0.15
Nodes (8): FocusGained, FocusLost, convert(), toKey(), translate(), Window::clear(), Window::pollEvent(), Window::Window()

### Community 58 - "SpriteBank"
Cohesion: 0.12
Nodes (11): explosionCue(), Replica::applyPrediction(), Replica::bossSprite(), Replica::localPlayer(), Replica::mapCursor(), Replica::predict(), Replica::record(), Replica::recordSpawnCue() (+3 more)

### Community 59 - "SpriteBank.cpp"
Cohesion: 0.13
Nodes (5): SpriteBank::add(), SpriteBank::addBackdrop(), SpriteBank::loadBackdrops(), SpriteBank::loadMisc(), SpriteBank::texture()

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
Nodes (11): EntityDestroyed, id, EntitySpawned, box, id, pos, slot, sprite (+3 more)

### Community 64 - "BossKind"
Cohesion: 0.20
Nodes (10): BossType, kind, BossKind, Count, Dasher, Fortress, Standard, Summoner (+2 more)

### Community 65 - "Controls.cpp"
Cohesion: 0.15
Nodes (7): KeyPressed, key, KeyReleased, key, Controls::applyKey(), Controls::handle(), Controls::poll()

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
Cohesion: 0.22
Nodes (3): Connection::handle(), Connection::handleUser(), Connection::sendPing()

### Community 70 - "Session Info"
Cohesion: 0.20
Nodes (9): DisconnectReason, Kicked, Quit, Shutdown, Timeout, Client::close(), Client::flush(), Client::sendDisconnect() (+1 more)

### Community 71 - "Renderer.cpp"
Cohesion: 0.09
Nodes (19): Color, a, b, g, r, Font, GLYPH_HEIGHT, GLYPH_WIDTH (+11 more)

### Community 72 - "SpriteBank"
Cohesion: 0.10
Nodes (10): TextureManager, _impl, SpriteBank, _assets, _backdrops, _clock, _sprites, Background::Background() (+2 more)

### Community 73 - "EntityState"
Cohesion: 0.27
Nodes (8): EntityState, id, pos, encodeStateChunk(), encodeStates(), StatesSnapshot, states, tick

### Community 74 - "WaveStep"
Cohesion: 0.14
Nodes (14): fittedBase(), fittedSpacing(), formationDelay(), formationSlot(), Slot, delay, y, WaveStep (+6 more)

### Community 75 - "BinaryReader"
Cohesion: 0.12
Nodes (11): BinaryReader, _cursor, PlayerStatus, alive, hp, ready, slot, decodeLeaderboard() (+3 more)

### Community 77 - "Renderer.cpp"
Cohesion: 0.29
Nodes (4): Renderer::draw(), Renderer::drawRect(), Renderer::drawResized(), Renderer::Renderer()

### Community 79 - "ShopPanel::drawCard"
Cohesion: 0.32
Nodes (5): augmentLabel(), rarityColor(), rarityName(), Hud::drawAugments(), ShopPanel::drawCard()

### Community 80 - "stdexcept"
Cohesion: 0.13
Nodes (3): main(), parsePort(), parsePort()

### Community 81 - "Obstacles.hpp"
Cohesion: 0.09
Nodes (33): The tick: `Game::update`, isFarOutOfBounds(), countBefore(), absorbProjectiles(), applyPortal(), beamKind(), crossPortals(), deadlyColliders() (+25 more)

### Community 82 - "Sound Manager Impl"
Cohesion: 0.25
Nodes (3): SoundManager, _impl, AudioPlayer::AudioPlayer()

### Community 84 - "MonsterDef"
Cohesion: 0.20
Nodes (10): MonsterDef, damage, fireCooldown, height, hp, projectile, speed, sprite (+2 more)

### Community 85 - "Background::drawAura"
Cohesion: 0.29
Nodes (3): Background::drawAura(), Background::drawDebris(), Background::drawNebula()

### Community 86 - "Connection Send"
Cohesion: 0.32
Nodes (4): Connection::flush(), Connection::send(), Connection::sendEngine(), Connection::takeMessages()

### Community 88 - "SlotTable"
Cohesion: 0.22
Nodes (9): Sound, BossAlert, Coin, Count, Defeat, Explosion, Hit, Shoot (+1 more)

### Community 89 - "DisconnectReason"
Cohesion: 0.14
Nodes (11): Cue, BossAppears, BossExplosion, CoinPickup, Explosion, Hit, PlayerShot, Purchase (+3 more)

### Community 90 - "Connection::readId"
Cohesion: 0.40
Nodes (3): Connection::handlePong(), Connection::nextSequence(), Connection::readId()

### Community 91 - "GameStatus"
Cohesion: 0.09
Nodes (17): GameStatus, bossHp, bossMaxHp, coins, countdown, deaths, elapsedMs, level (+9 more)

### Community 92 - "LaserCycle"
Cohesion: 0.50
Nodes (4): LaserCycle, age, beam, hasBeam

### Community 93 - "Replica::apply"
Cohesion: 0.15
Nodes (10): Prediction::reconcile(), Replica::apply(), BuyResult, id, ok, OwnState, inverted, lastInput (+2 more)

### Community 95 - "PlayerEffects"
Cohesion: 0.50
Nodes (4): PlayerEffects, inverted, lastPortal, speedFactor

### Community 96 - ".operator()"
Cohesion: 0.40
Nodes (5): Install, Keep it up to date: the git hook, Knowledge graph (graphify), Use the graph, With an AI agent (Claude Code)

### Community 97 - "Track"
Cohesion: 0.14
Nodes (13): Track, BossDasher, BossFortress, BossStandard, BossSummoner, Core, Count, DeathRun (+5 more)

### Community 98 - "CLAUDE.md"
Cohesion: 0.15
Nodes (11): isValid(), StatusHeader, bossHp, bossMaxHp, coins, count, deaths, elapsedMs (+3 more)

### Community 99 - "findPlayerBySlot"
Cohesion: 0.11
Nodes (9): Game rules, CoinCounter, quantity, total, coins, value, findPlayerBySlot(), kill() (+1 more)

### Community 100 - "LevelDef"
Cohesion: 0.16
Nodes (33): LevelDef, bosses, deathRun, length, obstacles, waves, wave(), asteroidField() (+25 more)

### Community 102 - "App"
Cohesion: 0.11
Nodes (13): App, _game, _lastInputs, _lastPhase, _leaderboard, LEADERBOARD_FILE, _sentAugments, _sentOffers (+5 more)

### Community 103 - "UdpSocket::Impl"
Cohesion: 0.10
Nodes (12): UdpSocket::Impl::startReceive(), UdpSocket::Impl, buffer, BUFFER_SIZE, io, localPort, MAX_QUEUED, mutex (+4 more)

### Community 105 - "Archetype"
Cohesion: 0.05
Nodes (26): Archetype, _columns, _columnsIndex, _entities, NO_COLUMN, _signature, Column, _data (+18 more)

### Community 106 - "Zone"
Cohesion: 0.08
Nodes (25): Variety by zone, canTakeFormation(), crowded(), scatter(), scatterLasers(), scatterLimit(), shapesOf(), shapeWaves() (+17 more)

### Community 107 - "Audio.cpp"
Cohesion: 0.14
Nodes (9): ShopSound, Accept, Count, Refuse, Audio::Audio(), Audio::playMusic(), Audio::playShopSound(), Audio::playSound() (+1 more)

### Community 110 - "GateSpec"
Cohesion: 0.09
Nodes (26): addGate(), GateSpec, at, gapHeight, gapY, kind, motion, pinch (+18 more)

### Community 111 - "Gameplay"
Cohesion: 0.14
Nodes (14): Bosses, Coins, time and level skip, Controls, Enemies, Gameplay, Goal, Leaderboard, Lobby and maps (+6 more)

### Community 112 - "buildMap"
Cohesion: 0.29
Nodes (4): appendLevel(), buildMap(), buildRun(), shifted()

### Community 113 - "PendingSpawn"
Cohesion: 0.50
Nodes (4): PendingSpawn, delay, kind, y

### Community 114 - "EntityManager"
Cohesion: 0.29
Nodes (3): EntityManager, _freeList, _generations

### Community 115 - "formatTime"
Cohesion: 0.47
Nodes (3): formatTime(), Hud::drawRecords(), twoDigits()

### Community 116 - "bossHazardSystem"
Cohesion: 0.28
Nodes (8): bossHazardSystem(), HazardClock, laser, meteor, time, hazardInterval(), spawnBossLaser(), spawnMeteor()

### Community 118 - "PlayerInput"
Cohesion: 0.09
Nodes (16): Add a player action (a new key), PlayerInput, down, fire, left, right, up, Prediction::advance() (+8 more)

### Community 119 - "ProjectileDef"
Cohesion: 0.25
Nodes (8): ProjectileDef, drop, height, shots, speed, spread, sprite, width

### Community 120 - "TextureManager::Impl"
Cohesion: 0.25
Nodes (3): TextureManager::Impl, ids, textures

### Community 121 - "Streak"
Cohesion: 0.40
Nodes (5): Streak, length, speed, x, y

### Community 122 - "GameView.cpp"
Cohesion: 0.12
Nodes (17): Vector2f, x, y, ScreenRect, position, size, GameView::drawBeamCore(), GameView::drawLaserPost() (+9 more)

### Community 124 - "FixedTimestep"
Cohesion: 0.25
Nodes (3): FixedTimestep, _accumulator, _maxAccumulator

### Community 125 - "TEST"
Cohesion: 0.06
Nodes (9): Maps and lobby, _shopOffers, clearEnemies(), gameAtTheEndOfALevel(), reachShop(), runFor(), shopWithCoins(), TEST() (+1 more)

### Community 126 - "StrikeSpec"
Cohesion: 0.25
Nodes (8): StrikeSpec, active, height, vy, warn, width, x, y

### Community 127 - "ObstacleKind"
Cohesion: 0.12
Nodes (18): ObstacleTag, kind, isPortal(), ObstacleKind, Beam, BeamWarn, Count, FastPortal (+10 more)

### Community 128 - "LeviathanMove"
Cohesion: 0.25
Nodes (8): LeviathanMove, Aim, Columns, Dash, Rest, Return, Sweep, Windup

### Community 130 - "Star"
Cohesion: 0.50
Nodes (4): Star, layer, x, y

### Community 131 - "Behaviours"
Cohesion: 0.22
Nodes (9): Behaviours, Kinds of monsters and bosses, BossTag, age, summonTimer, Strike, deadly, spec (+1 more)

### Community 134 - "TeamId"
Cohesion: 0.67
Nodes (3): TeamId, Monster, Player

### Community 135 - "Case"
Cohesion: 0.33
Nodes (4): Case, name, run, Registrar

### Community 136 - "Hud::drawTimer"
Cohesion: 0.38
Nodes (4): Hud::drawCoins(), Hud::drawLevel(), Hud::drawReady(), Hud::drawTimer()

### Community 137 - "Hud::drawVictoryStats"
Cohesion: 0.33
Nodes (3): count, clock(), Hud::drawVictoryStats()

### Community 138 - "Scene"
Cohesion: 0.13
Nodes (13): Aura, Dasher, Fortress, None, Standard, Summoner, Scene, aura (+5 more)

### Community 140 - "Boss.hpp"
Cohesion: 0.17
Nodes (17): countMinions(), dasherTarget(), DashTarget, pos, velocity, roamBoss(), summonerBoss(), summonMinions() (+9 more)

### Community 141 - "Sprite"
Cohesion: 0.20
Nodes (8): Sprite, animation, texture, Area, box, pos, GameView::drawTiled(), Hud::drawIcon()

### Community 142 - "Resized"
Cohesion: 0.50
Nodes (3): Resized, height, width

### Community 144 - "MonsterState"
Cohesion: 0.50
Nodes (4): MonsterState, age, locked, summoned

### Community 145 - "Regeneration"
Cohesion: 0.50
Nodes (4): Regeneration, hp, interval, timer

### Community 147 - "Shield"
Cohesion: 0.67
Nodes (3): Shield, cooldown, timer

### Community 150 - "How-to"
Cohesion: 0.33
Nodes (6): each(), Add a component, Add a map or a level, Add a new kind of monster, Add a system, How-to

### Community 152 - "TurretFire"
Cohesion: 0.67
Nodes (3): TurretFire, interval, timer

### Community 153 - "ObstacleMotion"
Cohesion: 0.40
Nodes (5): ObstacleMotion, age, amplitude, baseY, omega

## Knowledge Gaps
- **714 isolated node(s):** `_impl`, `voices`, `next`, `music`, `soundVolume` (+709 more)
  These have ≤1 connection - possible missing edges or undocumented components. (Counts symbols only; 1372 node(s) total have ≤1 connection when file, concept and rationale nodes are included.)
- **17 thin communities (<3 nodes) omitted from report** — run `graphify query` to explore isolated nodes.

## Suggested Questions
_Questions this graph is uniquely positioned to answer:_

- **Why does `TEST()` connect `Game Tests` to `Boss.hpp`, `Game`, `Collider`, `Leviathan.hpp`, `TEST`, `Registry`, `Shop.hpp`, `TEST`, `Segments`, `addGates`, `Layers`, `EntitySpawned`, `Obstacles.hpp`, `findPlayerBySlot`, `LevelDef`, `Zone`, `buildMap`, `bossHazardSystem`, `optional`, `TEST`?**
  _High betweenness centrality (0.105) - this node is a cross-community bridge._
- **Are the 77 inferred relationships involving `TEST()` (e.g. with `Registry` and `.alive()`) actually correct?**
  _`TEST()` has 77 INFERRED edges - model-reasoned connections that need verification._
- **What connects `_impl`, `voices`, `next` to the rest of the system?**
  _714 weakly-connected nodes found - possible documentation gaps or missing edges._
- **Should `IntRect` be split into smaller, more focused modules?**
  _Cohesion score 0.14619883040935672 - nodes in this community are weakly interconnected._
- **Why does `Game` connect `Game` to `TEST`, `Registry`, `Components.hpp`, `findPlayerBySlot`, `Phase`, `App`, `WaveStep`, `Shop.hpp`, `Game Tests`, `Obstacles.hpp`, `Collider`, `Segments`, `bossHazardSystem`, `PendingSpawn`, `PlayerInput`, `optional`, `TEST`, `EntitySpawned`?**
  _High betweenness centrality (0.104) - this node is a cross-community bridge._
- **Are the 2 inferred relationships involving `Registry` (e.g. with `TEST()` and `TEST()`) actually correct?**
  _`Registry` has 2 INFERRED edges - model-reasoned connections that need verification._
- **Should `string` be split into smaller, more focused modules?**
  _Cohesion score 0.11578947368421053 - nodes in this community are weakly interconnected._