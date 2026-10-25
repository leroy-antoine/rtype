# Knowledge graph (graphify)

The repository ships a **knowledge graph** of the code and of this
documentation, built with [graphify](https://github.com/safishamsi/graphify).
It maps the classes, functions, files and documents of `engine/`, `src/`,
`tests/` and `docs/`, and how they relate (calls, includes, references), grouped
in communities (HUD, reliable channel, ECS storage...).

It is meant to help you, and the AI agents you work with, find their way in the
project quickly: instead of searching the sources blindly, you ask the graph
which pieces are involved in a feature.

The graph lives in `docs/graphify-out/`:

| File | Content |
|---|---|
| `graph.html` | interactive graph, open it in a browser |
| `GRAPH_REPORT.md` | the most connected nodes, the communities, suggested questions |
| `graph.json` | the raw graph, used by the commands below |

## Install

graphify is a Python tool (Python 3.10+):

```sh
pipx install graphifyy     # note the double "y" in the package name
graphify --help
```

`uv tool install graphifyy` works too.

graphify writes to `graphify-out/` by default. Here the output is in
`docs/graphify-out/`, so tell graphify with an environment variable. Add this
line to your shell profile (`~/.bashrc`, `~/.zshrc`) and open a new terminal:

```sh
export GRAPHIFY_OUT=docs/graphify-out
```

Run the commands from the root of the repository.

## Use the graph

```sh
graphify query "how does the server use the Registry"   # broad context
graphify query "how does a ping work" --dfs             # follow one path
graphify path "Server" "Registry"                       # shortest link between two nodes
graphify explain "Connection"                           # one node and its neighbours
```

`query` accepts `--budget <tokens>` when the answer is truncated. Answers quote
the source file and line of every node, so you can jump straight to the code.

Open `docs/graphify-out/graph.html` for the visual version, and read the
"God nodes" section of `GRAPH_REPORT.md` for the main abstractions (`Registry`,
`Entity`, `Game`, `Server`, `Connection`, `Client`...).

### With an AI agent (Claude Code)

`CLAUDE.md` at the root tells the agent to query the graph before grepping the
sources. The agent needs `GRAPHIFY_OUT` too: export it as above before starting
it, or set it in your local `.claude/settings.json`:

```json
{ "env": { "GRAPHIFY_OUT": "docs/graphify-out" } }
```

`.claude/` is git-ignored, so this stays on your machine. Do not run
`graphify claude install`: it rewrites `CLAUDE.md` with the default
`graphify-out/` path.

## Keep it up to date: the git hook

The graph is a snapshot. If it is not rebuilt, it describes code that no longer
exists, and an agent trusting it will look in the wrong place.

**Install the hook once per clone:**

```sh
graphify hook install
graphify hook status      # check
graphify hook uninstall   # remove
```

Why you want it:

- after every commit, checkout, merge and rebase, it re-reads the changed code
  files and rebuilds `graph.json` and `GRAPH_REPORT.md`;
- it is local and takes a few seconds, with no AI call and no cost;
- it also registers a merge driver, so `graph.json` does not produce merge
  conflicts.

Notes:

- git hooks are not shared by `git clone`: every developer installs it.
- the hook needs `GRAPHIFY_OUT` in the environment of the shell that runs git.
  Without it, it writes to a stray `graphify-out/` at the root.
- the hook ignores Markdown files. After changing the documentation, run
  `/graphify docs --update` from Claude Code to refresh the document nodes. This
  step uses an AI model.
- set `GRAPHIFY_SKIP_HOOK=1` to skip the rebuild for one commit.
