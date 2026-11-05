# Graph Report - RTYPE  (2026-11-05)

## Corpus Check
- 128 files · ~62,877 words
- Verdict: corpus is large enough that graph structure adds value.
- Unclassified: 5 file(s) not represented in the graph (top: (none) 5)

## Summary
- 1955 nodes · 3957 edges · 118 communities (100 shown, 18 thin omitted)
- Extraction: 88% EXTRACTED · 12% INFERRED · 0% AMBIGUOUS · INFERRED: 478 edges (avg confidence: 0.87)
- Token cost: 0 input · 0 output

## Graph Freshness
- Built from commit: `cac5053a`
- Run `git rev-parse HEAD` and compare to check if the graph is stale.
- Run `graphify update .` after code changes (no API cost).

## Community Hubs (Navigation)
- ECS Column Storage
- IntRect
- Window
- TEST
- Registry
- Client Connection
- rtype/Protocol.hpp
- make_sounds.py
- Delivery and Sessions
- Build and Protocol Docs
- Server Events
- Vec2
- Game Tests
- ShopPanel
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
- BinaryReader
- .updatePlaying
- App
- Collider
- Delivery
- Packet.cpp
- Audio Player Impl
- ShopPanel.cpp
- Command Buffer
- App
- Design Choices Docs
- Audio Player
- Connection Impl
- Replica Events
- Build Tooling Docs
- game/App.cpp
- Reliable Channel
- AugmentId
- SpriteBank
- Audio
- Network protocol doc
- Architecture Targets
- EntitySpawned
- TEST
- Background
- Key
- Prediction.cpp
- EntityState
- PlayerInput
- shotVelocities
- window/Event.hpp
- .update
- Replica.cpp
- Message
- Audio Impl
- findPlayerBySlot
- Message Types
- PlayerStatus
- SpriteId
- Window.cpp
- R-Type game logic
- BulletLook
- time_point
- Connection::handle
- Session Info
- Renderer Draw
- Renderer
- Texture Manager Impl
- WaveStep
- rollOffers
- Window Impl Header
- TEST
- GameStatus
- Components
- Test Harness
- Audio.cpp
- Sound Manager Impl
- Entity Manager
- BinaryWriter.cpp
- Controls
- Connection Send
- Font.cpp
- Sound
- Cue
- Sample
- GameView.cpp
- Font
- CoinCounter
- Phase
- Knowledge graph (graphify)
- Track
- Rarity
- .addPlayer
- App::App
- FixedTimestep
- Color
- DisconnectReason
- pull_request_template.md
- .load
- Star
- PendingSpawn
- CLAUDE.md
- vcpkg.json
- Hud::drawPlayers
- Connection::readId
- Client::latency
- ShopSound
- Regeneration
- Network::Network

## God Nodes (most connected - your core abstractions)
1. `Registry` - 100 edges
2. `Game` - 87 edges
3. `Entity` - 86 edges
4. `TEST()` - 62 edges
5. `Replica` - 57 edges
6. `TEST()` - 49 edges
7. `Connection` - 43 edges
8. `Server` - 43 edges
9. `AugmentId` - 43 edges
10. `TEST()` - 42 edges

## Surprising Connections (you probably didn't know these)
- `Use the graph` --references--> `Entity`  [INFERRED]
  docs/graphify.md → engine/core/include/engine/core/Entity.hpp
- `Use the graph` --references--> `Connection`  [INFERRED]
  docs/graphify.md → engine/network/include/engine/network/Connection.hpp
- `Add a system` --references--> `each()`  [INFERRED]
  src/client/game/Rtype/doc/README.md → engine/core/include/engine/core/Registry.hpp
- `The ECS rules to remember` --references--> `each()`  [INFERRED]
  src/client/game/Rtype/doc/README.md → engine/core/include/engine/core/Registry.hpp
- `coinPickupSystem()` --references--> `Entity`  [INFERRED]
  src/client/game/Rtype/include/logic/Collectibles.hpp → engine/core/include/engine/core/Entity.hpp

## Import Cycles
- None detected.

## Hyperedges (group relationships)
- **Reliable game messages (SPAWN, DESTROY, YOU, RESPAWN)** — docs_protocol_spawn, docs_protocol_destroy, docs_protocol_you, docs_protocol_respawn [EXTRACTED 1.00]
- **Server glue: network + game + core via App** — docs_architecture_server_app_glue, docs_architecture_engine_network, docs_architecture_rtype_game [EXTRACTED 1.00]
- **Unreliable game messages (INPUT, STATES, STATUS)** — docs_protocol_input, docs_protocol_states, docs_protocol_status [EXTRACTED 1.00]

## Communities (118 total, 18 thin omitted)

### Community 0 - "ECS Column Storage"
Cohesion: 0.05
Nodes (26): Archetype, _columns, _columnsIndex, _entities, NO_COLUMN, _signature, Column, _data (+18 more)

### Community 1 - "IntRect"
Cohesion: 0.17
Nodes (13): Animation, firstFrame, frameCount, frameDuration, step, IntRect, height, left (+5 more)

### Community 2 - "Window"
Cohesion: 0.13
Nodes (5): Window, _impl, handle(), App::loop(), App::run()

### Community 3 - "TEST"
Cohesion: 0.07
Nodes (5): _shopOffers, clearEnemies(), reachShop(), shopWithCoins(), TEST()

### Community 4 - "Registry"
Cohesion: 0.07
Nodes (29): eachRow(), EntityRecord, archetype, row, IterationGuard, Registry, _archetypes, EMPTY_ARCHETYPE (+21 more)

### Community 5 - "Client Connection"
Cohesion: 0.05
Nodes (17): Client, _attempts, _connection, _events, _handshakeSequence, _lastAttempt, _server, _socket (+9 more)

### Community 6 - "rtype/Protocol.hpp"
Cohesion: 0.05
Nodes (16): SoundManager::Impl, buffers, ids, SoundManager::load(), isNewer(), TextureManager::load(), MaxHealth, hp (+8 more)

### Community 7 - "make_sounds.py"
Cohesion: 0.23
Nodes (21): boss_alert(), decay(), defeat(), drums(), encode_ogg(), explosion(), fade(), hit() (+13 more)

### Community 8 - "Delivery and Sessions"
Cohesion: 0.12
Nodes (16): RefuseReason, Full, Version, Server::admit(), Server::broadcast(), Server::checkSessions(), Server::clientCount(), Server::close() (+8 more)

### Community 9 - "Build and Protocol Docs"
Cohesion: 0.14
Nodes (20): engine_audio library, SFML::Audio, engine build root, engine_core library, Game protocol, Handshake and connection, Network module (engine::network), ReliableChannel (+12 more)

### Community 10 - "Server Events"
Cohesion: 0.06
Nodes (10): Server, _events, _maxClients, _nextClientId, _sessions, _socket, _version, Server::handleConnect() (+2 more)

### Community 11 - "Vec2"
Cohesion: 0.23
Nodes (13): Layers, Position, velocity, speed, direction, bossSystem(), isShooter(), createBullet(), calculatePos(), movementSystem(), length() (+5 more)

### Community 12 - "Game Tests"
Cohesion: 0.07
Nodes (5): kill(), killBoss(), playerOf(), skipToBoss(), TEST()

### Community 13 - "ShopPanel"
Cohesion: 0.08
Nodes (19): ShopPanel, BORDER, CARD_GAP, CARD_HEIGHT, CARD_TOP, CARD_WIDTH, COIN_GAP, COIN_SIZE (+11 more)

### Community 14 - "Entity Registry"
Cohesion: 0.16
Nodes (9): add(), create(), remove(), Entity, id, hash<Engine::Entity>, has(), remove() (+1 more)

### Community 15 - "Connection Channel"
Cohesion: 0.08
Nodes (10): Connection, _channel, _lastPing, _lastReceived, _outbox, _pingId, _pingSentAt, _received (+2 more)

### Community 16 - "Game"
Cohesion: 0.07
Nodes (20): What the server reads, Game, _augments, _bossSpawned, _coinCounter, _countdown, _events, _level (+12 more)

### Community 17 - "UdpSocket::Impl"
Cohesion: 0.05
Nodes (27): Address, ip, port, Datagram, bytes, from, toAddress(), toEndpoint() (+19 more)

### Community 18 - "Replica"
Cohesion: 0.07
Nodes (15): Replica, _augments, _buyResult, CLOCK_CORRECTION_RATE, _cues, _entities, INTERPOLATION_DELAY, _latestTick (+7 more)

### Community 19 - "Hud"
Cohesion: 0.09
Nodes (20): Hud, AUGMENT_GAP, AUGMENT_PADDING, AUGMENT_TEXT_SCALE, BAR_HEIGHT, BAR_WIDTH, BAR_X, BOSS_BAR_WIDTH (+12 more)

### Community 20 - "Entity State Payload"
Cohesion: 0.14
Nodes (20): decodeAugments(), decodeBuy(), decodeInput(), decodeOwnState(), decodeReady(), decodeShop(), decodeSpawn(), decodeStates() (+12 more)

### Community 21 - "Prediction"
Cohesion: 0.09
Nodes (13): Pending, dt, input, sequence, Prediction, _input, MAX_PENDING, _nextSequence (+5 more)

### Community 22 - "Binary Writer"
Cohesion: 0.14
Nodes (14): BinaryWriter, _data, MAX_VARINT, BinaryWriter::writeFloat(), BinaryWriter::writeU32(), Packet::serialize(), Server::sendAccept(), encode() (+6 more)

### Community 23 - "Handshake Sequencing"
Cohesion: 0.09
Nodes (13): Packet, _messages, Client::checkConnection(), Client::flush(), Client::flushConnection(), Client::handleDatagram(), Client::handleHandshake(), Client::handleSession() (+5 more)

### Community 24 - "Pending Messages"
Cohesion: 0.11
Nodes (8): Pending, message, sentAt, ReliableChannel, _early, _expected, _nextId, _pending

### Community 25 - "GameView"
Cohesion: 0.15
Nodes (10): GameView, ANIMATION_PHASE, _background, _hud, PLAYER_FRAME_DOWN, PLAYER_FRAME_IDLE, PLAYER_FRAME_UP, _worldToScreen (+2 more)

### Community 26 - "BinaryReader"
Cohesion: 0.12
Nodes (10): BinaryReader, _cursor, BinaryReader::BinaryReader(), BinaryReader::readBytes(), BinaryReader::readFloat(), BinaryReader::readU16(), BinaryReader::readU32(), BinaryReader::readU8() (+2 more)

### Community 27 - ".updatePlaying"
Cohesion: 0.21
Nodes (11): The tick: `Game::update`, collectMonsters(), collectPlayers(), collectProjectileEntities(), contactSystem(), deleteUnitsOutOfBounds(), regenerationSystem(), shootingSystem() (+3 more)

### Community 28 - "App"
Cohesion: 0.08
Nodes (17): App, _assets, _connectedSince, _controls, FRAMERATE, _last, LISTEN_DELAY, _network (+9 more)

### Community 29 - "Collider"
Cohesion: 0.14
Nodes (24): applyContact(), canHit(), collectProjectiles(), collectTargets(), Collider, box, entity, pos (+16 more)

### Community 30 - "Delivery"
Cohesion: 0.20
Nodes (5): Delivery, Reliable, Unreliable, Client::Client(), Client::send()

### Community 31 - "Packet.cpp"
Cohesion: 0.15
Nodes (9): Packet::add(), Packet::isHandshake(), Packet::isValid(), Packet::Packet(), Packet::parseMessage(), Packet::sequence(), Packet::size(), Packet::sizeOf() (+1 more)

### Community 32 - "Audio Player Impl"
Cohesion: 0.15
Nodes (5): AudioPlayer::AudioPlayer(), AudioPlayer::play(), AudioPlayer::playMusic(), AudioPlayer::setMusicVolume(), toSfmlVolume()

### Community 33 - "ShopPanel.cpp"
Cohesion: 0.16
Nodes (6): ShopPanel::affordable(), ShopPanel::confirm(), ShopPanel::draw(), ShopPanel::handle(), ShopPanel::isOpen(), ShopPanel::showResult()

### Community 34 - "Command Buffer"
Cohesion: 0.14
Nodes (5): CommandBuffer, _commands, Scheduler, _commands, _systems

### Community 35 - "App"
Cohesion: 0.12
Nodes (10): App, _game, _lastInputs, _sentAugments, _sentOffers, _server, _slots, STATUS_EVERY_TICKS (+2 more)

### Community 36 - "Design Choices Docs"
Cohesion: 0.20
Nodes (14): Audio/visual accessibility (health bars, per-slot sprites; palette, contrast proposed), rtype::config (Config.hpp constants), Motor accessibility (keyboard only done; remap, toggle fire, gamepad proposed), Choice: per-message reliability, Choice: event-based replication with NetId, Choice: sessions with token, ping, 5s timeout, DESTROY 0x21 (S->C, reliable, entity id), Connection lifecycle / Handshake (Connect, Accept(token), Refuse, YOU, SPAWN, Ping, timeout 5s) (+6 more)

### Community 38 - "Connection Impl"
Cohesion: 0.19
Nodes (9): Connection::Connection(), Connection::handleUser(), Connection::hasHandshakeMessage(), Connection::latency(), Connection::peerDisconnect(), Connection::resendDelay(), Connection::sendPing(), Connection::token() (+1 more)

### Community 39 - "Replica Events"
Cohesion: 0.15
Nodes (7): Refused, reason, Unreachable, applyDecoded(), Network, _client, _open

### Community 40 - "Build Tooling Docs"
Cohesion: 0.10
Nodes (23): Dependency rules, engine::audio (sounds and music, wraps SFML), Game::setInput / Game::update(dt), Server App class (only place including both network and game), Server threads: network thread and game thread, vcpkg submodule (external/vcpkg, manifest vcpkg.json, Asio + SFML 3), Technical and comparative study doc, Choice: AABB collision (+15 more)

### Community 42 - "Reliable Channel"
Cohesion: 0.23
Nodes (5): ReliableChannel::acknowledge(), ReliableChannel::receive(), ReliableChannel::resend(), ReliableChannel::send(), ReliableChannel::takeReady()

### Community 43 - "AugmentId"
Cohesion: 0.12
Nodes (19): applyAugment(), AugmentDef, apply, id, price, rarity, AugmentId, Count (+11 more)

### Community 44 - "SpriteBank"
Cohesion: 0.14
Nodes (7): Sprite, animation, texture, TextureManager, _impl, SpriteBank::set(), SpriteBank::SpriteBank()

### Community 45 - "Audio"
Cohesion: 0.11
Nodes (11): Audio, _dir, _enabled, _ids, _phase, _player, SHOP_SOUND_COUNT, _shopIds (+3 more)

### Community 46 - "Network protocol doc"
Cohesion: 0.17
Nodes (20): Accessibility doc, Cognitive accessibility (simple rules, short levels, clear feedback), Architecture doc, Build and run doc, CMake presets linux-debug / windows-debug (Ninja on Linux), Build troubleshooting table, Contributing doc, Branch prefixes and commit types (+12 more)

### Community 47 - "Architecture Targets"
Cohesion: 0.16
Nodes (17): r-type_client (display executable), Client replica of the world (entity kind + position), engine::core (Registry, Scheduler, CommandBuffer, FixedTimestep), engine::network (UDP transport, sessions, ping, reliability, wraps Asio), engine::window (rendering, wraps SFML), Three decoupled layers: rendering, networking, game logic, rtype::common (shared headers: Vec2, return codes), rtype::game (header-only game logic) (+9 more)

### Community 48 - "EntitySpawned"
Cohesion: 0.15
Nodes (12): bulletStartPos(), EntitySpawned, box, id, pos, slot, sprite, GameView::drawEntity() (+4 more)

### Community 49 - "TEST"
Cohesion: 0.08
Nodes (5): BuyResult, id, ok, decodeBuyResult(), TEST()

### Community 50 - "Background"
Cohesion: 0.20
Nodes (7): Background, LAYERS, _stars, STARS_PER_LAYER, _width, _worldToScreen, Background::draw()

### Community 51 - "Key"
Cohesion: 0.10
Nodes (20): Key, A, C, D, Down, E, Enter, Escape (+12 more)

### Community 52 - "Prediction.cpp"
Cohesion: 0.22
Nodes (5): Prediction::reconcile(), Prediction::record(), Prediction::step(), Prediction::velocity(), Vec2Velocity

### Community 53 - "EntityState"
Cohesion: 0.10
Nodes (19): EntityDestroyed, id, EntityState, id, pos, Replica::apply(), Replica::record(), decodeDestroy() (+11 more)

### Community 54 - "PlayerInput"
Cohesion: 0.25
Nodes (8): Add a player action (a new key), Public API: the `Game` class, PlayerInput, down, fire, left, right, up

### Community 55 - "shotVelocities"
Cohesion: 0.15
Nodes (15): Collisions, Data oriented design, Shooting, Systems, Damage, value, Health, hp (+7 more)

### Community 56 - "window/Event.hpp"
Cohesion: 0.11
Nodes (13): Closed, FocusGained, FocusLost, KeyPressed, key, KeyReleased, key, Resized (+5 more)

### Community 57 - ".update"
Cohesion: 0.20
Nodes (5): inputDirection(), lifetimeSystem(), playerControlSystem(), run(), runFor()

### Community 58 - "Replica.cpp"
Cohesion: 0.19
Nodes (7): explosionCue(), Replica::applyPrediction(), Replica::localPlayer(), Replica::predict(), Replica::recordSpawnCue(), Replica::slot(), Replica::takeBuyResult()

### Community 59 - "Message"
Cohesion: 0.15
Nodes (9): Connected, client, Disconnected, client, reason, Message, client, payload (+1 more)

### Community 60 - "Audio Impl"
Cohesion: 0.25
Nodes (6): AudioPlayer::Impl, music, musicVolume, next, soundVolume, voices

### Community 62 - "Message Types"
Cohesion: 0.22
Nodes (5): Message, id, payload, reliable, type

### Community 63 - "PlayerStatus"
Cohesion: 0.18
Nodes (9): PlayerStatus, alive, hp, ready, slot, Hud::drawCoins(), Hud::drawReady(), ShopPanel::drawPrice() (+1 more)

### Community 64 - "SpriteId"
Cohesion: 0.18
Nodes (11): Renderable, id, SpriteId, Boss, Coin, Count, Explosion, Monster (+3 more)

### Community 65 - "Window.cpp"
Cohesion: 0.17
Nodes (6): convert(), toKey(), translate(), Window::clear(), Window::pollEvent(), Window::Window()

### Community 66 - "R-Type game logic"
Cohesion: 0.09
Nodes (20): each(), Add a component, Add a new kind of monster, Add a system, Architecture, Coding conventions, Configuration, Current status (+12 more)

### Community 67 - "BulletLook"
Cohesion: 0.20
Nodes (10): bulletDirection(), BulletLook, height, sprite, width, Team, id, TeamId (+2 more)

### Community 68 - "time_point"
Cohesion: 0.25
Nodes (3): Connection::receive(), Connection::timedOut(), Connection::update()

### Community 70 - "Session Info"
Cohesion: 0.14
Nodes (10): _random, Session, address, connection, id, Server::findByAddress(), Server::findByToken(), Server::handleDatagram() (+2 more)

### Community 71 - "Renderer Draw"
Cohesion: 0.19
Nodes (9): Vector2f, x, y, Renderer::draw(), Renderer::drawRect(), Renderer::drawResized(), Renderer::Renderer(), Font::drawRow() (+1 more)

### Community 72 - "Renderer"
Cohesion: 0.20
Nodes (5): Renderer, SpriteBank, _sprites, Hud::Hud(), ShopPanel::ShopPanel()

### Community 73 - "Texture Manager Impl"
Cohesion: 0.25
Nodes (3): TextureManager::Impl, ids, textures

### Community 74 - "WaveStep"
Cohesion: 0.33
Nodes (6): WaveStep, at, count, dy, interval, y

### Community 75 - "rollOffers"
Cohesion: 0.29
Nodes (8): isExcluded(), pickWeighted(), rarityWeight(), rollOffers(), shopPool(), ShopTier, bossLevel, level

### Community 77 - "TEST"
Cohesion: 0.09
Nodes (6): bossFan(), fan(), isReadyToShoot(), shoot(), shotVelocities(), TEST()

### Community 78 - "GameStatus"
Cohesion: 0.13
Nodes (13): GameStatus, bossHp, bossMaxHp, coins, countdown, phase, players, App::isReady() (+5 more)

### Community 79 - "Components"
Cohesion: 0.18
Nodes (9): Components, BossTag, age, MonsterTag, slot, NetId, value, ProjectileTag (+1 more)

### Community 80 - "Test Harness"
Cohesion: 0.33
Nodes (4): Case, name, run, Registrar

### Community 81 - "Audio.cpp"
Cohesion: 0.18
Nodes (5): Audio::follow(), Audio::onPhase(), Audio::play(), Audio::playCue(), Audio::playSound()

### Community 83 - "Entity Manager"
Cohesion: 0.29
Nodes (3): EntityManager, _freeList, _generations

### Community 84 - "BinaryWriter.cpp"
Cohesion: 0.28
Nodes (4): BinaryWriter::writeBytes(), BinaryWriter::writeU16(), BinaryWriter::writeU8(), BinaryWriter::writeVarint()

### Community 85 - "Controls"
Cohesion: 0.22
Nodes (5): Controls, _held, _input, _pressed, _respawn

### Community 86 - "Connection Send"
Cohesion: 0.28
Nodes (4): Connection::flush(), Connection::send(), Connection::sendEngine(), Connection::sendId()

### Community 87 - "Font.cpp"
Cohesion: 0.28
Nodes (4): Font::draw(), Font::drawCentered(), Font::Font(), Font::width()

### Community 88 - "Sound"
Cohesion: 0.22
Nodes (9): Sound, BossAlert, Coin, Count, Defeat, Explosion, Hit, Shoot (+1 more)

### Community 89 - "Cue"
Cohesion: 0.18
Nodes (10): Cue, BossAppears, BossExplosion, CoinPickup, Explosion, Hit, PlayerShot, Purchase (+2 more)

### Community 90 - "Sample"
Cohesion: 0.22
Nodes (5): Sample, pos, tick, Replica::interpolate(), Replica::update()

### Community 91 - "GameView.cpp"
Cohesion: 0.22
Nodes (5): GameView::drawLayer(), GameView::drawWorld(), GameView::GameView(), GameView::playerFrame(), GameView::tiltFrame()

### Community 92 - "Font"
Cohesion: 0.18
Nodes (8): Font, GLYPH_HEIGHT, GLYPH_WIDTH, find(), Font::drawGlyph(), Glyph, rows, symbol

### Community 93 - "CoinCounter"
Cohesion: 0.33
Nodes (5): CoinCounter, quantity, total, coins, value

### Community 95 - "Phase"
Cohesion: 0.33
Nodes (6): Phase, Defeat, Playing, Shop, Victory, Hud::trackPhase()

### Community 96 - "Knowledge graph (graphify)"
Cohesion: 0.40
Nodes (5): Install, Keep it up to date: the git hook, Knowledge graph (graphify), Use the graph, With an AI agent (Claude Code)

### Community 97 - "Track"
Cohesion: 0.40
Nodes (5): Track, Boss, Level, None, Audio::playMusic()

### Community 98 - "Rarity"
Cohesion: 0.14
Nodes (14): augmentLabel(), rarityColor(), rarityName(), Rarity, Common, Epic, Legendary, Rare (+6 more)

### Community 101 - "FixedTimestep"
Cohesion: 0.25
Nodes (3): FixedTimestep, _accumulator, _maxAccumulator

### Community 102 - "Color"
Cohesion: 0.25
Nodes (6): Color, a, b, g, r, ShopPanel::showMessage()

### Community 103 - "DisconnectReason"
Cohesion: 0.29
Nodes (7): DisconnectReason, Kicked, Quit, Shutdown, Timeout, Client::close(), Server::sendDisconnect()

### Community 106 - "Star"
Cohesion: 0.33
Nodes (6): Star, layer, x, y, Background::Background(), Background::drawStar()

### Community 107 - "PendingSpawn"
Cohesion: 0.67
Nodes (3): PendingSpawn, delay, y

### Community 112 - "Connection::readId"
Cohesion: 0.40
Nodes (3): Connection::handlePong(), Connection::nextSequence(), Connection::readId()

### Community 114 - "ShopSound"
Cohesion: 0.40
Nodes (5): ShopSound, Accept, Count, Refuse, Audio::playShopSound()

### Community 115 - "Regeneration"
Cohesion: 0.50
Nodes (4): Regeneration, hp, interval, timer

## Knowledge Gaps
- **442 isolated node(s):** `_impl`, `voices`, `next`, `music`, `soundVolume` (+437 more)
  These have ≤1 connection - possible missing edges or undocumented components. (Counts symbols only; 897 node(s) total have ≤1 connection when file, concept and rationale nodes are included.)
- **18 thin communities (<3 nodes) omitted from report** — run `graphify query` to explore isolated nodes.

## Suggested Questions
_Questions this graph is uniquely positioned to answer:_

- **Why does `Registry` connect `Registry` to `ECS Column Storage`, `rtype/Protocol.hpp`, `Vec2`, `Game Tests`, `Entity Registry`, `Game`, `Replica`, `GameView`, `.updatePlaying`, `Collider`, `Command Buffer`, `AugmentId`, `EntitySpawned`, `.update`, `findPlayerBySlot`, `R-Type game logic`, `BulletLook`, `TEST`, `Entity Manager`, `GameView.cpp`, `.addPlayer`?**
  _High betweenness centrality (0.105) - this node is a cross-community bridge._
- **Are the 2 inferred relationships involving `Registry` (e.g. with `TEST()` and `TEST()`) actually correct?**
  _`Registry` has 2 INFERRED edges - model-reasoned connections that need verification._
- **What connects `_impl`, `voices`, `next` to the rest of the system?**
  _442 weakly-connected nodes found - possible documentation gaps or missing edges._
- **Should `ECS Column Storage` be split into smaller, more focused modules?**
  _Cohesion score 0.053005464480874315 - nodes in this community are weakly interconnected._
- **Why does `Game` connect `Game` to `.addPlayer`, `Registry`, `TEST`, `rtype/Protocol.hpp`, `App`, `AugmentId`, `rollOffers`, `Game Tests`, `Entity Registry`, `PendingSpawn`, `PlayerInput`, `.update`, `.updatePlaying`, `findPlayerBySlot`, `Phase`?**
  _High betweenness centrality (0.099) - this node is a cross-community bridge._
- **Are the 2 inferred relationships involving `Game` (e.g. with `TEST()` and `TEST()`) actually correct?**
  _`Game` has 2 INFERRED edges - model-reasoned connections that need verification._
- **Should `Window` be split into smaller, more focused modules?**
  _Cohesion score 0.13071895424836602 - nodes in this community are weakly interconnected._