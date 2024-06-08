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
    version: '1.1.0',
)

deps = []

deps += dependency('csvlib')
## https://github.com/jtv/libpqxx

src_files = []
header_files = []
inc_dirs = []

subdir('src')

executable(
    'import',
    src_files,
    header_files,
    dependencies: deps,
    include_directories: inc_dirs,
)
