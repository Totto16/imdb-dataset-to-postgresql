cpp = meson.get_compiler('cpp')

# check std::expected support
have_std_expected = cpp.compiles(
    '''
#include <expected>
int main() {
    using T = std::expected<int, char>;
    return 0;
}
''',
)

if not have_std_expected
    message('Compiler doesn\'t support std::expected, using fallback')

    deps += dependency('tl-expected', required: true)
    compile_args += '-D_USE_TL_EXPECTED'

endif

header_files += files(
    'cli_arguments.hpp',
    'expected.hpp',
    'postgres.hpp',
    'threads.cpp',
)

src_files += files(
    'cli_arguments.cpp',
    'postgres.cpp',
    'threads.hpp',
)
