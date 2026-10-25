# Contributing

## Branches

Work on a branch named after its purpose, then open a pull request on `main`.
The branches already in the repository use these prefixes:

| Prefix | For |
|---|---|
| `feat/...` | a new feature |
| `fix/...` | a bug fix |
| `build/...` | build system and tooling |
| `chore/...` | maintenance |

## Commits

Short messages in English, starting with a type: `feat: ...`, `fix: ...`,
`docs: ...`, `chore: ...`. One commit does one thing.

## Pull requests

The template (`.github/pull_request_template.md`) asks for:

- `Closes #<issue>`
- **Changes**: what was done
- **How to test**: how a reviewer can check it

Do not edit the code of another part without telling its owner: `engine/core`,
the server, the client and the game logic each have an owner.

## Coding conventions

Written for the game logic and recommended for new code elsewhere:

- lines of **80 columns** at most;
- functions of **25 lines** at most, **5 parameters** at most, **2 nested
  conditions** at most;
- no ternary operator;
- data in `struct`s, behaviour in functions, constants in a config header;
- `#pragma once` as include guard;
- private members prefixed with `_`.

More details and the reasoning:
[game logic conventions](../src/games/rtype/doc/README.md#coding-conventions).

## Documentation

When you add a feature, update the documentation of its part in the same pull
request: the tick table or status list for the game logic, the open questions
for the server and the client, the [protocol page](protocol.md) for anything
that changes what goes on the network.

## Knowledge graph

A graphify graph of the code is kept in `docs/graphify-out/`. Install the git
hook once so that it follows your commits; see the
[knowledge graph page](graphify.md).
