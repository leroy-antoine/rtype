# Project documentation

Documentation that concerns the whole project. Each major part also keeps its
own documentation next to its code.

## By part

| Part | Documentation |
|---|---|
| Game logic | [src/client/game/Rtype/doc](../src/client/game/Rtype/doc/README.md) |
| Server | [src/server/doc](../src/server/doc/README.md) |
| Client | [src/client/doc](../src/client/doc/README.md) |

## Project-wide

| Page | Content |
|---|---|
| [Gameplay](gameplay.md) | the rules as a player sees them: maps, monsters, bosses, walls, scoring, controls |
| [Architecture](architecture.md) | the layers, who depends on whom, how the pieces are glued |
| [Build and run](build.md) | requirements, building on Linux and Windows, IDE setup, troubleshooting |
| [Protocol](protocol.md) | client/server network protocol: messages, lifecycle, reliability, security |
| [Comparative study](comparative-study.md) | why each technology was chosen, and the alternatives |
| [Accessibility](accessibility.md) | what we offer to players with disabilities |
| [Contributing](contributing.md) | branches, commits, pull requests, conventions |
| [Knowledge graph](graphify.md) | the graphify graph of the code: install, query it, keep it up to date with the git hook |
| [Network module](../engine/docs/network.md) | the transport layer under the protocol: datagrams, sessions, reliability |

All documentation is written in English and in Markdown, so that it can be read
on GitHub and turned into a static website by any generator later.
