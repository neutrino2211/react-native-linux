# meson.build
project('my_js_project', 'cpp', version: '0.1', default_options: ['cpp_std=c++17'])

# Dependency for JavaScriptCore
deps = [
  dependency('javascriptcoregtk-6.0', required: true),
  dependency('libadwaita-1', required: true),
]

# Define source directory and the source files
src_dir = 'src'

# Create executable
executable('solid_native',
  [src_dir / 'main.cpp'],
  cpp_args: '-DBUNDLE_PATH=bundle.js',
  dependencies: deps,
  install: true
)

# Optionally, create a custom test
test('solid_native_test', executable('solid_native_test',
  [src_dir / 'main.cpp'], 
  dependencies: deps))
