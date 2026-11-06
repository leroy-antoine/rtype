# Graph Report - RTYPE  (2026-11-06)

## Corpus Check
- 182 files · ~146,979 words
- Verdict: corpus is large enough that graph structure adds value.
- Unclassified: 5 file(s) not represented in the graph (top: (none) 5)

## Summary
- 3318 nodes · 7006 edges · 180 communities (151 shown, 29 thin omitted)
- Extraction: 86% EXTRACTED · 14% INFERRED · 0% AMBIGUOUS · INFERRED: 957 edges (avg confidence: 0.88)
- Token cost: 0 input · 0 output

## Graph Freshness
- Built from commit: `e2e4d5ac`
- Run `git rev-parse HEAD` and compare to check if the graph is stale.
- Run `graphify update .` after code changes (no API cost).

## Community Hubs (Navigation)
- src/App.cpp
- IntRect
- string
- Components.hpp
- Background.cpp
- Client Connection
- string
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
- RegistryData
- map3.cpp
- Layers
- SpriteId
- Window.cpp
- SpriteBank
- CustomMapProtocol.hpp
- src/App.cpp
- Background
- Message Types
- LobbyMenu.cpp
- BossKind
- Controls.cpp
- describeSpawn
- SpriteId
- TEST
- Piece
- DisconnectReason
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
- Systems.hpp
- Sound Manager Impl
- Reader
- MonsterDef
- LobbyMenu
- Connection Send
- CustomLeaderboard
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
- MonsterKind
- EntityManager
- bossHazardSystem
- Prediction.cpp
- Shop.hpp
- TextureManager::Impl
- MapUpload
- Vector2f
- CustomMaps
- FixedTimestep
- TEST
- MapJsonTests.cpp
- ObstacleKind
- LeviathanMove
- Client::send
- TEST
- CommandBuffer
- HudLobby.cpp
- Message
- MapError
- .width
- HudVictory.cpp
- Aura
- Column
- StrikeSpec
- Signature
- window/Event.hpp
- Create your own map
- RefuseReason
- Regeneration
- GameTests.cpp
- ComponentId
- Folder.cpp
- Client side
- PlayerInput
- TimeFormat.hpp
- ObstacleMotion
- Case
- size_t
- Connection::handle
- Hud::drawVictoryStats
- Upload
- duration
- ShopSound
- Pending
- .startMap
- LaserCycle
- MonsterState
- wrap
- Hud::drawEffects
- Shield
- TeamId
- TurretFire
- CustomMaps/README.md

## God Nodes (most connected - your core abstractions)
1. `TEST()` - 197 edges
2. `Registry` - 164 edges
3. `Game` - 141 edges
4. `Entity` - 130 edges
5. `Replica` - 87 edges
6. `Hud` - 59 edges
7. `TEST()` - 59 edges
8. `LevelDef` - 55 edges
9. `App` - 54 edges
10. `TEST()` - 54 edges

## Surprising Connections (you probably didn't know these)
- `Add a kind of obstacle` --references--> `kindOf()`  [INFERRED]
  docs/custom-maps-internals.md → src/client/game/Rtype/src/mapjson/ObstacleParser.cpp
- `Add a field` --references--> `Reader`  [INFERRED]
  docs/custom-maps-internals.md → src/client/game/Rtype/src/mapjson/Reader.hpp
- `What you need` --references--> `CustomMaps`  [INFERRED]
  docs/custom-maps.md → src/server/game/CustomMaps.hpp
- `Your first map in two minutes` --references--> `CustomMaps`  [INFERRED]
  docs/custom-maps.md → src/server/game/CustomMaps.hpp
- `Overview` --references--> `Segments`  [INFERRED]
  docs/custom-maps-internals.md → src/client/game/Rtype/include/logic/Levels.hpp

## Import Cycles
- None detected.

## Communities (180 total, 29 thin omitted)

### Community 0 - "src/App.cpp"
Cohesion: 0.10
Nodes (7): App::App(), App::isReady(), App::listening(), App::playSounds(), App::sendReady(), App::useCustomMenu(), App::useShop()

### Community 1 - "IntRect"
Cohesion: 0.15
Nodes (14): Animation, firstFrame, frameCount, frameDuration, step, IntRect, height, left (+6 more)

### Community 2 - "string"
Cohesion: 0.13
Nodes (5): Window, _impl, handle(), App::loop(), App::run()

### Community 3 - "Components.hpp"
Cohesion: 0.14
Nodes (10): MaxHealth, hp, MonsterTag, slot, MultiShot, bullets, ObstacleTag, kind (+2 more)

### Community 4 - "Background.cpp"
Cohesion: 0.09
Nodes (23): Renderer, Color, a, b, g, r, Star, layer (+15 more)

### Community 5 - "Client Connection"
Cohesion: 0.06
Nodes (15): Client, _attempts, _connection, _events, _handshakeSequence, _lastAttempt, _server, _socket (+7 more)

### Community 6 - "string"
Cohesion: 0.10
Nodes (5): SoundManager::Impl, buffers, ids, SoundManager::load(), TextureManager::load()

### Community 7 - "Component.hpp"
Cohesion: 0.16
Nodes (5): add(), create(), remove(), hash<Engine::Entity>, ReliableChannel::acknowledge()

### Community 8 - "Delivery and Sessions"
Cohesion: 0.12
Nodes (14): Server::admit(), Server::broadcast(), Server::checkSessions(), Server::clientCount(), Server::close(), Server::disconnect(), Server::find(), Server::kick() (+6 more)

### Community 9 - "Build and Protocol Docs"
Cohesion: 0.16
Nodes (17): engine_audio library, SFML::Audio, engine build root, engine_core library, Handshake and connection, Network module (engine::network), ReliableChannel, UDP wire format (+9 more)

### Community 10 - "Server Events"
Cohesion: 0.06
Nodes (10): Server, _events, _maxClients, _nextClientId, _sessions, _socket, _version, Server::handleConnect() (+2 more)

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
Nodes (30): What the server reads, countBefore(), Game, _alive, _augments, _coinCounter, _countdown, _customZones (+22 more)

### Community 17 - "Address"
Cohesion: 0.11
Nodes (15): Address, ip, port, Datagram, bytes, from, toAddress(), toEndpoint() (+7 more)

### Community 18 - ".updatePlaying"
Cohesion: 0.05
Nodes (21): Replica, _augments, _buyResult, CLOCK_CORRECTION_RATE, _cues, _customInfo, _customRecords, _entities (+13 more)

### Community 19 - "Hud"
Cohesion: 0.07
Nodes (22): Hud, AUGMENT_GAP, AUGMENT_PADDING, AUGMENT_TEXT_SCALE, BAR_HEIGHT, BAR_WIDTH, BAR_X, BOSS_BAR_WIDTH (+14 more)

### Community 20 - "Entity State Payload"
Cohesion: 0.15
Nodes (30): decodeAugments(), decodeBuy(), decodeBuyResult(), decodeDestroy(), decodeInput(), decodeLeaderboard(), decodeOwnState(), decodePause() (+22 more)

### Community 21 - "Prediction"
Cohesion: 0.09
Nodes (11): Prediction, _box, _input, MAX_PENDING, _nextSequence, _offset, _pending, _predicted (+3 more)

### Community 22 - "Binary Writer"
Cohesion: 0.09
Nodes (15): BinaryWriter, _data, MAX_VARINT, BinaryWriter::writeBytes(), BinaryWriter::writeFloat(), BinaryWriter::writeU16(), BinaryWriter::writeU32(), BinaryWriter::writeU8() (+7 more)

### Community 23 - "Packet"
Cohesion: 0.09
Nodes (13): Packet, _messages, Client::checkConnection(), Client::flushConnection(), Client::handleDatagram(), Client::handleHandshake(), Client::handleSession(), Client::latency() (+5 more)

### Community 24 - "Pending Messages"
Cohesion: 0.11
Nodes (8): Pending, message, sentAt, ReliableChannel, _early, _expected, _nextId, _pending

### Community 25 - "Replica"
Cohesion: 0.08
Nodes (18): GameView, ANIMATION_PHASE, _background, _hud, HURT_DURATION, HURT_SHAKE, _hurtUntil, _lastBossHp (+10 more)

### Community 26 - "BinaryReader"
Cohesion: 0.20
Nodes (8): BinaryReader::BinaryReader(), BinaryReader::readBytes(), BinaryReader::readFloat(), BinaryReader::readU16(), BinaryReader::readU32(), BinaryReader::readU8(), BinaryReader::readVarint(), BinaryReader::remaining()

### Community 27 - "DeathRuns.hpp"
Cohesion: 0.09
Nodes (35): addCourse(), addCoursePortal(), checkPattern(), Course, after, gapFrom, gapTo, motion (+27 more)

### Community 28 - "GameView"
Cohesion: 0.10
Nodes (15): App, _assets, _connectedSince, _controls, FRAMERATE, _last, LISTEN_DELAY, _menu (+7 more)

### Community 29 - "Collider"
Cohesion: 0.09
Nodes (29): applyContact(), canHit(), collectProjectiles(), collectTargets(), Collider, box, entity, pos (+21 more)

### Community 30 - "ShopPanel"
Cohesion: 0.07
Nodes (20): ShopPanel, BORDER, CARD_GAP, CARD_HEIGHT, CARD_TOP, CARD_WIDTH, COIN_GAP, COIN_SIZE (+12 more)

### Community 31 - "Leviathan.hpp"
Cohesion: 0.08
Nodes (32): Behaviours, Kinds of monsters and bosses, BossTag, age, summonTimer, LeviathanState, move, step (+24 more)

### Community 32 - "Audio Player Impl"
Cohesion: 0.18
Nodes (4): AudioPlayer::play(), AudioPlayer::playMusic(), AudioPlayer::setMusicVolume(), toSfmlVolume()

### Community 33 - "TEST"
Cohesion: 0.10
Nodes (4): fan(), volley(), shotVelocities(), TEST()

### Community 34 - "Registry"
Cohesion: 0.05
Nodes (63): Entity, id, eachRow(), has(), Registry, _archetypes, EMPTY_ARCHETYPE, _entities (+55 more)

### Community 35 - "BackdropId"
Cohesion: 0.12
Nodes (16): BackdropId, Count, Girders, NebulaBlue, NebulaGreen, NebulaOrange, NebulaPurple, NebulaRed (+8 more)

### Community 36 - "Design Choices Docs"
Cohesion: 0.17
Nodes (16): Accessibility doc, Audio/visual accessibility (health bars, per-slot sprites; palette, contrast proposed), Cognitive accessibility (simple rules, short levels, clear feedback), rtype::config (Config.hpp constants), Motor accessibility (keyboard only done; remap, toggle fire, gamepad proposed), Choice: per-message reliability, Choice: event-based replication with NetId, Choice: sessions with token, ping, 5s timeout (+8 more)

### Community 38 - "Phase"
Cohesion: 0.29
Nodes (7): Phase, Defeat, Lobby, Playing, Shop, Victory, Hud::trackPhase()

### Community 39 - "Replica Events"
Cohesion: 0.12
Nodes (7): Unreachable, applyDecoded(), Network, CHUNKS_PER_UPDATE, _client, _open, _upload

### Community 40 - "Build Tooling Docs"
Cohesion: 0.10
Nodes (23): Dependency rules, engine::audio (sounds and music, wraps SFML), Game::setInput / Game::update(dt), Server App class (only place including both network and game), Server threads: network thread and game thread, vcpkg submodule (external/vcpkg, manifest vcpkg.json, Asio + SFML 3), Technical and comparative study doc, Choice: AABB collision (+15 more)

### Community 41 - "Reader.cpp"
Cohesion: 0.12
Nodes (22): Range, max, min, isPrintableText(), find(), Reader::allow(), Reader::fail(), Reader::find() (+14 more)

### Community 42 - "Reliable Channel"
Cohesion: 0.31
Nodes (4): ReliableChannel::receive(), ReliableChannel::resend(), ReliableChannel::send(), ReliableChannel::takeReady()

### Community 43 - "Shop.hpp"
Cohesion: 0.12
Nodes (19): applyAugment(), AugmentDef, apply, id, price, rarity, AugmentId, Count (+11 more)

### Community 44 - "Sample"
Cohesion: 0.22
Nodes (5): Sample, pos, tick, Replica::interpolate(), Replica::update()

### Community 45 - "Phase"
Cohesion: 0.09
Nodes (12): Audio, _dir, _enabled, _ids, _phase, _player, SHOP_SOUND_COUNT, _shopIds (+4 more)

### Community 46 - "Network protocol doc"
Cohesion: 0.16
Nodes (21): Architecture doc, Build and run doc, CMake presets linux-debug / windows-debug (Ninja on Linux), Build troubleshooting table, Contributing doc, Branch prefixes and commit types, Coding conventions (80 cols, 25-line functions, 5 params, 2 nested ifs, no ternary, _ private prefix), Part ownership (engine/core, server, client, game logic each have an owner) (+13 more)

### Community 47 - "Architecture Targets"
Cohesion: 0.16
Nodes (17): r-type_client (display executable), Client replica of the world (entity kind + position), engine::core (Registry, Scheduler, CommandBuffer, FixedTimestep), engine::network (UDP transport, sessions, ping, reliability, wraps Asio), engine::window (rendering, wraps SFML), Three decoupled layers: rendering, networking, game logic, rtype::common (shared headers: Vec2, return codes), rtype::game (header-only game logic) (+9 more)

### Community 48 - "TEST"
Cohesion: 0.09
Nodes (7): failsWith(), freshFolder(), mapOf(), replaced(), TEST(), tinyLevel(), write()

### Community 49 - "TEST"
Cohesion: 0.05
Nodes (3): Leaderboard, _records, TEST()

### Community 50 - "Segments"
Cohesion: 0.10
Nodes (18): firstLevel(), isDeathRun(), lastSpawnPoint(), levelAt(), levelEnd(), levelSpan(), levelStart(), levelStartOf() (+10 more)

### Community 51 - "Key"
Cohesion: 0.10
Nodes (21): Key, A, Backspace, C, D, Down, E, Enter (+13 more)

### Community 52 - "addGates"
Cohesion: 0.10
Nodes (31): Motion, amplitude, period, addCave(), addGates(), addRun(), addSweepers(), CavePiece (+23 more)

### Community 53 - "RegistryData"
Cohesion: 0.18
Nodes (5): ComponentRegistry::registerType(), RegistryData, ids, infos, mutex

### Community 54 - "map3.cpp"
Cohesion: 0.54
Nodes (6): gauntlet(), map3(), minefield(), pinch(), rhythm(), run()

### Community 55 - "Layers"
Cohesion: 0.06
Nodes (51): Architecture, Collisions, Data oriented design, Layers, Position, velocity, speed, direction, Shooting, Systems, The ECS rules to remember (+43 more)

### Community 56 - "SpriteId"
Cohesion: 0.15
Nodes (6): Controls, _held, _input, _pause, _pressed, _respawn

### Community 57 - "Window.cpp"
Cohesion: 0.17
Nodes (6): convert(), toKey(), translate(), Window::clear(), Window::pollEvent(), Window::Window()

### Community 58 - "SpriteBank"
Cohesion: 0.13
Nodes (12): explosionCue(), Replica::applyPrediction(), Replica::bossSprite(), Replica::localPlayer(), Replica::mapCursor(), Replica::record(), Replica::recordSpawnCue(), Replica::refreshSolids() (+4 more)

### Community 59 - "CustomMapProtocol.hpp"
Cohesion: 0.13
Nodes (23): CustomInfo, hash, name, zones, CustomRecords, hash, records, decodeCustomInfo() (+15 more)

### Community 60 - "src/App.cpp"
Cohesion: 0.24
Nodes (6): AudioPlayer::Impl, music, musicVolume, next, soundVolume, voices

### Community 61 - "Background"
Cohesion: 0.06
Nodes (25): Background, _aura, _auraLevel, _boosted, _fade, _height, _last, LAYERS (+17 more)

### Community 62 - "Message Types"
Cohesion: 0.22
Nodes (5): Message, id, payload, reliable, type

### Community 63 - "LobbyMenu.cpp"
Cohesion: 0.11
Nodes (12): LobbyMenu::cursor(), LobbyMenu::LobbyMenu(), LobbyMenu::moveCursor(), LobbyMenu::navigate(), LobbyMenu::openRecords(), LobbyMenu::readSelected(), LobbyMenu::selected(), LobbyMenu::setMessage() (+4 more)

### Community 64 - "BossKind"
Cohesion: 0.20
Nodes (10): BossType, kind, BossKind, Count, Dasher, Fortress, Standard, Summoner (+2 more)

### Community 65 - "Controls.cpp"
Cohesion: 0.18
Nodes (5): KeyReleased, key, Controls::applyKey(), Controls::handle(), Controls::poll()

### Community 66 - "describeSpawn"
Cohesion: 0.12
Nodes (11): Packet::add(), Packet::isHandshake(), Packet::isValid(), Packet::Packet(), Packet::parse(), Packet::parseMessage(), Packet::sequence(), Packet::serialize() (+3 more)

### Community 67 - "SpriteId"
Cohesion: 0.06
Nodes (35): isBossSprite(), Renderable, id, SpriteId, AlienShard, BeamOn, BeamWarn, Bomb (+27 more)

### Community 68 - "TEST"
Cohesion: 0.11
Nodes (3): bytes(), TEST(), truncated()

### Community 69 - "Piece"
Cohesion: 0.17
Nodes (18): isPortalPiece(), kindOf(), parseObstacle(), Piece, FastPortal, Gate, Laser, Mine (+10 more)

### Community 70 - "DisconnectReason"
Cohesion: 0.20
Nodes (9): DisconnectReason, Kicked, Quit, Shutdown, Timeout, Client::close(), Client::flush(), Client::sendDisconnect() (+1 more)

### Community 71 - "Renderer.cpp"
Cohesion: 0.12
Nodes (12): Font, GLYPH_HEIGHT, GLYPH_WIDTH, Font::draw(), Font::drawCentered(), Font::drawGlyph(), Font::drawRow(), Font::Font() (+4 more)

### Community 72 - "SpriteBank"
Cohesion: 0.06
Nodes (14): TextureManager, _impl, SpriteBank, _assets, _backdrops, _clock, _sprites, Background::Background() (+6 more)

### Community 73 - "EntityState"
Cohesion: 0.19
Nodes (11): EntityState, id, pos, encodeStateChunk(), encodeStates(), InputMessage, input, sequence (+3 more)

### Community 74 - "WaveStep"
Cohesion: 0.13
Nodes (15): fittedBase(), fittedSpacing(), formationDelay(), formationSlot(), Slot, delay, y, WaveStep (+7 more)

### Community 75 - "BinaryReader"
Cohesion: 0.12
Nodes (11): BinaryReader, _cursor, PlayerStatus, alive, hp, ready, slot, Hud::drawReady() (+3 more)

### Community 77 - "MapEntry"
Cohesion: 0.14
Nodes (9): MapEntry, error, file, hash, map, Hud::textTop(), Hud::drawCustomRow(), Hud::drawLines() (+1 more)

### Community 79 - "Rarity"
Cohesion: 0.13
Nodes (14): augmentLabel(), rarityColor(), rarityName(), Rarity, Common, Epic, Legendary, Rare (+6 more)

### Community 80 - "stdexcept"
Cohesion: 0.14
Nodes (3): main(), parsePort(), parsePort()

### Community 81 - "Systems.hpp"
Cohesion: 0.08
Nodes (36): Coding conventions, Components, Configuration, Level skip, Overview, R-Type game logic, The tick: `Game::update`, bossSystem() (+28 more)

### Community 82 - "Sound Manager Impl"
Cohesion: 0.25
Nodes (3): SoundManager, _impl, AudioPlayer::AudioPlayer()

### Community 83 - "Reader"
Cohesion: 0.24
Nodes (12): parseLevel(), readBosses(), readObstacles(), readWaves(), Reader, _json, _path, checkReach() (+4 more)

### Community 84 - "MonsterDef"
Cohesion: 0.11
Nodes (18): each(), Add a component, Add a map or a level, Add a new kind of monster, Add a system, How-to, addMonsterStats(), flyStraight() (+10 more)

### Community 85 - "LobbyMenu"
Cohesion: 0.11
Nodes (9): LobbyMenu, _entries, _folder, _message, _recordsWanted, Tab, Custom, Maps (+1 more)

### Community 86 - "Connection Send"
Cohesion: 0.38
Nodes (3): Connection::flush(), Connection::send(), Connection::sendEngine()

### Community 87 - "CustomLeaderboard"
Cohesion: 0.14
Nodes (7): CustomLeaderboard, MAX_MAPS, _records, addRecord(), RunRecord, players, timeMs

### Community 88 - "SlotTable"
Cohesion: 0.22
Nodes (9): Sound, BossAlert, Coin, Count, Defeat, Explosion, Hit, Shoot (+1 more)

### Community 89 - "DisconnectReason"
Cohesion: 0.14
Nodes (11): Cue, BossAppears, BossExplosion, CoinPickup, Explosion, Hit, PlayerShot, Purchase (+3 more)

### Community 90 - "Connection.cpp"
Cohesion: 0.14
Nodes (11): Connection::Connection(), Connection::handlePong(), Connection::handleUser(), Connection::hasHandshakeMessage(), Connection::nextSequence(), Connection::readId(), Connection::sendPing(), Connection::takeMessages() (+3 more)

### Community 91 - "GameStatus"
Cohesion: 0.08
Nodes (18): GameStatus, bossHp, bossMaxHp, coins, countdown, deaths, elapsedMs, level (+10 more)

### Community 92 - "Reader.hpp"
Cohesion: 0.23
Nodes (9): ParsedLevel, level, zone, choice(), choiceOr(), Name, name, value (+1 more)

### Community 93 - "Replica::apply"
Cohesion: 0.08
Nodes (20): EntityDestroyed, id, EntitySpawned, box, id, pos, slot, sprite (+12 more)

### Community 95 - "MapParser.cpp"
Cohesion: 0.13
Nodes (19): Add a field, Add a kind of obstacle, Add a monster, a boss, a zone, Change the format, Custom maps: for developers, From the file to the match, How-to, Records (+11 more)

### Community 96 - ".operator()"
Cohesion: 0.40
Nodes (5): Install, Keep it up to date: the git hook, Knowledge graph (graphify), Use the graph, With an AI agent (Claude Code)

### Community 97 - "Track"
Cohesion: 0.14
Nodes (13): Track, BossDasher, BossFortress, BossStandard, BossSummoner, Core, Count, DeathRun (+5 more)

### Community 98 - "CLAUDE.md"
Cohesion: 0.17
Nodes (10): isValid(), StatusHeader, bossHp, bossMaxHp, coins, deaths, elapsedMs, level (+2 more)

### Community 99 - "findPlayerBySlot"
Cohesion: 0.10
Nodes (7): Game rules, Maps and lobby, CoinCounter, quantity, total, coins, value

### Community 100 - "LevelDef"
Cohesion: 0.14
Nodes (35): Current status, The first map: 20 levels, LevelDef, bosses, deathRun, length, obstacles, waves (+27 more)

### Community 102 - "App"
Cohesion: 0.10
Nodes (15): App, _custom, CUSTOM_LEADERBOARD_FILE, _game, _lastInputs, _lastPhase, _leaderboard, LEADERBOARD_FILE (+7 more)

### Community 103 - "UdpSocket::Impl"
Cohesion: 0.10
Nodes (12): UdpSocket::Impl::startReceive(), UdpSocket::Impl, buffer, BUFFER_SIZE, io, localPort, MAX_QUEUED, mutex (+4 more)

### Community 105 - "Archetype"
Cohesion: 0.24
Nodes (6): Archetype, _columns, _columnsIndex, _entities, NO_COLUMN, _signature

### Community 106 - "Zone"
Cohesion: 0.08
Nodes (27): Who knows whom, Variety by zone, canTakeFormation(), crowded(), scatter(), scatterLasers(), scatterLimit(), shapesOf() (+19 more)

### Community 107 - "Audio.cpp"
Cohesion: 0.17
Nodes (6): Audio::Audio(), Audio::onPhase(), Audio::playCue(), Audio::playMusic(), Audio::playSound(), Audio::trackOfZone()

### Community 110 - "GateSpec"
Cohesion: 0.22
Nodes (9): GateSpec, at, gapHeight, gapY, kind, motion, pinch, speed (+1 more)

### Community 111 - "Gameplay"
Cohesion: 0.15
Nodes (13): Bosses, Coins, time and level skip, Controls, Enemies, Gameplay, Goal, Leaderboard, Music and scenery (+5 more)

### Community 112 - "buildMap"
Cohesion: 0.18
Nodes (6): Overview, appendLevel(), buildMap(), buildRun(), shifted(), map2()

### Community 113 - "MonsterKind"
Cohesion: 0.12
Nodes (17): MonsterType, kind, PendingSpawn, delay, kind, y, MonsterKind, Basic (+9 more)

### Community 114 - "EntityManager"
Cohesion: 0.29
Nodes (3): EntityManager, _freeList, _generations

### Community 116 - "bossHazardSystem"
Cohesion: 0.15
Nodes (9): bossHazardSystem(), HazardClock, laser, meteor, time, hazardInterval(), spawnMeteor(), collectObstacles() (+1 more)

### Community 118 - "Prediction.cpp"
Cohesion: 0.18
Nodes (4): Prediction::advance(), Prediction::position(), Prediction::record(), Prediction::velocity()

### Community 119 - "Shop.hpp"
Cohesion: 0.29
Nodes (8): isExcluded(), pickWeighted(), rarityWeight(), rollOffers(), shopPool(), ShopTier, bossLevel, level

### Community 120 - "TextureManager::Impl"
Cohesion: 0.25
Nodes (3): TextureManager::Impl, ids, textures

### Community 121 - "MapUpload"
Cohesion: 0.23
Nodes (5): MapUpload, _owner, _size, _text, uploaded()

### Community 122 - "Vector2f"
Cohesion: 0.08
Nodes (27): Sprite, animation, texture, Vector2f, x, y, Renderer::draw(), Renderer::drawRect() (+19 more)

### Community 123 - "CustomMaps"
Cohesion: 0.11
Nodes (10): Lobby and maps, CustomMaps, _board, _current, CustomMaps::CustomMaps(), _recordsFile, _upload, LoadedMap (+2 more)

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
Cohesion: 0.08
Nodes (37): Obstacles, PlayerEffects, inverted, lastPortal, speedFactor, isPortal(), ObstacleKind, Beam (+29 more)

### Community 128 - "LeviathanMove"
Cohesion: 0.25
Nodes (8): LeviathanMove, Aim, Columns, Dash, Rest, Return, Sweep, Windup

### Community 129 - "Client::send"
Cohesion: 0.20
Nodes (5): Delivery, Reliable, Unreliable, Client::Client(), Client::send()

### Community 130 - "TEST"
Cohesion: 0.18
Nodes (3): folder(), TEST(), write()

### Community 133 - "HudLobby.cpp"
Cohesion: 0.10
Nodes (8): firstVisible(), Hud::drawCustomDetail(), Hud::drawCustomList(), Hud::drawCustomRecords(), Hud::drawLobby(), Hud::drawLobbyFooter(), Hud::drawMapList(), Hud::drawTabs()

### Community 134 - "Message"
Cohesion: 0.15
Nodes (9): Connected, client, Disconnected, client, reason, Message, client, payload (+1 more)

### Community 135 - "MapError"
Cohesion: 0.21
Nodes (9): checkTotals(), countAll(), tooMany(), Totals, bosses, monsters, obstacles, MapError (+1 more)

### Community 136 - ".width"
Cohesion: 0.27
Nodes (7): Gates, Obstacles, Portals, Solid pieces, Hud::drawCoins(), Hud::drawLevel(), Hud::drawTimer()

### Community 137 - "HudVictory.cpp"
Cohesion: 0.40
Nodes (3): confettiColor(), Hud::drawConfetti(), Hud::timeColor()

### Community 138 - "Aura"
Cohesion: 0.13
Nodes (13): Aura, Dasher, Fortress, None, Standard, Summoner, Scene, aura (+5 more)

### Community 139 - "Column"
Cohesion: 0.18
Nodes (7): Column, _data, _info, ComponentInfo, infoOf(), swapRemove(), ComponentRegistry::getInfo()

### Community 140 - "StrikeSpec"
Cohesion: 0.17
Nodes (12): Strike, deadly, spec, time, StrikeSpec, active, height, vy (+4 more)

### Community 141 - "Signature"
Cohesion: 0.32
Nodes (3): Signature, WORD_BITS, _words

### Community 142 - "window/Event.hpp"
Cohesion: 0.20
Nodes (8): Closed, FocusGained, FocusLost, KeyPressed, key, Resized, height, width

### Community 143 - "Create your own map"
Cohesion: 0.10
Nodes (19): A complete example, Bosses, Create your own map, Distances and time, How a map is built, Level, Limits, Map (+11 more)

### Community 144 - "RefuseReason"
Cohesion: 0.40
Nodes (5): Refused, reason, RefuseReason, Full, Version

### Community 145 - "Regeneration"
Cohesion: 0.50
Nodes (4): Regeneration, hp, interval, timer

### Community 146 - "GameTests.cpp"
Cohesion: 0.24
Nodes (5): kill(), killBoss(), playerOf(), run(), skipToBoss()

### Community 147 - "ComponentId"
Cohesion: 0.22
Nodes (5): ComponentId, id, ComponentRegistry, getId(), idOf()

### Community 148 - "Folder.cpp"
Cohesion: 0.47
Nodes (5): isJson(), listJson(), loadEntry(), readMapFile(), scanMaps()

### Community 149 - "Client side"
Cohesion: 0.18
Nodes (3): Client side, App::useMenu(), Network::Network()

### Community 150 - "PlayerInput"
Cohesion: 0.18
Nodes (9): Add a player action (a new key), Public API: the `Game` class, PlayerInput, down, fire, left, right, up (+1 more)

### Community 151 - "TimeFormat.hpp"
Cohesion: 0.43
Nodes (3): formatTime(), twoDigits(), Hud::drawRecords()

### Community 153 - "ObstacleMotion"
Cohesion: 0.40
Nodes (5): ObstacleMotion, age, amplitude, baseY, omega

### Community 154 - "Case"
Cohesion: 0.33
Nodes (4): Case, name, run, Registrar

### Community 155 - "size_t"
Cohesion: 0.40
Nodes (4): EntityRecord, archetype, row, IterationGuard

### Community 157 - "Hud::drawVictoryStats"
Cohesion: 0.33
Nodes (3): clock(), Hud::drawVictoryStats(), count

### Community 158 - "Upload"
Cohesion: 0.33
Nodes (4): Upload, begun, sent, text

### Community 159 - "duration"
Cohesion: 0.40
Nodes (4): Connection::latency(), Connection::peerDisconnect(), Connection::resendDelay(), duration

### Community 160 - "ShopSound"
Cohesion: 0.40
Nodes (5): ShopSound, Accept, Count, Refuse, Audio::playShopSound()

### Community 161 - "Pending"
Cohesion: 0.40
Nodes (4): Pending, dt, input, sequence

### Community 163 - "LaserCycle"
Cohesion: 0.50
Nodes (4): LaserCycle, age, beam, hasBeam

### Community 164 - "MonsterState"
Cohesion: 0.50
Nodes (4): MonsterState, age, locked, summoned

### Community 167 - "Shield"
Cohesion: 0.67
Nodes (3): Shield, cooldown, timer

### Community 168 - "TeamId"
Cohesion: 0.67
Nodes (3): TeamId, Monster, Player

### Community 169 - "TurretFire"
Cohesion: 0.67
Nodes (3): TurretFire, interval, timer

## Knowledge Gaps
- **779 isolated node(s):** `WINDOW_WIDTH`, `WINDOW_HEIGHT`, `FRAMERATE`, `WORLD_TO_SCREEN`, `LISTEN_DELAY` (+774 more)
  These have ≤1 connection - possible missing edges or undocumented components. (Counts symbols only; 1551 node(s) total have ≤1 connection when file, concept and rationale nodes are included.)
- **29 thin communities (<3 nodes) omitted from report** — run `graphify query` to explore isolated nodes.

## Suggested Questions
_Questions this graph is uniquely positioned to answer:_

- **Why does `Game` connect `Game` to `Components.hpp`, `Game Tests`, `GameTests.cpp`, `PlayerInput`, `Collider`, `Registry`, `.startMap`, `Phase`, `Shop.hpp`, `Segments`, `WaveStep`, `Systems.hpp`, `findPlayerBySlot`, `App`, `Zone`, `MonsterKind`, `bossHazardSystem`, `Shop.hpp`, `TEST`?**
  _High betweenness centrality (0.117) - this node is a cross-community bridge._
- **Are the 77 inferred relationships involving `TEST()` (e.g. with `Registry` and `.alive()`) actually correct?**
  _`TEST()` has 77 INFERRED edges - model-reasoned connections that need verification._
- **What connects `WINDOW_WIDTH`, `WINDOW_HEIGHT`, `FRAMERATE` to the rest of the system?**
  _779 weakly-connected nodes found - possible documentation gaps or missing edges._
- **Should `src/App.cpp` be split into smaller, more focused modules?**
  _Cohesion score 0.09523809523809523 - nodes in this community are weakly interconnected._
- **Why does `TEST()` connect `Game Tests` to `Game`, `GameTests.cpp`, `DeathRuns.hpp`, `Collider`, `Leviathan.hpp`, `TEST`, `Registry`, `.startMap`, `Shop.hpp`, `TEST`, `Segments`, `Layers`, `Systems.hpp`, `findPlayerBySlot`, `LevelDef`, `Zone`, `buildMap`, `bossHazardSystem`, `TEST`, `ObstacleKind`?**
  _High betweenness centrality (0.084) - this node is a cross-community bridge._
- **Are the 2 inferred relationships involving `Registry` (e.g. with `TEST()` and `TEST()`) actually correct?**
  _`Registry` has 2 INFERRED edges - model-reasoned connections that need verification._
- **Should `IntRect` be split into smaller, more focused modules?**
  _Cohesion score 0.14619883040935672 - nodes in this community are weakly interconnected._