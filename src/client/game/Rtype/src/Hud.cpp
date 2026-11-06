#include <algorithm>
#include <array>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <optional>
#include <string>
#include <vector>
#include "logic/Config.hpp"
#include "client/AugmentStyle.hpp"
#include "client/DifficultyStyle.hpp"
#include "client/Hud.hpp"

namespace client {
    namespace {
        using Engine::Color;

        constexpr Color WHITE{255, 255, 255, 255};
        constexpr Color DARK{20, 20, 30, 255};
        constexpr Color GREEN{90, 220, 90, 255};
        constexpr Color RED{235, 70, 60, 255};
        constexpr Color YELLOW{255, 220, 40, 255};
        constexpr Color MAGENTA{235, 90, 255, 255};
        constexpr Color CYAN{90, 230, 255, 255};

        // Une couleur par slot, comme les vaisseaux.
        std::string entryName(const std::size_t entry)
        {
            if (entry == LEADERBOARD_ENTRY) {
                return "LEADERBOARD";
            }
            return rtype::MAP_NAMES[entry];
        }

        std::string onOff(const bool value)
        {
            if (value) {
                return "ON";
            }
            return "OFF";
        }

        constexpr std::uint32_t MS_PER_MINUTE = 60000;
        constexpr std::uint32_t MS_PER_SECOND = 1000;
        constexpr std::uint32_t MS_PER_TENTH = 100;

        std::string twoDigits(const std::uint32_t value)
        {
            std::string text = std::to_string(value);

            if (text.size() < 2) {
                text.insert(0, "0");
            }
            return text;
        }

        std::string formatTime(const std::uint32_t ms)
        {
            const std::uint32_t minutes = ms / MS_PER_MINUTE;
            const std::uint32_t seconds = ms / MS_PER_SECOND % 60;
            const std::uint32_t tenths = ms / MS_PER_TENTH % 10;

            return twoDigits(minutes) + ":" + twoDigits(seconds) + "."
                + std::to_string(tenths);
        }

        constexpr std::array<Color, 4> SLOT_COLORS = {{
            {90, 170, 255, 255}, {255, 90, 90, 255},
            {110, 230, 110, 255}, {255, 220, 80, 255}}};

        std::string shopHint(const rtype::GameStatus &status,
            const std::optional<int> self)
        {
            const bool ready = std::ranges::any_of(status.players,
                [&self](const rtype::PlayerStatus &p) {
                    return self && p.slot == *self && p.ready;
                });

            if (ready) {
                return "WAITING FOR THE OTHER PLAYERS";
            }
            return "PRESS SPACE WHEN READY";
        }
    }

    Hud::Hud(Engine::Renderer &renderer, const SpriteBank &sprites,
        const float screenWidth, const float screenHeight)
        : _renderer(renderer), _sprites(sprites), _font(renderer), _width(screenWidth),
        _height(screenHeight)
    {
    }

    void Hud::draw(const Replica &replica, const CustomMapList &custom,
        const float elapsed)
    {
        const std::optional<rtype::GameStatus> &status = replica.status();

        if (!status) {
            _font.drawCentered("CONNECTING...", _width / 2.f, _height / 2.f,
                4.f, WHITE);
            return;
        }
        trackPhase(status->phase, elapsed);
        if (status->phase == rtype::Phase::Lobby) {
            drawLobby(replica, custom);
            return;
        }
        drawPlayers(*status, replica.augments());
        drawCoins(*status, elapsed);
        drawBossBar(*status);
        if (status->phase == rtype::Phase::Victory) {
            drawVictoryScreen(*status, replica, elapsed);
        } else if (status->phase == rtype::Phase::Shop) {
            drawShopScreen(*status, replica, elapsed);
        } else if (status->phase != rtype::Phase::Playing) {
            drawEndScreen(elapsed);
        } else if (!status->paused) {
            drawRespawnHint(*status, replica.slot(), elapsed);
            drawMenuHint(replica);
        }
        drawTimer(*status, elapsed);
        drawLevel(*status);
        drawEffects(replica);
        if (status->paused) {
            drawPauseMenu(replica);
        }
    }

    // Retient depuis quand la partie est finie, pour le compte a rebours.
    void Hud::trackPhase(const rtype::Phase phase, const float elapsed)
    {
        if (phase != _phase) {
            _phase = phase;
            _phaseSince = elapsed;
        }
    }

    // Une ligne par joueur : son nom (P1...) et sa barre de vie.
    void Hud::drawPlayers(const rtype::GameStatus &status,
        const rtype::protocol::OwnedAugment &augments)
    {
        float top = PLAYERS_TOP;

        for (const rtype::PlayerStatus &player : status.players) {
            const auto slot = static_cast<std::size_t>(player.slot);
            const float ratio = static_cast<float>(player.hp)
                / static_cast<float>(rtype::config::PLAYER_HP);

            _font.draw("P" + std::to_string(player.slot + 1), {MARGIN, top},
                2.f, SLOT_COLORS[slot]);
            drawBar({BAR_X, top}, {BAR_WIDTH, BAR_HEIGHT}, ratio, GREEN);
            float x = BAR_X + BAR_WIDTH + AUGMENT_GAP;

            x = drawLives(player.lives, x, top);
            if (status.phase == rtype::Phase::Shop) {
                x = drawReady(player, x, top);
            }
            if (slot < augments.size()) {
                drawAugments(augments[slot], x, top);
            }
            top += ROW_STEP;
        }
    }

    // Un carre par vie restante, a droite de la barre de vie.
    float Hud::drawLives(const int lives, const float x, const float top)
    {
        constexpr float SIZE = 10.f;
        constexpr float GAP = 4.f;
        constexpr Color HEART{235, 70, 110, 255};

        if (lives <= 0) {
            _font.draw("OUT", {x, top}, 2.f, RED);
            return x + Font::width("OUT", 2.f) + AUGMENT_GAP;
        }
        for (int i = 0; i < lives; ++i) {
            _renderer.drawRect({x + static_cast<float>(i) * (SIZE + GAP),
                top + 2.f}, {SIZE, SIZE}, HEART);
        }
        return x + static_cast<float>(lives) * (SIZE + GAP) + AUGMENT_GAP;
    }

    float Hud::textTop()
    {
        return MARGIN + (ICON_SIZE - Font::height(TEXT_SCALE)) / 2.f;
    }

    void Hud::drawIcon(const Engine::Sprite &sprite,
        const Engine::Vector2f position, const float elapsed)
    {
        _renderer.drawResized(sprite.texture, position,
            sprite.animation.frameAt(elapsed), {ICON_SIZE, ICON_SIZE});
    }

    void Hud::drawTimer(const rtype::GameStatus &status, const float elapsed)
    {
        const std::string text = formatTime(status.elapsedMs);
        const float iconX = MARGIN + Font::width(text, TEXT_SCALE) + ICON_GAP;

        _font.draw(text, {MARGIN, textTop()}, TEXT_SCALE, WHITE);
        drawIcon(_sprites.clock(), {iconX, MARGIN}, elapsed);
    }

    void Hud::drawLevel(const rtype::GameStatus &status)
    {
        _font.drawCentered("LEVEL " + std::to_string(status.level),
            _width / 2.f, textTop(), TEXT_SCALE, WHITE);
    }

    void Hud::drawEffects(const Replica &replica)
    {
        const Prediction &prediction = replica.prediction();
        float top = _height - 90.f;

        if (prediction.inverted()) {
            _font.draw("CONTROLS REVERSED", {MARGIN, top}, 3.f, MAGENTA);
            top -= 36.f;
        }
        if (prediction.speedFactor() > 1.f) {
            _font.draw("SPEED UP", {MARGIN, top}, 3.f, CYAN);
        }
    }

    void Hud::drawAugments(const std::vector<rtype::AugmentId> &ids,
        float x, const float top)
    {
        const float textHeight = Font::height(AUGMENT_TEXT_SCALE);
        const float textY = top + (BAR_HEIGHT - textHeight) / 2.f;

        for (const rtype::AugmentId id : ids) {
            const std::string label = augmentLabel(id);
            const float textWidth = Font::width(label, AUGMENT_TEXT_SCALE);

            _renderer.drawRect({x - AUGMENT_PADDING, textY - AUGMENT_PADDING},
                {textWidth + 2.f * AUGMENT_PADDING,
                    textHeight + 2.f * AUGMENT_PADDING}, DARK);
            _font.draw(label, {x, textY}, AUGMENT_TEXT_SCALE,
                rarityColor(id));
            x += textWidth + 2.f * AUGMENT_PADDING + AUGMENT_PADDING;
        }
    }

    void Hud::drawCoins(const rtype::GameStatus &status, const float elapsed)
    {
        const std::string text = std::to_string(status.coins);
        const float textX = _width - MARGIN - Font::width(text, TEXT_SCALE);
        const float iconX = textX - ICON_GAP - ICON_SIZE;

        drawIcon(_sprites.get(rtype::SpriteId::Coin), {iconX, MARGIN}, elapsed);
        _font.draw(text, {textX, textTop()}, TEXT_SCALE, YELLOW);
    }

    void Hud::drawBar(const Engine::Vector2f position,
        const Engine::Vector2f size, const float ratio, const Color fill)
    {
        const float clamped = std::clamp(ratio, 0.f, 1.f);

        _renderer.drawRect({position.x - 2.f, position.y - 2.f},
            {size.x + 4.f, size.y + 4.f}, DARK);
        _renderer.drawRect(position, {size.x * clamped, size.y}, fill);
    }

    void Hud::drawBossBar(const rtype::GameStatus &status)
    {
        if (status.bossMaxHp <= 0 || status.phase != rtype::Phase::Playing) {
            return;
        }
        const float ratio = static_cast<float>(status.bossHp)
            / static_cast<float>(status.bossMaxHp);
        const float top = _height - 38.f;

        _font.drawCentered("BOSS", _width / 2.f, top - 22.f, 2.f, WHITE);
        drawBar({(_width - BOSS_BAR_WIDTH) / 2.f, top}, {BOSS_BAR_WIDTH, 16.f},
            ratio, RED);
    }

    void Hud::drawMenuHint(const Replica &replica)
    {
        if (replica.isHost()) {
            _font.drawCentered("BACKSPACE: PAUSE", _width - 120.f,
                _height - 20.f, 2.f, {160, 160, 160, 255});
        }
    }

    void Hud::drawPauseMenu(const Replica &replica)
    {
        std::string hint = "WAITING FOR P1 TO RESUME";

        if (replica.isHost()) {
            hint = "BACKSPACE: RESUME   ENTER: MAP MENU";
        }
        _renderer.drawRect({0.f, 0.f}, {_width, _height}, {0, 0, 0, 170});
        _font.drawCentered("PAUSED", _width / 2.f, _height / 2.f - 40.f, 7.f,
            WHITE);
        _font.drawCentered(hint, _width / 2.f, _height / 2.f + 40.f, 3.f,
            WHITE);
    }

    // Message au joueur dont le vaisseau est detruit.
    void Hud::drawRespawnHint(const rtype::GameStatus &status,
        const std::optional<int> self, const float elapsed)
    {
        const auto mine = std::ranges::find_if(status.players,
            [&self](const rtype::PlayerStatus &p) {
                return self && p.slot == *self;
            });

        if (mine == status.players.end() || mine->alive) {
            return;
        }
        if (static_cast<int>(elapsed * 2.f) % 2 == 0) {
            _font.drawCentered("PRESS ENTER TO RESPAWN", _width / 2.f,
                _height / 2.f, 4.f, WHITE);
        }
    }

    void Hud::drawEndScreen(const float elapsed)
    {
        const float left = rtype::config::RESTART_DELAY - (elapsed - _phaseSince);
        const int seconds = std::max(0, static_cast<int>(std::ceil(left)));

        _renderer.drawRect({0.f, 0.f}, {_width, _height}, {0, 0, 0, 150});
        _font.drawCentered("GAME OVER", _width / 2.f, _height / 2.f - 50.f,
            10.f, RED);
        _font.drawCentered("NEW GAME IN " + std::to_string(seconds),
            _width / 2.f, _height / 2.f + 50.f, 3.f, WHITE);
    }

    void Hud::drawLobby(const Replica &replica, const CustomMapList &custom)
    {
        const LobbyStep step = replica.menu().step();

        _renderer.drawRect({0.f, 0.f}, {_width, _height}, {0, 0, 0, 210});
        if (replica.isHost() && step == LobbyStep::Settings) {
            drawSettingsStep(replica, settingsMapName(replica, custom));
            return;
        }
        if (step == LobbyStep::Scores) {
            drawScoresStep(replica);
            return;
        }
        if (step == LobbyStep::Custom || step == LobbyStep::CustomScores) {
            drawCustomStep(replica, custom);
            return;
        }
        drawMapStep(replica);
    }

    void Hud::drawMapStep(const Replica &replica)
    {
        const rtype::MapId cursor = replica.mapCursor();
        std::string footer = "WAITING FOR P1 TO CHOOSE   LEFT/RIGHT: RANKING";

        if (replica.onLeaderboard()) {
            footer = "ENTER: SEE ALL THE SCORES";
        } else if (replica.isHost()) {
            footer = "UP/DOWN: CHOOSE  ENTER: SETTINGS  LEFT/RIGHT: RANKING";
        }
        _font.drawCentered("SELECT A GAME", _width / 2.f, 50.f, 6.f, WHITE);
        drawTabs(0);
        drawMapList(cursor);
        if (!replica.onLeaderboard()) {
            drawRecords(replica.records()[cursor][static_cast<std::size_t>(
                replica.menu().shownDifficulty())],
                replica.menu().shownDifficulty());
        }
        _font.drawCentered(footer, _width / 2.f, _height - 50.f, 3.f, WHITE);
        _font.drawCentered("TAB: CUSTOM MAPS", _width / 2.f, _height - 90.f,
            2.f, WHITE);
    }

    void Hud::drawSettingsStep(const Replica &replica,
        const std::string &mapName)
    {
        constexpr float ROW_TOP = 190.f;
        constexpr float ROW_GAP = 64.f;
        const rtype::Settings &settings = replica.menu().settings();
        const bool onFire = replica.menu().row() == SettingsRow::FriendlyFire;

        _font.drawCentered("GAME SETTINGS", _width / 2.f, 50.f, 6.f, WHITE);
        _font.drawCentered(mapName, _width / 2.f, 120.f, 4.f, YELLOW);
        drawSettingRow("DIFFICULTY", difficultyName(settings.difficulty),
            difficultyColor(settings.difficulty), !onFire, ROW_TOP);
        drawSettingRow("FRIENDLY FIRE", onOff(settings.friendlyFire), WHITE,
            onFire, ROW_TOP + ROW_GAP);
        drawScaleSummary(settings.difficulty);
        _font.drawCentered("UP/DOWN: ROW  LEFT/RIGHT: CHANGE  ENTER: START"
            "  ESC: BACK", _width / 2.f, _height - 50.f, 2.f, WHITE);
    }

    void Hud::drawSettingRow(const std::string &label,
        const std::string &value, const Color color, const bool selected,
        const float y)
    {
        constexpr float STRIP_WIDTH = 700.f;
        constexpr float PADDING = 20.f;
        const float left = (_width - STRIP_WIDTH) / 2.f;
        const float right = left + STRIP_WIDTH - PADDING;

        if (selected) {
            _renderer.drawRect({left, y - 10.f}, {STRIP_WIDTH, 52.f},
                {40, 70, 140, 255});
        }
        _font.draw(label, {left + PADDING, y}, 4.f, WHITE);
        _font.draw(value, {right - Font::width(value, 4.f), y}, 4.f, color);
    }

    void Hud::drawScaleSummary(const rtype::Difficulty difficulty)
    {
        constexpr float TOP = 350.f;
        constexpr float STEP = 34.f;
        const rtype::DifficultyScale &scale = rtype::scaleOf(difficulty);
        const std::array<std::string, 7> lines = {
            "LIVES " + std::to_string(scale.lives),
            "ENEMY HP " + multiplierText(scale.enemyHp),
            "ENEMY DAMAGE " + multiplierText(scale.enemyDamage),
            "YOUR DAMAGE " + multiplierText(scale.allyDamage),
            "ENEMY FIRE RATE " + multiplierText(scale.enemyFireRate),
            "ENEMY DENSITY " + multiplierText(scale.density),
            "COINS " + multiplierText(scale.economy)};

        for (std::size_t i = 0; i < lines.size(); ++i) {
            _font.drawCentered(lines[i], _width / 2.f,
                TOP + STEP * static_cast<float>(i), 3.f, WHITE);
        }
    }

    void Hud::drawMapList(const rtype::MapId cursor)
    {
        constexpr float TOP = 150.f;
        constexpr float STEP = 52.f;
        constexpr float STRIP_WIDTH = 520.f;

        for (std::size_t i = 0; i < ENTRY_COUNT; ++i) {
            const float y = TOP + static_cast<float>(i) * STEP;
            Color color = WHITE;

            if (i == cursor) {
                _renderer.drawRect({(_width - STRIP_WIDTH) / 2.f, y - 8.f},
                    {STRIP_WIDTH, STEP - 6.f}, {40, 70, 140, 255});
                color = YELLOW;
            }
            _font.drawCentered(entryName(i), _width / 2.f, y, 4.f, color);
        }
    }

    void Hud::drawRecords(const rtype::MapRecords &records,
        const rtype::Difficulty difficulty)
    {
        constexpr float TOP = 360.f;
        constexpr float STEP = 36.f;

        _font.drawCentered("BEST TIMES", _width / 2.f - 80.f, TOP, 3.f,
            WHITE);
        _font.drawCentered(difficultyName(difficulty), _width / 2.f + 120.f,
            TOP, 3.f, difficultyColor(difficulty));
        if (records.empty()) {
            _font.drawCentered("NO RECORD YET", _width / 2.f, TOP + STEP,
                3.f, GREEN);
        }
        for (std::size_t i = 0; i < records.size(); ++i) {
            drawRunRow(i + 1, records[i],
                TOP + STEP * static_cast<float>(i + 1));
        }
    }

    // Une partie gagnee : rang, temps, joueurs, difficulte et tir allie.
    void Hud::drawRunRow(const std::size_t rank,
        const rtype::RunRecord &record, const float y)
    {
        const float left = _width / 2.f - RUN_ROW_WIDTH / 2.f;
        const rtype::Settings &settings = record.settings;

        _font.draw(std::to_string(rank) + ".", {left, y}, 3.f, WHITE);
        _font.draw(formatTime(record.timeMs), {left + 60.f, y}, 3.f, GREEN);
        _font.draw(std::to_string(record.players) + "P", {left + 230.f, y},
            3.f, WHITE);
        _font.draw(difficultyName(settings.difficulty), {left + 300.f, y},
            3.f, difficultyColor(settings.difficulty));
        if (settings.friendlyFire) {
            _font.draw("FF", {left + 470.f, y}, 3.f, YELLOW);
        }
    }

    // Toutes les victoires de la map choisie, la plus rapide en premier.
    void Hud::drawScoresStep(const Replica &replica)
    {
        constexpr float TOP = 190.f;
        constexpr float STEP = 34.f;
        constexpr std::size_t VISIBLE = 13;
        const rtype::MapId map = replica.menu().scoresMap();
        const rtype::MapRecords &runs = replica.runs(map);
        const std::size_t first = replica.menu().scroll();

        _font.drawCentered("LEADERBOARD", _width / 2.f, 50.f, 6.f, WHITE);
        _font.drawCentered(rtype::MAP_NAMES[map], _width / 2.f, 120.f, 4.f,
            YELLOW);
        if (runs.empty()) {
            _font.drawCentered("NO RECORD YET", _width / 2.f, TOP, 3.f, GREEN);
        }
        for (std::size_t i = first; i < runs.size() && i < first + VISIBLE;
            ++i) {
            drawRunRow(i + 1, runs[i],
                TOP + STEP * static_cast<float>(i - first));
        }
        _font.drawCentered("LEFT/RIGHT: GAME  UP/DOWN: SCROLL  ENTER: BACK",
            _width / 2.f, _height - 50.f, 3.f, WHITE);
    }

    float Hud::drawReady(const rtype::PlayerStatus &player, const float x,
        const float top)
    {
        const std::string label = player.ready ? "READY" : "NOT READY";

        _font.draw(label, {x, top}, 2.f, player.ready ? GREEN : RED);
        return x + Font::width(label, 2.f) + AUGMENT_GAP;
    }

    void Hud::drawShopScreen(const rtype::GameStatus &status,
        const Replica &replica, const float elapsed)
    {
        const float bottom = _height - SHOP_TEXT_MARGIN;

        if (replica.shopOffers().empty()) {
            _font.drawCentered("SHOP", _width / 2.f, _height / 2.f - 50.f,
                10.f, YELLOW);
        }
        if (status.countdown > 0.f) {
            _font.drawCentered("NEXT LEVEL IN "
                + std::to_string(std::lround(std::ceil(status.countdown))),
                _width / 2.f, bottom, 3.f, WHITE);
            return;
        }
        if (std::fmod(elapsed, 1.f) < 0.5f) {
            _font.drawCentered(shopHint(status, replica.slot()), _width / 2.f,
                bottom, 3.f, WHITE);
        }
    }
}
