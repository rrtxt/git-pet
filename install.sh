#!/usr/bin/env bash
#
# git-pet installer
#
# Installs the git-pet binary and the default pet assets, then sets up the
# GIT_PET_PATH environment variable in your shell config.
#
# Usage:
#   ./install.sh              Download and install the latest GitHub release
#   ./install.sh --local      Install your own build (build/git-pet) instead
#   ./install.sh --uninstall  Remove git-pet and its shell config
#
# Environment overrides:
#   PREFIX   Install root (default: ~/.local)
#            Binary  -> $PREFIX/bin/git-pet
#            Assets  -> $PREFIX/share/git-pet/<pet-id>/

set -euo pipefail

REPO="rrtxt/git-pet"
PET_ID="cat"
PREFIX="${PREFIX:-$HOME/.local}"
BIN_DIR="$PREFIX/bin"
SHARE_DIR="$PREFIX/share/git-pet"
MARKER="# >>> git-pet >>>"
MARKER_END="# <<< git-pet <<<"

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]:-$0}")" && pwd)"

info() { printf '\033[1;34m==>\033[0m %s\n' "$*"; }
warn() { printf '\033[1;33mwarning:\033[0m %s\n' "$*" >&2; }
usage() {
  cat <<'EOF'
Usage:
  ./install.sh              Download and install the latest GitHub release
  ./install.sh --local      Install your own build (build/git-pet) instead
  ./install.sh --uninstall  Remove git-pet and its shell config

Environment overrides:
  PREFIX   Install root (default: ~/.local)
EOF
}
die()  { printf '\033[1;31merror:\033[0m %s\n' "$*" >&2; exit 1; }

# Shell config files we touch: only those for shells the user actually has.
shell_rc_files() {
  local files=()
  [[ -f "$HOME/.bashrc" || "${SHELL:-}" == */bash ]] && files+=("$HOME/.bashrc")
  [[ -f "$HOME/.zshrc"  || "${SHELL:-}" == */zsh  ]] && files+=("$HOME/.zshrc")
  [[ ${#files[@]} -eq 0 ]] && files+=("$HOME/.profile")
  printf '%s\n' "${files[@]}"
}

remove_shell_block() {
  local rc="$1"
  [[ -f "$rc" ]] || return 0
  if grep -qF "$MARKER" "$rc"; then
    sed -i.bak "/$MARKER/,/$MARKER_END/d" "$rc" && rm -f "$rc.bak"
  fi
}

setup_shell() {
  local rc
  while IFS= read -r rc; do
    remove_shell_block "$rc"
    {
      echo "$MARKER"
      echo "export GIT_PET_PATH=\"$SHARE_DIR\""
      echo "case \":\$PATH:\" in *\":$BIN_DIR:\"*) ;; *) export PATH=\"$BIN_DIR:\$PATH\" ;; esac"
      echo "$MARKER_END"
    } >> "$rc"
    info "Updated $rc"
  done < <(shell_rc_files)

  # fish uses its own syntax
  if command -v fish >/dev/null 2>&1; then
    local fish_conf="$HOME/.config/fish/conf.d/git-pet.fish"
    mkdir -p "$(dirname "$fish_conf")"
    cat > "$fish_conf" <<EOF
set -gx GIT_PET_PATH "$SHARE_DIR"
fish_add_path -g "$BIN_DIR"
EOF
    info "Updated $fish_conf"
  fi
}


download_release() {
  local tmp="$1"
  case "$(uname -s)-$(uname -m)" in
    Linux-x86_64) ;;
    *) die "No prebuilt package for $(uname -s) $(uname -m). Build from source (see README.md), then run './install.sh --local' from the repo root." ;;
  esac

  local url="https://github.com/$REPO/releases/latest/download/git-pet-linux-x86_64.tar.gz"
  info "Downloading $url"
  if command -v curl >/dev/null 2>&1; then
    curl -fsSL "$url" -o "$tmp/git-pet.tar.gz"
  elif command -v wget >/dev/null 2>&1; then
    wget -qO "$tmp/git-pet.tar.gz" "$url"
  else
    die "curl or wget is required to download git-pet."
  fi
  tar -xzf "$tmp/git-pet.tar.gz" -C "$tmp"
}

install_pet() {
  local src_dir bin_path

  if [[ "${1:-}" == "--local" ]]; then
    src_dir="$SCRIPT_DIR"
    bin_path="$SCRIPT_DIR/build/git-pet"
    [[ -x "$bin_path" && -d "$src_dir/assets" ]] ||
      die "No local build found. Run 'cmake -B build && cmake --build build' from the repo root first."
    info "Installing local build $bin_path"
  else
    TMP_DIR="$(mktemp -d)"
    trap 'rm -rf "$TMP_DIR"' EXIT
    download_release "$TMP_DIR"
    src_dir="$TMP_DIR/git-pet"
    bin_path="$src_dir/git-pet"
  fi

  # Binary
  mkdir -p "$BIN_DIR"
  install -m 755 "$bin_path" "$BIN_DIR/git-pet"
  info "Installed binary to $BIN_DIR/git-pet"

  # Assets: GIT_PET_PATH/<pet-id>/{pet.toml,egg/,baby/,...}
  local pet_dir="$SHARE_DIR/$PET_ID"
  mkdir -p "$pet_dir"
  local stage
  for stage in egg baby teen adult; do
    if [[ -d "$src_dir/assets/$stage" ]]; then
      rm -rf "${pet_dir:?}/$stage"
      cp -r "$src_dir/assets/$stage" "$pet_dir/$stage"
    fi
  done

  # Keep a user-edited pet.toml; only write the default if missing.
  if [[ -f "$src_dir/assets/pet.toml" ]]; then
    [[ -f "$pet_dir/pet.toml" ]] || cp "$src_dir/assets/pet.toml" "$pet_dir/pet.toml"
  elif [[ ! -f "$pet_dir/pet.toml" ]]; then
    cat > "$pet_dir/pet.toml" <<'EOF'
name = "Cat"
stages = ["egg", "baby", "teen", "adult"]

[egg]
fps = 5

[baby]
fps = 8

[teen]
fps = 10

[adult]
fps = 10
EOF
  fi
  info "Installed pet assets to $pet_dir"

  setup_shell

  # Release builds are static; this mainly catches --local builds whose
  # shared libraries (e.g. libgit2) are missing at runtime.
  if command -v ldd >/dev/null 2>&1 && ldd "$BIN_DIR/git-pet" 2>/dev/null | grep -q "not found"; then
    warn "Some shared libraries are missing:"
    ldd "$BIN_DIR/git-pet" | grep "not found" >&2
    warn "Install the missing libraries with your package manager and try again."
  fi

  echo
  info "Done! Restart your shell or run:"
  echo "    export GIT_PET_PATH=\"$SHARE_DIR\""
  echo "    export PATH=\"$BIN_DIR:\$PATH\""
  echo "  then run 'git-pet' inside any git repository."
}

uninstall_pet() {
  rm -f "$BIN_DIR/git-pet"
  rm -rf "$SHARE_DIR"
  local rc
  for rc in "$HOME/.bashrc" "$HOME/.zshrc" "$HOME/.profile"; do
    remove_shell_block "$rc"
  done
  rm -f "$HOME/.config/fish/conf.d/git-pet.fish"
  info "git-pet uninstalled. Restart your shell to clear GIT_PET_PATH."
}

case "${1:-}" in
  ""|--install)  install_pet ;;
  --local)       install_pet --local ;;
  --uninstall)   uninstall_pet ;;
  -h|--help)     usage ;;
  *)             die "Unknown option: $1 (try --help)" ;;
esac
