# Dungeon
# Copyright (c) 2023 Jovan Lanik

dungeon_set = files([
  'src/d_draw.c',
  'src/d_format.c',
  'src/d_menu.c',
  'src/d_text.c',
  'src/wip.c',
  'src/wip_game.c',
  'src/wip_keys.c',
])

dungeon_glsl = files([
  'glsl/d_background.frag',
  'glsl/d_background.vert',
  'glsl/d_texture.frag',
  'glsl/d_texture.vert',
])

foreach shader : dungeon_glsl
  if glslang.found()
    run_command(glslang, shader, check : true)
  endif
  shader_id = fs.name(shader)
  shader_c = custom_target(
    shader_id,
    output : shader_id + '.c',
    input : [ bake, shader ],
    command : [ python, '@INPUT0@', '@INPUT1@', '-o', '@OUTPUT@' ],
  )
  dungeon_set += shader_c
endforeach

# TODO: move to dungeon.d subdir
dungeon_conf = files('../res/conf/dungeon.conf')
dungeon_conf_id = fs.name(dungeon_conf)
dungeon_conf_c = custom_target(
  dungeon_conf_id,
  output : dungeon_conf_id + '.c',
  input : [ bake, dungeon_conf ],
  command : [ python, '@INPUT0@', '@INPUT1@', '-o', '@OUTPUT@' ],
)
dungeon_set += dungeon_conf_c

#dungeon_lua = files('../res/lua/dungeon.lua')
#dungeon_lua_id = fs.name(dungeon_lua)
#dungeon_lua_c = custom_target(
#  dungeon_lua_id,
#  output : dungeon_lua_id + '.c',
#  input : [ bake, dungeon_lua ],
#  command : [ python, '@INPUT0@', '@INPUT1@', '-o', '@OUTPUT@' ],
#)
#dungeon_set += dungeon_lua_c

dungeon_include_dirs = include_directories('include')

executable(
  'dungeon',
  sources: dungeon_set,
  include_directories : dungeon_include_dirs,
  dependencies : wip_dep,
  export_dynamic : true,
)
