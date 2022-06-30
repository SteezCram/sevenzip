{
    "targets": 
    [
        {
            "target_name": "sevenzip",
            "cflags!": [ "-fno-exceptions" ],
            "cflags_cc!": [ "-fno-exceptions", "-std=c++17" ],
            "conditions": 
            [
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
                "src/convert.hpp",
                "src/main.cpp",
                "src/worker.hpp",
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
        },
        {
            "target_name": "action_after_build",
            "type": "none",
            "dependencies": [ "<(module_name)" ],
            "copies": [
                {
                    "files": [ "<(PRODUCT_DIR)/<(module_name).node" ],
                    "destination": "<(module_path)"
                }
            ]
        }
    ]
}