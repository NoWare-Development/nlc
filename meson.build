project('nlc', 'cpp')

libnlc_src = []
libnlc_inc = include_directories('include')
libnlc_deps = []
subdir('libnlc')

nlc_src = []
nlc_inc = include_directories('src')
subdir('src')

libnlc = shared_library(
  'nlc',
  libnlc_src,
  include_directories: libnlc_inc,
  dependencies: libnlc_deps,
)

nlc = executable(
  'nlc',
  nlc_src,
  include_directories: [ nlc_inc, libnlc_inc ],
  link_with: libnlc,
)
