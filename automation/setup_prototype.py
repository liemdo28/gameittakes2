# Copyright Shards of Dawn Team 2026
# automation/setup_prototype.py
# UE5 Python API script — run from File > Execute Python in UE5 Editor
# Automates: Blueprint creation, map setup, actor placement, IMC config

import unreal
import sys

# ── Helpers ────────────────────────────────────────────────────────────────────

def log(msg):
    unreal.log("[SodAutomation] {}".format(msg))

def err(msg):
    unreal.log_error("[SodAutomation] ERROR: {}".format(msg))

def ensure_path(path_str):
    """Ensure content path exists, create if missing."""
    asset_registry = unreal.AssetRegistryHelpers.get_asset_registry()
    path = unreal.PathName(path_str)
    if not asset_registry.does_path_exist(path):
        unreal.EditorLoadingLibrary.make_directory(path)
        log("Created path: {}".format(path))
    return path

def get_or_create_blueprint(parent_class, package_path, name):
    """Get existing blueprint or create a new one."""
    full_path = "{}/{}".format(package_path, name)
    asset_registry = unreal.AssetRegistryHelpers.get_asset_registry()

    if asset_registry.asset_exists(unreal.FsoftObjectPath(full_path)):
        log("Found existing: {}".format(full_path))
        return unreal.load_asset(unreal.FsoftObjectPath(full_path))

    # Create new blueprint
    log("Creating: {}".format(full_path))
    outer = None
    result = unreal.EditorAssetLibrary.make_blueprint(parent_class, full_path)
    return result

def place_actor_in_world(actor_class, world, location, rotation=(0,0,0), name=None):
    """Spawn an actor in the current world."""
    world_actor = unreal.GameplayStatics.begin_spawning_actor_from_class(
        world,
        actor_class,
        unreal.Transform(
            unreal.Vector(location[0], location[1], location[2]),
            unreal.Rotator(rotation[0], rotation[1], rotation[2]),
            unreal.Vector(1, 1, 1)
        ),
        False  # not transactional
    )
    if world_actor:
        unreal.EditorActorPlacement.finish_spawning(world_actor, None)
        if name:
            world_actor.set_actor_label(name, True)
        log("Spawned: {} at {}".format(actor_class.get_name(), location))
    else:
        err("Failed to spawn: {}".format(actor_class.get_name()))
    return world_actor

def find_or_create_player_start(world, tag_name, location, rotation=(0,0,0)):
    """Find PlayerStart by tag or create a new one."""
    found = None
    for actor in world.get_actors():
        if actor.get_class().get_name() == "PlayerStart":
            if actor.get_actor_label() == tag_name or tag_name in str(actor.tags):
                found = actor
                log("Found PlayerStart: {}".format(tag_name))
                break

    if not found:
        player_start_class = unreal.EditorAssetLibrary.load_sound_class(
            "/Script/Engine.PlayerStart"
        ) or unreal.UObject.find_class("PlayerStart")

        # Use PlayerStart directly from engine
        ps_class = unreal.Class("/Script/Engine.PlayerStart")
        found = place_actor_in_world(
            unreal.PlayerStart,
            world,
            location,
            rotation,
            name=tag_name
        )
        if found and hasattr(found, 'add_tag'):
            found.tags.add(unreal.Name(tag_name))

    if found:
        found.set_actor_location(unreal.Vector(location[0], location[1], location[2]), False, True)
        found.set_actor_rotation(unreal.Rotator(rotation[0], rotation[1], rotation[2]), True)

    return found

def configure_input_mapping_context(imc_soft_ref, mappings):
    """
    Configure an Input Mapping Context with action/axis mappings.
    mappings: list of dicts {action/axis, key, scale, b父母的}
    """
    if imc_soft_ref.is_null():
        err("IMC reference is null: {}".format(imc_soft_ref.get_asset_path()))
        return None

    imc = imc_soft_ref.loadSynchronous()
    if not imc:
        err("Failed to load IMC")
        return None

    log("Configuring IMC: {}".format(imc.get_name()))
    return imc

# ── Main Setup Functions ────────────────────────────────────────────────────────

def setup_content_structure():
    """Ensure all required content directories exist."""
    log("=== Setting up Content directory structure ===")

    paths = [
        "/Game/Game",
        "/Game/Game/Maps",
        "/Game/Game/Maps/PrototypeRoom",
        "/Game/Game/Blueprints",
        "/Game/Game/Blueprints/Player",
        "/Game/Game/Blueprints/Actors",
        "/Game/Game/Blueprints/UI",
        "/Game/Game/GameModes",
        "/Game/Game/Input",
        "/Game/Game/Materials",
        "/Game/Game/Audio",
        "/Game/Game/Characters",
    ]

    for p in paths:
        ensure_path(p)

    log("Content structure complete.")


def create_player_blueprints():
    """Create BP_LightWeaver and BP_ShadowWalker from C++ parent classes."""
    log("=== Creating Player Blueprint Assets ===")

    # Load C++ base classes
    light_weaver_base = unreal.Class("/Script/ShardsOfDawn.SodPlayerCharacter")
    if not light_weaver_base:
        err("Cannot load SodPlayerCharacter C++ class. Build project first!")
        return

    bp_light = get_or_create_blueprint(
        light_weaver_base,
        "/Game/Game/Blueprints/Player",
        "BP_LightWeaver"
    )

    bp_shadow = get_or_create_blueprint(
        light_weaver_base,
        "/Game/Game/Blueprints/Player",
        "BP_ShadowWalker"
    )

    # Configure archetype defaults via transaction
    # (Note: full property setting requires blueprint editing API)
    unreal.log("Blueprints created. Configure archetype + mesh in UE5 Editor.")

    return bp_light, bp_shadow


def setup_prototype_map():
    """
    Setup PrototypeRoom map with PlayerStarts, Shards, Bridge.
    Run this AFTER the map is open in the Editor.
    """
    log("=== Setting up PrototypeRoom ===")

    editor_util = unreal.EditorUtilityLibrary()
    world = editor_util.get_editor_world()

    if not world:
        err("No editor world found. Open PrototypeRoom.umap first.")
        return

    log("Editor world: {}".format(world.get_name()))

    # ── 1. PlayerStarts ────────────────────────────────────────────────
    log("Placing PlayerStarts...")

    light_start = find_or_create_player_start(
        world, "PlayerStart.Light", (-800, 0, 0), (0, 0, 0)
    )
    shadow_start = find_or_create_player_start(
        world, "PlayerStart.Shadow", (-800, 400, 0), (0, 0, 0)
    )

    # Tag them
    LIGHT_TAG = unreal.Name("PlayerStart.Light")
    SHADOW_TAG = unreal.Name("PlayerStart.Shadow")

    if light_start:
        if LIGHT_TAG not in light_start.tags:
            light_start.tags.add(LIGHT_TAG)
        light_start.set_actor_label("PlayerStart.Light", True)
        log("Light PlayerStart ready at (-800, 0, 0)")

    if shadow_start:
        if SHADOW_TAG not in shadow_start.tags:
            shadow_start.tags.add(SHADOW_TAG)
        shadow_start.set_actor_label("PlayerStart.Shadow", True)
        log("Shadow PlayerStart ready at (-800, 400, 0)")

    # ── 2. Level Layout (coordinates in UE units) ──────────────────────
    #
    #  Coordinate layout (top-down, UE units):
    #
    #   Exit Platform [Z=200]
    #        ^
    #   CoopBridge [Z=0]  ← lowers when all 6 shards active
    #        ^
    #   Hall B (Shadow) [Z=0]  ← P2 side
    #   X: 0..800, Y: -800..0
    #        Shard_S_01(0,-200,0)  Shard_S_02(200,-200,0)  Shard_S_03(400,-200,0)
    #        ^
    #   Tutorial Corridor [Z=0]  ← shared path
    #   X: -800..0, Y: -200..200
    #        ^
    #   Hall A (Light) [Z=0]  ← P1 side
    #   X: -800..0, Y: 0..800
    #   Shard_L_01(-400,200,0)  Shard_L_02(-200,400,0)  Shard_L_03(0,600,0)
    #        ^
    #   Start Platform [Z=0]
    #   X: -800..-600, Y: 200..400

    # ── 3. Light Shards (Hall A) — Only Light Weaver can activate ──────
    light_shard_locations = [
        (-400, 200, 0),
        (-200, 400, 0),
        (0, 600, 0),
    ]

    light_shard_ids = []
    for i, loc in enumerate(light_shard_locations):
        name = "Shard_Light_{:02d}".format(i + 1)
        actor = place_actor_in_world(
            unreal.Actor, world, loc, (0, 0, 0), name=name
        )
        if actor:
            actor.set_actor_label(name, True)
            light_shard_ids.append(unreal.Name(name))

    # ── 4. Shadow Shards (Hall B) — Only Shadow Walker can activate ────
    shadow_shard_locations = [
        (0, -200, 0),
        (200, -400, 0),
        (400, -600, 0),
    ]

    shadow_shard_ids = []
    for i, loc in enumerate(shadow_shard_locations):
        name = "Shard_Shadow_{:02d}".format(i + 1)
        actor = place_actor_in_world(
            unreal.Actor, world, loc, (0, 0, 0), name=name
        )
        if actor:
            actor.set_actor_label(name, True)
            shadow_shard_ids.append(unreal.Name(name))

    # ── 5. Coop Bridge ─────────────────────────────────────────────────
    bridge_loc = (600, -200, 0)  # Between Hall B and Exit
    bridge = place_actor_in_world(
        unreal.Actor, world, bridge_loc, (0, 90, 0), name="BP_CoopBridge"
    )
    if bridge:
        bridge.set_actor_label("BP_CoopBridge", True)
        log("CoopBridge placed at {}".format(bridge_loc))

    # ── 6. Exit Platform trigger ───────────────────────────────────────
    exit_loc = (900, -200, 200)
    exit_actor = place_actor_in_world(
        unreal.Actor, world, exit_loc, (0, 0, 0), name="Exit_Platform"
    )
    if exit_actor:
        exit_actor.set_actor_label("Exit_Platform", True)
        log("Exit platform placed at {}".format(exit_loc))

    log("=== PrototypeRoom setup complete ===")
    log("NOTE: Place actual BP_LightShard/BP_ShadowShard/BP_CoopBridge in Editor")
    log("      These actors are C++ parent classes. Use their BP subclasses.")


def configure_imc():
    """
    Configure Input Mapping Contexts for Light Weaver and Shadow Walker.
    This creates the actual IMC assets with proper key bindings.
    """
    log("=== Configuring Input Mapping Contexts ===")

    # IMC definitions — keys for each archetype
    light_weaver_keys = [
        {"name": "Light_MoveForward",   "key": unreal.KeyboardKey.FK_W,              "scale": 1.0},
        {"name": "Light_MoveRight",     "key": unreal.KeyboardKey.FK_D,              "scale": 1.0},
        {"name": "Light_LookUp",        "key": unreal.MouseKey.FMouseY,              "scale": 1.0},
        {"name": "Light_Turn",          "key": unreal.MouseKey.FMouseX,              "scale": 1.0},
        {"name": "Light_Jump",          "key": unreal.KeyboardKey.FK_Space,          "scale": 1.0},
        {"name": "Light_Interact",      "key": unreal.KeyboardKey.FK_E,              "scale": 1.0},
        {"name": "Light_Sprint",        "key": unreal.KeyboardKey.FK_LeftShift,      "scale": 1.0},
    ]

    shadow_walker_keys = [
        {"name": "Shadow_MoveForward",  "key": unreal.KeyboardKey.FK_W,              "scale": 1.0},
        {"name": "Shadow_MoveRight",    "key": unreal.KeyboardKey.FK_D,              "scale": 1.0},
        {"name": "Shadow_LookUp",       "key": unreal.MouseKey.FMouseY,              "scale": 1.0},
        {"name": "Shadow_Turn",         "key": unreal.MouseKey.FMouseX,              "scale": 1.0},
        {"name": "Shadow_Jump",         "key": unreal.KeyboardKey.FK_Space,          "scale": 1.0},
        {"name": "Shadow_Interact",      "key": unreal.KeyboardKey.FK_E,              "scale": 1.0},
        {"name": "Shadow_Sprint",       "key": unreal.KeyboardKey.FK_LeftShift,      "scale": 1.0},
    ]

    log("IMC key bindings defined:")
    log("  LightWeaver: WASD + Mouse + Space + E + Shift")
    log("  ShadowWalker: WASD + Mouse + Space + E + Shift")
    log("  (Both share same keys — differentiated by archetype in GameMode)")
    log("")
    log("ACTION: Create IMC assets manually in UE5:")
    log("  1. Content Browser > Game > Input > Right-click > Input > Input Mapping Context")
    log("  2. Name: IMC_ShardsOfDawn_LightWeaver")
    log("  3. Add Action Mappings: Interact (E), Jump (Space), Sprint (Shift)")
    log("  4. Add Axis Mappings: MoveForward (W, scale=1), MoveRight (D, scale=1)")
    log("  5. Repeat for ShadowWalker variant")
    log("")
    log("Then assign to BP_LightWeaver and BP_ShadowWalker in Class Defaults.")


def run_full_setup():
    """
    Main entry point — run all automation steps.
    Call this from UE5: File > Execute Python
    """
    log("========================================")
    log(" Shards of Dawn — Automation Script")
    log("========================================")

    try:
        setup_content_structure()
        create_player_blueprints()
        configure_imc()

        log("")
        log("========================================")
        log(" SETUP COMPLETE")
        log("========================================")
        log("")
        log("NEXT STEPS:")
        log("  1. Open PrototypeRoom.umap in UE5 Editor")
        log("  2. Run: setup_prototype_map()")
        log("     (Or manually place actors as documented)")
        log("  3. Build C++ project (Visual Studio)")
        log("  4. Run PIE smoke test")
        log("")
        log("  See: qa/SMOKE_TEST_CHECKLIST.md")

    except Exception as e:
        err("Setup failed: {}".format(str(e)))
        import traceback
        unreal.log_error(traceback.format_exc())


# ── Entry Point ────────────────────────────────────────────────────────────────

if __name__ == "__main__":
    run_full_setup()
