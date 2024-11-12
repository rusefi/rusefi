## devcontainer 101

#### why use this?:
A development container is a Docker container that vsCode, through an extension [(Visual Studio Code Dev Containers)](https://marketplace.visualstudio.com/items?itemName=ms-vscode-remote.remote-containers), uses to set up a development environment isolated from the host.

A more nerdy explanation is available at: https://code.visualstudio.com/docs/devcontainers/containers

In the case of rusefi, it is used to set up the relatively complicated toolset as an alternative to [setup_linux_environment.sh](https://github.com/rusefi/rusefi/blob/master/firmware/setup_linux_environment.sh),Since the script, in addition to only working on Linux, only works for installations that use APT as a package manager (ie any Debian/Ubuntu derivative), and would only work to build the firmware (a couple of extra dependencies are used for tests and the simulator)


#### System requirements:

Linux:
- Docker CE with Docker Compose, also vscode

macOS: 
- Docker Desktop 2.0+.

Windows:
- Docker Desktop 2.0 and up
- WSL 2

for WSL 2:
- Windows 10 version 2004 and higher (Build 19041 and higher) or Windows 11 

howto install WSL2: https://learn.microsoft.com/en-us/windows/wsl/install



in case of a error similar to:

| Unable to detect if server is already installed: Error: Failed to probe if server is already installed: code: Failed to probe if server is already installed: code: 4294967295, , Unsupported console settings. In order to use this feature, the legacy console must be disabled.

https://learn.microsoft.com/en-us/windows/wsl/troubleshooting#error-0x80040306-on-installation
> This has to do with the fact that we do not support legacy console. To turn off legacy console:
> 
> Open cmd.exe
> Right click title bar -> Properties -> Uncheck Use legacy console
> Click OK

In addition, on Windows you have to clone the repository inside WSL, otherwise you will have performance problems since the container will have a longer bind (having to go from the docker container to WSL and from WSL to Windows)

Once the container creation is complete, you will be able to compile the firmware or the simulator or the tests.
For the firmware you will want to use (firmware/bin/compile.sh) instead of make to be able to select the variant of your board.

note:
This still needs a little testing and feedback to see if this same readme or the docker image needs to be improved. If you have any questions, ask the Discord server or personally to @derek_boom (Diego)