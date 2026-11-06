# Graph Report - RTYPE  (2026-11-06)

## Corpus Check
- 166 files · ~136,450 words
- Verdict: corpus is large enough that graph structure adds value.
- Unclassified: 5 file(s) not represented in the graph (top: (none) 5)

## Summary
- 3026 nodes · 6396 edges · 152 communities (134 shown, 18 thin omitted)
- Extraction: 87% EXTRACTED · 13% INFERRED · 0% AMBIGUOUS · INFERRED: 859 edges (avg confidence: 0.87)
- Token cost: 0 input · 0 output

## Graph Freshness
- Built from commit: `6e90fe3f`
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
- Reader.cpp
- Reliable Channel
- Shop.hpp
- Sample
- Phase
- Network protocol doc
- Architecture Targets
- TEST
- TEST
- Segments
- Key
- addGates
- ComponentId
- map3.cpp
- Layers
- SpriteId
- Window.cpp
- SpriteBank
- SpriteBank.cpp
- src/App.cpp
- Background
- Message Types
- Replication.hpp
- BossKind
- Controls.cpp
- describeSpawn
- SpriteId
- time_point
- Piece
- Client.cpp
- Renderer.cpp
- SpriteBank
- EntityState
- WaveStep
- BinaryReader
- Window Impl Header
- MapEntry
- TEST
- Rarity
- stdexcept
- Obstacles.hpp
- Sound Manager Impl
- Reader
- MonsterDef
- .restartDeathRun
- Connection Send
- Hud::drawPlayers
- SlotTable
- DisconnectReason
- Connection.cpp
- GameStatus
- Reader.hpp
- Replica::apply
- game/App.cpp
- MapParser.cpp
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
- .clearSpawnY
- EntityManager
- formatTime
- bossHazardSystem
- Prediction.cpp
- Shop.hpp
- TextureManager::Impl
- Streak
- Vector2f
- Hud.cpp
- FixedTimestep
- TEST
- MapJsonTests.cpp
- ObstacleKind
- LeviathanMove
- Client::send
- Star
- CommandBuffer
- Hud::drawLobby
- Message
- MapError
- Hud::drawTimer
- HudVictory.cpp
- Aura
- GameView.cpp
- Behaviours
- Area
- Resized
- Custom maps
- .serialize
- Regeneration
- CoinCounter
- scatter
- State
- Network::Network
- PlayerInput
- ObstacleMotion

## God Nodes (most connected - your core abstractions)
1. `TEST()` - 197 edges
2. `Registry` - 164 edges
3. `Game` - 138 edges
4. `Entity` - 130 edges
5. `Replica` - 76 edges
6. `TEST()` - 59 edges
7. `LevelDef` - 54 edges
8. `TEST()` - 54 edges
9. `Hud` - 51 edges
10. `SpriteId` - 48 edges

## Surprising Connections (you probably didn't know these)
- `For developers` --references--> `Reader`  [INFERRED]
  docs/custom-maps.md → src/client/game/Rtype/src/mapjson/Reader.hpp
- `File format` --references--> `deathRun()`  [INFERRED]
  docs/custom-maps.md → src/client/game/Rtype/src/maps/map1.cpp
- `Obstacles` --references--> `wall()`  [INFERRED]
  docs/custom-maps.md → src/client/game/Rtype/include/logic/LevelBuilder.hpp
- `Obstacles` --references--> `pinch()`  [INFERRED]
  docs/custom-maps.md → src/client/game/Rtype/src/maps/map3.cpp
- `Use the graph` --references--> `Connection`  [INFERRED]
  docs/graphify.md → engine/network/include/engine/network/Connection.hpp

## Import Cycles
- None detected.

## Communities (152 total, 18 thin omitted)

### Community 0 - "src/App.cpp"
Cohesion: 0.11
Nodes (6): App::App(), App::isReady(), App::listening(), App::playSounds(), App::sendReady(), App::useShop()

### Community 1 - "IntRect"
Cohesion: 0.13
Nodes (15): Animation, firstFrame, frameCount, frameDuration, step, IntRect, height, left (+7 more)

### Community 2 - "string"
Cohesion: 0.14
Nodes (5): Window, _impl, handle(), App::loop(), App::run()

### Community 3 - "Components.hpp"
Cohesion: 0.14
Nodes (14): LaserCycle, age, beam, hasBeam, MaxHealth, hp, MultiShot, bullets (+6 more)

### Community 4 - "Background.cpp"
Cohesion: 0.13
Nodes (17): Renderer, Background::drawAura(), Background::drawBand(), Background::drawBars(), Background::drawDebris(), Background::drawFade(), Background::drawNebula(), Background::drawSky() (+9 more)

### Community 5 - "Client Connection"
Cohesion: 0.07
Nodes (11): Client, _attempts, _connection, _events, _handshakeSequence, _lastAttempt, _server, _socket (+3 more)

### Community 6 - "vector"
Cohesion: 0.16
Nodes (4): SoundManager::Impl, buffers, ids, SoundManager::load()

### Community 7 - "Component.hpp"
Cohesion: 0.13
Nodes (8): add(), create(), remove(), ComponentRegistry, idOf(), infoOf(), swapRemove(), hash<Engine::Entity>

### Community 8 - "Delivery and Sessions"
Cohesion: 0.08
Nodes (25): Delivery, Reliable, Unreliable, DisconnectReason, Kicked, Quit, Shutdown, Timeout (+17 more)

### Community 9 - "Build and Protocol Docs"
Cohesion: 0.16
Nodes (17): engine_audio library, SFML::Audio, engine build root, engine_core library, Handshake and connection, Network module (engine::network), ReliableChannel, UDP wire format (+9 more)

### Community 10 - "Server Events"
Cohesion: 0.06
Nodes (9): Server, _events, _maxClients, _nextClientId, _sessions, _socket, _version, Server::handleConnect() (+1 more)

### Community 11 - "ShopPanel.cpp"
Cohesion: 0.13
Nodes (10): ShopPanel::affordable(), ShopPanel::confirm(), ShopPanel::drawPrice(), ShopPanel::handle(), ShopPanel::hint(), ShopPanel::isOpen(), ShopPanel::selection(), ShopPanel::ShopPanel() (+2 more)

### Community 14 - "Session"
Cohesion: 0.18
Nodes (8): _random, Session, address, connection, id, Server::findByAddress(), Server::findByToken(), Server::makeToken()

### Community 15 - "Connection Channel"
Cohesion: 0.07
Nodes (11): Connection, _channel, _lastPing, _lastReceived, _outbox, _pingId, _pingSentAt, _received (+3 more)

### Community 16 - "Game"
Cohesion: 0.05
Nodes (33): Public API: the `Game` class, Game, _alive, _augments, _coinCounter, _countdown, _deaths, _effectLevel (+25 more)

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
Nodes (32): decodeAugments(), decodeBuy(), decodeBuyResult(), decodeDestroy(), decodeInput(), decodeOwnState(), decodePause(), decodeReady() (+24 more)

### Community 21 - "Prediction"
Cohesion: 0.07
Nodes (16): Pending, dt, input, sequence, Prediction, _box, _input, MAX_PENDING (+8 more)

### Community 22 - "Binary Writer"
Cohesion: 0.12
Nodes (15): BinaryWriter, _data, MAX_VARINT, BinaryWriter::writeBytes(), BinaryWriter::writeFloat(), BinaryWriter::writeU16(), BinaryWriter::writeU32(), BinaryWriter::writeU8() (+7 more)

### Community 23 - "Packet"
Cohesion: 0.11
Nodes (10): Packet, _messages, Client::checkConnection(), Client::handleDatagram(), Client::handleHandshake(), Client::handleSession(), Client::tryConnect(), Connection::hasHandshakeMessage() (+2 more)

### Community 24 - "Pending Messages"
Cohesion: 0.11
Nodes (8): Pending, message, sentAt, ReliableChannel, _early, _expected, _nextId, _pending

### Community 25 - "Replica"
Cohesion: 0.10
Nodes (16): GameView, ANIMATION_PHASE, _background, _hud, HURT_DURATION, HURT_SHAKE, _hurtUntil, _lastBossHp (+8 more)

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
Cohesion: 0.11
Nodes (26): applyContact(), canHit(), collectProjectiles(), collectTargets(), Collider, box, entity, pos (+18 more)

### Community 30 - "ShopPanel"
Cohesion: 0.07
Nodes (20): ShopPanel, BORDER, CARD_GAP, CARD_HEIGHT, CARD_TOP, CARD_WIDTH, COIN_GAP, COIN_SIZE (+12 more)

### Community 31 - "Leviathan.hpp"
Cohesion: 0.12
Nodes (32): LeviathanState, move, step, swing, timer, StrikeSpec, active, height (+24 more)

### Community 32 - "Audio Player Impl"
Cohesion: 0.18
Nodes (4): AudioPlayer::play(), AudioPlayer::playMusic(), AudioPlayer::setMusicVolume(), toSfmlVolume()

### Community 33 - "TEST"
Cohesion: 0.10
Nodes (6): fan(), volley(), isReadyToShoot(), shoot(), shotVelocities(), TEST()

### Community 34 - "Registry"
Cohesion: 0.05
Nodes (61): Entity, id, eachRow(), EntityRecord, archetype, row, has(), IterationGuard (+53 more)

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
Cohesion: 0.13
Nodes (7): Refused, reason, Unreachable, applyDecoded(), Network, _client, _open

### Community 40 - "Build Tooling Docs"
Cohesion: 0.12
Nodes (20): Dependency rules, Game::setInput / Game::update(dt), Server App class (only place including both network and game), Server threads: network thread and game thread, Technical and comparative study doc, Choice: AABB collision, Choice: hand-written binary big-endian encoding, Choice: CMake with presets (+12 more)

### Community 41 - "Reader.cpp"
Cohesion: 0.18
Nodes (6): Range, max, min, isPrintable(), Reader::Reader(), show()

### Community 42 - "Reliable Channel"
Cohesion: 0.23
Nodes (5): ReliableChannel::acknowledge(), ReliableChannel::receive(), ReliableChannel::resend(), ReliableChannel::send(), ReliableChannel::takeReady()

### Community 43 - "Shop.hpp"
Cohesion: 0.12
Nodes (18): applyAugment(), AugmentDef, apply, id, price, rarity, AugmentId, Count (+10 more)

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

### Community 48 - "TEST"
Cohesion: 0.10
Nodes (7): failsWith(), freshFolder(), mapOf(), replaced(), TEST(), tinyLevel(), write()

### Community 49 - "TEST"
Cohesion: 0.05
Nodes (6): Leaderboard, _records, RunRecord, players, timeMs, TEST()

### Community 50 - "Segments"
Cohesion: 0.12
Nodes (17): firstLevel(), isDeathRun(), lastSpawnPoint(), levelAt(), levelSpan(), levelStart(), levelStartOf(), Segments (+9 more)

### Community 51 - "Key"
Cohesion: 0.10
Nodes (21): Key, A, Backspace, C, D, Down, E, Enter (+13 more)

### Community 52 - "addGates"
Cohesion: 0.11
Nodes (31): Obstacles, portal(), wall(), addCave(), addGates(), addRun(), addSweepers(), CavePiece (+23 more)

### Community 53 - "ComponentId"
Cohesion: 0.10
Nodes (13): Column, _data, _info, ComponentId, id, ComponentInfo, getId(), ComponentRegistry::getInfo() (+5 more)

### Community 54 - "map3.cpp"
Cohesion: 0.54
Nodes (6): gauntlet(), map3(), minefield(), pinch(), rhythm(), run()

### Community 55 - "Layers"
Cohesion: 0.06
Nodes (57): File format, Architecture, Collisions, Components, Data oriented design, Layers, Position, velocity, speed, direction, Shooting (+49 more)

### Community 56 - "SpriteId"
Cohesion: 0.15
Nodes (6): Controls, _held, _input, _pause, _pressed, _respawn

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
Cohesion: 0.24
Nodes (6): AudioPlayer::Impl, music, musicVolume, next, soundVolume, voices

### Community 61 - "Background"
Cohesion: 0.09
Nodes (17): Background, _aura, _auraLevel, _boosted, _fade, _height, _last, LAYERS (+9 more)

### Community 62 - "Message Types"
Cohesion: 0.22
Nodes (5): Message, id, payload, reliable, type

### Community 63 - "Replication.hpp"
Cohesion: 0.27
Nodes (6): collectSpawns(), collectStates(), collectUntracked(), describeSpawn(), isReplicable(), slotOf()

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
Nodes (33): isBossSprite(), SpriteId, AlienShard, BeamOn, BeamWarn, Bomb, Bomber, Boss (+25 more)

### Community 68 - "time_point"
Cohesion: 0.17
Nodes (5): Connection::handleUser(), Connection::receive(), Connection::sendPing(), Connection::timedOut(), Connection::update()

### Community 69 - "Piece"
Cohesion: 0.17
Nodes (18): isPortalPiece(), kindOf(), parseObstacle(), Piece, FastPortal, Gate, Laser, Mine (+10 more)

### Community 70 - "Client.cpp"
Cohesion: 0.22
Nodes (4): Client::flush(), Client::latency(), Client::pollEvent(), Client::sendDisconnect()

### Community 71 - "Renderer.cpp"
Cohesion: 0.09
Nodes (19): Color, a, b, g, r, Font, GLYPH_HEIGHT, GLYPH_WIDTH (+11 more)

### Community 72 - "SpriteBank"
Cohesion: 0.10
Nodes (9): TextureManager, _impl, SpriteBank, _assets, _backdrops, _clock, _sprites, Background::Background() (+1 more)

### Community 73 - "EntityState"
Cohesion: 0.19
Nodes (11): EntityState, id, pos, encodeStateChunk(), encodeStates(), InputMessage, input, sequence (+3 more)

### Community 74 - "WaveStep"
Cohesion: 0.06
Nodes (33): MonsterType, kind, fittedBase(), fittedSpacing(), formationDelay(), formationSlot(), Slot, delay (+25 more)

### Community 75 - "BinaryReader"
Cohesion: 0.17
Nodes (4): BinaryReader, _cursor, decodeLeaderboard(), readRecords()

### Community 77 - "MapEntry"
Cohesion: 0.16
Nodes (10): MapEntry, error, file, hash, map, isJson(), listJson(), loadEntry() (+2 more)

### Community 79 - "Rarity"
Cohesion: 0.14
Nodes (14): augmentLabel(), rarityColor(), rarityName(), Rarity, Common, Epic, Legendary, Rare (+6 more)

### Community 80 - "stdexcept"
Cohesion: 0.10
Nodes (7): main(), parsePort(), parsePort(), Case, name, run, Registrar

### Community 81 - "Obstacles.hpp"
Cohesion: 0.07
Nodes (42): The tick: `Game::update`, bossSystem(), addToPurse(), coinPickupSystem(), collectCoins(), touchesAny(), PlayerEffects, inverted (+34 more)

### Community 82 - "Sound Manager Impl"
Cohesion: 0.25
Nodes (3): SoundManager, _impl, AudioPlayer::AudioPlayer()

### Community 83 - "Reader"
Cohesion: 0.20
Nodes (12): parseLevel(), readBosses(), readObstacles(), readWaves(), Reader, _json, _path, checkReach() (+4 more)

### Community 84 - "MonsterDef"
Cohesion: 0.08
Nodes (25): Add a new kind of monster, Kinds of monsters and bosses, BossTag, age, summonTimer, addMonsterStats(), flyStraight(), MonsterDef (+17 more)

### Community 85 - ".restartDeathRun"
Cohesion: 0.17
Nodes (5): countBefore(), levelEnd(), collectObstacles(), collectProjectileEntities(), strikeOfKind()

### Community 86 - "Connection Send"
Cohesion: 0.32
Nodes (4): Connection::flush(), Connection::send(), Connection::sendEngine(), Connection::takeMessages()

### Community 88 - "SlotTable"
Cohesion: 0.22
Nodes (9): Sound, BossAlert, Coin, Count, Defeat, Explosion, Hit, Shoot (+1 more)

### Community 89 - "DisconnectReason"
Cohesion: 0.14
Nodes (11): Cue, BossAppears, BossExplosion, CoinPickup, Explosion, Hit, PlayerShot, Purchase (+3 more)

### Community 90 - "Connection.cpp"
Cohesion: 0.17
Nodes (9): Connection::Connection(), Connection::handlePong(), Connection::latency(), Connection::nextSequence(), Connection::peerDisconnect(), Connection::readId(), Connection::resendDelay(), Connection::token() (+1 more)

### Community 91 - "GameStatus"
Cohesion: 0.10
Nodes (16): GameStatus, bossHp, bossMaxHp, coins, countdown, deaths, elapsedMs, level (+8 more)

### Community 92 - "Reader.hpp"
Cohesion: 0.28
Nodes (9): ParsedLevel, level, zone, choice(), choiceOr(), Name, name, value (+1 more)

### Community 93 - "Replica::apply"
Cohesion: 0.09
Nodes (19): EntityDestroyed, id, EntitySpawned, box, id, pos, slot, sprite (+11 more)

### Community 95 - "MapParser.cpp"
Cohesion: 0.24
Nodes (8): checkVersion(), contentHash(), parseJson(), parseMap(), readLevels(), readMap(), readName(), tooDeep()

### Community 96 - ".operator()"
Cohesion: 0.40
Nodes (5): Install, Keep it up to date: the git hook, Knowledge graph (graphify), Use the graph, With an AI agent (Claude Code)

### Community 97 - "Track"
Cohesion: 0.14
Nodes (13): Track, BossDasher, BossFortress, BossStandard, BossSummoner, Core, Count, DeathRun (+5 more)

### Community 98 - "CLAUDE.md"
Cohesion: 0.14
Nodes (12): encodeKeys(), phaseByte(), StatusHeader, bossHp, bossMaxHp, coins, count, deaths (+4 more)

### Community 99 - "findPlayerBySlot"
Cohesion: 0.12
Nodes (8): Coding conventions, Configuration, Current status, Game rules, Maps and lobby, Overview, R-Type game logic, The first map: 20 levels

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
Cohesion: 0.11
Nodes (12): Waves, Archetype, _columns, _columnsIndex, _entities, NO_COLUMN, _signature, each() (+4 more)

### Community 106 - "Zone"
Cohesion: 0.11
Nodes (16): Scene, aura, boosted, zone, Zone, Arena, Asteroids, Core (+8 more)

### Community 107 - "Audio.cpp"
Cohesion: 0.14
Nodes (9): ShopSound, Accept, Count, Refuse, Audio::Audio(), Audio::playMusic(), Audio::playShopSound(), Audio::playSound() (+1 more)

### Community 110 - "GateSpec"
Cohesion: 0.08
Nodes (28): addGate(), GateSpec, at, gapHeight, gapY, kind, motion, pinch (+20 more)

### Community 111 - "Gameplay"
Cohesion: 0.14
Nodes (14): Bosses, Coins, time and level skip, Controls, Enemies, Gameplay, Goal, Leaderboard, Lobby and maps (+6 more)

### Community 112 - "buildMap"
Cohesion: 0.29
Nodes (4): appendLevel(), buildMap(), buildRun(), shifted()

### Community 113 - ".clearSpawnY"
Cohesion: 0.15
Nodes (5): What the server reads, PendingSpawn, delay, kind, y

### Community 114 - "EntityManager"
Cohesion: 0.29
Nodes (3): EntityManager, _freeList, _generations

### Community 115 - "formatTime"
Cohesion: 0.47
Nodes (3): formatTime(), Hud::drawRecords(), twoDigits()

### Community 116 - "bossHazardSystem"
Cohesion: 0.18
Nodes (8): Variety by zone, bossHazardSystem(), HazardClock, meteor, time, hazardInterval(), spawnMeteor(), stormInterval()

### Community 118 - "Prediction.cpp"
Cohesion: 0.17
Nodes (4): Prediction::advance(), Prediction::position(), Prediction::record(), Prediction::setWorld()

### Community 119 - "Shop.hpp"
Cohesion: 0.30
Nodes (9): isExcluded(), pickWeighted(), rarityWeight(), rollOffers(), shopPool(), ShopTier, bossLevel, level (+1 more)

### Community 120 - "TextureManager::Impl"
Cohesion: 0.15
Nodes (4): TextureManager::Impl, ids, textures, TextureManager::load()

### Community 121 - "Streak"
Cohesion: 0.40
Nodes (5): Streak, length, speed, x, y

### Community 122 - "Vector2f"
Cohesion: 0.11
Nodes (18): Sprite, animation, texture, Vector2f, x, y, Renderer::draw(), Renderer::drawRect() (+10 more)

### Community 124 - "FixedTimestep"
Cohesion: 0.25
Nodes (3): FixedTimestep, _accumulator, _maxAccumulator

### Community 125 - "TEST"
Cohesion: 0.06
Nodes (8): _shopOffers, clearEnemies(), gameAtTheEndOfALevel(), reachShop(), runFor(), shopWithCoins(), TEST(), twoLevels()

### Community 126 - "MapJsonTests.cpp"
Cohesion: 0.25
Nodes (8): CustomMap, author, levels, name, zones, ParseResult, error, map

### Community 127 - "ObstacleKind"
Cohesion: 0.11
Nodes (19): ObstacleTag, kind, isPortal(), ObstacleKind, Beam, BeamWarn, Count, FastPortal (+11 more)

### Community 128 - "LeviathanMove"
Cohesion: 0.25
Nodes (8): LeviathanMove, Aim, Columns, Dash, Rest, Return, Sweep, Windup

### Community 130 - "Star"
Cohesion: 0.50
Nodes (4): Star, layer, x, y

### Community 134 - "Message"
Cohesion: 0.15
Nodes (9): Connected, client, Disconnected, client, reason, Message, client, payload (+1 more)

### Community 135 - "MapError"
Cohesion: 0.23
Nodes (8): checkTotals(), countAll(), tooMany(), Totals, bosses, monsters, obstacles, MapError

### Community 136 - "Hud::drawTimer"
Cohesion: 0.20
Nodes (9): PlayerStatus, alive, hp, ready, slot, Hud::drawCoins(), Hud::drawLevel(), Hud::drawReady() (+1 more)

### Community 137 - "HudVictory.cpp"
Cohesion: 0.14
Nodes (7): count, clock(), confettiColor(), Hud::drawConfetti(), Hud::drawVictoryScreen(), Hud::drawVictoryStats(), Hud::timeColor()

### Community 138 - "Aura"
Cohesion: 0.29
Nodes (7): Aura, Dasher, Fortress, None, Standard, Summoner, GameView::auraOf()

### Community 139 - "GameView.cpp"
Cohesion: 0.15
Nodes (8): GameView::drawLaser(), GameView::drawLayer(), GameView::drawWorld(), GameView::GameView(), GameView::isBlock(), GameView::isPortalSprite(), GameView::tiltFrame(), GameView::trackBossHurt()

### Community 140 - "Behaviours"
Cohesion: 0.12
Nodes (16): Behaviours, dasherTarget(), DashTarget, pos, velocity, roamBoss(), MonsterState, age (+8 more)

### Community 141 - "Area"
Cohesion: 0.40
Nodes (4): Area, box, pos, GameView::drawTiled()

### Community 142 - "Resized"
Cohesion: 0.50
Nodes (3): Resized, height, width

### Community 143 - "Custom maps"
Cohesion: 0.20
Nodes (8): Bosses, Custom maps, Errors, For developers, Limits, Obstacles, Quick start, Hud::textTop()

### Community 144 - ".serialize"
Cohesion: 0.29
Nodes (6): RefuseReason, Full, Version, Client::flushConnection(), Server::flush(), Server::sendRefuse()

### Community 145 - "Regeneration"
Cohesion: 0.50
Nodes (4): Regeneration, hp, interval, timer

### Community 146 - "CoinCounter"
Cohesion: 0.33
Nodes (5): CoinCounter, quantity, total, coins, value

### Community 147 - "scatter"
Cohesion: 0.40
Nodes (4): scatter(), scatterLasers(), scatterLimit(), zoneOf()

### Community 148 - "State"
Cohesion: 0.50
Nodes (4): State, Closed, Connected, Connecting

### Community 150 - "PlayerInput"
Cohesion: 0.15
Nodes (11): Add a component, Add a map or a level, Add a player action (a new key), How-to, PlayerInput, down, fire, left (+3 more)

### Community 153 - "ObstacleMotion"
Cohesion: 0.40
Nodes (5): ObstacleMotion, age, amplitude, baseY, omega

## Knowledge Gaps
- **744 isolated node(s):** `name`, `author`, `levels`, `zones`, `map` (+739 more)
  These have ≤1 connection - possible missing edges or undocumented components. (Counts symbols only; 1434 node(s) total have ≤1 connection when file, concept and rationale nodes are included.)
- **18 thin communities (<3 nodes) omitted from report** — run `graphify query` to explore isolated nodes.

## Suggested Questions
_Questions this graph is uniquely positioned to answer:_

- **Why does `TEST()` connect `Game Tests` to `cstddef`, `Behaviours`, `Game`, `Collider`, `Leviathan.hpp`, `TEST`, `Registry`, `Shop.hpp`, `TEST`, `Segments`, `addGates`, `Layers`, `Obstacles.hpp`, `.restartDeathRun`, `findPlayerBySlot`, `LevelDef`, `buildMap`, `.clearSpawnY`, `bossHazardSystem`, `TEST`?**
  _High betweenness centrality (0.092) - this node is a cross-community bridge._
- **Are the 77 inferred relationships involving `TEST()` (e.g. with `Registry` and `.alive()`) actually correct?**
  _`TEST()` has 77 INFERRED edges - model-reasoned connections that need verification._
- **What connects `name`, `author`, `levels` to the rest of the system?**
  _744 weakly-connected nodes found - possible documentation gaps or missing edges._
- **Should `src/App.cpp` be split into smaller, more focused modules?**
  _Cohesion score 0.10526315789473684 - nodes in this community are weakly interconnected._
- **Why does `Replica` connect `.updatePlaying` to `Hud::drawLobby`, `HudVictory.cpp`, `GameView.cpp`, `ShopPanel.cpp`, `Components.hpp`, `Hud`, `Prediction`, `PlayerInput`, `Replica`, `GameView`, `ShopPanel`, `Registry`, `Replica Events`, `Shop.hpp`, `Sample`, `Phase`, `Layers`, `SpriteBank`, `Rarity`, `DisconnectReason`, `GameStatus`, `Replica::apply`, `Track`, `Zone`?**
  _High betweenness centrality (0.079) - this node is a cross-community bridge._
- **Are the 2 inferred relationships involving `Registry` (e.g. with `TEST()` and `TEST()`) actually correct?**
  _`Registry` has 2 INFERRED edges - model-reasoned connections that need verification._
- **Should `IntRect` be split into smaller, more focused modules?**
  _Cohesion score 0.13333333333333333 - nodes in this community are weakly interconnected._