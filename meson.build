project(
  'testzmq',
  'cpp',
  version : '1.0.0',
  default_options : [
    'warning_level=3',
    'cpp_std=c++2a',
    'optimization=g',
  ])

thread_dep = dependency('threads')
cppzmq_dep = dependency('cppzmq')
nlohmann_json_dep = subproject('nlohmann_json').get_variable('nlohmann_json_dep')

ludo_incude_dep = declare_dependency(
  include_directories: include_directories('include')
)

executable(
  'zmqserver',
  'src/server.cpp',
  dependencies : [thread_dep, cppzmq_dep, nlohmann_json_dep, ludo_incude_dep],
)
executable(
  'zmqclient',
  'src/client.cpp',
  dependencies : [thread_dep, cppzmq_dep, nlohmann_json_dep, ludo_incude_dep],
)
executable(
  'zmqparallel',
  'src/tasks.cpp',
  dependencies : [thread_dep, cppzmq_dep, nlohmann_json_dep, ludo_incude_dep],
)
