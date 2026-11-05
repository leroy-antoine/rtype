# Graph Report - RTYPE  (2026-11-05)

## Corpus Check
- 150 files · ~130,714 words
- Verdict: corpus is large enough that graph structure adds value.
- Unclassified: 5 file(s) not represented in the graph (top: (none) 5)

## Summary
- 2806 nodes · 5847 edges · 154 communities (132 shown, 22 thin omitted)
- Extraction: 87% EXTRACTED · 13% INFERRED · 0% AMBIGUOUS · INFERRED: 782 edges (avg confidence: 0.87)
- Token cost: 0 input · 0 output

## Graph Freshness
- Built from commit: `fcf5cdce`
- Run `git rev-parse HEAD` and compare to check if the graph is stale.
- Run `graphify update .` after code changes (no API cost).

## Community Hubs (Navigation)
- src/App.cpp
- IntRect
- string
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
- .getIndex
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
- Replication.hpp
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
- .level
- Window Impl Header
- Signature
- TEST
- Column
- stdexcept
- Phase
- Sound Manager Impl
- HudVictory.cpp
- MonsterDef
- RefuseReason
- Connection Send
- createPlayer
- SlotTable
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
- Hud.cpp
- bossHazardSystem
- PlayerInput
- ProjectileDef
- TextureManager::Impl
- Streak
- Vector2f
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
- Aura
- Prediction.cpp
- Case
- Hud::drawTimer
- Hud::drawVictoryStats
- Scene
- ShopSound
- DashTarget
- Area
- Resized
- Hud::drawVictoryScreen
- MonsterState
- Regeneration
- .operator()
- Shield
- MaxHealth
- R-Type game logic
- MultiShot
- TurretFire
- ObstacleMotion

## God Nodes (most connected - your core abstractions)
1. `TEST()` - 194 edges
2. `Registry` - 164 edges
3. `Entity` - 130 edges
4. `Game` - 126 edges
5. `Replica` - 75 edges
6. `TEST()` - 54 edges
7. `LevelDef` - 49 edges
8. `Hud` - 48 edges
9. `SpriteId` - 48 edges
10. `App` - 44 edges

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

## Communities (154 total, 22 thin omitted)

### Community 0 - "src/App.cpp"
Cohesion: 0.12
Nodes (4): App::App(), App::listening(), App::playSounds(), App::useShop()

### Community 1 - "IntRect"
Cohesion: 0.14
Nodes (15): Animation, firstFrame, frameCount, frameDuration, step, IntRect, height, left (+7 more)

### Community 2 - "string"
Cohesion: 0.14
Nodes (5): Window, _impl, handle(), App::loop(), App::run()

### Community 4 - "Background.cpp"
Cohesion: 0.14
Nodes (16): Renderer, Background::drawAura(), Background::drawBand(), Background::drawBars(), Background::drawDebris(), Background::drawFade(), Background::drawNebula(), Background::drawSky() (+8 more)

### Community 5 - "Client Connection"
Cohesion: 0.06
Nodes (15): Client, _attempts, _connection, _events, _handshakeSequence, _lastAttempt, _server, _socket (+7 more)

### Community 6 - "vector"
Cohesion: 0.10
Nodes (5): SoundManager::Impl, buffers, ids, SoundManager::load(), TextureManager::load()

### Community 7 - "Component.hpp"
Cohesion: 0.26
Nodes (3): add(), create(), remove()

### Community 8 - "Delivery and Sessions"
Cohesion: 0.10
Nodes (18): Delivery, Reliable, Unreliable, Server::admit(), Server::broadcast(), Server::checkSessions(), Server::clientCount(), Server::close() (+10 more)

### Community 9 - "Build and Protocol Docs"
Cohesion: 0.16
Nodes (17): engine_audio library, SFML::Audio, engine build root, engine_core library, Handshake and connection, Network module (engine::network), ReliableChannel, UDP wire format (+9 more)

### Community 10 - "Server Events"
Cohesion: 0.06
Nodes (11): Server, _events, _maxClients, _nextClientId, _random, _sessions, _socket, _version (+3 more)

### Community 11 - "ShopPanel.cpp"
Cohesion: 0.09
Nodes (16): augmentLabel(), rarityColor(), rarityName(), Hud::drawAugments(), Hud::textTop(), ShopPanel::affordable(), ShopPanel::confirm(), ShopPanel::drawCard() (+8 more)

### Community 14 - ".getIndex"
Cohesion: 0.15
Nodes (7): EntityRecord, archetype, row, has(), IterationGuard, remove(), tryGet()

### Community 15 - "Connection Channel"
Cohesion: 0.08
Nodes (10): Connection, _channel, _lastPing, _lastReceived, _outbox, _pingId, _pingSentAt, _received (+2 more)

### Community 16 - "Game"
Cohesion: 0.05
Nodes (33): Maps and lobby, Game, _alive, _augments, _coinCounter, _deaths, _effectLevel, _elapsed (+25 more)

### Community 17 - "Address"
Cohesion: 0.14
Nodes (12): Address, ip, port, toAddress(), toEndpoint(), UdpSocket::Impl::Impl(), UdpSocket::Impl::store(), UdpSocket::port() (+4 more)

### Community 18 - ".updatePlaying"
Cohesion: 0.05
Nodes (21): Replica, _augments, _buyResult, CLOCK_CORRECTION_RATE, _cues, _entities, INTERPOLATION_DELAY, _latestTick (+13 more)

### Community 19 - "Hud"
Cohesion: 0.08
Nodes (20): Hud, AUGMENT_GAP, AUGMENT_PADDING, AUGMENT_TEXT_SCALE, BAR_HEIGHT, BAR_WIDTH, BAR_X, BOSS_BAR_WIDTH (+12 more)

### Community 20 - "Entity State Payload"
Cohesion: 0.17
Nodes (22): decodeAugments(), decodeInput(), decodeLeaderboard(), decodeOwnState(), decodePause(), decodeSelectMap(), decodeSpawn(), decodeStates() (+14 more)

### Community 21 - "Prediction"
Cohesion: 0.07
Nodes (16): Pending, dt, input, sequence, Prediction, _box, _input, MAX_PENDING (+8 more)

### Community 22 - "Binary Writer"
Cohesion: 0.12
Nodes (15): BinaryWriter, _data, MAX_VARINT, BinaryWriter::writeBytes(), BinaryWriter::writeFloat(), BinaryWriter::writeU16(), BinaryWriter::writeU32(), BinaryWriter::writeU8() (+7 more)

### Community 23 - "Packet"
Cohesion: 0.07
Nodes (17): Packet, _messages, Datagram, bytes, from, Client::checkConnection(), Client::flushConnection(), Client::handleDatagram() (+9 more)

### Community 24 - "Pending Messages"
Cohesion: 0.11
Nodes (8): Pending, message, sentAt, ReliableChannel, _early, _expected, _nextId, _pending

### Community 25 - "Replica"
Cohesion: 0.08
Nodes (17): GameView, ANIMATION_PHASE, _background, _hud, HURT_DURATION, HURT_SHAKE, _hurtUntil, _lastBossHp (+9 more)

### Community 26 - "BinaryReader"
Cohesion: 0.12
Nodes (10): BinaryReader, _cursor, BinaryReader::BinaryReader(), BinaryReader::readBytes(), BinaryReader::readFloat(), BinaryReader::readU16(), BinaryReader::readU32(), BinaryReader::readU8() (+2 more)

### Community 27 - "DeathRuns.hpp"
Cohesion: 0.17
Nodes (19): addCourse(), addCoursePortal(), checkPattern(), Course, after, gapFrom, gapTo, motion (+11 more)

### Community 28 - "GameView"
Cohesion: 0.09
Nodes (14): App, _assets, _connectedSince, _controls, FRAMERATE, _last, LISTEN_DELAY, _network (+6 more)

### Community 29 - "Collider"
Cohesion: 0.06
Nodes (46): Coding conventions, applyContact(), canHit(), collectProjectiles(), collectTargets(), Collider, box, entity (+38 more)

### Community 30 - "ShopPanel"
Cohesion: 0.07
Nodes (20): ShopPanel, BORDER, CARD_GAP, CARD_HEIGHT, CARD_TOP, CARD_WIDTH, COIN_GAP, COIN_SIZE (+12 more)

### Community 31 - "Leviathan.hpp"
Cohesion: 0.18
Nodes (18): LeviathanState, move, step, swing, timer, bossStopX(), horizontalVelocity(), leviathanBoss() (+10 more)

### Community 32 - "Audio Player Impl"
Cohesion: 0.18
Nodes (4): AudioPlayer::play(), AudioPlayer::playMusic(), AudioPlayer::setMusicVolume(), toSfmlVolume()

### Community 33 - "TEST"
Cohesion: 0.09
Nodes (6): findAugment(), priceOf(), fan(), volley(), shotVelocities(), TEST()

### Community 34 - "Registry"
Cohesion: 0.06
Nodes (69): Entity, id, eachRow(), Registry, _archetypes, EMPTY_ARCHETYPE, _entities, _iterating (+61 more)

### Community 35 - "BackdropId"
Cohesion: 0.13
Nodes (15): BackdropId, Count, Girders, NebulaBlue, NebulaGreen, NebulaOrange, NebulaPurple, NebulaRed (+7 more)

### Community 36 - "Design Choices Docs"
Cohesion: 0.17
Nodes (16): Accessibility doc, Audio/visual accessibility (health bars, per-slot sprites; palette, contrast proposed), Cognitive accessibility (simple rules, short levels, clear feedback), rtype::config (Config.hpp constants), Motor accessibility (keyboard only done; remap, toggle fire, gamepad proposed), Choice: per-message reliability, Choice: event-based replication with NetId, Choice: sessions with token, ping, 5s timeout (+8 more)

### Community 38 - "AugmentId"
Cohesion: 0.13
Nodes (20): AugmentId, Count, DamageUp, FireRateUp, MaxHpUp, MultiShot, Regeneration, Shield (+12 more)

### Community 39 - "Replica Events"
Cohesion: 0.12
Nodes (6): Unreachable, applyDecoded(), Network, _client, Network::Network(), _open

### Community 40 - "Build Tooling Docs"
Cohesion: 0.12
Nodes (20): Dependency rules, Game::setInput / Game::update(dt), Server App class (only place including both network and game), Server threads: network thread and game thread, Technical and comparative study doc, Choice: AABB collision, Choice: hand-written binary big-endian encoding, Choice: CMake with presets (+12 more)

### Community 41 - "Message"
Cohesion: 0.15
Nodes (9): Connected, client, Disconnected, client, reason, Message, client, payload (+1 more)

### Community 42 - "Reliable Channel"
Cohesion: 0.14
Nodes (9): Connection::latency(), Connection::peerDisconnect(), Connection::resendDelay(), ReliableChannel::acknowledge(), ReliableChannel::receive(), ReliableChannel::resend(), ReliableChannel::send(), ReliableChannel::takeReady() (+1 more)

### Community 43 - "Shop.hpp"
Cohesion: 0.10
Nodes (24): AugmentDef, apply, id, price, rarity, Rarity, Common, Epic (+16 more)

### Community 44 - "Zone"
Cohesion: 0.20
Nodes (10): Zone, Arena, Asteroids, Core, Count, DeepSpace, Nebula, Warp (+2 more)

### Community 45 - "Phase"
Cohesion: 0.11
Nodes (11): Audio, _dir, _enabled, _ids, _phase, _player, SHOP_SOUND_COUNT, _shopIds (+3 more)

### Community 46 - "Network protocol doc"
Cohesion: 0.16
Nodes (22): Architecture doc, Build and run doc, CMake presets linux-debug / windows-debug (Ninja on Linux), Build troubleshooting table, vcpkg submodule (external/vcpkg, manifest vcpkg.json, Asio + SFML 3), Contributing doc, Branch prefixes and commit types, Coding conventions (80 cols, 25-line functions, 5 params, 2 nested ifs, no ternary, _ private prefix) (+14 more)

### Community 47 - "Architecture Targets"
Cohesion: 0.14
Nodes (19): r-type_client (display executable), Client replica of the world (entity kind + position), engine::audio (sounds and music, wraps SFML), engine::core (Registry, Scheduler, CommandBuffer, FixedTimestep), engine::network (UDP transport, sessions, ping, reliability, wraps Asio), engine::window (rendering, wraps SFML), Three decoupled layers: rendering, networking, game logic, rtype::common (shared headers: Vec2, return codes) (+11 more)

### Community 48 - "Replication.hpp"
Cohesion: 0.14
Nodes (9): Add a player action (a new key), Public API: the `Game` class, What the server reads, collectSpawns(), collectStates(), collectUntracked(), describeSpawn(), isReplicable() (+1 more)

### Community 49 - "TEST"
Cohesion: 0.05
Nodes (6): Leaderboard, _records, RunRecord, players, timeMs, TEST()

### Community 50 - "Segments"
Cohesion: 0.10
Nodes (19): createCoinCounter(), firstLevel(), isDeathRun(), lastSpawnPoint(), levelAt(), levelSpan(), levelStart(), levelStartOf() (+11 more)

### Community 51 - "Key"
Cohesion: 0.10
Nodes (21): Key, A, Backspace, C, D, Down, E, Enter (+13 more)

### Community 52 - "addGates"
Cohesion: 0.11
Nodes (30): Obstacles, PlayerEffects, inverted, lastPortal, speedFactor, portal(), wall(), addCave() (+22 more)

### Community 53 - "MonsterKind"
Cohesion: 0.15
Nodes (13): MonsterType, kind, MonsterKind, Basic, Bomber, Count, Elite, Kamikaze (+5 more)

### Community 54 - "map3.cpp"
Cohesion: 0.54
Nodes (6): gauntlet(), map3(), minefield(), pinch(), rhythm(), run()

### Community 55 - "Layers"
Cohesion: 0.05
Nodes (56): Architecture, Collisions, Components, Data oriented design, Layers, Shooting, Systems, The ECS rules to remember (+48 more)

### Community 56 - "SpriteId"
Cohesion: 0.11
Nodes (10): KeyPressed, key, KeyReleased, key, Controls, _input, _pause, _pressed (+2 more)

### Community 57 - "Window.cpp"
Cohesion: 0.15
Nodes (8): FocusGained, FocusLost, convert(), toKey(), translate(), Window::clear(), Window::pollEvent(), Window::Window()

### Community 58 - "SpriteBank"
Cohesion: 0.14
Nodes (10): explosionCue(), Replica::applyPrediction(), Replica::bossSprite(), Replica::localPlayer(), Replica::mapCursor(), Replica::record(), Replica::recordSpawnCue(), Replica::refreshSolids() (+2 more)

### Community 59 - "SpriteBank.cpp"
Cohesion: 0.13
Nodes (5): SpriteBank::add(), SpriteBank::addBackdrop(), SpriteBank::loadBackdrops(), SpriteBank::loadMisc(), SpriteBank::texture()

### Community 60 - "src/App.cpp"
Cohesion: 0.25
Nodes (6): AudioPlayer::Impl, music, musicVolume, next, soundVolume, voices

### Community 61 - "Background"
Cohesion: 0.08
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

### Community 66 - "describeSpawn"
Cohesion: 0.15
Nodes (9): Packet::add(), Packet::isHandshake(), Packet::isValid(), Packet::Packet(), Packet::parseMessage(), Packet::sequence(), Packet::size(), Packet::sizeOf() (+1 more)

### Community 67 - "SpriteId"
Cohesion: 0.06
Nodes (35): isBossSprite(), Renderable, id, SpriteId, AlienShard, BeamOn, BeamWarn, Bomb (+27 more)

### Community 68 - "time_point"
Cohesion: 0.14
Nodes (7): Connection::Connection(), Connection::handlePong(), Connection::nextSequence(), Connection::receive(), Connection::timedOut(), Connection::token(), Connection::update()

### Community 69 - "Connection::readId"
Cohesion: 0.17
Nodes (5): Connection::handle(), Connection::handleUser(), Connection::readId(), Connection::sendPing(), Connection::takeMessages()

### Community 70 - "Session Info"
Cohesion: 0.11
Nodes (15): DisconnectReason, Kicked, Quit, Shutdown, Timeout, Session, address, connection (+7 more)

### Community 71 - "Renderer.cpp"
Cohesion: 0.10
Nodes (18): Color, a, b, g, r, Font, GLYPH_HEIGHT, GLYPH_WIDTH (+10 more)

### Community 72 - "SpriteBank"
Cohesion: 0.10
Nodes (9): TextureManager, _impl, SpriteBank, _assets, _backdrops, _clock, _sprites, Background::Background() (+1 more)

### Community 73 - "EntityState"
Cohesion: 0.19
Nodes (11): EntityState, id, pos, encodeStateChunk(), encodeStates(), InputMessage, input, sequence (+3 more)

### Community 74 - "WaveStep"
Cohesion: 0.11
Nodes (19): fittedBase(), fittedSpacing(), formationDelay(), formationSlot(), Slot, delay, y, Shape (+11 more)

### Community 75 - ".level"
Cohesion: 0.12
Nodes (7): countBefore(), levelEnd(), PlayerStatus, alive, hp, slot, stormInterval()

### Community 77 - "Signature"
Cohesion: 0.24
Nodes (5): each(), Signature, WORD_BITS, _words, Add a system

### Community 79 - "Column"
Cohesion: 0.18
Nodes (7): Column, _data, _info, ComponentInfo, infoOf(), swapRemove(), ComponentRegistry::getInfo()

### Community 80 - "stdexcept"
Cohesion: 0.14
Nodes (3): main(), parsePort(), parsePort()

### Community 81 - "Phase"
Cohesion: 0.16
Nodes (9): collectObstacles(), collectProjectileEntities(), Phase, Defeat, Lobby, Playing, Victory, Hud::trackPhase() (+1 more)

### Community 82 - "Sound Manager Impl"
Cohesion: 0.25
Nodes (3): SoundManager, _impl, AudioPlayer::AudioPlayer()

### Community 83 - "HudVictory.cpp"
Cohesion: 0.40
Nodes (3): confettiColor(), Hud::drawConfetti(), Hud::timeColor()

### Community 84 - "MonsterDef"
Cohesion: 0.17
Nodes (12): Add a new kind of monster, flyStraight(), MonsterDef, damage, fireCooldown, height, hp, projectile (+4 more)

### Community 85 - "RefuseReason"
Cohesion: 0.33
Nodes (6): Refused, reason, RefuseReason, Full, Version, Server::sendRefuse()

### Community 86 - "Connection Send"
Cohesion: 0.38
Nodes (3): Connection::flush(), Connection::send(), Connection::sendEngine()

### Community 87 - "createPlayer"
Cohesion: 0.33
Nodes (5): CoinCounter, quantity, total, coins, value

### Community 88 - "SlotTable"
Cohesion: 0.22
Nodes (9): Sound, BossAlert, Coin, Count, Defeat, Explosion, Hit, Shoot (+1 more)

### Community 89 - "DisconnectReason"
Cohesion: 0.14
Nodes (11): Cue, BossAppears, BossExplosion, CoinPickup, Explosion, Hit, PlayerShot, Purchase (+3 more)

### Community 90 - "RegistryData"
Cohesion: 0.18
Nodes (5): ComponentRegistry::registerType(), RegistryData, ids, infos, mutex

### Community 91 - "GameStatus"
Cohesion: 0.10
Nodes (14): GameStatus, bossHp, bossMaxHp, coins, deaths, elapsedMs, level, map (+6 more)

### Community 92 - "GameView.cpp"
Cohesion: 0.20
Nodes (7): GameView::drawLayer(), GameView::drawWorld(), GameView::GameView(), GameView::isBlock(), GameView::isPortalSprite(), GameView::tiltFrame(), GameView::trackBossHurt()

### Community 93 - "OwnState"
Cohesion: 0.22
Nodes (8): encodeKeys(), isValidSpawn(), OwnState, inverted, lastInput, pos, speedPercent, phaseByte()

### Community 95 - ".updatePlaying"
Cohesion: 0.14
Nodes (15): Position, velocity, speed, direction, The tick: `Game::update`, bossSystem(), explosionSize(), calculatePos(), inputDirection(), obstacleMotionSystem(), contactSystem() (+7 more)

### Community 96 - ".operator()"
Cohesion: 0.40
Nodes (5): Install, Keep it up to date: the git hook, Knowledge graph (graphify), Use the graph, With an AI agent (Claude Code)

### Community 97 - "Track"
Cohesion: 0.14
Nodes (13): Track, BossDasher, BossFortress, BossStandard, BossSummoner, Core, Count, DeathRun (+5 more)

### Community 98 - "CLAUDE.md"
Cohesion: 0.18
Nodes (10): StatusHeader, bossHp, bossMaxHp, coins, count, deaths, elapsedMs, level (+2 more)

### Community 99 - "findPlayerBySlot"
Cohesion: 0.18
Nodes (4): Game rules, findPlayerBySlot(), kill(), playerOf()

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
Cohesion: 0.22
Nodes (6): Archetype, _columns, _columnsIndex, _entities, NO_COLUMN, _signature

### Community 106 - "ObstacleStep"
Cohesion: 0.11
Nodes (20): Variety by zone, asteroid(), canTakeFormation(), crowded(), placeScattered(), scatter(), scatterLasers(), scatterLimit() (+12 more)

### Community 107 - "Audio.cpp"
Cohesion: 0.13
Nodes (7): Audio::Audio(), Audio::follow(), Audio::onPhase(), Audio::playCue(), Audio::playMusic(), Audio::playSound(), Audio::trackOfZone()

### Community 110 - "GateSpec"
Cohesion: 0.10
Nodes (20): addGate(), GateSpec, at, gapHeight, gapY, kind, motion, pinch (+12 more)

### Community 111 - "Gameplay"
Cohesion: 0.14
Nodes (14): Bosses, Coins, time and level skip, Controls, Enemies, Gameplay, Goal, Leaderboard, Lobby and maps (+6 more)

### Community 112 - "buildMap"
Cohesion: 0.29
Nodes (4): appendLevel(), buildMap(), buildRun(), shifted()

### Community 113 - "ComponentId"
Cohesion: 0.25
Nodes (5): ComponentId, id, ComponentRegistry, getId(), idOf()

### Community 114 - "EntityManager"
Cohesion: 0.29
Nodes (3): EntityManager, _freeList, _generations

### Community 115 - "Hud.cpp"
Cohesion: 0.14
Nodes (9): formatTime(), Hud::drawLobby(), Hud::drawMapList(), Hud::drawMenuHint(), Hud::drawPauseMenu(), Hud::drawRecords(), Hud::drawRespawnHint(), Hud::Hud() (+1 more)

### Community 116 - "bossHazardSystem"
Cohesion: 0.22
Nodes (8): bossHazardSystem(), HazardClock, laser, meteor, time, hazardInterval(), spawnBossLaser(), spawnMeteor()

### Community 118 - "PlayerInput"
Cohesion: 0.18
Nodes (8): PlayerInput, down, fire, left, right, up, Prediction::record(), Replica::predict()

### Community 119 - "ProjectileDef"
Cohesion: 0.25
Nodes (8): ProjectileDef, drop, height, shots, speed, spread, sprite, width

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
Cohesion: 0.08
Nodes (4): _shopOffers, findFreeSlot(), shopWithCoins(), TEST()

### Community 126 - "StrikeSpec"
Cohesion: 0.25
Nodes (8): StrikeSpec, active, height, vy, warn, width, x, y

### Community 127 - "ObstacleKind"
Cohesion: 0.11
Nodes (19): ObstacleTag, kind, isPortal(), ObstacleKind, Beam, BeamWarn, Count, FastPortal (+11 more)

### Community 128 - "LeviathanMove"
Cohesion: 0.25
Nodes (8): LeviathanMove, Aim, Columns, Dash, Rest, Return, Sweep, Windup

### Community 130 - "Star"
Cohesion: 0.50
Nodes (4): Star, layer, x, y

### Community 131 - "Behaviours"
Cohesion: 0.22
Nodes (9): Behaviours, Kinds of monsters and bosses, BossTag, age, summonTimer, Strike, deadly, spec (+1 more)

### Community 133 - "Aura"
Cohesion: 0.29
Nodes (7): Aura, Dasher, Fortress, None, Standard, Summoner, GameView::auraOf()

### Community 135 - "Case"
Cohesion: 0.33
Nodes (4): Case, name, run, Registrar

### Community 136 - "Hud::drawTimer"
Cohesion: 0.47
Nodes (3): Hud::drawCoins(), Hud::drawLevel(), Hud::drawTimer()

### Community 137 - "Hud::drawVictoryStats"
Cohesion: 0.33
Nodes (3): count, clock(), Hud::drawVictoryStats()

### Community 138 - "Scene"
Cohesion: 0.25
Nodes (6): Scene, aura, boosted, zone, Background::follow(), GameView::sceneOf()

### Community 139 - "ShopSound"
Cohesion: 0.40
Nodes (5): ShopSound, Accept, Count, Refuse, Audio::playShopSound()

### Community 140 - "DashTarget"
Cohesion: 0.40
Nodes (5): dasherTarget(), DashTarget, pos, velocity, roamBoss()

### Community 141 - "Area"
Cohesion: 0.40
Nodes (4): Area, box, pos, GameView::drawTiled()

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

### Community 150 - "R-Type game logic"
Cohesion: 0.25
Nodes (8): Add a component, Add a map or a level, Configuration, Current status, How-to, Overview, R-Type game logic, The first map: 20 levels

### Community 152 - "TurretFire"
Cohesion: 0.67
Nodes (3): TurretFire, interval, timer

### Community 153 - "ObstacleMotion"
Cohesion: 0.40
Nodes (5): ObstacleMotion, age, amplitude, baseY, omega

## Knowledge Gaps
- **706 isolated node(s):** `_impl`, `voices`, `next`, `music`, `soundVolume` (+701 more)
  These have ≤1 connection - possible missing edges or undocumented components. (Counts symbols only; 1351 node(s) total have ≤1 connection when file, concept and rationale nodes are included.)
- **22 thin communities (<3 nodes) omitted from report** — run `graphify query` to explore isolated nodes.

## Suggested Questions
_Questions this graph is uniquely positioned to answer:_

- **Why does `Game` connect `Game` to `Registry`, `Components.hpp`, `findPlayerBySlot`, `AugmentId`, `App`, `WaveStep`, `.level`, `Shop.hpp`, `Game Tests`, `Replication.hpp`, `Phase`, `Segments`, `Collider`, `bossHazardSystem`, `TEST`, `.updatePlaying`?**
  _High betweenness centrality (0.111) - this node is a cross-community bridge._
- **Are the 75 inferred relationships involving `TEST()` (e.g. with `Registry` and `.alive()`) actually correct?**
  _`TEST()` has 75 INFERRED edges - model-reasoned connections that need verification._
- **What connects `_impl`, `voices`, `next` to the rest of the system?**
  _706 weakly-connected nodes found - possible documentation gaps or missing edges._
- **Should `src/App.cpp` be split into smaller, more focused modules?**
  _Cohesion score 0.125 - nodes in this community are weakly interconnected._
- **Why does `TEST()` connect `Game Tests` to `Components.hpp`, `.getIndex`, `Game`, `Collider`, `Leviathan.hpp`, `TEST`, `Registry`, `Replication.hpp`, `TEST`, `Segments`, `addGates`, `Layers`, `.level`, `Phase`, `.updatePlaying`, `findPlayerBySlot`, `LevelDef`, `buildMap`, `bossHazardSystem`, `TEST`?**
  _High betweenness centrality (0.108) - this node is a cross-community bridge._
- **Are the 2 inferred relationships involving `Registry` (e.g. with `TEST()` and `TEST()`) actually correct?**
  _`Registry` has 2 INFERRED edges - model-reasoned connections that need verification._
- **Should `IntRect` be split into smaller, more focused modules?**
  _Cohesion score 0.14210526315789473 - nodes in this community are weakly interconnected._