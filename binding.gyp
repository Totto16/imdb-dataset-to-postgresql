{
    "targets": [
        {
            "target_name": "imdb-dataset-parser",
            "cflags_cc": [
                "-fexceptions",
                "-g3",
                "-std=c++20",
                "-Werror",
                "-Wall",
                "-Wpedantic",
                "-Wno-cast-function-type"  # since nan.h -> node.h has some warnings regarding that
            ],
            "sources": [
                "addon.cpp",
                "src/TSVParser.cpp",
                "src/ParserStructure.cpp",
                "src/InternalParsers.cpp",
                "src/Models.cpp"
            ],
            "include_dirs": [
                "<!(node -e \"require('nan')\")",
                "submodules/expected/include/tl/"
            ],
            'dependencies': [
                'csvlib',
            ]
        },
        {
            'target_name': 'csvlib',
            'type': '<(library)',
            "cflags_cc": [
                "-fexceptions",
                "-O3",
                "-std=c++20",
                "-Wall",
                "-Wpedantic"
            ],
            "sources": [
                "submodules/tsvlib/csvlib/csv/parser.cpp",
                "submodules/tsvlib/csvlib/csv/datasource/utf8/DataSource.cpp"
            ],
            "include_dirs": [
                "submodules/tsvlib/csvlib/",
            ],
            'direct_dependent_settings': {
                'include_dirs': [
                    "submodules/tsvlib/csvlib/"
                ],
            },
        },
    ]
}
