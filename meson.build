project('rnl', 'cpp', version: '0.1', default_options: ['cpp_std=c++17'])

# Dependency for JavaScriptCore
deps = [
  dependency('javascriptcoregtk-6.0', required: true),
  dependency('libadwaita-1', required: true),
]

# Include both the native/include and native/include/rnl directories
incdir = include_directories('native/include')
rnl_incdir = include_directories('native/include/rnl')

source_dir = meson.current_source_dir()

message('Include directory (relative): native/include')
message('Include directory (absolute): ' + join_paths(source_dir, 'native/include'))

# Define source directory and the source files
src_dir = 'native/src'

# Create executable
executable('rnl',
  [src_dir / 'main.cpp'],
  cpp_args: '-DBUNDLE_PATH=bundle.js',
  dependencies: deps,
  install: true,
  include_directories : [incdir, rnl_incdir]  # Include both directories
)

# Optionally, create a custom test
test('rnl_test', executable('rnl_test',
  [src_dir / 'main.cpp'],
  dependencies: deps,
  include_directories : [incdir, rnl_incdir]  # Include both directories
))
