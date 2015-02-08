{
  "targets": [{
      "target_name": "w1direct",
      "cflags" : ["-std=c++11"],
      "sources": [ 
      	"src/w1direct.cc", "src/manager.cc", "src/shared/util.cc", "src/shared/match.cc",  "src/shared/v8_helper.cc",
      	"src/master/master.cc", "src/master/ds2482.cc", "src/master/bus/bus.cc", "src/master/bus/search.cc", 
      	"src/device/device.cc", "src/device/ds18b20.cc", "src/device/ds18s20.cc", "src/device/ds2408.cc", "src/device/unsupported.cc", 
      	"src/device/lib/crc.cc", "src/device/lib/temp.cc", "src/controller/controller.cc", "src/controller/device_store.cc", 
      	"src/api/api.cc", "src/api/broadcast.cc", "src/api/read.cc", "src/api/register.cc", "src/api/sync.cc", "src/api/update.cc"
      ],
      "configurations": {'Release':{'msvs_settings':{'VCCLCompilerTool':{'ExceptionHandling':1}}}}
    }
  ]
}
