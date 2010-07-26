set(BASE_SRC 
src/configuration.cpp
src/game.cpp
src/globals.cpp
src/debug.cpp
src/loading.cpp
src/music.cpp
src/main-menu.cpp
src/init.cpp
src/nameplacer.cpp
src/select_player.cpp
src/resource.cpp
src/world.cpp
src/versus_world.cpp)

set(GAME_SRC
src/game/console.cpp
src/game/adventure_world.cpp
src/game/mod.cpp
src/input/input.cpp
src/input/text-input.cpp
src/input/input-manager.cpp)

set(SCRIPT_SRC
src/script/script.cpp
src/script/modules/python.cpp
src/script/modules/ruby.cpp)

set(OGG_SRC
src/ogg/logg.c)

set(LEVEL_SRC
src/level/block.cpp
src/level/blockobject.cpp
src/level/cacher.cpp
src/level/scene.cpp
src/level/random-scene.cpp
src/level/utils.cpp)

set(NETWORK_SRC
src/network/cacher.cpp
src/network/client.cpp
src/network/chat_client.cpp
src/network/chat_server.cpp
src/network/chat-widget.cpp
src/network/messages.cpp
src/network/network.cpp
src/network/network_world.cpp
src/network/network_world_client.cpp
src/network/server.cpp)

set(ENV_SRC
src/environment/atmosphere.cpp
src/environment/rain_atmosphere.cpp
src/environment/snow_atmosphere.cpp
src/environment/night_atmosphere.cpp
src/environment/fog_atmosphere.cpp)

set(FACTORY_SRC
src/factory/font_factory.cpp
src/factory/heart_factory.cpp
src/factory/object_factory.cpp
src/factory/font_render.cpp
src/factory/collector.cpp
src/factory/shadow.cpp)

set(OBJECT_SRC
src/object/actor.cpp
src/object/animation.cpp
src/object/animation_event.cpp
src/object/animation_trail.cpp
src/object/buddy_player.cpp
src/object/cat.cpp
src/object/display_character.cpp
src/object/draw-effect.cpp
src/object/effect.cpp
src/object/enemy.cpp
src/object/gib.cpp
src/object/item.cpp
src/object/network_character.cpp
src/object/network_player.cpp
src/object/object.cpp
src/object/object_attack.cpp
src/object/object_nonattack.cpp
src/object/player.cpp
src/object/player-common.cpp
src/object/versus_player.cpp
src/object/versus_enemy.cpp
src/object/projectile.cpp
src/object/attack.cpp
src/object/stimulation.cpp
src/object/health_stimulation.cpp
src/object/character.cpp
src/object/heart.cpp)

set(UTIL_SRC
src/util/token.cpp
src/util/token_exception.cpp
src/util/tokenreader.cpp
src/util/timedifference.cpp
src/util/sound.cpp
src/util/timer.cpp
src/input/keyboard.cpp
src/util/bitmap.cpp
src/util/events.cpp
src/util/font.cpp
src/util/fire.cpp
src/util/ftalleg.cpp
src/util/funcs.cpp
src/util/file-system.cpp
src/util/gradient.cpp
src/util/music-player.cpp
src/util/ebox.cpp
src/util/regex.cpp
src/util/language-string.cpp
src/util/thread.cpp
src/input/joystick.cpp
src/input/allegro/allegro-joystick.cpp
src/input/sdl/joystick.cpp
src/input/psp/joystick.cpp
src/input/wii/joystick.cpp
src/util/system.cpp
src/util/compress.cpp
src/util/message-queue.cpp
src/input/linux_joystick.cpp
src/util/load_exception.cpp
src/windows/funcs.cpp
src/windows/system.cpp
src/exceptions/exception.cpp
src/openbor/pack-reader.cpp
src/openbor/animation.cpp
src/openbor/display-character.cpp
src/openbor/mod.cpp
src/openbor/util.cpp
src/openbor/data.cpp)

set(LOADPNG_SRC
src/loadpng/loadpng.c
src/loadpng/regpng.c
src/loadpng/savepng.c)

set(MENU_SRC
src/menu/actionfactory.cpp
src/menu/action_speed.cpp
src/menu/menu.cpp
src/menu/menu_action.cpp
src/menu/menu_global.cpp
src/menu/menu_option.cpp
src/menu/options.cpp
src/menu/optionfactory.cpp
src/menu/tab_menu.cpp
)

set(GUI_SRC
src/gui/al_keyinput.cpp
src/gui/animation.cpp
src/gui/box.cpp
src/gui/container.cpp
src/gui/context-box.cpp
src/gui/coordinate.cpp
src/gui/fadetool.cpp
src/gui/keys.cpp
src/gui/keyinput.cpp
src/gui/keyinput_manager.cpp
src/gui/lineedit.cpp
src/gui/rectarea.cpp
src/gui/popup-box.cpp
src/gui/tabbed-box.cpp
src/gui/timer.cpp
src/gui/widget.cpp
)

set(MUGEN_SRC
src/mugen/background.cpp
src/mugen/behavior.cpp
src/mugen/characterhud.cpp
src/mugen/character.cpp
src/mugen/character-select.cpp
src/mugen/config.cpp
src/mugen/evaluator.cpp
src/mugen/compiler.cpp
src/mugen/game.cpp
src/mugen/command.cpp
src/mugen/storyboard.cpp
src/mugen/state.cpp
src/mugen/animation.cpp
src/mugen/exception.cpp
src/mugen/font.cpp
src/mugen/item.cpp
src/mugen/item-content.cpp
src/mugen/menu.cpp
src/mugen/reader.cpp
src/mugen/section.cpp
src/mugen/sound.cpp
src/mugen/sprite.cpp
src/mugen/stage.cpp
src/mugen/util.cpp
src/mugen/option-arcade.cpp
src/mugen/option-options.cpp
src/mugen/option-versus.cpp
src/mugen/parse-cache.cpp
src/mugen/parser/parse-exception.cpp
src/mugen/parser/def.cpp
src/mugen/parser/cmd.cpp
src/mugen/parser/air.cpp
)

set(TRIGGER_SRC
src/trigger/trigger.cpp
)
