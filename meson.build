project(
    'IMDB Dataset to PostgresSQL',
    'cpp',
    license: 'MIT',
    meson_version: '>=1.4.0',
    default_options: {
        'buildtype': 'release',
        'optimization': '3',
        'cpp_std': ['c++26', 'c++23', 'c++latest'],
        'warning_level': '3',
        'werror': true,
        'b_ndebug': 'if-release',
    },
    version: '1.2.2',
)

deps = []

deps += dependency('csvlib')
deps += dependency('argparse')
deps += dependency(
    'postgres-cxx-client',
    default_options: {
        'postgres_version_range': '>=16',
    },
)
deps += dependency('bshoshany-thread-pool')
src_files = []
header_files = []
inc_dirs = []
compile_args = []

cpp = meson.get_compiler('cpp')

compile_args += cpp.get_supported_arguments(
    '-Wassign-enum',
    '-Wenum-conversion',
    '-Wenum-switch',
    '-fstrict-enums',
    '-Wenum-identifier',
    '-Wenum-compare',
    '-Wenum-enum-conversion',
    '-Wenum-float-conversion',
    '-Wenum-too-large',
    '-Wenum-compare-switch',
    '-Wenum-compare-conditional',
    '-fno-strict-aliasing',
    '-Wpointer-arith',
    '-Wmissing-declarations',
    '-Wformat=2',
    '-Wstrict-prototypes',
    '-Wmissing-prototypes',
    '-Wnested-externs',
    '-Wold-style-definition',
    '-Wundef',
    '-Wunused',
    '-Wcast-align',
    '-Wmissing-noreturn',
    '-Wmissing-format-attribute',
    '-Wmissing-include-dirs',
    '-Wlogical-op',
    '-Wignored-qualifiers',
    '-Werror=redundant-decls',
    '-Werror=implicit',
    '-Werror=nonnull',
    '-Werror=init-self',
    '-Werror=main',
    '-Werror=missing-braces',
    '-Werror=sequence-point',
    '-Werror=return-type',
    '-Werror=trigraphs',
    '-Werror=array-bounds',
    '-Werror=write-strings',
    '-Werror=address',
    '-Werror=int-to-pointer-cast',
    '-Werror=pointer-to-int-cast',
    '-Werror=empty-body',
    '-Werror=write-strings',
    '-Werror=strict-aliasing',
    '-Wno-sign-compare',
    '-Wno-cast-function-type',
    '-Wno-unused-parameter',
    '-Wno-missing-field-initializers',
    '-Wno-type-limits',
    '-Wshadow',
    '-Wfloat-conversion',
)


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
