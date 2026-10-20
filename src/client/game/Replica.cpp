#include "Replica.hpp"

namespace client {
    void Replica::apply(const rtype::EntitySpawned &spawn)
    {
        apply(rtype::EntityDestroyed{spawn.id});
        const Engine::Entity e = _reg.create();

        _reg.add(e, spawn.pos);
        _reg.add(e, spawn.box);
        _reg.add(e, gameLogic::Vec2Velocity{});
        _reg.add(e, rtype::Renderable{spawn.sprite});
        if (spawn.sprite == rtype::SpriteId::Player) {
            _reg.add(e, rtype::PlayerTag{spawn.slot});
        }
        if (spawn.sprite == rtype::SpriteId::Explosion) {
            _reg.add(e, rtype::Lifetime{});
        }
        _entities[spawn.id] = e;
    }

    void Replica::apply(const rtype::EntityDestroyed &destroy)
    {
        const auto found = _entities.find(destroy.id);

        if (found == _entities.end()) {
            return;
        }
        _reg.destroy(found->second);
        _entities.erase(found);
    }

    void Replica::apply(const std::vector<rtype::EntityState> &states)
    {
        for (const rtype::EntityState &state : states) {
            apply(state);
        }
    }

    // Un etat peut arriver avant son SPAWN (reliable renvoye): on l'ignore.
    // La vitesse n'est qu'un deplacement par tick, GameView n'en lit que le signe.
    void Replica::apply(const rtype::EntityState &state)
    {
        const auto found = _entities.find(state.id);

        if (found == _entities.end()) {
            return;
        }
        gameLogic::Vec2Pos &pos = _reg.get<gameLogic::Vec2Pos>(found->second);

        _reg.get<gameLogic::Vec2Velocity>(found->second) =
            gameLogic::Vec2Velocity{{state.pos.x - pos.x, state.pos.y - pos.y}};
        pos = state.pos;
    }

    // Anime les explosions; c'est le serveur qui les detruit.
    void Replica::update(const float dt)
    {
        _reg.each<rtype::Lifetime>([dt](Engine::Entity, rtype::Lifetime &lifetime) {
            lifetime.remaining -= dt;
            if (lifetime.remaining < 0.f) {
                lifetime.remaining = 0.f;
            }
        });
    }

    Engine::Registry &Replica::registry()
    {
        return _reg;
    }
}
