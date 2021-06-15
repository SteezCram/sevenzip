{
    "targets": [{
        "target_name": "sevenzip-node",
        "cflags!": [ "-fno-exceptions" ],
        "cflags_cc!": [ "-fno-exceptions", "-std=c++17" ],
        "conditions": [
            ['OS=="mac"', {
                "xcode_settings": {
                    'GCC_ENABLE_CPP_EXCEPTIONS': 'YES',
                    "CLANG_CXX_LIBRARY": "libc++",
                    "CLANG_CXX_LANGUAGE_STANDARD":"c++17",
                    'MACOSX_DEPLOYMENT_TARGET': '10.14'
                }
            }],
            ['OS=="win"', {
                "msvs_settings": {
                    "VCCLCompilerTool": {
                    "AdditionalOptions": [ "-std:c++17", ],
                    },
                },
            }]
        ],
        "sources": [
            "src/algorithm/7z.cc",
            "src/convert.h",
            "src/main.cc",
            "src/algorithm/zip.cc",
        ],
        'include_dirs': [
            "<!@(node -p \"require('node-addon-api').include\")",
            "<(module_root_dir)/src/lib/bit7z/include",
        ],
        'libraries': [
            "<(module_root_dir)/src/lib/bit7z/lib/bit7z64.lib",
            "<(module_root_dir)/src/lib/bit7z/lib/bit7z64_d.lib"
        ],
        'dependencies': [
            "<!(node -p \"require('node-addon-api').gyp\")"
        ],
        'defines': [ 'NAPI_DISABLE_CPP_EXCEPTIONS' ]
    }]
}