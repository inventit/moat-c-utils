MOAT C SDK
===

This is a Software Development Kit enabling you to develop MOAT C client application.

## What is MOAT C?
[MOAT C](http://dev.inventit.io/references/moat-c-api-document.html) is a C API to build a client side application interacting with cloud server.
It is a part of [MOAT IoT](http://dev.inventit.io/guides/moat-iot.html), which is a specification set of creating IoT/M2M applications running on Inventit® ServiceSync environment.

*****
The library offers you:

* to build and package a MOAT C client application.

## Prerequisites
#### for build envirnment
* gyp
* python 2.6 or 2.7
* GNU Make 3.81 or later
* gcc 4.2 or later
* JDK 1.6 or later (for signing)

#### for runtime environment (on host PC)
* OpenSSL 0.9.8 (libssl0.9.8)
* libev 4.11 (libev4)
* readline6 (libreadline6)

## How to use

### 1. Download a security token
At first, download a security token  for your application package using [IIDN-CLI Tool](https://github.com/inventit/iidn-cli).

    $ sh $CLI/iidn tokengen simple-example

### 2. Deploy your security token into SDK
Copy the downloaded security token into `${SDK_ROOT}/package` as `token.bin`

    $ cp 1363966226152-token.bin ${SDK_ROOT}/package/token.bin

### 3. Configure
Perform the `configure` command

    $ cd ${SDK_ROOT}
    $ ./configure
    verifying the token
    MOAT package name: simple-example
    generating a keystore
    generating a pkcs#12 certificate
    exporting a key
    exporting a public certificate
    {
        "name" : "simple-example",
        "version" : "1.0.0",
        "description" : "[description]",
        "maintainers" : [
            {
                "name" : "[name]",
                "email" : "[email]"
            }
        ],
        "bugs" : {
            "mail" : "[email]"
        },
        "main" : "simple-example.so",
        "models" : {}
    }
    
    { 'target_defaults': { 'cflags': [],
                           'default_configuration': 'Release',
                           'defines': [],
                           'include_dirs': [],
                           'ldflags': [ '-L/path/to/moat-c-sdk/test/x86_64/lib'],
                           'libraries': [],
                           'library_dirs': [ '/path/to/moat-c-sdk/test/x86_64/lib']},
      'variables': { 'application_id': '2c57d062-437f-466e-8d1a-dbde43ccd88c',
                     'clang': 0,
                     'gcc_version': 45,
                     'host_arch': 'x86_64',
                     'package_name': 'simple-example',
                     'target_arch': 'x86_64',
                     'token_path': '/path/to/moat-c-sdk/package/token.bin'}}
    creating  /path/to/moat-c-sdk/config.gypi
    creating  /path/to/moat-c-sdk/config.mk
    $ 

#### configuration for cross-compilation
If you wish to cross-compile, you can specify target arch to use option `--dest-cpu` or to set environment variable `CC` to gcc for cross-compiling.

example for arm-linux-gnu-eabi

    $ ./configure --dest-cpu=arm

or

    $ CC=arm-linux-gnueabi-gcc ./configure

#### `configure` options
    $ ./configure --help
    Options:
      -h, --help           show this help message and exit
      --debug              Also build debug build
      --dest-cpu=DEST_CPU  CPU architecture to build for. Valid values are: arm,
                           ia32, x64
      --name=PACKAGE_NAME  Package name of your M2M/IoT application
      --token=TOKEN_PATH   Path to the security token file. Default path is
                          './package/token.bin'

### 4. Describe your models
Describe your models and meta information of the package in  `${SDK_ROOT}/package/package.json`.

The format of `package.json` file is described in [here](http://dev.inventit.io/references/moat-rest-api-document.html#package).

example of `package.json` for MOAT-C

    {
    	"name": "simple-example",
    	"version": "1.0",
    	"description": "MOAT IoT Example Application Package",
    	"maintainers": [
    		{
    			"name": "Example Maintainers",
    			"email": "maintainers@example.com"
    		}
    	],
    	"bugs": {
    		"mail": "bugs@example.com"
    	},
    	"notification" : { "http" : "http://localhost:3000/request_histories" },
    	"main" : "simple-example.so",
    	"directories" : {
    		"lib" : "lib",
    		"bin" : "bin"
    	},
    	"models" : {
    		"ShakeEvent" : {
    			"array" : true,
    			"attributes" : {
    				"time" : {"type" : "int64"},
    				"x" : {"type" : "float"},
    				"y" : {"type" : "float"},
    				"z" : {"type" : "float"},
    				"acceleration" : {"type" : "float"}
    			}
    		},
    		"VibrationDevice" : {
    			"array" : false,
    			"attributes" : {
    				"image" : {"type" : "string"}
    			},
    			"commands" : {
    				"vibrate" : {"paramType" : "string"}
    			}
    		}
    	}
    }

### 5. Write your code
Write your application code into template `${SDK_ROOT}/src/${PACKAGE_NAME}.c`.

You can add source files into `${SDK_ROOT}/src`.

### 6. Build your application
Perform `make` command to build your application

    $ make
    make -C out/x86_64 BUILDTYPE=Release V=
    make[1]: Entering directory `/path/to/moat-c-sdk/out/x86_64'
      CC(target) /path/to/moat-c-sdk/out/x86_64/Release/obj.target/simple-example/src/simple-example.o
      SOLINK(target) /path/to/moat-c-sdk/out/x86_64/Release/obj.target/simple-example.so
      SOLINK(target) /path/to/moat-c-sdk/out/x86_64/Release/obj.target/simple-example.so: Finished
      COPY /path/to/moat-c-sdk/out/x86_64/Release/lib.target/simple-example.so
    make[1]: Leaving directory `/path/to/moat-c-sdk/out/x86_64'

For verbose output, you can specify `make V=1` option.

If you have added some source code, you can add path to the files into `${SDK_ROOT}/moatapp.gyp

    ...(snip)...
        # your M2M/IoT application
        {
          'target_name': '<(package_name)',
          'sources': [
            'src/<(package_name).c',
            'src/your_additional_source_1.c',   # added file
            'src/your_additional_source_2.c',   # added file
           ],
          'product_prefix': '',
    ...(snip)... 

Please refer to [GYP's Wiki page](https://code.google.com/p/gyp/w/list) for more details.

#### Cross compile (example for arm-linux-gnueabi)
configure target architecture to arm using 'configure' command in advance.

    $ export CROSS=arm-linux-gnueabi-
    $ export CC=${CROSS}gcc
    $ export CXX=${CROSS}gcc
    $ export AR=${CROSS}ar
    $ export strip=${CROSS}strip
    $ cd ${SDK_ROOT}
    $ make

### 7. Run and Debug application on host PC
If the environment is configured for host PC, you can run your application on SDK to perform `make preparetest` command.

    $ make preparetest
    python tools/test.py --prepare
    copy /path/to/sdk/out/x86_64/Release/lib.target/simple-example.so to /path/to/sdk/test/x86_64/simple-example.so
    copy /path/to/sdk/package/package.json to /path/to/sdk/test/x86_64/package.json
    creating  /path/to/moat-c-sdk/test/x86_64/.app-id


    cd /path/to/sdk/test/x86_64

    You can run your application here with the following command:
    $ LD_LIBRARY_PATH=./lib/ ./apprunner

    $ 

After `make preparetest` command, your current directory is changed to `${SDK_ROOT}/test/${HOST_ARCH}`. You can run and debug your application in this directory.

#### Run application

To run your application, you can perform `apprunner` command:

    $ LD_LIBRARY_PATH=./lib/ ./apprunner
    [>>MOAT<<] Call MOAT API [moat_init]
    [>>MOAT<<] function=[moat_init] urn=[2c57d062-437f-466e-8d1a-dbde43ccd88c:simple-example], out_moat=[0x7fff436944f8]
    [>>MOAT<<] Call MOAT API [moat_register_model]
    [>>MOAT<<] function=[moat_register_model] model_name=[ShakeEvent], mapper=[0x7fff436944c0], model_context=[(nil)]
    [>>MOAT<<] Call MOAT API [moat_register_model]
    [>>MOAT<<] function=[moat_register_model] model_name=[VibrationDevice], mapper=[0x7fff436944c0], model_context=[(nil)]
    [>>MOAT<<] Call MOAT API [moat_run]

The `apprunner` tool output messages with  `[>>MOAT<<]` tag. The messages shows:

* every MOAT API call and result
* passed parameters information
* missing manadatory parameter error
* invalid pointer of `Moat` instance
* full data output of `moat_send_notification()` data

#### Command Line Interface tool
The `apprunner_cli` command is the command line interface for `apprunner`. 
You can talk with your application using this command, and you can debug `ModelMapper` interface and Command interface implementation.

To perform this command, you need to open another terminal and move to `${SDK_ROOT}/test/${HOST_ARCH}` directory and perform the command.

    $ cd ${SDK_ROOT}/test/${HOST_ARCH}
    $ LD_LIBRARY_PATH=./lib/ ./apprunner_cli 
    Command Line Interface for MOAT C Application.
    Type 'help' for help.
    >> help
    CLI command:
      quit
      history
    Runner command:
      help 
      quit-app 
      list-models 
      show-model ModelName 
      add ModelName ModelDataFile 
      remove ModelName UID 
      update ModelName ModelDataFile 
      update-fields ModelName ModelDataFile 
      find-all-uids ModelName 
      find-by-uid ModelName UID 
      count ModelName 
      command ModelName CommandFile 
    
    >> 
`apprunner_cli` provides the following command to debug application.

##### CLI command
CLI commands are internal command of  `apprunner_cli`.

* quit

    quit `apprunner_cli` console.
   
* history

    display command history.

##### Runner command
Runner commands are commands to debug the application.

* help

    display command list.

* quit-app

    quit application and `apprunner` (also `apprunner_cli`)

* list-models

    display all registered models.

* show-model *ModelName*

    show detail information of model specified by *ModelName* argument.

* add *ModelName* *ModelDataFile*

   invoke `ModelMapper.AddProc` method of the model specified by *ModelName* argument.

* remove *ModelName* *UID*

   invoke `ModelMapper.RemoveProc` method of the model specified by *ModelName* argument.

* update *ModelName* *ModelDataFile*

   invoke `ModelMapper.UpdateProc` method of the model specified by *ModelName* argument.

* update-fields *ModelName* *ModelDataFile*

   invoke `ModelMapper.UpdateFieldsProc` method of the model specified by *ModelName* argument.

* find-all-uids *ModelName*

   invoke `ModelMapper.FindAllUidsProc` method of the model specified by *ModelName* argument.

* find-by-uid *ModelName* *UID*

   invoke `ModelMapper.FindByUdProc` method of the model specified by *ModelName* argument.

* count *ModelName*

   invoke `ModelMapper.CountProc` method of the model specified by *ModelName* argument.

* command *ModelName* *CommandFile*

   invoke command interface of the model specified by *ModelName* argument.

##### *ModelDataFile* argument
The *ModelDataFile* argument is path to a json file that represents an entity of a model.
The user need to create this file to perform [`add`|`update`|`update-fields`] commands in advance.

The file format as follow:

* array model

    Set an UID as a key of root, and Set the entity of model to the value.

* singleton model

    Set the entity of model as root.

###### example:
In case of array model:

    {
        "098fb3be-5123-11e3-aa70-080027f89995" : {
            "time" : 1384869795000,
            "x" : 11.223344,
            "y" : 55.667788,
            "z" : 99.001122,
            "acceleration" : 123.4567
        }
    }

In case of singleton model:

    {
        "url" : "http://localhost/content",
        "result" : "error",
        "code" : 404
    }

##### *Command* argument
The *CommandFile* argument is path to a json file that represents detail of command interface.
The user need to create this file to perform `command` command in advance.

This file has the following keys.

* name

    command name to invoke (mandatory).

* uid

   uid string of model entity (optional).

* key

  key string for asynchronous execution.
  If the command interface implements as asynchronous result notification mode, this value should be set.

* param

  parameter data for command.
  If the `paramType` of the command in model descriptor is not `null`, this value should be set.

###### example:
    {
        "name" : "downloadAndUpdate",
        "key" : "ff8080814268dc7001426912a91e0021",
        "param" : ""
    }

### 8. Create a package

Perform 'make package' command.

    $ make package
    make -C out/x86_64 BUILDTYPE=Release V=
    make[1]: Entering directory `/path/to/moat-c-sdk/out/x86_64'
    make[1]: Nothing to be done for `all'.
    make[1]: Leaving directory `/path/to/moat-c-sdk/out/x86_64'
    python tools/package.py
    creating a package of simple-example...
    copying a file [/path/to/moat-c-sdk/out/x86_64/Release/lib.target/simple-example.so] into [/path/to/moat-c-sdk/.package]...
    verifying the token
    signing /path/to/moat-c-sdk/.package/token.bin
    archinving to /path/to/moat-c-sdk/simple-example_1.0_x86_64.zip
    signing /path/to/moat-c-sdk/simple-example_1.0_x86_64.zip

If the command is successful, the package file is generated in `${SDK_ROOT}/${PACKAGE_NAME}_${VERSION}_${TARGET_ARCH}.zip


### 9. Deploy onto Inventit IoT Developer Network(IIDN) Sandbox Server.
You can deploy the created package onto Inventit IoT Developer Network(IIDN) Sandbox Server.
Visit [IIDN site](http://dev.yourinventit.com) 

    $ sh $CLI/iidn bindeploy simple-example_1.0.0_arm.zip

## Where can you deploy?


You can deploy the created package onto Inventit IoT Developer Network(IIDN) Sandbox Server.
Visit [IIDN site](http://dev.yourinventit.com) 

## Source Code License

All program source codes are available under the MIT style License.

The use of IIDN service requires [our term of service](http://dev.yourinventit.com/legal/term-of-service).

Copyright (c) 2013 Inventit Inc.

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

## Change History

1.0.4 : July 10, 2014
* new api (`moat_get_device_id()`, `moat_get_package_urn()`, `moat_create_notification_id_with_moat()`)
* minor bug fixes

1.0.3 : January 8, 2014

* API enhancements (new api support)
    * sse_string : string object
    * sse_base64 : Base64 decoder/encoder
    * sse_hashlib : MD5/SHA1/SHA256 hash
    * moat_uuid : UUID (v1/v4) generator
    * moat_url : URL object
    * moat_httpc : HTTP client 
    * moat_xxxloader : HTTP downloader/uploader
    * moat_timer : Timer
    * moat_event : app internal event service
    * moat_io : I/O watcher/handler
    * moat_idle : idle handler
    * moat_periodic : periodic handler
    * moat_json : JSON utility
    * MOAT_LOG : logging macros

1.0.2 : November 19,2013

* remove stub code
* add `apprunner` `apprunner_cli` tool for run/debug application on host PC

1.0.1 : November 13, 2013

* update header version to 1.0.3
* add *version*_*arch* suffix to the generated package file name 
* minor bug fix

1.0.0 : June 6, 2013

* Initial Release.

