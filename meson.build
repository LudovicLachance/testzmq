project(
  'testzmq',
  'cpp',
  version : '0.1',
  default_options : [
    'warning_level=3',
    'cpp_std=c++2a',
    'optimization=3',
  ])

cppzmq = dependency('cppzmq')
nlohmann_json = subproject('nlohmann_json')
nlohmann_json_dep = nlohmann_json.get_variable('nlohmann_json_dep')

ludo_incude_dep = declare_dependency(
  include_directories: include_directories('include')
)

executable(
  'zmqserver',
  'src/server.cpp',
  dependencies : [cppzmq, nlohmann_json_dep, ludo_incude_dep],
)
executable(
  'zmqclient',
  'src/client.cpp',
  dependencies : [cppzmq, nlohmann_json_dep, ludo_incude_dep],
)
