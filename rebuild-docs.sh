#!/usr/bin/env bash
set -e
SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"
cd "$SCRIPT_DIR"
echo "Rebuilding documentation with MkDocs..."
mkdocs build --config-file ../mkdocs.yml
