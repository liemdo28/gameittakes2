#!/bin/bash
# ============================================================================
#  Shards of Dawn - Build Script for Testing (macOS/Linux)
#  Usage: ./BuildGame.sh [build|cook|package|clean]
#  Default: package (full build + cook + package)
# ============================================================================

set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
UPROJECT="${SCRIPT_DIR}/ShardsOfDawn.uproject"
BUILD_DIR="${SCRIPT_DIR}/Build/Packaged"
PLATFORM="Linux"
CONFIG="Development"

# ── Auto-detect UE5 installation ──────────────────────────────────────────
if [ -z "${UE5_ROOT}" ]; then
    # Common Linux/Mac paths
    for UE_PATH in \
        "/opt/UnrealEngine" \
        "$HOME/UnrealEngine" \
        "/Users/Shared/Epic Games/UE_5.4" \
        "/Applications/UnrealEngine/UE_5.4"; do
        if [ -f "${UE_PATH}/Engine/Build/BatchFiles/RunUAT.sh" ]; then
            UE5_ROOT="${UE_PATH}"
            break
        fi
    done
fi

if [ -z "${UE5_ROOT}" ]; then
    echo "[ERROR] Could not find Unreal Engine 5.4+"
    echo "Please set UE5_ROOT environment variable"
    echo "Example: export UE5_ROOT=/opt/UnrealEngine"
    exit 1
fi

echo "[INFO] Using Unreal Engine at: ${UE5_ROOT}"

UAT="${UE5_ROOT}/Engine/Build/BatchFiles/RunUAT.sh"
EDITOR_CMD="${UE5_ROOT}/Engine/Binaries/Linux/UnrealEditor-Cmd"

if [[ "$(uname)" == "Darwin" ]]; then
    PLATFORM="Mac"
    EDITOR_CMD="${UE5_ROOT}/Engine/Binaries/Mac/UnrealEditor-Cmd"
fi

COMMAND="${1:-package}"

echo ""
echo "============================================================================"
echo "  SHARDS OF DAWN - Build System"
echo "  Command: ${COMMAND}"
echo "  Platform: ${PLATFORM}"
echo "  Config: ${CONFIG}"
echo "============================================================================"
echo ""

case "${COMMAND}" in
    build)
        echo "[STEP 1/1] Compiling C++ source..."
        "${UAT}" BuildCookRun \
            -project="${UPROJECT}" \
            -noP4 \
            -platform="${PLATFORM}" \
            -clientconfig="${CONFIG}" \
            -build \
            -skipstage \
            -utf8output
        echo "[SUCCESS] C++ compilation completed!"
        ;;

    cook)
        echo "[STEP 1/1] Cooking content..."
        "${EDITOR_CMD}" "${UPROJECT}" \
            -run=cook \
            -targetplatform="${PLATFORM}" \
            -iterate \
            -unversioned
        echo "[SUCCESS] Content cooking completed!"
        ;;

    package)
        echo "[STEP 1/1] Running full package pipeline..."
        echo "  Output: ${BUILD_DIR}"

        "${UAT}" BuildCookRun \
            -project="${UPROJECT}" \
            -noP4 \
            -platform="${PLATFORM}" \
            -clientconfig="${CONFIG}" \
            -cook \
            -build \
            -stage \
            -pak \
            -archive \
            -archivedirectory="${BUILD_DIR}" \
            -prereqs \
            -nodebuginfo \
            -utf8output \
            -compile

        echo ""
        echo "============================================================================"
        echo "  [SUCCESS] Game packaged successfully!"
        echo "  Output: ${BUILD_DIR}/${PLATFORM}NoEditor"
        echo "============================================================================"
        ;;

    clean)
        echo "[CLEANING] Removing build artifacts..."
        rm -rf "${SCRIPT_DIR}/Binaries"
        rm -rf "${SCRIPT_DIR}/Intermediate"
        rm -rf "${SCRIPT_DIR}/Saved"
        rm -rf "${BUILD_DIR}"
        echo "[SUCCESS] Build artifacts cleaned!"
        ;;

    *)
        echo "[ERROR] Unknown command: ${COMMAND}"
        echo "Usage: ./BuildGame.sh [build|cook|package|clean]"
        exit 1
        ;;
esac

echo ""
echo "Done."
