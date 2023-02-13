{
    "targets": [
        {
            "target_name": "imdb-dataset-parser",
            "cflags!": ["-fno-exceptions -O3 -std=c20 -WError -WAll -WPedantic"],
            "cflags_cc!": ["-fno-exceptions -O3 -std=c++20 -WError -WAll -WPedantic"],
            "sources": [
                "addon.cpp",
                "src/TSVParser.cpp"
            ],
            "cflags": ["-Wall", "-std=c++20"],
            "include_dirs": ["<!(node -e \"require('nan')\")"],
        }
    ]
}
