
project(
    'PostgresCxxClient',
    'cpp',
    default_options: ['cpp_std=c++17'],
    version: '1.0.1',
    license: 'MIT',
)

deps = []
src_files = []
inc_dirs = include_directories('include')

deps += dependency('PostgreSQL')

subdir('src')

client_lib = library(
    'postgres-cxx-client',
    src_files,
    dependencies: deps,
    include_directories: inc_dirs,
)

client_dep = declare_dependency(
    include_directories: inc_dirs,
    link_with: client_lib,
    dependencies: deps,
    version: meson.project_version(),
)

if get_option('examples')
    subdir('examples')
endif

if get_option('tests')
    subdir('tests/unit')
endif
