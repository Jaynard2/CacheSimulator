# Cache Simulator
Assembler for the Mano Machine designed in Cedarville's Computer Architecture class.

## Hardware Requirements
Your system must support at minumum version 4.3 of OpenGL. To see if your computer is supported, check your hardware and driver version using the following links:
 - [Intel](https://www.intel.com/content/www/us/en/support/articles/000005524/graphics.html)
 - [NVIDIA](https://developer.nvidia.com/opengl-driver)

## Environment Setup
### Install vcpkg
Install [vcpkg](https://github.com/microsoft/vcpkg), and follow the instructions to integrate it with Visual Studio. After it is installed, run the following commands:
```
vcpkg install glfw3
vcpkg install glew
vcpkg install glm
```

If you integrated vcpkg with Visual Studio, the libraries will be automatically added to your projects. 

### Compile NFD
Next, download [Native File Dialogue](https://github.com/mlabbe/nativefiledialog) and compile it using Visual Studio. The .sln file can be found in `build/vs2010`. 
Make sure to use the same compiler versionas you plan on using for this project!

Once compilation of NFD is finished, you will need to tell this project where to find the files for NFD. Under project properties, add `(NFD)/src/include` to
Additional Include Directories and `(NFD)/build/lib/Debug/x86/`to Additional Library Dependencies under the Debug configuration, and `(NFD)/build/lib/Release/x86/` under
the Release configuration (optional).

# How to Use
Load a .csv file using the "Load File" button. The expected format for the .csv is for the address, in binary, to be the first column, and for the second column to be the data. 
To load a line into RAM, type in the address (in binary) and click Load Memory. If the line is cached, a message will say that there was a cache hit. If the line was not cached,
it will be loaded into the cache and highlighted red, and a message will appear saying that there was a cache miss.

## Known Bugs
If cancel is pressed when selecting a file, main memory is given a single line of memory with no data, but will not be displayed. To fix, simply load another file.
