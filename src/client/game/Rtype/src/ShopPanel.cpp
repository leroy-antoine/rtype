#include <algorithm>
#include "client/AugmentStyle.hpp"
#include "client/ShopPanel.hpp"

namespace client {
    namespace {
        using Engine::Color;

        constexpr Color WHITE{255, 255, 255, 255};
        constexpr Color GREY{110, 110, 120, 255};
        constexpr Color GREEN{90, 220, 90, 255};
        constexpr Color RED{235, 70, 60, 255};
        constexpr Color YELLOW{255, 220, 40, 255};
        constexpr Color SHADE{0, 0, 0, 140};
        constexpr Color PANEL{15, 15, 30, 235};
        constexpr Color PANEL_BORDER{80, 80, 120, 255};
        constexpr Color CARD{35, 35, 55, 255};
        constexpr Color CARD_LOCKED{25, 25, 30, 255};
        constexpr Color CARD_BORDER{20, 20, 30, 255};
    }

    ShopPanel::ShopPanel(Engine::Renderer &renderer, const SpriteBank &sprites,
        const float screenWidth, const float screenHeight)
        : _renderer(renderer), _sprites(sprites), _font(renderer), _width(screenWidth),
        _height(screenHeight)
    {
    }

    bool ShopPanel::isOpen(const Replica &replica)
    {
        return !replica.shopOffers().empty();
    }

    std::optional<rtype::AugmentId> ShopPanel::handle(const std::vector<Engine::Key> &keys,
        const Replica &replica, const float elapsed)
    {
        const std::size_t count = replica.shopOffers().size();

        _selected = selection(count);
        for (const Engine::Key key : keys) {
            if (key == Engine::Key::Left && _selected > 0) {
                --_selected;
            } else if (key == Engine::Key::Right && _selected + 1 < count) {
                ++_selected;
            } else if (key == Engine::Key::Enter) {
                return confirm(replica, elapsed);
            }
        }
        return std::nullopt;
    }

    std::optional<rtype::AugmentId> ShopPanel::confirm(const Replica &replica, const float elapsed)
    {
        const std::vector<rtype::AugmentId> &offers = replica.shopOffers();

        if (_pending || offers.empty()) {
            return std::nullopt;
        }
        const rtype::AugmentId id = offers[selection(offers.size())];

        if (!affordable(replica, id)) {
            showMessage("NOT ENOUGH COINS", RED, elapsed);
            return std::nullopt;
        }
        _pending = id;
        return id;
    }

    void ShopPanel::showResult(const rtype::protocol::BuyResult &result, const float elapsed)
    {
        _pending.reset();
        if (result.ok) {
            showMessage("BOUGHT: " + augmentLabel(result.id), GREEN, elapsed);
        } else {
            showMessage("PURCHASE REFUSED", RED, elapsed);
        }
    }

    void ShopPanel::showMessage(const std::string &text, const Color color, const float elapsed)
    {
        _message = text;
        _messageColor = color;
        _messageUntil = elapsed + MESSAGE_DURATION;
    }

    bool ShopPanel::affordable(const Replica &replica, const rtype::AugmentId id)
    {
        const std::optional<rtype::GameStatus> &status = replica.status();

        return status && status->coins >= rtype::priceOf(id);
    }

    std::size_t ShopPanel::selection(const std::size_t count) const
    {
        if (count == 0) {
            return 0;
        }
        return std::min(_selected, count - 1);
    }

    std::string ShopPanel::hint(const Replica &replica)
    {
        if (replica.isHost()) {
            return "LEFT/RIGHT: CHOOSE   ENTER: BUY   TAB: NEXT LEVEL";
        }
        return "LEFT/RIGHT: CHOOSE   ENTER: BUY   WAITING FOR P1";
    }

    void ShopPanel::draw(const Replica &replica, const float elapsed)
    {
        const std::vector<rtype::AugmentId> &offers = replica.shopOffers();

        if (offers.empty()) {
            drawMessage(elapsed);
            return;
        }
        const float left = (_width - PANEL_WIDTH) / 2.f;
        const float top = (_height - PANEL_HEIGHT) / 2.f;
        const auto count = static_cast<float>(offers.size());
        const float cardsWidth = count * CARD_WIDTH + (count - 1.f) * CARD_GAP;
        const std::size_t selected = selection(offers.size());
        float x = (_width - cardsWidth) / 2.f;

        _renderer.drawRect({0.f, 0.f}, {_width, _height}, SHADE);
        _renderer.drawRect({left - BORDER, top - BORDER},
            {PANEL_WIDTH + 2.f * BORDER, PANEL_HEIGHT + 2.f * BORDER}, PANEL_BORDER);
        _renderer.drawRect({left, top}, {PANEL_WIDTH, PANEL_HEIGHT}, PANEL);
        _font.drawCentered("SHOP", _width / 2.f, top + 20.f, 4.f, WHITE);
        for (std::size_t i = 0; i < offers.size(); ++i) {
            drawCard(replica, offers[i], {x, top + CARD_TOP}, i == selected, elapsed);
            x += CARD_WIDTH + CARD_GAP;
        }
        _font.drawCentered(hint(replica), _width / 2.f,
            top + PANEL_HEIGHT - 40.f, 2.f, GREY);
        drawMessage(elapsed);
    }

    void ShopPanel::drawCard(const Replica &replica, const rtype::AugmentId id,
        const Engine::Vector2f position, const bool selected, const float elapsed)
    {
        const std::optional<rtype::AugmentDef> def = rtype::findAugment(id);
        const bool canBuy = affordable(replica, id);
        const float centerX = position.x + CARD_WIDTH / 2.f;
        const Color border = selected ? YELLOW : CARD_BORDER;
        const Color text = canBuy ? WHITE : GREY;

        if (!def) {
            return;
        }
        _renderer.drawRect({position.x - BORDER, position.y - BORDER},
            {CARD_WIDTH + 2.f * BORDER, CARD_HEIGHT + 2.f * BORDER}, border);
        _renderer.drawRect(position, {CARD_WIDTH, CARD_HEIGHT}, canBuy ? CARD : CARD_LOCKED);
        _font.drawCentered(augmentLabel(id), centerX, position.y + 30.f, 5.f, text);
        _font.drawCentered(rarityName(def->rarity), centerX, position.y + 100.f, 2.f,
            canBuy ? rarityColor(def->rarity) : GREY);
        drawPrice(def->price, centerX, position.y + 150.f, canBuy ? YELLOW : RED, elapsed);
    }

    void ShopPanel::drawPrice(const std::size_t price, const float centerX, const float top,
        const Color color, const float elapsed)
    {
        const std::string text = std::to_string(price);
        const float textWidth = Font::width(text, 3.f);
        const float total = COIN_SIZE + COIN_GAP + textWidth;
        const float iconX = centerX - total / 2.f;
        const float textY = top + (COIN_SIZE - Font::height(3.f)) / 2.f;
        const Engine::Sprite &coin = _sprites.get(rtype::SpriteId::Coin);

        _renderer.drawResized(coin.texture, {iconX, top}, coin.animation.frameAt(elapsed),
            {COIN_SIZE, COIN_SIZE});
        _font.draw(text, {iconX + COIN_SIZE + COIN_GAP, textY}, 3.f, color);
    }

    void ShopPanel::drawMessage(const float elapsed)
    {
        if (_message.empty() || elapsed > _messageUntil) {
            return;
        }
        _font.drawCentered(_message, _width / 2.f, (_height + PANEL_HEIGHT) / 2.f + 24.f, 3.f,
            _messageColor);
    }
}
