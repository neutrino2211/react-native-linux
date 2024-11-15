# meson.build
project('my_js_project', 'cpp', version: '0.1', default_options: ['cpp_std=c++17'])

# Dependency for JavaScriptCore
js_core_dep = dependency('JavaScriptCore', required: true)

# Define source directory and the source files
src_dir = 'src'

# Create executable
executable('solid_native',
  [src_dir / 'main.cpp', src_dir / 'namespace.cpp'],
  dependencies: js_core_dep,
  install: true
)

# Optionally, create a custom test
test('solid_native_test', executable('solid_native', 
  [src_dir / 'main.cpp', src_dir / 'namespace.cpp'], 
  dependencies: js_core_dep))
