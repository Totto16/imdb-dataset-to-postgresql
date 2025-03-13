project(
    'IMDB Dataset to PostgresSQL',
    'cpp',
    license: 'MIT',
    meson_version: '>=1.4.0',
    default_options: {
        'buildtype': 'release',
        'optimization': '3',
        'cpp_std': ['c++26', 'c++23'],
        'warning_level': '3',
        'werror': true,
        'b_ndebug': 'if-release',
    },
    version: '1.2.2',
)

deps = []

deps += dependency('csvlib')
deps += dependency('argparse')
deps += dependency('postgres-cxx-client', default_options: {'postgres_version_range': '>=16'})
deps += dependency('bshoshany-thread-pool')
src_files = []
header_files = []
inc_dirs = []
compile_args = []

subdir('src')

executable(
    'imdb-sql-importer',
    src_files,
    header_files,
    dependencies: deps,
    include_directories: inc_dirs,
    cpp_args: [
        compile_args,
        '-D_APP_VERSION=' + meson.project_version(),
        '-D_APP_NAME=imdb-sql-importer',
    ],
)
