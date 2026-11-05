# Graph Report - RTYPE  (2026-11-05)

## Corpus Check
- 151 files · ~132,678 words
- Verdict: corpus is large enough that graph structure adds value.
- Unclassified: 5 file(s) not represented in the graph (top: (none) 5)

## Summary
- 2860 nodes · 5956 edges · 148 communities (129 shown, 19 thin omitted)
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
- size_t
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
- Vec2
- Registry
- BackdropId
- Design Choices Docs
- Audio Player
- AugmentId
- Replica Events
- Build Tooling Docs
- Message
- Reliable Channel
- Shop.hpp
- Zone
- Phase
- Network protocol doc
- Architecture Targets
- describeSpawn
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
- AudioPlayer::Impl
- Background
- Message Types
- EntitySpawned
- BossKind
- window/Event.hpp
- describeSpawn
- SpriteId
- .clearSpawnY
- Connection.cpp
- Session Info
- Renderer.cpp
- SpriteBank
- EntityState
- WaveStep
- BinaryReader
- Window Impl Header
- Signature
- TEST
- Column
- stdexcept
- Phase
- Sound Manager Impl
- HudVictory.cpp
- Steps
- Sample
- Connection Send
- createPlayer
- scatter
- DisconnectReason
- RegistryData
- GameStatus
- GameView.cpp
- OwnState
- game/App.cpp
- .updatePlaying
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
- ObstacleStep
- Audio.cpp
- App::run
- vcpkg.json
- GateSpec
- Gameplay
- buildMap
- ComponentId
- EntityManager
- formatTime
- bossHazardSystem
- readAugmentIds
- Hud::drawPlayers
- TextureManager::Impl
- Streak
- Vector2f
- cstddef
- FixedTimestep
- TEST
- duration
- ObstacleKind
- LeviathanMove
- Client::send
- Star
- Behaviours
- CommandBuffer
- Aura
- Prediction.cpp
- Case
- Hud::drawTimer
- Hud::drawLobby
- ShopSound
- Area
- Resized
- Hud::drawVictoryScreen
- Regeneration
- Shield
- R-Type game logic
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
- `Current status` --references--> `LevelDef`  [INFERRED]
  src/client/game/Rtype/doc/README.md → src/client/game/Rtype/include/logic/LevelBuilder.hpp

## Import Cycles
- None detected.

## Communities (148 total, 19 thin omitted)

### Community 0 - "src/App.cpp"
Cohesion: 0.11
Nodes (6): App::App(), App::isReady(), App::listening(), App::playSounds(), App::sendReady(), App::useShop()

### Community 1 - "IntRect"
Cohesion: 0.13
Nodes (15): Animation, firstFrame, frameCount, frameDuration, step, IntRect, height, left (+7 more)

### Community 2 - "string"
Cohesion: 0.15
Nodes (4): Window, _impl, App::loop(), App::run()

### Community 3 - "Components.hpp"
Cohesion: 0.24
Nodes (4): MaxHealth, hp, MultiShot, bullets

### Community 4 - "Background.cpp"
Cohesion: 0.12
Nodes (18): Renderer, Background::drawAura(), Background::drawBand(), Background::drawBars(), Background::drawDebris(), Background::drawFade(), Background::drawNebula(), Background::drawSky() (+10 more)

### Community 5 - "Client Connection"
Cohesion: 0.06
Nodes (15): Client, _attempts, _connection, _events, _handshakeSequence, _lastAttempt, _server, _socket (+7 more)

### Community 6 - "vector"
Cohesion: 0.10
Nodes (5): SoundManager::Impl, buffers, ids, SoundManager::load(), TextureManager::load()

### Community 7 - "Component.hpp"
Cohesion: 0.21
Nodes (3): add(), create(), remove()

### Community 8 - "Delivery and Sessions"
Cohesion: 0.09
Nodes (21): Delivery, Reliable, Unreliable, RefuseReason, Full, Version, Server::admit(), Server::broadcast() (+13 more)

### Community 9 - "Build and Protocol Docs"
Cohesion: 0.16
Nodes (17): engine_audio library, SFML::Audio, engine build root, engine_core library, Handshake and connection, Network module (engine::network), ReliableChannel, UDP wire format (+9 more)

### Community 10 - "Server Events"
Cohesion: 0.06
Nodes (12): Server, _events, _maxClients, _nextClientId, _random, _sessions, _socket, _version (+4 more)

### Community 11 - "ShopPanel.cpp"
Cohesion: 0.13
Nodes (10): ShopPanel::affordable(), ShopPanel::confirm(), ShopPanel::drawPrice(), ShopPanel::handle(), ShopPanel::hint(), ShopPanel::isOpen(), ShopPanel::selection(), ShopPanel::ShopPanel() (+2 more)

### Community 14 - "size_t"
Cohesion: 0.40
Nodes (4): EntityRecord, archetype, row, IterationGuard

### Community 15 - "Connection Channel"
Cohesion: 0.07
Nodes (12): Connection, _channel, _lastPing, _lastReceived, _outbox, _pingId, _pingSentAt, _received (+4 more)

### Community 16 - "Game"
Cohesion: 0.05
Nodes (32): Game, _alive, _augments, _coinCounter, _countdown, _deaths, _effectLevel, _elapsed (+24 more)

### Community 17 - "Address"
Cohesion: 0.11
Nodes (15): Address, ip, port, Datagram, bytes, from, toAddress(), toEndpoint() (+7 more)

### Community 18 - ".updatePlaying"
Cohesion: 0.05
Nodes (18): Replica, _augments, _buyResult, CLOCK_CORRECTION_RATE, _cues, _entities, INTERPOLATION_DELAY, _latestTick (+10 more)

### Community 19 - "Hud"
Cohesion: 0.08
Nodes (21): Hud, AUGMENT_GAP, AUGMENT_PADDING, AUGMENT_TEXT_SCALE, BAR_HEIGHT, BAR_WIDTH, BAR_X, BOSS_BAR_WIDTH (+13 more)

### Community 20 - "Entity State Payload"
Cohesion: 0.14
Nodes (22): BuyResult, id, ok, decodeBuy(), decodeBuyResult(), decodeInput(), decodeOwnState(), decodePause() (+14 more)

### Community 21 - "Prediction"
Cohesion: 0.05
Nodes (29): Add a player action (a new key), Pending, dt, input, sequence, Prediction, _box, _input (+21 more)

### Community 22 - "Binary Writer"
Cohesion: 0.11
Nodes (16): BinaryWriter, _data, MAX_VARINT, BinaryWriter::writeBytes(), BinaryWriter::writeFloat(), BinaryWriter::writeU16(), BinaryWriter::writeU32(), BinaryWriter::writeU8() (+8 more)

### Community 23 - "Packet"
Cohesion: 0.08
Nodes (13): Packet, _messages, Client::checkConnection(), Client::flushConnection(), Client::handleDatagram(), Client::handleHandshake(), Client::handleSession(), Client::latency() (+5 more)

### Community 24 - "Pending Messages"
Cohesion: 0.11
Nodes (8): Pending, message, sentAt, ReliableChannel, _early, _expected, _nextId, _pending

### Community 25 - "Replica"
Cohesion: 0.09
Nodes (17): GameView, ANIMATION_PHASE, _background, _hud, HURT_DURATION, HURT_SHAKE, _hurtUntil, _lastBossHp (+9 more)

### Community 26 - "BinaryReader"
Cohesion: 0.18
Nodes (8): BinaryReader::BinaryReader(), BinaryReader::readBytes(), BinaryReader::readFloat(), BinaryReader::readU16(), BinaryReader::readU32(), BinaryReader::readU8(), BinaryReader::readVarint(), BinaryReader::remaining()

### Community 27 - "DeathRuns.hpp"
Cohesion: 0.17
Nodes (19): addCourse(), addCoursePortal(), checkPattern(), Course, after, gapFrom, gapTo, motion (+11 more)

### Community 28 - "GameView"
Cohesion: 0.09
Nodes (14): App, _assets, _connectedSince, _controls, FRAMERATE, _last, LISTEN_DELAY, _network (+6 more)

### Community 29 - "Collider"
Cohesion: 0.08
Nodes (31): Layers, canHit(), Collider, box, entity, pos, team, isFarOutOfBounds() (+23 more)

### Community 30 - "ShopPanel"
Cohesion: 0.07
Nodes (20): ShopPanel, BORDER, CARD_GAP, CARD_HEIGHT, CARD_TOP, CARD_WIDTH, COIN_GAP, COIN_SIZE (+12 more)

### Community 31 - "Leviathan.hpp"
Cohesion: 0.12
Nodes (32): LeviathanState, move, step, swing, timer, StrikeSpec, active, height (+24 more)

### Community 32 - "Audio Player Impl"
Cohesion: 0.18
Nodes (4): AudioPlayer::play(), AudioPlayer::playMusic(), AudioPlayer::setMusicVolume(), toSfmlVolume()

### Community 33 - "Vec2"
Cohesion: 0.06
Nodes (26): Coding conventions, Position, velocity, speed, direction, fan(), volley(), aimVelocity(), centerOf(), dive(), lockOn() (+18 more)

### Community 34 - "Registry"
Cohesion: 0.05
Nodes (63): Entity, id, hash<Engine::Entity>, eachRow(), has(), Registry, _archetypes, EMPTY_ARCHETYPE (+55 more)

### Community 35 - "BackdropId"
Cohesion: 0.13
Nodes (15): BackdropId, Count, Girders, NebulaBlue, NebulaGreen, NebulaOrange, NebulaPurple, NebulaRed (+7 more)

### Community 36 - "Design Choices Docs"
Cohesion: 0.17
Nodes (16): Accessibility doc, Audio/visual accessibility (health bars, per-slot sprites; palette, contrast proposed), Cognitive accessibility (simple rules, short levels, clear feedback), rtype::config (Config.hpp constants), Motor accessibility (keyboard only done; remap, toggle fire, gamepad proposed), Choice: per-message reliability, Choice: event-based replication with NetId, Choice: sessions with token, ping, 5s timeout (+8 more)

### Community 38 - "AugmentId"
Cohesion: 0.16
Nodes (14): augmentLabel(), rarityColor(), rarityName(), AugmentId, Count, DamageUp, FireRateUp, MaxHpUp (+6 more)

### Community 39 - "Replica Events"
Cohesion: 0.11
Nodes (8): Refused, reason, Unreachable, applyDecoded(), Network, _client, Network::Network(), _open

### Community 40 - "Build Tooling Docs"
Cohesion: 0.12
Nodes (20): Dependency rules, Game::setInput / Game::update(dt), Server App class (only place including both network and game), Server threads: network thread and game thread, Technical and comparative study doc, Choice: AABB collision, Choice: hand-written binary big-endian encoding, Choice: CMake with presets (+12 more)

### Community 41 - "Message"
Cohesion: 0.15
Nodes (9): Connected, client, Disconnected, client, reason, Message, client, payload (+1 more)

### Community 42 - "Reliable Channel"
Cohesion: 0.23
Nodes (5): ReliableChannel::acknowledge(), ReliableChannel::receive(), ReliableChannel::resend(), ReliableChannel::send(), ReliableChannel::takeReady()

### Community 43 - "Shop.hpp"
Cohesion: 0.09
Nodes (24): AugmentDef, apply, id, price, rarity, Rarity, Common, Epic (+16 more)

### Community 44 - "Zone"
Cohesion: 0.12
Nodes (15): Scene, aura, boosted, zone, Zone, Arena, Asteroids, Core (+7 more)

### Community 45 - "Phase"
Cohesion: 0.11
Nodes (11): Audio, _dir, _enabled, _ids, _phase, _player, SHOP_SOUND_COUNT, _shopIds (+3 more)

### Community 46 - "Network protocol doc"
Cohesion: 0.16
Nodes (22): Architecture doc, Build and run doc, CMake presets linux-debug / windows-debug (Ninja on Linux), Build troubleshooting table, vcpkg submodule (external/vcpkg, manifest vcpkg.json, Asio + SFML 3), Contributing doc, Branch prefixes and commit types, Coding conventions (80 cols, 25-line functions, 5 params, 2 nested ifs, no ternary, _ private prefix) (+14 more)

### Community 47 - "Architecture Targets"
Cohesion: 0.14
Nodes (19): r-type_client (display executable), Client replica of the world (entity kind + position), engine::audio (sounds and music, wraps SFML), engine::core (Registry, Scheduler, CommandBuffer, FixedTimestep), engine::network (UDP transport, sessions, ping, reliability, wraps Asio), engine::window (rendering, wraps SFML), Three decoupled layers: rendering, networking, game logic, rtype::common (shared headers: Vec2, return codes) (+11 more)

### Community 48 - "describeSpawn"
Cohesion: 0.24
Nodes (6): collectSpawns(), collectStates(), collectUntracked(), describeSpawn(), isReplicable(), slotOf()

### Community 49 - "TEST"
Cohesion: 0.05
Nodes (6): Leaderboard, _records, RunRecord, players, timeMs, TEST()

### Community 50 - "Segments"
Cohesion: 0.07
Nodes (23): Level skip, Variety by zone, createCoinCounter(), countBefore(), firstLevel(), isDeathRun(), lastSpawnPoint(), levelAt() (+15 more)

### Community 51 - "Key"
Cohesion: 0.10
Nodes (21): Key, A, Backspace, C, D, Down, E, Enter (+13 more)

### Community 52 - "addGates"
Cohesion: 0.14
Nodes (23): addCave(), addGates(), addRun(), addSweepers(), CavePiece, bottom, top, Gap (+15 more)

### Community 53 - "MonsterKind"
Cohesion: 0.05
Nodes (42): each(), Add a component, Add a map or a level, Add a new kind of monster, Add a system, How-to, Kinds of monsters and bosses, BossTag (+34 more)

### Community 54 - "map3.cpp"
Cohesion: 0.54
Nodes (6): gauntlet(), map3(), minefield(), pinch(), rhythm(), run()

### Community 55 - "Layers"
Cohesion: 0.06
Nodes (42): Architecture, Collisions, Components, Data oriented design, Shooting, Systems, The ECS rules to remember, BulletLook (+34 more)

### Community 56 - "SpriteId"
Cohesion: 0.14
Nodes (7): Controls, _held, _input, _pause, _pressed, _respawn, handle()

### Community 57 - "Window.cpp"
Cohesion: 0.17
Nodes (6): convert(), toKey(), translate(), Window::clear(), Window::pollEvent(), Window::Window()

### Community 58 - "SpriteBank"
Cohesion: 0.15
Nodes (9): explosionCue(), Replica::applyPrediction(), Replica::bossSprite(), Replica::localPlayer(), Replica::mapCursor(), Replica::recordSpawnCue(), Replica::refreshSolids(), Replica::slot() (+1 more)

### Community 59 - "SpriteBank.cpp"
Cohesion: 0.13
Nodes (5): SpriteBank::add(), SpriteBank::addBackdrop(), SpriteBank::loadBackdrops(), SpriteBank::loadMisc(), SpriteBank::texture()

### Community 60 - "AudioPlayer::Impl"
Cohesion: 0.12
Nodes (15): AudioPlayer::Impl, music, musicVolume, next, soundVolume, voices, Sound, BossAlert (+7 more)

### Community 61 - "Background"
Cohesion: 0.09
Nodes (17): Background, _aura, _auraLevel, _boosted, _fade, _height, _last, LAYERS (+9 more)

### Community 62 - "Message Types"
Cohesion: 0.22
Nodes (5): Message, id, payload, reliable, type

### Community 63 - "EntitySpawned"
Cohesion: 0.14
Nodes (11): EntityDestroyed, id, EntitySpawned, box, id, pos, slot, sprite (+3 more)

### Community 64 - "BossKind"
Cohesion: 0.20
Nodes (10): BossType, kind, BossKind, Count, Dasher, Fortress, Standard, Summoner (+2 more)

### Community 65 - "window/Event.hpp"
Cohesion: 0.12
Nodes (10): Closed, FocusGained, FocusLost, KeyPressed, key, KeyReleased, key, Controls::applyKey() (+2 more)

### Community 66 - "describeSpawn"
Cohesion: 0.15
Nodes (9): Packet::add(), Packet::isHandshake(), Packet::isValid(), Packet::Packet(), Packet::parseMessage(), Packet::sequence(), Packet::size(), Packet::sizeOf() (+1 more)

### Community 67 - "SpriteId"
Cohesion: 0.06
Nodes (35): isBossSprite(), Renderable, id, SpriteId, AlienShard, BeamOn, BeamWarn, Bomb (+27 more)

### Community 68 - ".clearSpawnY"
Cohesion: 0.16
Nodes (5): What the server reads, PendingSpawn, delay, kind, y

### Community 69 - "Connection.cpp"
Cohesion: 0.13
Nodes (12): Connection::Connection(), Connection::handle(), Connection::handlePong(), Connection::handleUser(), Connection::hasHandshakeMessage(), Connection::nextSequence(), Connection::readId(), Connection::sendId() (+4 more)

### Community 70 - "Session Info"
Cohesion: 0.11
Nodes (15): DisconnectReason, Kicked, Quit, Shutdown, Timeout, Session, address, connection (+7 more)

### Community 71 - "Renderer.cpp"
Cohesion: 0.09
Nodes (19): Color, a, b, g, r, Font, GLYPH_HEIGHT, GLYPH_WIDTH (+11 more)

### Community 72 - "SpriteBank"
Cohesion: 0.10
Nodes (9): TextureManager, _impl, SpriteBank, _assets, _backdrops, _clock, _sprites, Background::Background() (+1 more)

### Community 73 - "EntityState"
Cohesion: 0.24
Nodes (9): EntityState, id, pos, Replica::record(), encodeStateChunk(), encodeStates(), StatesSnapshot, states (+1 more)

### Community 74 - "WaveStep"
Cohesion: 0.10
Nodes (20): fittedBase(), fittedSpacing(), formationDelay(), formationSlot(), Slot, delay, y, canTakeFormation() (+12 more)

### Community 75 - "BinaryReader"
Cohesion: 0.12
Nodes (11): BinaryReader, _cursor, PlayerStatus, alive, hp, ready, slot, decodeLeaderboard() (+3 more)

### Community 77 - "Signature"
Cohesion: 0.30
Nodes (3): Signature, WORD_BITS, _words

### Community 79 - "Column"
Cohesion: 0.18
Nodes (7): Column, _data, _info, ComponentInfo, infoOf(), swapRemove(), ComponentRegistry::getInfo()

### Community 80 - "stdexcept"
Cohesion: 0.14
Nodes (3): main(), parsePort(), parsePort()

### Community 81 - "Phase"
Cohesion: 0.29
Nodes (7): Phase, Defeat, Lobby, Playing, Shop, Victory, Hud::trackPhase()

### Community 82 - "Sound Manager Impl"
Cohesion: 0.25
Nodes (3): SoundManager, _impl, AudioPlayer::AudioPlayer()

### Community 83 - "HudVictory.cpp"
Cohesion: 0.20
Nodes (6): count, clock(), confettiColor(), Hud::drawConfetti(), Hud::drawVictoryStats(), Hud::timeColor()

### Community 84 - "Steps"
Cohesion: 0.15
Nodes (12): 1. Settings model, 2. Apply the settings in the game logic (done), 3. Brutal escort mini-bosses (done), 4. Protocol and server (done), 5. Client: lobby flow, settings screen, lives HUD (done), 6. Leaderboard (done), 7. Documentation, 8. Final code review (+4 more)

### Community 85 - "Sample"
Cohesion: 0.22
Nodes (5): Sample, pos, tick, Replica::interpolate(), Replica::update()

### Community 86 - "Connection Send"
Cohesion: 0.38
Nodes (3): Connection::flush(), Connection::send(), Connection::sendEngine()

### Community 87 - "createPlayer"
Cohesion: 0.33
Nodes (5): CoinCounter, quantity, total, coins, value

### Community 88 - "scatter"
Cohesion: 0.25
Nodes (6): crowded(), scatter(), scatterLasers(), scatterLimit(), shapesOf(), zoneOf()

### Community 89 - "DisconnectReason"
Cohesion: 0.14
Nodes (11): Cue, BossAppears, BossExplosion, CoinPickup, Explosion, Hit, PlayerShot, Purchase (+3 more)

### Community 90 - "RegistryData"
Cohesion: 0.18
Nodes (5): ComponentRegistry::registerType(), RegistryData, ids, infos, mutex

### Community 91 - "GameStatus"
Cohesion: 0.10
Nodes (16): GameStatus, bossHp, bossMaxHp, coins, countdown, deaths, elapsedMs, level (+8 more)

### Community 92 - "GameView.cpp"
Cohesion: 0.18
Nodes (7): GameView::drawLayer(), GameView::drawWorld(), GameView::isBlock(), GameView::isPortalSprite(), GameView::sceneOf(), GameView::tiltFrame(), GameView::trackBossHurt()

### Community 93 - "OwnState"
Cohesion: 0.22
Nodes (8): Prediction::reconcile(), isValidSpawn(), OwnState, inverted, lastInput, pos, speedPercent, phaseByte()

### Community 95 - ".updatePlaying"
Cohesion: 0.16
Nodes (17): The tick: `Game::update`, bossSystem(), isOutOfBounds(), obstacleSystem(), collectMonsters(), collectPlayers(), collectProjectileEntities(), contactSystem() (+9 more)

### Community 96 - ".operator()"
Cohesion: 0.40
Nodes (5): Install, Keep it up to date: the git hook, Knowledge graph (graphify), Use the graph, With an AI agent (Claude Code)

### Community 97 - "Track"
Cohesion: 0.14
Nodes (13): Track, BossDasher, BossFortress, BossStandard, BossSummoner, Core, Count, DeathRun (+5 more)

### Community 98 - "CLAUDE.md"
Cohesion: 0.15
Nodes (13): decodeStatus(), isValid(), readStatusHeader(), StatusHeader, bossHp, bossMaxHp, coins, count (+5 more)

### Community 99 - "findPlayerBySlot"
Cohesion: 0.14
Nodes (7): Game rules, applyAugment(), findAugment(), priceOf(), findPlayerBySlot(), kill(), playerOf()

### Community 100 - "LevelDef"
Cohesion: 0.15
Nodes (34): LevelDef, bosses, deathRun, length, obstacles, waves, wave(), shapeWaves() (+26 more)

### Community 102 - "App"
Cohesion: 0.11
Nodes (13): App, _game, _lastInputs, _lastPhase, _leaderboard, LEADERBOARD_FILE, _sentAugments, _sentOffers (+5 more)

### Community 103 - "UdpSocket::Impl"
Cohesion: 0.10
Nodes (12): UdpSocket::Impl::startReceive(), UdpSocket::Impl, buffer, BUFFER_SIZE, io, localPort, MAX_QUEUED, mutex (+4 more)

### Community 105 - "Archetype"
Cohesion: 0.24
Nodes (6): Archetype, _columns, _columnsIndex, _entities, NO_COLUMN, _signature

### Community 106 - "ObstacleStep"
Cohesion: 0.10
Nodes (27): Obstacles, PlayerEffects, inverted, lastPortal, speedFactor, addObstacleMotion(), createObstacle(), obstacleSprite() (+19 more)

### Community 107 - "Audio.cpp"
Cohesion: 0.13
Nodes (7): Audio::Audio(), Audio::follow(), Audio::onPhase(), Audio::playCue(), Audio::playMusic(), Audio::playSound(), Audio::trackOfZone()

### Community 110 - "GateSpec"
Cohesion: 0.12
Nodes (17): GateSpec, at, gapHeight, gapY, kind, motion, pinch, speed (+9 more)

### Community 111 - "Gameplay"
Cohesion: 0.14
Nodes (14): Bosses, Coins, time and level skip, Controls, Enemies, Gameplay, Goal, Leaderboard, Lobby and maps (+6 more)

### Community 112 - "buildMap"
Cohesion: 0.29
Nodes (4): appendLevel(), buildMap(), buildRun(), shifted()

### Community 113 - "ComponentId"
Cohesion: 0.22
Nodes (5): ComponentId, id, ComponentRegistry, getId(), idOf()

### Community 114 - "EntityManager"
Cohesion: 0.29
Nodes (3): EntityManager, _freeList, _generations

### Community 115 - "formatTime"
Cohesion: 0.47
Nodes (3): formatTime(), Hud::drawRecords(), twoDigits()

### Community 116 - "bossHazardSystem"
Cohesion: 0.28
Nodes (8): bossHazardSystem(), HazardClock, laser, meteor, time, hazardInterval(), spawnBossLaser(), spawnMeteor()

### Community 118 - "readAugmentIds"
Cohesion: 0.36
Nodes (5): decodeAugments(), decodeShop(), encodeAugments(), readAugmentIds(), writeAugmentIds()

### Community 120 - "TextureManager::Impl"
Cohesion: 0.25
Nodes (3): TextureManager::Impl, ids, textures

### Community 121 - "Streak"
Cohesion: 0.40
Nodes (5): Streak, length, speed, x, y

### Community 122 - "Vector2f"
Cohesion: 0.11
Nodes (19): Sprite, animation, texture, Vector2f, x, y, Renderer::draw(), Renderer::drawRect() (+11 more)

### Community 124 - "FixedTimestep"
Cohesion: 0.25
Nodes (3): FixedTimestep, _accumulator, _maxAccumulator

### Community 125 - "TEST"
Cohesion: 0.06
Nodes (9): addToPurse(), _shopOffers, clearEnemies(), gameAtTheEndOfALevel(), reachShop(), runFor(), shopWithCoins(), TEST() (+1 more)

### Community 126 - "duration"
Cohesion: 0.40
Nodes (4): Connection::latency(), Connection::peerDisconnect(), Connection::resendDelay(), duration

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
Cohesion: 0.12
Nodes (16): Behaviours, dasherTarget(), DashTarget, pos, velocity, roamBoss(), MonsterState, age (+8 more)

### Community 133 - "Aura"
Cohesion: 0.29
Nodes (7): Aura, Dasher, Fortress, None, Standard, Summoner, GameView::auraOf()

### Community 135 - "Case"
Cohesion: 0.33
Nodes (4): Case, name, run, Registrar

### Community 136 - "Hud::drawTimer"
Cohesion: 0.38
Nodes (4): Hud::drawCoins(), Hud::drawLevel(), Hud::drawReady(), Hud::drawTimer()

### Community 139 - "ShopSound"
Cohesion: 0.40
Nodes (5): ShopSound, Accept, Count, Refuse, Audio::playShopSound()

### Community 141 - "Area"
Cohesion: 0.40
Nodes (4): Area, box, pos, GameView::drawTiled()

### Community 142 - "Resized"
Cohesion: 0.50
Nodes (3): Resized, height, width

### Community 145 - "Regeneration"
Cohesion: 0.50
Nodes (4): Regeneration, hp, interval, timer

### Community 147 - "Shield"
Cohesion: 0.67
Nodes (3): Shield, cooldown, timer

### Community 150 - "R-Type game logic"
Cohesion: 0.17
Nodes (7): Configuration, Current status, Maps and lobby, Overview, Public API: the `Game` class, R-Type game logic, The first map: 20 levels

### Community 152 - "TurretFire"
Cohesion: 0.67
Nodes (3): TurretFire, interval, timer

### Community 153 - "ObstacleMotion"
Cohesion: 0.40
Nodes (5): ObstacleMotion, age, amplitude, baseY, omega

## Knowledge Gaps
- **714 isolated node(s):** `_impl`, `voices`, `next`, `music`, `soundVolume` (+709 more)
  These have ≤1 connection - possible missing edges or undocumented components. (Counts symbols only; 1383 node(s) total have ≤1 connection when file, concept and rationale nodes are included.)
- **19 thin communities (<3 nodes) omitted from report** — run `graphify query` to explore isolated nodes.

## Suggested Questions
_Questions this graph is uniquely positioned to answer:_

- **Why does `TEST()` connect `Game Tests` to `Behaviours`, `Game`, `R-Type game logic`, `Collider`, `Leviathan.hpp`, `Vec2`, `Registry`, `TEST`, `Segments`, `Layers`, `.clearSpawnY`, `.updatePlaying`, `findPlayerBySlot`, `LevelDef`, `ObstacleStep`, `buildMap`, `bossHazardSystem`, `cstddef`, `TEST`?**
  _High betweenness centrality (0.104) - this node is a cross-community bridge._
- **Are the 77 inferred relationships involving `TEST()` (e.g. with `Registry` and `.alive()`) actually correct?**
  _`TEST()` has 77 INFERRED edges - model-reasoned connections that need verification._
- **What connects `_impl`, `voices`, `next` to the rest of the system?**
  _714 weakly-connected nodes found - possible documentation gaps or missing edges._
- **Should `src/App.cpp` be split into smaller, more focused modules?**
  _Cohesion score 0.10526315789473684 - nodes in this community are weakly interconnected._
- **Why does `Game` connect `Game` to `Registry`, `Components.hpp`, `.clearSpawnY`, `findPlayerBySlot`, `AugmentId`, `App`, `WaveStep`, `Shop.hpp`, `Game Tests`, `describeSpawn`, `Phase`, `Segments`, `bossHazardSystem`, `Prediction`, `R-Type game logic`, `TEST`, `.updatePlaying`?**
  _High betweenness centrality (0.103) - this node is a cross-community bridge._
- **Are the 2 inferred relationships involving `Registry` (e.g. with `TEST()` and `TEST()`) actually correct?**
  _`Registry` has 2 INFERRED edges - model-reasoned connections that need verification._
- **Should `IntRect` be split into smaller, more focused modules?**
  _Cohesion score 0.13333333333333333 - nodes in this community are weakly interconnected._