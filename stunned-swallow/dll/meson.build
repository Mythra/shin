project('ss-dll', 'cpp',
  version : '0.1.0',
  default_options : ['cpp_std=c++latest'],
  meson_version : '>=0.50.0')

build_specific_sources = []
build_specific_include_dirs = []
build_specific_pch = []

if get_option('build-id') == 'b6326492'
  build_specific_sources = [
    'versioned/b6326492/win/appdata/il2cpp-api-functions-ptr.h', 'versioned/b6326492/win/appdata/il2cpp-api-functions.h', 'versioned/b6326492/win/appdata/il2cpp-functions.h', 'versioned/b6326492/win/appdata/il2cpp-metadata-version.h', 'versioned/b6326492/win/appdata/il2cpp-types-ptr.h', 'versioned/b6326492/win/appdata/il2cpp-types.h',
    'versioned/b6326492/win/framework/dllmain.cpp', 'versioned/b6326492/win/framework/helpers.cpp', 'versioned/b6326492/win/framework/helpers.h', 'versioned/b6326492/win/framework/il2cpp-appdata.h', 'versioned/b6326492/win/framework/il2cpp-init.cpp', 'versioned/b6326492/win/framework/il2cpp-init.h'
  ]
  build_specific_include_dirs = ['versioned/b6326492/win/appdata/', 'versioned/b6326492/win/framework/']
  build_specific_pch = ['versioned/b6326492/win/framework/pch-il2cpp.h']

  add_global_arguments('-DPHASMO_BID=b6326492', language : 'cpp')
else
  assert(false, 'Phasmophobia Build ID is unknown!')
endif

nlohmann_json_proj = subproject('nlohmann_json')
libjson_dep = nlohmann_json_proj.get_variable('nlohmann_json_dep')
thread_dep = dependency('threads')

library('ss', 
  [
    'src/main.cc',
    'src/main.h',
    'src/named_pipe.h',
    'src/named_pipe.cc',
    'src/command_handler.h',
    'src/command_handler.cc',
    'src/handlers/dump_localization.h',
    'src/handlers/dump_localization.cc',
    'src/handlers/dump_voice.h',
    'src/handlers/dump_voice.cc',
    'src/il2.h',
    'src/il2.cc',
    'src/csv.h',
    'src/concurrentqueue.h'
  ] + build_specific_sources,
  include_directories : ['src/'] + build_specific_include_dirs,
  dependencies : [libjson_dep, thread_dep],
  cpp_pch : build_specific_pch)