## author = Tobias Niederbrunner

project(
    'IMDB Dataset to PostgresSQL',
    'cpp',
    license: 'MIT',
    meson_version: '>=1·2.0',
    default_options: {
        'buildtype': 'release',
        'optimization': '3',
        'c_std': 'c23',
        'cpp_std': 'c++23',
        'warning_level': '3',
        'werror': 'true',
        'b_ndebug': 'if-release',
    },
    version: '1.0.0',
)


deps = []





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
