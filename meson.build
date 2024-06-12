# WIP
# Copyright (c) 2023 Jovan Lanik

# TODO: default compiler options
# TODO: default linker options
# TODO: debug options
# TODO: check windows support
# TODO: support building as subproject

project('WIP', 'c', version : 'prerelease', default_options : ['c_std=c11'])

game = get_option('game')
lua_backend = get_option('lua_backend')
window_backend = get_option('window_backend')

add_project_arguments(
  '-DWIP_GAME=' + game,
  '-DWIP_LUA_BACKEND=wip_' + lua_backend,
  '-DWIP_WINDOW_BACKEND=wip_' + window_backend,
  language : 'c',
)

wip_set = files([
  'src/wip_arg.c',
  'src/wip_conf.c',
  'src/wip_event.c',
  'src/wip_fn.c',
  'src/wip_gl.c',
  'src/wip_img.c',
  'src/wip_input.c',
  'src/wip_mdl.c',
  'src/wip_obj.c',
  'src/wip_lua.c',
  'src/wip_hashmap.c',
  'src/optional/wip_window_' + window_backend + '.c',
])

wip_glsl = files([])

fs = import('fs')
bake = files('util/bake.py')
python = find_program('python3')
glslang = find_program('glslangValidator', required : false)

foreach shader : wip_glsl
  if glslang.found()
    run_command(glslang, shader, check : true)
  endif
  shader_id = fs.name(shader)
  shader_c = custom_target(
    shader_id,
    output : shader_id + '.c',
    input : [ bake, shader ],
    command : [ python, '@INPUT0@', '@INPUT1@', '-o', '@OUTPUT@' ]
  )
  wip_set += shader_c
endforeach

wip_include_dirs = include_directories('include')

cc = meson.get_compiler('c')

m_dep = cc.find_library('m', required : false)
gl_dep = dependency('gl')
libconfig_dep = dependency('libconfig')
lua_backend_dep = dependency(lua_backend)
window_backend_dep = dependency(window_backend)

external_include_dirs = include_directories('include/external')

glad_set = files('src/external/glad.c')

glad_dep = declare_dependency(
  sources: glad_set,
  include_directories : external_include_dirs,
)

wip_deps = [
  m_dep,
  gl_dep,
  libconfig_dep,
  lua_backend_dep,
  window_backend_dep,
  glad_dep,
]

wip_dep = declare_dependency(
  sources: wip_set,
  include_directories : wip_include_dirs,
  dependencies : wip_deps,
)

subdir(game + '.d')
