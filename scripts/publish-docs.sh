#!/bin/sh
# Builds the documentation website into the public repository (by default the
# folder ../DOC_RTYPE, next to this one). Nothing is committed or pushed.
set -eu

PUBLIC="${1:-../DOC_RTYPE}"

if [ ! -d "$PUBLIC/.git" ]; then
    echo "error: $PUBLIC is not a git repository" >&2
    exit 1
fi
mkdocs build --strict --clean --site-dir "$PUBLIC"
touch "$PUBLIC/.nojekyll"
cat > "$PUBLIC/README.md" <<'TEXT'
# R-Type documentation

Generated website of the R-Type project documentation. Do not edit it by hand:
it is rebuilt from the `docs` folder of the project with MkDocs.
TEXT
echo "Website written in $PUBLIC. To publish it:"
echo "  cd $PUBLIC && git add -A && git commit -m 'docs: update' && git push"
