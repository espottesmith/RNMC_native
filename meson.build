project(
  'RNMC',
  'c',
  version: '1.0',
  license: 'BSD3-Clause-LBNL',
)

add_project_arguments('-Wno-unused-but-set-variable', language: 'c')
add_project_arguments('-Wno-unused-variable', language: 'c')


cc = meson.get_compiler('c')
gsl = cc.find_library('gsl')
gslcblas = cc.find_library('gslcblas')
pthread = cc.find_library('pthread')
# linux has a standalone math header so we need to have it as an explicit dependency
math_dep = cc.find_library('m', required : false)

common_files = [
  'src/reaction_network.c',
  'src/sampler.c',
  'src/simulation.c',
  'src/solvers.c',
  'src/dispatcher.c'
]

RNMC_include = include_directories('src')

executable(
  'RNMC',
  files(['src/RNMC.c'] + common_files),
  dependencies: [gsl, gslcblas, math_dep, pthread],
  include_directories: [RNMC_include],
  install: true
)

tests = executable(
  'RNMC_test',
  files(['src/test.c'] + common_files),
  dependencies: [gsl, gslcblas, math_dep, pthread],
  include_directories: [RNMC_include],
  install: false
)

test('tests', tests, args: ['../test_materials'])
