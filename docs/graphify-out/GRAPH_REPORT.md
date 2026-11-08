# Graph Report - RTYPE  (2026-11-07)

## Corpus Check
- 204 files · ~164,591 words
- Verdict: corpus is large enough that graph structure adds value.
- Unclassified: 5 file(s) not represented in the graph (top: (none) 5)

## Summary
- 3855 nodes · 8505 edges · 183 communities (160 shown, 23 thin omitted)
- Extraction: 84% EXTRACTED · 16% INFERRED · 0% AMBIGUOUS · INFERRED: 1325 edges (avg confidence: 0.87)
- Token cost: 0 input · 0 output

## Graph Freshness
- Built from commit: `10056002`
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
- Layers
- Game Tests
- Components.hpp
- Players: charge, secondary weapon, drone
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
- LeviathanState
- Audio Player Impl
- TEST
- Registry
- BackdropId
- Design Choices Docs
- Audio Player
- AugmentStyle.hpp
- Network
- Build Tooling Docs
- EntitySpawned
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
- Window::Window
- SpriteBank
- TEST
- src/App.cpp
- Background
- Message Types
- CustomInfo
- BossKind
- Controls.cpp
- describeSpawn
- SpriteId
- ObstacleKind
- Connection.cpp
- Client.cpp
- Font
- SpriteBank
- EntityState
- WaveStep
- BinaryReader
- Window Impl Header
- LobbyMenu
- TEST
- Phase
- TEST
- Systems.hpp
- Sound Manager Impl
- Reader
- MonsterDef
- TEST
- Connection Send
- UdpSocket
- SlotTable
- DisconnectReason
- TEST
- GameStatus
- Create your own map
- Hud::drawSettingsStep
- RunRecord
- Zone
- .operator()
- Track
- CLAUDE.md
- .isUsedSlot
- LevelDef
- App
- UdpSocket::Impl
- map1
- Archetype
- ObstacleStep
- Audio.cpp
- App::run
- vcpkg.json
- string
- Gameplay
- ComponentId
- The reader: `rtype::mapjson`
- ShopPanel.cpp
- Hud.hpp
- BossHazards.hpp
- duration
- .width
- TEST
- CustomMaps
- Piece
- GameView.cpp
- game/App.cpp
- LifeBook
- TEST
- StrikeSpec
- Column
- LeviathanMove
- LaserCycle
- Signature
- CustomMapList
- AugmentId
- Aura
- PlayerInput
- Case
- .restartDeathRun
- MapUpload
- TEST
- TEST
- parseLevel
- CustomLeaderboard
- Resized
- Reader.hpp
- WeaponKind
- Regeneration
- convert
- Drone.hpp
- App::useCustomStep
- Message
- CoinCounter
- CustomMapList.cpp
- time_point
- ObstacleMotion
- Steps
- ComponentId
- CustomMap
- checkTotals
- Hud::drawPlayers
- Folder.cpp
- Datagram
- DifficultyScale
- .handle
- Sample
- Rarity
- EntityManager
- FixedTimestep
- Session
- TextureManager::Impl
- Hud::drawCustomStep
- LoadedMap
- Hud::drawVictoryStats
- ShopSound
- How a map is built
- MonsterState
- wrap
- Audio::Audio
- Shield
- TeamId
- TurretFire
- CustomMaps/README.md

## God Nodes (most connected - your core abstractions)
1. `Registry` - 245 edges
2. `TEST()` - 202 edges
3. `Entity` - 187 edges
4. `Game` - 158 edges
5. `Replica` - 107 edges
6. `TEST()` - 76 edges
7. `Hud` - 71 edges
8. `Layers` - 62 edges
9. `TEST()` - 59 edges
10. `AugmentId` - 58 edges

## Surprising Connections (you probably didn't know these)
- `Decisions already made` --references--> `applyDamages()`  [INFERRED]
  roadmap.md → src/client/game/Rtype/include/logic/Damages.hpp
- `Add a kind of obstacle` --references--> `kindOf()`  [INFERRED]
  docs/custom-maps-internals.md → src/client/game/Rtype/src/mapjson/ObstacleParser.cpp
- `Add a field` --references--> `Reader`  [INFERRED]
  docs/custom-maps-internals.md → src/client/game/Rtype/src/mapjson/Reader.hpp
- `What you need` --references--> `CustomMaps`  [INFERRED]
  docs/custom-maps.md → src/server/game/CustomMaps.hpp
- `Your first map in two minutes` --references--> `CustomMaps`  [INFERRED]
  docs/custom-maps.md → src/server/game/CustomMaps.hpp

## Import Cycles
- None detected.

## Communities (183 total, 23 thin omitted)

### Community 0 - "src/App.cpp"
Cohesion: 0.09
Nodes (8): App::App(), App::currentSettings(), App::isReady(), App::listening(), App::playSounds(), App::pressed(), App::sendReady(), App::useShop()

### Community 1 - "IntRect"
Cohesion: 0.15
Nodes (14): Animation, firstFrame, frameCount, frameDuration, step, IntRect, height, left (+6 more)

### Community 2 - "string"
Cohesion: 0.12
Nodes (6): Window, _impl, handle(), App::handleBack(), App::loop(), App::run()

### Community 3 - "Components.hpp"
Cohesion: 0.12
Nodes (14): Charge, time, MaxHealth, hp, Missile, radius, MonsterTag, slot (+6 more)

### Community 4 - "Background.cpp"
Cohesion: 0.11
Nodes (19): Renderer, Background::drawAura(), Background::drawBand(), Background::drawBars(), Background::drawDebris(), Background::drawFade(), Background::drawNebula(), Background::drawSky() (+11 more)

### Community 5 - "Client Connection"
Cohesion: 0.06
Nodes (15): Client, _attempts, _connection, _events, _handshakeSequence, _lastAttempt, _server, _socket (+7 more)

### Community 6 - "vector"
Cohesion: 0.10
Nodes (5): SoundManager::Impl, buffers, ids, SoundManager::load(), TextureManager::load()

### Community 7 - "Component.hpp"
Cohesion: 0.10
Nodes (7): add(), create(), remove(), hash<Engine::Entity>, main(), parsePort(), parsePort()

### Community 8 - "Delivery and Sessions"
Cohesion: 0.10
Nodes (18): Delivery, Reliable, Unreliable, Server::admit(), Server::broadcast(), Server::checkSessions(), Server::clientCount(), Server::close() (+10 more)

### Community 9 - "Build and Protocol Docs"
Cohesion: 0.16
Nodes (17): engine_audio library, SFML::Audio, engine build root, engine_core library, Handshake and connection, Network module (engine::network), ReliableChannel, UDP wire format (+9 more)

### Community 10 - "Server Events"
Cohesion: 0.06
Nodes (11): Server, _events, _maxClients, _nextClientId, _random, _sessions, _socket, _version (+3 more)

### Community 11 - "Layers"
Cohesion: 0.07
Nodes (35): Game rules, Layers, Position, velocity, speed, direction, bulletStartPos(), isShooter(), addToPurse(), coinPickupSystem(), scaledCoin() (+27 more)

### Community 14 - "Players: charge, secondary weapon, drone"
Cohesion: 0.06
Nodes (34): Players: charge, secondary weapon, drone, addDrone(), boostDrone(), setDroneLevel(), twinDrone(), chargeFactor(), chargeRatio(), chargeSize() (+26 more)

### Community 15 - "Connection Channel"
Cohesion: 0.08
Nodes (10): Connection, _channel, _lastPing, _lastReceived, _outbox, _pingId, _pingSentAt, _received (+2 more)

### Community 16 - "Game"
Cohesion: 0.04
Nodes (41): Public API: the `Game` class, What the server reads, Game, _alive, _augments, _coinCounter, _countdown, _customZones (+33 more)

### Community 17 - "Address"
Cohesion: 0.14
Nodes (12): Address, ip, port, toAddress(), toEndpoint(), UdpSocket::Impl::Impl(), UdpSocket::Impl::store(), UdpSocket::port() (+4 more)

### Community 18 - ".updatePlaying"
Cohesion: 0.04
Nodes (25): Replica, _augments, _buyResult, CLOCK_CORRECTION_RATE, _cues, _customInfo, _customRecords, _entities (+17 more)

### Community 19 - "Hud"
Cohesion: 0.06
Nodes (24): Hud, AUGMENT_GAP, AUGMENT_PADDING, AUGMENT_TEXT_SCALE, BAR_HEIGHT, BAR_WIDTH, BAR_X, BOSS_BAR_WIDTH (+16 more)

### Community 20 - "Entity State Payload"
Cohesion: 0.12
Nodes (28): decodeAugments(), decodeBuy(), decodeBuyResult(), decodeHistory(), decodeInput(), decodeOwnState(), decodePause(), decodeReady() (+20 more)

### Community 21 - "Prediction"
Cohesion: 0.07
Nodes (16): Pending, dt, input, sequence, Prediction, _box, _input, MAX_PENDING (+8 more)

### Community 22 - "Binary Writer"
Cohesion: 0.09
Nodes (21): BinaryWriter, _data, MAX_VARINT, BinaryWriter::writeBytes(), BinaryWriter::writeFloat(), BinaryWriter::writeU16(), BinaryWriter::writeU32(), BinaryWriter::writeU8() (+13 more)

### Community 23 - "Packet"
Cohesion: 0.09
Nodes (15): Packet, _messages, RefuseReason, Full, Version, Client::checkConnection(), Client::flushConnection(), Client::handleHandshake() (+7 more)

### Community 24 - "Pending Messages"
Cohesion: 0.11
Nodes (8): Pending, message, sentAt, ReliableChannel, _early, _expected, _nextId, _pending

### Community 25 - "Replica"
Cohesion: 0.06
Nodes (23): Area, box, pos, GameView, ANIMATION_PHASE, _background, _hud, HURT_DURATION (+15 more)

### Community 26 - "BinaryReader"
Cohesion: 0.18
Nodes (8): BinaryReader::BinaryReader(), BinaryReader::readBytes(), BinaryReader::readFloat(), BinaryReader::readU16(), BinaryReader::readU32(), BinaryReader::readU8(), BinaryReader::readVarint(), BinaryReader::remaining()

### Community 27 - "DeathRuns.hpp"
Cohesion: 0.17
Nodes (19): addCourse(), addCoursePortal(), checkPattern(), Course, after, gapFrom, gapTo, motion (+11 more)

### Community 28 - "GameView"
Cohesion: 0.08
Nodes (16): App, _assets, _connectedSince, _controls, _custom, FRAMERATE, _last, LISTEN_DELAY (+8 more)

### Community 29 - "Collider"
Cohesion: 0.10
Nodes (41): applyContact(), canHit(), canStrike(), collectProjectiles(), collectTargets(), Collider, box, entity (+33 more)

### Community 30 - "ShopPanel"
Cohesion: 0.06
Nodes (23): ShopPanel, BORDER, CARD_GAP, CARD_HEIGHT, CARD_PADDING, CARD_TOP, CARD_WIDTH, COIN_GAP (+15 more)

### Community 31 - "LeviathanState"
Cohesion: 0.11
Nodes (25): Behaviours, dasherTarget(), DashTarget, pos, velocity, roamBoss(), LeviathanState, move (+17 more)

### Community 32 - "Audio Player Impl"
Cohesion: 0.18
Nodes (4): AudioPlayer::play(), AudioPlayer::playMusic(), AudioPlayer::setMusicVolume(), toSfmlVolume()

### Community 33 - "TEST"
Cohesion: 0.05
Nodes (7): escortFor(), escortSize(), findPlayerBySlot(), hpOf(), kill(), TEST(), with()

### Community 34 - "Registry"
Cohesion: 0.04
Nodes (82): CommandBuffer, _commands, Entity, id, eachRow(), EntityRecord, archetype, row (+74 more)

### Community 35 - "BackdropId"
Cohesion: 0.07
Nodes (19): BackdropId, Count, Girders, NebulaBlue, NebulaGreen, NebulaOrange, NebulaPurple, NebulaRed (+11 more)

### Community 36 - "Design Choices Docs"
Cohesion: 0.17
Nodes (16): Accessibility doc, Audio/visual accessibility (health bars, per-slot sprites; palette, contrast proposed), Cognitive accessibility (simple rules, short levels, clear feedback), rtype::config (Config.hpp constants), Motor accessibility (keyboard only done; remap, toggle fire, gamepad proposed), Choice: per-message reliability, Choice: event-based replication with NetId, Choice: sessions with token, ping, 5s timeout (+8 more)

### Community 38 - "AugmentStyle.hpp"
Cohesion: 0.40
Nodes (11): augmentInfo(), augmentLabel(), augmentName(), droneInfo(), percentLess(), rarityName(), skillInfo(), statInfo() (+3 more)

### Community 39 - "Network"
Cohesion: 0.09
Nodes (10): Network, CHUNKS_PER_UPDATE, _client, Network::Network(), _open, _upload, Upload, begun (+2 more)

### Community 40 - "Build Tooling Docs"
Cohesion: 0.10
Nodes (23): Dependency rules, engine::audio (sounds and music, wraps SFML), Game::setInput / Game::update(dt), Server App class (only place including both network and game), Server threads: network thread and game thread, vcpkg submodule (external/vcpkg, manifest vcpkg.json, Asio + SFML 3), Technical and comparative study doc, Choice: AABB collision (+15 more)

### Community 41 - "EntitySpawned"
Cohesion: 0.09
Nodes (17): EntityDestroyed, id, EntitySpawned, box, id, pos, slot, sprite (+9 more)

### Community 42 - "Reliable Channel"
Cohesion: 0.23
Nodes (5): ReliableChannel::acknowledge(), ReliableChannel::receive(), ReliableChannel::resend(), ReliableChannel::send(), ReliableChannel::takeReady()

### Community 43 - "Shop.hpp"
Cohesion: 0.16
Nodes (13): AugmentDef, apply, id, price, rarity, isExcluded(), pickWeighted(), rarityWeight() (+5 more)

### Community 44 - "Zone"
Cohesion: 0.12
Nodes (15): Who knows whom, canTakeFormation(), crowded(), scatter(), scatterLasers(), scatterLimit(), shapesOf(), shapeWaves() (+7 more)

### Community 45 - "Phase"
Cohesion: 0.11
Nodes (11): Audio, _dir, _enabled, _ids, _phase, _player, SHOP_SOUND_COUNT, _shopIds (+3 more)

### Community 46 - "Network protocol doc"
Cohesion: 0.22
Nodes (19): Architecture doc, Build and run doc, CMake presets linux-debug / windows-debug (Ninja on Linux), Build troubleshooting table, Contributing doc, Branch prefixes and commit types, Coding conventions (80 cols, 25-line functions, 5 params, 2 nested ifs, no ternary, _ private prefix), Part ownership (engine/core, server, client, game logic each have an owner) (+11 more)

### Community 47 - "Architecture Targets"
Cohesion: 0.14
Nodes (19): r-type_client (display executable), Client replica of the world (entity kind + position), engine::core (Registry, Scheduler, CommandBuffer, FixedTimestep), engine::network (UDP transport, sessions, ping, reliability, wraps Asio), engine::window (rendering, wraps SFML), Three decoupled layers: rendering, networking, game logic, rtype::common (shared headers: Vec2, return codes), rtype::game (header-only game logic) (+11 more)

### Community 48 - "HudLobby.cpp"
Cohesion: 0.10
Nodes (14): MapEntry, error, file, hash, map, firstVisible(), Hud::drawCustomDetail(), Hud::drawCustomList() (+6 more)

### Community 50 - "Segments"
Cohesion: 0.11
Nodes (17): firstLevel(), isDeathRun(), lastSpawnPoint(), levelAt(), levelSpan(), levelStart(), levelStartOf(), Segments (+9 more)

### Community 51 - "Key"
Cohesion: 0.10
Nodes (21): Key, A, Backspace, C, D, Down, E, Enter (+13 more)

### Community 52 - "addGates"
Cohesion: 0.09
Nodes (35): Obstacles, PlayerEffects, inverted, lastPortal, speedFactor, portal(), wall(), addCave() (+27 more)

### Community 53 - "MonsterKind"
Cohesion: 0.11
Nodes (18): MonsterType, kind, EscortSpawn, delay, kind, y, MonsterKind, Basic (+10 more)

### Community 54 - "map3"
Cohesion: 0.23
Nodes (9): Overview, buildRun(), map2(), gauntlet(), map3(), minefield(), pinch(), rhythm() (+1 more)

### Community 55 - "Layers"
Cohesion: 0.05
Nodes (50): Architecture, Coding conventions, Collisions, Components, Configuration, Data oriented design, Kinds of monsters and bosses, Overview (+42 more)

### Community 56 - "SpriteId"
Cohesion: 0.13
Nodes (7): Controls, _back, _held, _input, _pause, _pressed, _respawn

### Community 58 - "SpriteBank"
Cohesion: 0.10
Nodes (10): explosionCue(), grown(), Replica::applyPrediction(), Replica::bossSprite(), Replica::localPlayer(), Replica::mapCursor(), Replica::recordSpawnCue(), Replica::refreshSolids() (+2 more)

### Community 59 - "TEST"
Cohesion: 0.06
Nodes (12): createPlayer(), secondarySystem(), beams(), holdX(), laserShip(), releaseX(), tankAt(), TEST() (+4 more)

### Community 60 - "src/App.cpp"
Cohesion: 0.25
Nodes (6): AudioPlayer::Impl, music, musicVolume, next, soundVolume, voices

### Community 61 - "Background"
Cohesion: 0.07
Nodes (26): Background, _aura, _auraLevel, _boosted, _fade, _height, _last, LAYERS (+18 more)

### Community 62 - "Message Types"
Cohesion: 0.22
Nodes (5): Message, id, payload, reliable, type

### Community 63 - "CustomInfo"
Cohesion: 0.09
Nodes (28): Replica::takeUploadResult(), CustomInfo, hash, name, zones, CustomRecords, hash, records (+20 more)

### Community 64 - "BossKind"
Cohesion: 0.20
Nodes (10): BossType, kind, BossKind, Count, Dasher, Fortress, Standard, Summoner (+2 more)

### Community 65 - "Controls.cpp"
Cohesion: 0.13
Nodes (7): KeyPressed, key, KeyReleased, key, Controls::applyKey(), Controls::handle(), Controls::poll()

### Community 66 - "describeSpawn"
Cohesion: 0.15
Nodes (9): Packet::add(), Packet::isHandshake(), Packet::isValid(), Packet::Packet(), Packet::parseMessage(), Packet::sequence(), Packet::size(), Packet::sizeOf() (+1 more)

### Community 67 - "SpriteId"
Cohesion: 0.05
Nodes (41): isBossSprite(), Renderable, id, SpriteId, AlienShard, BeamOn, BeamWarn, Bomb (+33 more)

### Community 68 - "ObstacleKind"
Cohesion: 0.10
Nodes (21): ObstacleTag, kind, obstacleSprite(), isPortal(), ObstacleKind, Beam, BeamWarn, Count (+13 more)

### Community 69 - "Connection.cpp"
Cohesion: 0.13
Nodes (8): Connection::handle(), Connection::handlePong(), Connection::handleUser(), Connection::nextSequence(), Connection::readId(), Connection::sendId(), Connection::sendPing(), Connection::takeMessages()

### Community 70 - "Client.cpp"
Cohesion: 0.09
Nodes (13): DisconnectReason, Kicked, Quit, Shutdown, Timeout, Client::Client(), Client::close(), Client::flush() (+5 more)

### Community 71 - "Font"
Cohesion: 0.18
Nodes (8): Font, GLYPH_HEIGHT, GLYPH_WIDTH, find(), Font::drawGlyph(), Glyph, rows, symbol

### Community 72 - "SpriteBank"
Cohesion: 0.09
Nodes (10): TextureManager, _impl, SpriteBank, _assets, _backdrops, _clock, _sprites, Background::Background() (+2 more)

### Community 73 - "EntityState"
Cohesion: 0.12
Nodes (18): EntityState, id, pos, Prediction::reconcile(), Replica::record(), encodeStateChunk(), encodeStates(), InputMessage (+10 more)

### Community 74 - "WaveStep"
Cohesion: 0.15
Nodes (14): fittedBase(), fittedSpacing(), formationDelay(), formationSlot(), Slot, delay, y, WaveStep (+6 more)

### Community 75 - "BinaryReader"
Cohesion: 0.11
Nodes (14): BinaryReader, _cursor, isValidDifficulty(), PlayerStatus, alive, hp, lives, ready (+6 more)

### Community 77 - "LobbyMenu"
Cohesion: 0.08
Nodes (16): Client side, LobbyMenu, _draft, _forCustom, _scoresFromCustom, _shownRank, LobbyStep, Custom (+8 more)

### Community 78 - "TEST"
Cohesion: 0.05
Nodes (4): appendLevel(), buildMap(), shifted(), TEST()

### Community 79 - "Phase"
Cohesion: 0.29
Nodes (7): Phase, Defeat, Lobby, Playing, Shop, Victory, Hud::trackPhase()

### Community 80 - "TEST"
Cohesion: 0.07
Nodes (5): applyAugment(), findAugment(), priceOf(), TEST(), TEST()

### Community 81 - "Systems.hpp"
Cohesion: 0.05
Nodes (46): The tick: `Game::update`, bossSystem(), fan(), volley(), isFarOutOfBounds(), applyPortal(), beamKind(), crossPortals() (+38 more)

### Community 82 - "Sound Manager Impl"
Cohesion: 0.25
Nodes (3): SoundManager, _impl, AudioPlayer::AudioPlayer()

### Community 83 - "Reader"
Cohesion: 0.19
Nodes (9): Range, max, min, isPrintableText(), Reader, _json, _path, Reader::Reader() (+1 more)

### Community 84 - "MonsterDef"
Cohesion: 0.08
Nodes (25): each(), Add a component, Add a map or a level, Add a new kind of monster, Add a player action (a new key), Add a system, How-to, MonsterDef (+17 more)

### Community 85 - "TEST"
Cohesion: 0.14
Nodes (3): folder(), TEST(), write()

### Community 86 - "Connection Send"
Cohesion: 0.38
Nodes (3): Connection::flush(), Connection::send(), Connection::sendEngine()

### Community 88 - "SlotTable"
Cohesion: 0.22
Nodes (9): Sound, BossAlert, Coin, Count, Defeat, Explosion, Hit, Shoot (+1 more)

### Community 89 - "DisconnectReason"
Cohesion: 0.18
Nodes (10): Cue, BossAppears, BossExplosion, CoinPickup, Explosion, Hit, PlayerShot, Purchase (+2 more)

### Community 90 - "TEST"
Cohesion: 0.09
Nodes (7): failsWith(), freshFolder(), mapOf(), replaced(), TEST(), tinyLevel(), write()

### Community 91 - "GameStatus"
Cohesion: 0.08
Nodes (21): GameStatus, bossHp, bossMaxHp, coins, countdown, deaths, elapsedMs, level (+13 more)

### Community 92 - "Create your own map"
Cohesion: 0.18
Nodes (11): A complete example, Bosses, Create your own map, Limits, Play it, The leaderboard of the custom maps, Tips to make a good map, Waves of monsters (+3 more)

### Community 93 - "Hud::drawSettingsStep"
Cohesion: 0.11
Nodes (13): difficultyColor(), difficultyName(), multiplierText(), formatTime(), twoDigits(), entryName(), Hud::drawMapList(), Hud::drawRecords() (+5 more)

### Community 94 - "RunRecord"
Cohesion: 0.11
Nodes (12): addRecord(), Leaderboard, _history, _records, RunEntry, map, record, RunRecord (+4 more)

### Community 95 - "Zone"
Cohesion: 0.17
Nodes (13): Variety by zone, stormInterval(), Zone, Arena, Asteroids, Core, Count, DeepSpace (+5 more)

### Community 96 - ".operator()"
Cohesion: 0.40
Nodes (5): Install, Keep it up to date: the git hook, Knowledge graph (graphify), Use the graph, With an AI agent (Claude Code)

### Community 97 - "Track"
Cohesion: 0.14
Nodes (13): Track, BossDasher, BossFortress, BossStandard, BossSummoner, Core, Count, DeathRun (+5 more)

### Community 98 - "CLAUDE.md"
Cohesion: 0.12
Nodes (16): HistoryPage, entries, start, total, StatusHeader, bossHp, bossMaxHp, coins (+8 more)

### Community 100 - "LevelDef"
Cohesion: 0.14
Nodes (35): Current status, The first map: 20 levels, LevelDef, bosses, deathRun, length, obstacles, waves (+27 more)

### Community 102 - "App"
Cohesion: 0.09
Nodes (15): App, _custom, CUSTOM_LEADERBOARD_FILE, _game, _lastInputs, _lastPhase, _leaderboard, LEADERBOARD_FILE (+7 more)

### Community 103 - "UdpSocket::Impl"
Cohesion: 0.10
Nodes (12): UdpSocket::Impl::startReceive(), UdpSocket::Impl, buffer, BUFFER_SIZE, io, localPort, MAX_QUEUED, mutex (+4 more)

### Community 105 - "Archetype"
Cohesion: 0.26
Nodes (6): Archetype, _columns, _columnsIndex, _entities, NO_COLUMN, _signature

### Community 106 - "ObstacleStep"
Cohesion: 0.09
Nodes (26): addGate(), GateSpec, at, gapHeight, gapY, kind, motion, pinch (+18 more)

### Community 107 - "Audio.cpp"
Cohesion: 0.13
Nodes (7): Audio::follow(), Audio::onPhase(), Audio::play(), Audio::playCue(), Audio::playMusic(), Audio::playSound(), Audio::trackOfZone()

### Community 111 - "Gameplay"
Cohesion: 0.11
Nodes (18): Bosses, Charged shot, Coins, time and level skip, Controls, Difficulty and settings, Drone, Enemies, Gameplay (+10 more)

### Community 112 - "ComponentId"
Cohesion: 0.12
Nodes (9): ComponentInfo, infoOf(), swapRemove(), ComponentRegistry::getInfo(), ComponentRegistry::registerType(), RegistryData, ids, infos (+1 more)

### Community 113 - "The reader: `rtype::mapjson`"
Cohesion: 0.11
Nodes (21): Add a field, Add a kind of obstacle, Add a monster, a boss, a zone, Change the format, Custom maps: for developers, From the file to the match, How-to, Records (+13 more)

### Community 114 - "ShopPanel.cpp"
Cohesion: 0.11
Nodes (11): ShopPanel::affordable(), ShopPanel::confirm(), ShopPanel::drawCard(), ShopPanel::handle(), ShopPanel::hint(), ShopPanel::infoColumns(), ShopPanel::isOpen(), ShopPanel::selection() (+3 more)

### Community 116 - "BossHazards.hpp"
Cohesion: 0.22
Nodes (7): bossHazardSystem(), HazardClock, laser, meteor, time, hazardInterval(), spawnMeteor()

### Community 117 - "duration"
Cohesion: 0.40
Nodes (4): Connection::latency(), Connection::peerDisconnect(), Connection::resendDelay(), duration

### Community 118 - ".width"
Cohesion: 0.12
Nodes (16): Gates, Level, Map, Obstacle ranges, Obstacles, Portals, Reference, Scenery and death runs (+8 more)

### Community 119 - "TEST"
Cohesion: 0.10
Nodes (5): fireRocket(), hpOf(), missiles(), rocketAt(), TEST()

### Community 120 - "CustomMaps"
Cohesion: 0.14
Nodes (7): Lobby and maps, CustomMaps, _board, _current, CustomMaps::CustomMaps(), _recordsFile, _upload

### Community 121 - "Piece"
Cohesion: 0.17
Nodes (18): isPortalPiece(), kindOf(), parseObstacle(), Piece, FastPortal, Gate, Laser, Mine (+10 more)

### Community 122 - "GameView.cpp"
Cohesion: 0.06
Nodes (34): Sprite, animation, texture, Color, a, b, g, r (+26 more)

### Community 124 - "LifeBook"
Cohesion: 0.19
Nodes (4): LifeBook, _down, SLOTS, _timers

### Community 125 - "TEST"
Cohesion: 0.05
Nodes (8): _shopOffers, clearEnemies(), gameAtTheEndOfALevel(), reachShop(), runFor(), shopWithCoins(), TEST(), twoLevels()

### Community 126 - "StrikeSpec"
Cohesion: 0.17
Nodes (12): Strike, deadly, spec, time, StrikeSpec, active, height, vy (+4 more)

### Community 127 - "Column"
Cohesion: 0.38
Nodes (3): Column, _data, _info

### Community 128 - "LeviathanMove"
Cohesion: 0.25
Nodes (8): LeviathanMove, Aim, Columns, Dash, Rest, Return, Sweep, Windup

### Community 129 - "LaserCycle"
Cohesion: 0.50
Nodes (4): LaserCycle, age, beam, hasBeam

### Community 130 - "Signature"
Cohesion: 0.32
Nodes (3): Signature, WORD_BITS, _words

### Community 131 - "CustomMapList"
Cohesion: 0.11
Nodes (6): CustomMapList, _entries, _folder, _message, Hud::drawScoresStep(), Hud::drawCustomScores()

### Community 132 - "AugmentId"
Cohesion: 0.12
Nodes (17): AugmentId, Count, DamageUp, Drone, DroneBoost, DroneTwin, FireRateUp, Laser (+9 more)

### Community 133 - "Aura"
Cohesion: 0.13
Nodes (13): Aura, Dasher, Fortress, None, Standard, Summoner, Scene, aura (+5 more)

### Community 134 - "PlayerInput"
Cohesion: 0.10
Nodes (12): PlayerInput, alt, down, fire, left, right, up, Prediction::advance() (+4 more)

### Community 135 - "Case"
Cohesion: 0.33
Nodes (4): Case, name, run, Registrar

### Community 136 - ".restartDeathRun"
Cohesion: 0.16
Nodes (3): countBefore(), levelEnd(), collectObstacles()

### Community 137 - "MapUpload"
Cohesion: 0.18
Nodes (7): MapUpload, _owner, _size, _text, bytes(), truncated(), uploaded()

### Community 139 - "TEST"
Cohesion: 0.12
Nodes (10): Difficulty, Brutal, Easy, Hard, Normal, Settings, difficulty, friendlyFire (+2 more)

### Community 140 - "parseLevel"
Cohesion: 0.18
Nodes (12): parseLevel(), readBosses(), readObstacles(), readWaves(), ParsedLevel, level, zone, checkReach() (+4 more)

### Community 141 - "CustomLeaderboard"
Cohesion: 0.18
Nodes (3): CustomLeaderboard, MAX_MAPS, _records

### Community 142 - "Resized"
Cohesion: 0.50
Nodes (3): Resized, height, width

### Community 143 - "Reader.hpp"
Cohesion: 0.29
Nodes (6): choice(), choiceOr(), Name, name, value, namesOf()

### Community 144 - "WeaponKind"
Cohesion: 0.16
Nodes (11): equippedWeapon(), weaponOf(), WeaponKind, Count, Laser, Lightning, Rocket, SecondaryDef (+3 more)

### Community 145 - "Regeneration"
Cohesion: 0.50
Nodes (4): Regeneration, hp, interval, timer

### Community 146 - "convert"
Cohesion: 0.18
Nodes (7): FocusGained, FocusLost, convert(), toKey(), translate(), Window::clear(), Window::pollEvent()

### Community 147 - "Drone.hpp"
Cohesion: 0.27
Nodes (12): centerOfBox(), cooldownOf(), createDrone(), droneFire(), dronePerch(), droneSystem(), followOwner(), hasDrone() (+4 more)

### Community 148 - "App::useCustomStep"
Cohesion: 0.32
Nodes (3): App::useCustomScoresPage(), App::useCustomStep(), App::useMapStep()

### Community 149 - "Message"
Cohesion: 0.15
Nodes (9): Connected, client, Disconnected, client, reason, Message, client, payload (+1 more)

### Community 150 - "CoinCounter"
Cohesion: 0.33
Nodes (5): CoinCounter, quantity, total, coins, value

### Community 151 - "CustomMapList.cpp"
Cohesion: 0.17
Nodes (5): CustomMapList::cursor(), CustomMapList::CustomMapList(), CustomMapList::readSelected(), CustomMapList::selected(), CustomMapList::setMessage()

### Community 152 - "time_point"
Cohesion: 0.18
Nodes (5): Connection::Connection(), Connection::receive(), Connection::timedOut(), Connection::token(), Connection::update()

### Community 153 - "ObstacleMotion"
Cohesion: 0.40
Nodes (5): ObstacleMotion, age, amplitude, baseY, omega

### Community 154 - "Steps"
Cohesion: 0.18
Nodes (10): Decisions already made, Method, Open points to settle step by step, Roadmap: weapons, Steps, W1. Charged shot on the base weapon, W2. Secondary weapon slot (done), W3. Secondary weapons (+2 more)

### Community 155 - "ComponentId"
Cohesion: 0.20
Nodes (5): ComponentId, id, ComponentRegistry, getId(), idOf()

### Community 156 - "CustomMap"
Cohesion: 0.20
Nodes (8): CustomMap, author, levels, name, zones, ParseResult, error, map

### Community 157 - "checkTotals"
Cohesion: 0.27
Nodes (7): checkTotals(), countAll(), tooMany(), Totals, bosses, monsters, obstacles

### Community 159 - "Folder.cpp"
Cohesion: 0.40
Nodes (5): isJson(), listJson(), loadEntry(), readMapFile(), scanMaps()

### Community 160 - "Datagram"
Cohesion: 0.22
Nodes (5): Datagram, bytes, from, Client::handleDatagram(), Server::handleDatagram()

### Community 161 - "DifficultyScale"
Cohesion: 0.20
Nodes (10): DifficultyScale, allyDamage, density, economy, enemyDamage, enemyFireRate, enemyHp, extraOffers (+2 more)

### Community 163 - ".handle"
Cohesion: 0.25
Nodes (4): Refused, reason, Unreachable, applyDecoded()

### Community 164 - "Sample"
Cohesion: 0.22
Nodes (5): Sample, pos, tick, Replica::interpolate(), Replica::update()

### Community 165 - "Rarity"
Cohesion: 0.22
Nodes (9): Rarity, Common, Epic, Legendary, Rare, RarityOdds, bossBonus, perLevel (+1 more)

### Community 167 - "EntityManager"
Cohesion: 0.29
Nodes (3): EntityManager, _freeList, _generations

### Community 168 - "FixedTimestep"
Cohesion: 0.25
Nodes (3): FixedTimestep, _accumulator, _maxAccumulator

### Community 169 - "Session"
Cohesion: 0.25
Nodes (6): Session, address, connection, id, Server::findByAddress(), Server::findByToken()

### Community 170 - "TextureManager::Impl"
Cohesion: 0.25
Nodes (3): TextureManager::Impl, ids, textures

### Community 172 - "LoadedMap"
Cohesion: 0.29
Nodes (3): LoadedMap, hash, map

### Community 173 - "Hud::drawVictoryStats"
Cohesion: 0.33
Nodes (3): count, clock(), Hud::drawVictoryStats()

### Community 174 - "ShopSound"
Cohesion: 0.40
Nodes (5): ShopSound, Accept, Count, Refuse, Audio::playShopSound()

### Community 175 - "How a map is built"
Cohesion: 0.50
Nodes (4): Distances and time, How a map is built, The end of a map, The screen

### Community 176 - "MonsterState"
Cohesion: 0.50
Nodes (4): MonsterState, age, locked, summoned

### Community 179 - "Shield"
Cohesion: 0.67
Nodes (3): Shield, cooldown, timer

### Community 180 - "TeamId"
Cohesion: 0.67
Nodes (3): TeamId, Monster, Player

### Community 181 - "TurretFire"
Cohesion: 0.67
Nodes (3): TurretFire, interval, timer

## Knowledge Gaps
- **858 isolated node(s):** `_impl`, `voices`, `next`, `music`, `soundVolume` (+853 more)
  These have ≤1 connection - possible missing edges or undocumented components. (Counts symbols only; 1805 node(s) total have ≤1 connection when file, concept and rationale nodes are included.)
- **23 thin communities (<3 nodes) omitted from report** — run `graphify query` to explore isolated nodes.

## Suggested Questions
_Questions this graph is uniquely positioned to answer:_

- **Why does `Replica` connect `.updatePlaying` to `CustomMapList`, `AugmentId`, `Aura`, `PlayerInput`, `Components.hpp`, `Hud`, `Prediction`, `Replica`, `GameView`, `ShopPanel`, `Registry`, `.handle`, `Sample`, `Network`, `EntitySpawned`, `Hud::drawCustomStep`, `Hud::drawVictoryStats`, `HudLobby.cpp`, `SpriteBank`, `CustomInfo`, `LobbyMenu`, `DisconnectReason`, `GameStatus`, `Hud::drawSettingsStep`, `RunRecord`, `Track`, `Audio.cpp`, `ShopPanel.cpp`, `.width`, `GameView.cpp`?**
  _High betweenness centrality (0.111) - this node is a cross-community bridge._
- **Are the 9 inferred relationships involving `Registry` (e.g. with `TEST()` and `TEST()`) actually correct?**
  _`Registry` has 9 INFERRED edges - model-reasoned connections that need verification._
- **What connects `_impl`, `voices`, `next` to the rest of the system?**
  _858 weakly-connected nodes found - possible documentation gaps or missing edges._
- **Should `src/App.cpp` be split into smaller, more focused modules?**
  _Cohesion score 0.08695652173913043 - nodes in this community are weakly interconnected._
- **Why does `Game` connect `Game` to `Components.hpp`, `AugmentId`, `.restartDeathRun`, `Layers`, `Game Tests`, `Players: charge, secondary weapon, drone`, `TEST`, `Registry`, `Segments`, `TEST`, `WaveStep`, `Phase`, `TEST`, `Systems.hpp`, `Zone`, `.isUsedSlot`, `App`, `BossHazards.hpp`, `TEST`, `LifeBook`, `TEST`?**
  _High betweenness centrality (0.104) - this node is a cross-community bridge._
- **Are the 80 inferred relationships involving `TEST()` (e.g. with `Registry` and `.alive()`) actually correct?**
  _`TEST()` has 80 INFERRED edges - model-reasoned connections that need verification._
- **Should `IntRect` be split into smaller, more focused modules?**
  _Cohesion score 0.14619883040935672 - nodes in this community are weakly interconnected._