# Flipdot demo
# Copyright (c) 2023 Jovan Lanik

flipdot_set = files([
  'src/wip.c',
  'src/wip_game.c',
  'src/wip_keys.c',
])

flipdot_glsl = files([
  'glsl/flipdot.frag',
  'glsl/flipdot.vert',
])

foreach shader : flipdot_glsl
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
  flipdot_set += shader_c
endforeach

# TODO: move to flipdot.d subdir
flipdot_conf = files('../res/conf/flipdot.conf')
flipdot_conf_id = fs.name(flipdot_conf)
flipdot_conf_c = custom_target(
  flipdot_conf_id,
  output : flipdot_conf_id + '.c',
  input : [ bake, flipdot_conf ],
  command : [ python, '@INPUT0@', '@INPUT1@', '-o', '@OUTPUT@' ]
)
flipdot_set += flipdot_conf_c

flipdot_include_dirs = include_directories('include')

# TODO: move to flipdot.d subdir
wfdd_set = files('../src/external/wfdd.c')
wfdd_dep = declare_dependency(
  sources: wfdd_set,
  include_directories : external_include_dirs,
)

executable(
  'flipdot',
  sources: flipdot_set,
  include_directories : flipdot_include_dirs,
  dependencies : [ wip_dep, wfdd_dep],
)
