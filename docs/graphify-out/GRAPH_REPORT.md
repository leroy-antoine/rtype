# Graph Report - RTYPE  (2026-11-07)

## Corpus Check
- 204 files · ~163,624 words
- Verdict: corpus is large enough that graph structure adds value.
- Unclassified: 5 file(s) not represented in the graph (top: (none) 5)

## Summary
- 3028 nodes · 6286 edges · 160 communities (134 shown, 26 thin omitted)
- Extraction: 86% EXTRACTED · 14% INFERRED · 0% AMBIGUOUS · INFERRED: 897 edges (avg confidence: 0.87)
- Token cost: 0 input · 0 output

## Graph Freshness
- Built from commit: `4251cbc3`
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
- Vec2
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
- Systems.hpp
- Registry
- BackdropId
- Design Choices Docs
- Audio Player
- Rarity
- Replica Events
- Build Tooling Docs
- Replica::apply
- Reliable Channel
- Shop.hpp
- Zone
- Phase
- Network protocol doc
- Architecture Targets
- HudLobby.cpp
- TEST
- Segments
- Key
- addGates
- MonsterKind
- map3
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
- Connection::receive
- Connection.cpp
- DisconnectReason
- Renderer.cpp
- SpriteBank
- EntityState
- WaveStep
- PlayerStatus
- Window Impl Header
- LobbyMenu
- TEST
- Phase
- stdexcept
- Obstacles.hpp
- Sound Manager Impl
- HudVictory.cpp
- MonsterDef
- TEST
- Connection Send
- UdpSocket
- SlotTable
- DisconnectReason
- .getIndex
- GameStatus
- Create your own map
- Sample
- game/App.cpp
- Zone
- .operator()
- Track
- CLAUDE.md
- findPlayerBySlot
- LevelDef
- Renderer.cpp
- App
- UdpSocket::Impl
- map1
- Archetype
- ObstacleStep
- Audio.cpp
- App::run
- vcpkg.json
- EntityManager
- Gameplay
- ComponentId
- Custom maps: for developers
- Client::send
- optional
- bossHazardSystem
- Hud.cpp
- ProjectileDef
- TextureManager::Impl
- Streak
- GameView.cpp
- Hud::drawPlayers
- FixedTimestep
- TEST
- StrikeSpec
- Column
- LeviathanMove
- LaserCycle
- Signature
- Hud::drawScoresStep
- CommandBuffer
- Aura
- Prediction.cpp
- Case
- Star
- Hud::drawVictoryStats
- Message
- Hud::drawMapStep
- TeamId
- Sprite
- Resized
- TurretFire
- MonsterState
- Regeneration
- convert
- Shield
- App::useCustomStep
- LobbyStep
- CoinCounter
- string
- RefuseReason
- ObstacleMotion
- .startMap
- .Archetype

## God Nodes (most connected - your core abstractions)
1. `TEST()` - 197 edges
2. `Registry` - 164 edges
3. `Game` - 138 edges
4. `Entity` - 130 edges
5. `Replica` - 83 edges
6. `Hud` - 71 edges
7. `TEST()` - 59 edges
8. `TEST()` - 54 edges
9. `LevelDef` - 50 edges
10. `SpriteId` - 48 edges

## Surprising Connections (you probably didn't know these)
- `Overview` --references--> `Segments`  [INFERRED]
  docs/custom-maps-internals.md → src/client/game/Rtype/include/logic/Levels.hpp
- `Overview` --references--> `map1()`  [INFERRED]
  docs/custom-maps-internals.md → src/client/game/Rtype/src/maps/map1.cpp
- `Who knows whom` --references--> `Leaderboard`  [INFERRED]
  docs/custom-maps-internals.md → src/client/game/Rtype/include/logic/Leaderboard.hpp
- `Who knows whom` --references--> `LevelDef`  [INFERRED]
  docs/custom-maps-internals.md → src/client/game/Rtype/include/logic/LevelBuilder.hpp
- `Client side` --references--> `Replica`  [INFERRED]
  docs/custom-maps-internals.md → src/client/game/Rtype/include/client/Replica.hpp

## Import Cycles
- None detected.

## Communities (160 total, 26 thin omitted)

### Community 0 - "src/App.cpp"
Cohesion: 0.10
Nodes (7): App::App(), App::isReady(), App::listening(), App::playSounds(), App::pressed(), App::sendReady(), App::useShop()

### Community 1 - "IntRect"
Cohesion: 0.15
Nodes (14): Animation, firstFrame, frameCount, frameDuration, step, IntRect, height, left (+6 more)

### Community 2 - "string"
Cohesion: 0.11
Nodes (6): Window, _impl, handle(), App::handleBack(), App::loop(), App::run()

### Community 3 - "Components.hpp"
Cohesion: 0.15
Nodes (11): MaxHealth, hp, MonsterTag, slot, MultiShot, bullets, ObstacleTag, kind (+3 more)

### Community 4 - "Background.cpp"
Cohesion: 0.12
Nodes (18): Renderer, Background::drawAura(), Background::drawBand(), Background::drawBars(), Background::drawDebris(), Background::drawFade(), Background::drawNebula(), Background::drawSky() (+10 more)

### Community 5 - "Client Connection"
Cohesion: 0.06
Nodes (15): Client, _attempts, _connection, _events, _handshakeSequence, _lastAttempt, _server, _socket (+7 more)

### Community 6 - "vector"
Cohesion: 0.12
Nodes (5): SoundManager::Impl, buffers, ids, SoundManager::load(), TextureManager::load()

### Community 7 - "Component.hpp"
Cohesion: 0.16
Nodes (4): add(), create(), remove(), hash<Engine::Entity>

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
Cohesion: 0.13
Nodes (10): ShopPanel::affordable(), ShopPanel::confirm(), ShopPanel::drawPrice(), ShopPanel::handle(), ShopPanel::hint(), ShopPanel::isOpen(), ShopPanel::selection(), ShopPanel::ShopPanel() (+2 more)

### Community 14 - "Vec2"
Cohesion: 0.13
Nodes (20): Position, velocity, speed, direction, fan(), volley(), aimVelocity(), centerOf(), dive(), lockOn(), calculatePos() (+12 more)

### Community 15 - "Connection Channel"
Cohesion: 0.07
Nodes (12): Connection, _channel, _lastPing, _lastReceived, _outbox, _pingId, _pingSentAt, _received (+4 more)

### Community 16 - "Game"
Cohesion: 0.05
Nodes (31): Game, _alive, _augments, _coinCounter, _countdown, _deaths, _effectLevel, _elapsed (+23 more)

### Community 17 - "Address"
Cohesion: 0.14
Nodes (12): Address, ip, port, toAddress(), toEndpoint(), UdpSocket::Impl::Impl(), UdpSocket::Impl::store(), UdpSocket::port() (+4 more)

### Community 18 - ".updatePlaying"
Cohesion: 0.06
Nodes (16): Replica, _augments, _buyResult, CLOCK_CORRECTION_RATE, _cues, _entities, INTERPOLATION_DELAY, _latestTick (+8 more)

### Community 19 - "Hud"
Cohesion: 0.06
Nodes (24): Hud, AUGMENT_GAP, AUGMENT_PADDING, AUGMENT_TEXT_SCALE, BAR_HEIGHT, BAR_WIDTH, BAR_X, BOSS_BAR_WIDTH (+16 more)

### Community 20 - "Entity State Payload"
Cohesion: 0.10
Nodes (36): BinaryReader, _cursor, decodeAugments(), decodeBuy(), decodeBuyResult(), decodeDestroy(), decodeInput(), decodeLeaderboard() (+28 more)

### Community 21 - "Prediction"
Cohesion: 0.05
Nodes (24): Add a player action (a new key), Pending, dt, input, sequence, Prediction, _box, _input (+16 more)

### Community 22 - "Binary Writer"
Cohesion: 0.10
Nodes (17): BinaryWriter, _data, MAX_VARINT, BinaryWriter::writeBytes(), BinaryWriter::writeFloat(), BinaryWriter::writeU16(), BinaryWriter::writeU32(), BinaryWriter::writeU8() (+9 more)

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
Cohesion: 0.18
Nodes (8): BinaryReader::BinaryReader(), BinaryReader::readBytes(), BinaryReader::readFloat(), BinaryReader::readU16(), BinaryReader::readU32(), BinaryReader::readU8(), BinaryReader::readVarint(), BinaryReader::remaining()

### Community 27 - "DeathRuns.hpp"
Cohesion: 0.16
Nodes (21): addCourse(), addCoursePortal(), checkPattern(), Course, after, gapFrom, gapTo, motion (+13 more)

### Community 28 - "GameView"
Cohesion: 0.09
Nodes (16): App, _assets, _connectedSince, _controls, _custom, FRAMERATE, _last, LISTEN_DELAY (+8 more)

### Community 29 - "Collider"
Cohesion: 0.12
Nodes (25): Layers, bulletStartPos(), isShooter(), applyContact(), canHit(), collectProjectiles(), collectTargets(), Collider (+17 more)

### Community 30 - "ShopPanel"
Cohesion: 0.07
Nodes (20): ShopPanel, BORDER, CARD_GAP, CARD_HEIGHT, CARD_TOP, CARD_WIDTH, COIN_GAP, COIN_SIZE (+12 more)

### Community 31 - "Leviathan.hpp"
Cohesion: 0.14
Nodes (20): LeviathanState, move, step, swing, timer, bossStopX(), horizontalVelocity(), leviathanMotion() (+12 more)

### Community 32 - "Audio Player Impl"
Cohesion: 0.18
Nodes (4): AudioPlayer::play(), AudioPlayer::playMusic(), AudioPlayer::setMusicVolume(), toSfmlVolume()

### Community 33 - "Systems.hpp"
Cohesion: 0.09
Nodes (9): isReadyToShoot(), lifetimeSystem(), movementSystem(), regenerationSystem(), shoot(), shootingSystem(), updateShieldTimers(), wantsToShoot() (+1 more)

### Community 34 - "Registry"
Cohesion: 0.06
Nodes (67): Entity, id, eachRow(), Registry, _archetypes, EMPTY_ARCHETYPE, _entities, _iterating (+59 more)

### Community 35 - "BackdropId"
Cohesion: 0.13
Nodes (15): BackdropId, Count, Girders, NebulaBlue, NebulaGreen, NebulaOrange, NebulaPurple, NebulaRed (+7 more)

### Community 36 - "Design Choices Docs"
Cohesion: 0.17
Nodes (17): Accessibility doc, Audio/visual accessibility (health bars, per-slot sprites; palette, contrast proposed), Cognitive accessibility (simple rules, short levels, clear feedback), rtype::config (Config.hpp constants), Motor accessibility (keyboard only done; remap, toggle fire, gamepad proposed), Choice: per-message reliability, Choice: event-based replication with NetId, Choice: sessions with token, ping, 5s timeout (+9 more)

### Community 38 - "Rarity"
Cohesion: 0.14
Nodes (13): augmentLabel(), rarityColor(), rarityName(), Rarity, Common, Epic, Legendary, Rare (+5 more)

### Community 39 - "Replica Events"
Cohesion: 0.12
Nodes (6): Unreachable, applyDecoded(), Network, _client, Network::Network(), _open

### Community 40 - "Build Tooling Docs"
Cohesion: 0.11
Nodes (21): Dependency rules, Game::setInput / Game::update(dt), Server App class (only place including both network and game), Server threads: network thread and game thread, vcpkg submodule (external/vcpkg, manifest vcpkg.json, Asio + SFML 3), Technical and comparative study doc, Choice: AABB collision, Choice: hand-written binary big-endian encoding (+13 more)

### Community 41 - "Replica::apply"
Cohesion: 0.15
Nodes (10): Prediction::reconcile(), Replica::apply(), BuyResult, id, ok, OwnState, inverted, lastInput (+2 more)

### Community 42 - "Reliable Channel"
Cohesion: 0.23
Nodes (5): ReliableChannel::acknowledge(), ReliableChannel::receive(), ReliableChannel::resend(), ReliableChannel::send(), ReliableChannel::takeReady()

### Community 43 - "Shop.hpp"
Cohesion: 0.09
Nodes (27): applyAugment(), AugmentDef, apply, id, price, rarity, AugmentId, Count (+19 more)

### Community 44 - "Zone"
Cohesion: 0.12
Nodes (17): Variety by zone, asteroid(), canTakeFormation(), crowded(), placeScattered(), scatter(), scatterLasers(), scatterLimit() (+9 more)

### Community 45 - "Phase"
Cohesion: 0.10
Nodes (12): Audio, _dir, _enabled, _ids, _phase, _player, SHOP_SOUND_COUNT, _shopIds (+4 more)

### Community 46 - "Network protocol doc"
Cohesion: 0.17
Nodes (21): Architecture doc, Build and run doc, CMake presets linux-debug / windows-debug (Ninja on Linux), Build troubleshooting table, Contributing doc, Branch prefixes and commit types, Coding conventions (80 cols, 25-line functions, 5 params, 2 nested ifs, no ternary, _ private prefix), Part ownership (engine/core, server, client, game logic each have an owner) (+13 more)

### Community 47 - "Architecture Targets"
Cohesion: 0.15
Nodes (18): r-type_client (display executable), Client replica of the world (entity kind + position), engine::audio (sounds and music, wraps SFML), engine::core (Registry, Scheduler, CommandBuffer, FixedTimestep), engine::network (UDP transport, sessions, ping, reliability, wraps Asio), engine::window (rendering, wraps SFML), Three decoupled layers: rendering, networking, game logic, rtype::common (shared headers: Vec2, return codes) (+10 more)

### Community 48 - "HudLobby.cpp"
Cohesion: 0.10
Nodes (11): firstVisible(), Hud::drawCustomDetail(), Hud::drawCustomList(), Hud::drawCustomRow(), Hud::drawCustomScores(), Hud::drawCustomStep(), Hud::drawLines(), Hud::drawNoCustomScores() (+3 more)

### Community 49 - "TEST"
Cohesion: 0.05
Nodes (6): Leaderboard, _records, RunRecord, players, timeMs, TEST()

### Community 50 - "Segments"
Cohesion: 0.10
Nodes (18): firstLevel(), isDeathRun(), lastSpawnPoint(), levelAt(), levelEnd(), levelSpan(), levelStart(), levelStartOf() (+10 more)

### Community 51 - "Key"
Cohesion: 0.10
Nodes (21): Key, A, Backspace, C, D, Down, E, Enter (+13 more)

### Community 52 - "addGates"
Cohesion: 0.11
Nodes (28): addCave(), addGates(), addRun(), addSweepers(), CavePiece, bottom, top, Gap (+20 more)

### Community 53 - "MonsterKind"
Cohesion: 0.12
Nodes (17): MonsterType, kind, PendingSpawn, delay, kind, y, MonsterKind, Basic (+9 more)

### Community 54 - "map3"
Cohesion: 0.17
Nodes (11): Overview, appendLevel(), buildMap(), buildRun(), map2(), gauntlet(), map3(), minefield() (+3 more)

### Community 55 - "Layers"
Cohesion: 0.08
Nodes (35): Architecture, Coding conventions, Collisions, Components, Configuration, Current status, Data oriented design, Overview (+27 more)

### Community 56 - "SpriteId"
Cohesion: 0.15
Nodes (6): Controls, _held, _input, _pause, _pressed, _respawn

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
Cohesion: 0.09
Nodes (19): Public API: the `Game` class, What the server reads, NetId, value, EntityDestroyed, id, EntitySpawned, box (+11 more)

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

### Community 69 - "Connection.cpp"
Cohesion: 0.14
Nodes (13): Connection::Connection(), Connection::handlePong(), Connection::handleUser(), Connection::latency(), Connection::nextSequence(), Connection::peerDisconnect(), Connection::readId(), Connection::resendDelay() (+5 more)

### Community 70 - "DisconnectReason"
Cohesion: 0.11
Nodes (15): DisconnectReason, Kicked, Quit, Shutdown, Timeout, Session, address, connection (+7 more)

### Community 71 - "Renderer.cpp"
Cohesion: 0.10
Nodes (18): Color, a, b, g, r, Font, GLYPH_HEIGHT, GLYPH_WIDTH (+10 more)

### Community 72 - "SpriteBank"
Cohesion: 0.10
Nodes (9): TextureManager, _impl, SpriteBank, _assets, _backdrops, _clock, _sprites, Background::Background() (+1 more)

### Community 73 - "EntityState"
Cohesion: 0.22
Nodes (10): EntityState, id, pos, encodeShop(), encodeStateChunk(), encodeStates(), StatesSnapshot, states (+2 more)

### Community 74 - "WaveStep"
Cohesion: 0.13
Nodes (15): fittedBase(), fittedSpacing(), formationDelay(), formationSlot(), Slot, delay, y, shifted() (+7 more)

### Community 75 - "PlayerStatus"
Cohesion: 0.33
Nodes (6): PlayerStatus, alive, hp, ready, slot, Hud::drawReady()

### Community 77 - "LobbyMenu"
Cohesion: 0.11
Nodes (12): Client side, LobbyMenu, _draft, _forCustom, _scoresFromCustom, _shownRank, SettingsRow, Difficulty (+4 more)

### Community 79 - "Phase"
Cohesion: 0.17
Nodes (9): Phase, Defeat, Lobby, Playing, Shop, Victory, Audio::onPhase(), Audio::playCue() (+1 more)

### Community 80 - "stdexcept"
Cohesion: 0.12
Nodes (3): main(), parsePort(), parsePort()

### Community 81 - "Obstacles.hpp"
Cohesion: 0.06
Nodes (54): Obstacles, The tick: `Game::update`, isFarOutOfBounds(), addToPurse(), coinPickupSystem(), PlayerEffects, inverted, lastPortal (+46 more)

### Community 82 - "Sound Manager Impl"
Cohesion: 0.25
Nodes (3): SoundManager, _impl, AudioPlayer::AudioPlayer()

### Community 83 - "HudVictory.cpp"
Cohesion: 0.40
Nodes (3): confettiColor(), Hud::drawConfetti(), Hud::timeColor()

### Community 84 - "MonsterDef"
Cohesion: 0.12
Nodes (17): each(), Add a component, Add a map or a level, Add a new kind of monster, Add a system, How-to, flyStraight(), MonsterDef (+9 more)

### Community 85 - "TEST"
Cohesion: 0.11
Nodes (3): folder(), TEST(), write()

### Community 86 - "Connection Send"
Cohesion: 0.38
Nodes (3): Connection::flush(), Connection::send(), Connection::sendEngine()

### Community 88 - "SlotTable"
Cohesion: 0.22
Nodes (9): Sound, BossAlert, Coin, Count, Defeat, Explosion, Hit, Shoot (+1 more)

### Community 89 - "DisconnectReason"
Cohesion: 0.14
Nodes (11): Cue, BossAppears, BossExplosion, CoinPickup, Explosion, Hit, PlayerShot, Purchase (+3 more)

### Community 90 - ".getIndex"
Cohesion: 0.15
Nodes (7): EntityRecord, archetype, row, has(), IterationGuard, remove(), tryGet()

### Community 91 - "GameStatus"
Cohesion: 0.10
Nodes (16): GameStatus, bossHp, bossMaxHp, coins, countdown, deaths, elapsedMs, level (+8 more)

### Community 92 - "Create your own map"
Cohesion: 0.12
Nodes (16): A complete example, Bosses, Create your own map, Distances and time, How a map is built, Limits, Play it, Scenery and death runs (+8 more)

### Community 93 - "Sample"
Cohesion: 0.22
Nodes (5): Sample, pos, tick, Replica::interpolate(), Replica::update()

### Community 95 - "Zone"
Cohesion: 0.12
Nodes (15): Scene, aura, boosted, zone, Zone, Arena, Asteroids, Core (+7 more)

### Community 96 - ".operator()"
Cohesion: 0.40
Nodes (5): Install, Keep it up to date: the git hook, Knowledge graph (graphify), Use the graph, With an AI agent (Claude Code)

### Community 97 - "Track"
Cohesion: 0.14
Nodes (13): Track, BossDasher, BossFortress, BossStandard, BossSummoner, Core, Count, DeathRun (+5 more)

### Community 98 - "CLAUDE.md"
Cohesion: 0.15
Nodes (11): phaseByte(), StatusHeader, bossHp, bossMaxHp, coins, count, deaths, elapsedMs (+3 more)

### Community 100 - "LevelDef"
Cohesion: 0.16
Nodes (33): LevelDef, bosses, deathRun, length, obstacles, waves, wave(), asteroidField() (+25 more)

### Community 101 - "Renderer.cpp"
Cohesion: 0.29
Nodes (4): Renderer::draw(), Renderer::drawRect(), Renderer::drawResized(), Renderer::Renderer()

### Community 102 - "App"
Cohesion: 0.11
Nodes (13): App, _game, _lastInputs, _lastPhase, _leaderboard, LEADERBOARD_FILE, _sentAugments, _sentOffers (+5 more)

### Community 103 - "UdpSocket::Impl"
Cohesion: 0.10
Nodes (12): UdpSocket::Impl::startReceive(), UdpSocket::Impl, buffer, BUFFER_SIZE, io, localPort, MAX_QUEUED, mutex (+4 more)

### Community 105 - "Archetype"
Cohesion: 0.26
Nodes (6): Archetype, _columns, _columnsIndex, _entities, NO_COLUMN, _signature

### Community 106 - "ObstacleStep"
Cohesion: 0.09
Nodes (24): addGate(), GateSpec, at, gapHeight, gapY, kind, motion, pinch (+16 more)

### Community 107 - "Audio.cpp"
Cohesion: 0.14
Nodes (9): ShopSound, Accept, Count, Refuse, Audio::Audio(), Audio::playMusic(), Audio::playShopSound(), Audio::playSound() (+1 more)

### Community 110 - "EntityManager"
Cohesion: 0.29
Nodes (3): EntityManager, _freeList, _generations

### Community 111 - "Gameplay"
Cohesion: 0.11
Nodes (19): Bosses, Charged shot, Coins, time and level skip, Controls, Difficulty and settings, Drone, Enemies, Gameplay (+11 more)

### Community 112 - "ComponentId"
Cohesion: 0.14
Nodes (9): ComponentId, id, getId(), ComponentRegistry::getInfo(), ComponentRegistry::registerType(), RegistryData, ids, infos (+1 more)

### Community 113 - "Custom maps: for developers"
Cohesion: 0.15
Nodes (11): Add a field, Add a kind of obstacle, Add a monster, a boss, a zone, Change the format, Custom maps: for developers, From the file to the match, How-to, Records (+3 more)

### Community 116 - "bossHazardSystem"
Cohesion: 0.29
Nodes (7): bossHazardSystem(), HazardClock, laser, meteor, time, hazardInterval(), spawnMeteor()

### Community 118 - "Hud.cpp"
Cohesion: 0.09
Nodes (22): Gates, Level, Map, Obstacle ranges, Obstacles, Portals, Reference, Solid pieces (+14 more)

### Community 119 - "ProjectileDef"
Cohesion: 0.11
Nodes (18): Behaviours, Kinds of monsters and bosses, dasherTarget(), DashTarget, pos, velocity, roamBoss(), BossTag (+10 more)

### Community 120 - "TextureManager::Impl"
Cohesion: 0.25
Nodes (3): TextureManager::Impl, ids, textures

### Community 121 - "Streak"
Cohesion: 0.40
Nodes (5): Streak, length, speed, x, y

### Community 122 - "GameView.cpp"
Cohesion: 0.11
Nodes (17): Vector2f, x, y, ScreenRect, position, size, GameView::drawBeamCore(), GameView::drawLaserPost() (+9 more)

### Community 124 - "FixedTimestep"
Cohesion: 0.25
Nodes (3): FixedTimestep, _accumulator, _maxAccumulator

### Community 125 - "TEST"
Cohesion: 0.06
Nodes (7): Maps and lobby, _shopOffers, gameAtTheEndOfALevel(), runFor(), shopWithCoins(), TEST(), twoLevels()

### Community 126 - "StrikeSpec"
Cohesion: 0.17
Nodes (12): Strike, deadly, spec, time, StrikeSpec, active, height, vy (+4 more)

### Community 127 - "Column"
Cohesion: 0.19
Nodes (6): Column, _data, _info, ComponentInfo, infoOf(), swapRemove()

### Community 128 - "LeviathanMove"
Cohesion: 0.25
Nodes (8): LeviathanMove, Aim, Columns, Dash, Rest, Return, Sweep, Windup

### Community 129 - "LaserCycle"
Cohesion: 0.50
Nodes (4): LaserCycle, age, beam, hasBeam

### Community 130 - "Signature"
Cohesion: 0.32
Nodes (3): Signature, WORD_BITS, _words

### Community 133 - "Aura"
Cohesion: 0.29
Nodes (7): Aura, Dasher, Fortress, None, Standard, Summoner, GameView::auraOf()

### Community 134 - "Prediction.cpp"
Cohesion: 0.22
Nodes (3): Prediction::advance(), Prediction::position(), Prediction::velocity()

### Community 135 - "Case"
Cohesion: 0.33
Nodes (4): Case, name, run, Registrar

### Community 136 - "Star"
Cohesion: 0.50
Nodes (4): Star, layer, x, y

### Community 137 - "Hud::drawVictoryStats"
Cohesion: 0.33
Nodes (3): count, clock(), Hud::drawVictoryStats()

### Community 138 - "Message"
Cohesion: 0.15
Nodes (9): Connected, client, Disconnected, client, reason, Message, client, payload (+1 more)

### Community 140 - "TeamId"
Cohesion: 0.67
Nodes (3): TeamId, Monster, Player

### Community 141 - "Sprite"
Cohesion: 0.20
Nodes (8): Sprite, animation, texture, Area, box, pos, GameView::drawTiled(), Hud::drawIcon()

### Community 142 - "Resized"
Cohesion: 0.50
Nodes (3): Resized, height, width

### Community 143 - "TurretFire"
Cohesion: 0.67
Nodes (3): TurretFire, interval, timer

### Community 144 - "MonsterState"
Cohesion: 0.50
Nodes (4): MonsterState, age, locked, summoned

### Community 145 - "Regeneration"
Cohesion: 0.50
Nodes (4): Regeneration, hp, interval, timer

### Community 146 - "convert"
Cohesion: 0.33
Nodes (6): FocusGained, FocusLost, convert(), toKey(), translate(), Window::pollEvent()

### Community 147 - "Shield"
Cohesion: 0.67
Nodes (3): Shield, cooldown, timer

### Community 148 - "App::useCustomStep"
Cohesion: 0.32
Nodes (3): App::useCustomScoresPage(), App::useCustomStep(), App::useMapStep()

### Community 149 - "LobbyStep"
Cohesion: 0.25
Nodes (6): LobbyStep, Custom, Map, Scores, Settings, App::currentSettings()

### Community 150 - "CoinCounter"
Cohesion: 0.29
Nodes (6): Game rules, CoinCounter, quantity, total, coins, value

### Community 151 - "string"
Cohesion: 0.48
Nodes (3): formatTime(), twoDigits(), Hud::drawCustomDifficulty()

### Community 152 - "RefuseReason"
Cohesion: 0.33
Nodes (6): Refused, reason, RefuseReason, Full, Version, Server::sendRefuse()

### Community 153 - "ObstacleMotion"
Cohesion: 0.40
Nodes (5): ObstacleMotion, age, amplitude, baseY, omega

## Knowledge Gaps
- **724 isolated node(s):** `WINDOW_WIDTH`, `WINDOW_HEIGHT`, `FRAMERATE`, `WORLD_TO_SCREEN`, `LISTEN_DELAY` (+719 more)
  These have ≤1 connection - possible missing edges or undocumented components. (Counts symbols only; 1425 node(s) total have ≤1 connection when file, concept and rationale nodes are included.)
- **26 thin communities (<3 nodes) omitted from report** — run `graphify query` to explore isolated nodes.

## Suggested Questions
_Questions this graph is uniquely positioned to answer:_

- **Why does `TEST()` connect `Game Tests` to `Vec2`, `Game`, `.startMap`, `DeathRuns.hpp`, `Collider`, `Leviathan.hpp`, `Systems.hpp`, `Registry`, `Shop.hpp`, `Zone`, `TEST`, `Segments`, `map3`, `Layers`, `EntitySpawned`, `Obstacles.hpp`, `.getIndex`, `findPlayerBySlot`, `LevelDef`, `bossHazardSystem`, `TEST`?**
  _High betweenness centrality (0.092) - this node is a cross-community bridge._
- **Are the 77 inferred relationships involving `TEST()` (e.g. with `Registry` and `.alive()`) actually correct?**
  _`TEST()` has 77 INFERRED edges - model-reasoned connections that need verification._
- **What connects `WINDOW_WIDTH`, `WINDOW_HEIGHT`, `FRAMERATE` to the rest of the system?**
  _724 weakly-connected nodes found - possible documentation gaps or missing edges._
- **Should `src/App.cpp` be split into smaller, more focused modules?**
  _Cohesion score 0.09523809523809523 - nodes in this community are weakly interconnected._
- **Why does `Replica` connect `.updatePlaying` to `Hud::drawScoresStep`, `Hud::drawVictoryStats`, `Hud::drawMapStep`, `ShopPanel.cpp`, `Hud`, `Prediction`, `Replica`, `GameView`, `ShopPanel`, `Registry`, `Rarity`, `Replica Events`, `Replica::apply`, `Shop.hpp`, `Phase`, `HudLobby.cpp`, `SpriteBank`, `LobbyMenu`, `HudVictory.cpp`, `DisconnectReason`, `GameStatus`, `Sample`, `Track`, `optional`, `Hud.cpp`, `GameView.cpp`?**
  _High betweenness centrality (0.088) - this node is a cross-community bridge._
- **Are the 2 inferred relationships involving `Registry` (e.g. with `TEST()` and `TEST()`) actually correct?**
  _`Registry` has 2 INFERRED edges - model-reasoned connections that need verification._
- **Should `IntRect` be split into smaller, more focused modules?**
  _Cohesion score 0.14619883040935672 - nodes in this community are weakly interconnected._