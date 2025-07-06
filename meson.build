project('nlc', 'cpp')

libnlc_src = []
libnlc_inc = include_directories('include')
libnlc_deps = [ dependency('llvm', version: '>=14.0.0') ]

subdir('libnlc')

src = []
nlc_inc = include_directories('src/')

subdir('src')

libnlc = shared_library(
  'nlc',
  libnlc_src,
  include_directories: libnlc_inc,
  dependencies: libnlc_deps,
)

nlc = executable(
  'nlc',
  src,
  include_directories: [ nlc_inc, libnlc_inc ],
  link_with: libnlc,
)

subdir('tests')
