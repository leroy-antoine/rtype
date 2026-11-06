#pragma once

#include <cstddef>
#include <optional>
#include <string>
#include <vector>
#include "engine/window/Event.hpp"
#include "engine/window/Renderer.hpp"
#include "Font.hpp"
#include "Replica.hpp"
#include "SpriteBank.hpp"
#include "logic/Augments.hpp"
#include "rtype/Protocol.hpp"

namespace client {
    class ShopPanel {
    public:
        ShopPanel(Engine::Renderer &renderer, const SpriteBank &sprites,
            float screenWidth, float screenHeight);

        [[nodiscard]] static bool isOpen(const Replica &replica);
        std::optional<rtype::AugmentId> handle(const std::vector<Engine::Key> &keys,
            const Replica &replica, float elapsed);
        void showResult(const rtype::protocol::BuyResult &result, float elapsed);
        void draw(const Replica &replica, float elapsed);

    private:
        static constexpr float PANEL_WIDTH = 760.f;
        static constexpr float PANEL_HEIGHT = 400.f;
        static constexpr float CARD_WIDTH = 200.f;
        static constexpr float CARD_HEIGHT = 220.f;
        static constexpr float CARD_GAP = 40.f;
        static constexpr float CARD_TOP = 80.f;
        static constexpr float BORDER = 4.f;
        static constexpr float COIN_SIZE = 28.f;
        static constexpr float COIN_GAP = 8.f;
        static constexpr float MESSAGE_DURATION = 2.f;
        static constexpr float CARD_PADDING = 8.f;
        static constexpr float INFO_SCALE = 2.f;
        static constexpr float INFO_STEP = 18.f;

        [[nodiscard]] static std::string hint(const Replica &replica);
        [[nodiscard]] static bool affordable(const Replica &replica, rtype::AugmentId id);
        [[nodiscard]] std::size_t selection(std::size_t count) const;
        std::optional<rtype::AugmentId> confirm(const Replica &replica, float elapsed);
        void showMessage(const std::string &text, Engine::Color color, float elapsed);
        void drawCard(const Replica &replica, rtype::AugmentId id, Engine::Vector2f position,
            bool selected, float elapsed);
        void drawAugmentText(rtype::AugmentId id, float centerX, float top,
            Engine::Color color);
        [[nodiscard]] static std::size_t infoColumns();
        void drawPrice(std::size_t price, float centerX, float top, Engine::Color color, float elapsed);
        void drawMessage(float elapsed);

        Engine::Renderer &_renderer;
        const SpriteBank &_sprites;
        Font _font;
        float _width;
        float _height;
        std::size_t _selected = 0;
        std::optional<rtype::AugmentId> _pending;
        std::string _message;
        Engine::Color _messageColor{255, 255, 255, 255};
        float _messageUntil = 0.f;
    };
}
