{
    "targets": [
        {
            "target_name": "addon",
            "sources": [ "addon.cc" ],
            'include_dirs': ["<!(node -p \"require('node-addon-api').include_dir\")"],
            "libraries": [],
            "defines": [ "NAPI_DISABLE_CPP_EXCEPTIONS" ]
        }
    ]
}