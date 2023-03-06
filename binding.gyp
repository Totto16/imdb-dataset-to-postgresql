# type: ignore
{
    "target_defaults": {
        "cflags": [
            "-std=c++20",
            "-Wall",
            "-Wextra",
            "-Wno-unused-parameter",
            "-fexceptions",
            "-O3",
            "-Werror",
            "-Wpedantic",
        ],
        "cflags_cc": [
            "-std=c++20",
            "-Wall",
            "-Wextra",
            "-Wno-unused-parameter",
            "-fexceptions",
            "-O3",
            "-Werror",
            "-Wpedantic",
        ],
        "defines": ["V8_DEPRECATION_WARNINGS=1"],
        "include_dirs": ["<!(node -e \"require('nan')\")"],
        "target_conditions": [
            [
                'OS=="linux"',
                {
                    "cflags": [
                        "-std=c++20",
                        "-Wall",
                        "-Wextra",
                        "-Wno-unused-parameter",
                        "-fexceptions",
                        "-O3",
                        "-Werror",
                        "-Wpedantic",
                    ],
                    "cflags_cc": [
                        "-std=c++20",
                        "-Wall",
                        "-Wextra",
                        "-Wno-unused-parameter",
                        "-fexceptions",
                        "-O3",
                        "-Werror",
                        "-Wpedantic",
                        "-fno-omit-frame-pointer",
                    ],
                    "ldflags": ["-pthread", "-fuse-ld=mold"],
                },
            ],
        ],
    },
    "targets": [
        {
            "target_name": "imdb-dataset-parser",
            "cflags_cc": [
                "-Wno-cast-function-type"  # since nan.h -> node.h has some warnings regarding that
            ],
            "sources": [
                "src/TSVParser.cpp",
                "src/ParserStructure.cpp",
                "src/InternalParsers.cpp",
                "src/Models.cpp",
                "src/EventEmitter.cpp",
            ],
            "include_dirs": [
                "<!(node -e \"require('nan')\")",
                "submodules/expected/include/tl/",
            ],
            "dependencies": ["csvlib", "eventemitterlib"],
        },
        {
            "target_name": "csvlib",
            "type": "<(library)",
            "sources": [
                "submodules/tsvlib/csvlib/csv/parser.cpp",
                "submodules/tsvlib/csvlib/csv/datasource/utf8/DataSource.cpp",
            ],
            "include_dirs": [
                "submodules/tsvlib/csvlib/",
            ],
            "direct_dependent_settings": {
                "include_dirs": ["submodules/tsvlib/csvlib/"]
            },
        },
        {
            "target_name": "eventemitterlib",
            "type": "<(library)",
            "sources": [
                "submodules/eventemitter/constructable.cpp",
                "submodules/eventemitter/eventemitter_impl.cpp",
                "submodules/eventemitter/uv_rwlock_adaptor.cpp",
            ],
            "include_dirs": [
                "submodules/eventemitter/",
            ],
            "direct_dependent_settings": {
                "include_dirs": [
                    "submodules/eventemitter/",
                ]
            },
        },
    ],
}
