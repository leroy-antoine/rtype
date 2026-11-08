# R-Type documentation

A networked clone of the arcade shoot'em up **R-Type**, written in C++20 for
the Epitech R-Type project: up to four players, each with their own client, on
a server that has the final authority. The project is built around a small
game engine (an ECS, a window module, audio and a reliable UDP transport) and
three decoupled layers: **rendering**, **networking** and **game logic**.

## Play

| Page | Content |
|---|---|
| [Gameplay](gameplay.md) | the rules as a player sees them: maps, monsters, bosses, walls, scoring, controls |
| [Create your own map](custom-maps.md) | write a map in JSON, step by step, with examples, every field and the common errors |
| [Accessibility](accessibility.md) | what we offer to players with disabilities |

## Develop

| Page | Content |
|---|---|
| [Build and run](build.md) | requirements, building on Linux and Windows, IDE setup, troubleshooting |
| [Architecture](architecture.md) | the layers, who depends on whom, how the pieces are glued |
| [Game logic](game-logic.md) | rules, architecture, systems, the `Game` API, how-to guides |
| [Server](server.md) | role, loop, how it plugs the game in |
| [Client](client.md) | rendering, input, interpolation and prediction, lobby screen |
| [Network module](network.md) | the transport layer under the protocol: datagrams, sessions, reliability |
| [Custom maps, for developers](custom-maps-internals.md) | how the maps written by the players are read, checked, sent and played; how to extend them |
| [Contributing](contributing.md) | branches, commits, pull requests, conventions |
| [Knowledge graph](graphify.md) | the graphify graph of the code: install, query it, keep it up to date with the git hook |

## Reference and study

| Page | Content |
|---|---|
| [Network protocol](protocol.md) | client/server messages: lifecycle, reliability, security |
| [Technical and comparative study](comparative-study.md) | why each technology was chosen, and the alternatives |

All the documentation is written in English and in Markdown. This website is
generated from the `docs` folder of the project with
[MkDocs](https://www.mkdocs.org/) (see [Contributing](contributing.md#documentation-website)).
