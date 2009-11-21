set(BASE_SRC 
src/configuration.cpp
src/game.cpp
src/globals.cpp
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
src/level/scene.cpp
src/level/utils.cpp)

set(NETWORK_SRC
src/network/client.cpp
src/network/chat_client.cpp
src/network/chat_server.cpp
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
src/object/animation_event_attack.cpp
src/object/animation_event_bbox.cpp
src/object/animation_event_coords.cpp
src/object/animation_event_delay.cpp
src/object/animation_event_face.cpp
src/object/animation_event_frame.cpp
src/object/animation_event_jump.cpp
src/object/animation_event_move.cpp
src/object/animation_event_nop.cpp
src/object/animation_event_projectile.cpp
src/object/animation_event_offset.cpp
src/object/animation_event_shadow.cpp
src/object/animation_event_sound.cpp
src/object/animation_event_status.cpp
src/object/animation_event_ticket.cpp
src/object/animation_event_trails.cpp
src/object/animation_event_zdistance.cpp
src/object/animation_trail.cpp
src/object/buddy_player.cpp
src/object/cat.cpp
src/object/display_character.cpp
src/object/draw-effect.cpp
src/object/draw-countdown-effect.cpp
src/object/draw-normal-effect.cpp
src/object/draw-glow-effect.cpp
src/object/draw-until-effect.cpp
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
src/input/keyboard.cpp
src/util/bitmap.cpp
src/util/font.cpp
src/util/ftalleg.cpp
src/util/funcs.cpp
src/util/file-system.cpp
src/util/gradient.cpp
src/util/ebox.cpp
src/util/regex.cpp
src/util/language-string.cpp
src/input/joystick.cpp
src/input/allegro-joystick.cpp
src/util/system.cpp
src/util/message-queue.cpp
src/input/linux_joystick.cpp
src/util/load_exception.cpp
src/windows/funcs.cpp
src/windows/system.cpp)

set(LOADPNG_SRC
src/loadpng/loadpng.c
src/loadpng/regpng.c
src/loadpng/savepng.c)

set(MENU_SRC
src/menu/actionfactory.cpp
src/menu/action_speed.cpp
src/menu/menu.cpp
src/menu/menu_action.cpp
src/menu/menu_animation.cpp
src/menu/menu_global.cpp
src/menu/menu_option.cpp
src/menu/option_adventure.cpp
src/menu/option_adventure_cpu.cpp
src/menu/option_credits.cpp
src/menu/option_dummy.cpp
src/menu/option_fullscreen.cpp
src/menu/option_invincible.cpp
src/menu/option_change_mod.cpp
src/menu/option_key.cpp
src/menu/option_joystick.cpp
src/menu/option_level.cpp
src/menu/option_lives.cpp
src/menu/option_playmode.cpp
src/menu/option_network_host.cpp
src/menu/option_network_join.cpp
src/menu/option_npc_buddies.cpp
src/menu/option_menu.cpp
src/menu/option_mugen_menu.cpp
src/menu/option_quit.cpp
src/menu/option_versus.cpp
src/menu/option_select_font.cpp
src/menu/option_screen_size.cpp
src/menu/option_speed.cpp
src/menu/option_tabmenu.cpp
src/menu/optionfactory.cpp
src/menu/tab_menu.cpp
)

set(GUI_SRC
src/gui/al_keyinput.cpp
src/gui/box.cpp
src/gui/container.cpp
src/gui/keys.cpp
src/gui/keyinput.cpp
src/gui/keyinput_manager.cpp
src/gui/lineedit.cpp
src/gui/rectarea.cpp
src/gui/timer.cpp
src/gui/widget.cpp
)

set(MUGEN_SRC
src/mugen/characterhud.cpp
src/mugen/mugen_animation.cpp
src/mugen/mugen_background.cpp
src/mugen/mugen_character.cpp
src/mugen/mugen_exception.cpp
src/mugen/mugen_fadetool.cpp
src/mugen/mugen_font.cpp
src/mugen/mugen_item.cpp
src/mugen/mugen_item_content.cpp
src/mugen/mugen_menu.cpp
src/mugen/mugen_reader.cpp
src/mugen/mugen_section.cpp
src/mugen/mugen_sound.cpp
src/mugen/mugen_sprite.cpp
src/mugen/mugen_stage.cpp
src/mugen/mugen_storyboard.cpp
src/mugen/mugen_util.cpp
src/mugen/option_versus.cpp
src/mugen/parser/parse-exception.cpp
src/mugen/parser/def.cpp
)

set(TRIGGER_SRC
src/trigger/trigger.cpp
)
