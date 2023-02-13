{
    "targets": [
        {
            "target_name": "imdb-dataset-parser",
            "cflags": [
                "-fno-exceptions",
                "-O3",
                "-std=c20",
                "-Werror",
                "-Wall",
                "-Wpedantic"
            ],
            "cflags_cc": [
                "-fno-exceptions",
                "-O3",
                "-std=c++20",
                "-Werror",
                "-Wall",
                "-Wpedantic",
                "-Wno-cast-function-type"  # since nan.h -> node.h has some warnings regarding that
            ],
            "sources": [
                "addon.cpp",
                "src/TSVParser.cpp"
            ],
            "cflags": ["-Wall", "-std=c++20"],
            "include_dirs": [
                "<!(node -e \"require('nan')\")",
                "submodules/expected/include/tl/",
                "submodules/tsvlib/csvlib/",
            ],
        }
    ]
}
