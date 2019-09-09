{
  "targets": [
    {
      "target_name": "sdrplay",
      "link_settings" : {
      	"libraries": [
      		"-lmirsdrapi-rsp"
      	],
      	"ldflags":[
      		"-L/usr/local/lib"
      	],
      	"cflags":[
		"-Wno-deprecated-declarations"
        ]
      },
      "sources": [ "src/sdrplay.cc", "src/deviceinfo.cc", "src/stream.cc", "src/settings.cc", "src/rsp2.cc", "src/gain.cc" ],
      "include_dirs" : [
          "include",
          "<!(node -e \"require('nan')\")"
      ]
    }
  ]
}
