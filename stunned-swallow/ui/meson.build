project('stunned-swallow', 'c', 'cpp',
  version : '0.1.0',
  default_options : ['cpp_std=c++latest', 'b_vscrt=static_from_buildtype'],
  meson_version : '>=0.51.0')

cmake = import('cmake')
sub_proj = cmake.subproject('nana')
nana_lib = sub_proj.dependency('nana')

nlohmann_json_proj = subproject('nlohmann_json')
libjson_dep = nlohmann_json_proj.get_variable('nlohmann_json_dep')

executable('stunned-swallow',
  ['src/main.cc', 'src/injector.h', 'src/injector.cc', 'src/named_pipe.h', 'src/named_pipe.cc'] +
  ['nfd/include/nfd.h', 'nfd/simple_exec.h', 'nfd/nfd_win.cpp', 'nfd/nfd_common.h', 'nfd/nfd_common.c', 'nfd/common.h'],
  include_directories : ['nfd/include/', 'nfd/'],
  dependencies : [libjson_dep, nana_lib])