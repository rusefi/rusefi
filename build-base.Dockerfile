# rusEFI base build Docker image

ARG UBUNTU_VERSION=22.04
FROM ubuntu:${UBUNTU_VERSION}

ARG RUSEFI_DIR=/rusefi

WORKDIR ${RUSEFI_DIR}

RUN apt-get update && \
    DEBIAN_FRONTEND=noninteractive apt-get install --assume-yes build-essential gcc-multilib g++-multilib \
    gcc-mingw-w64 g++-mingw-w64 \
    ant curl dosfstools file lsb-release mtools openjdk-8-jdk wget xxd zip && \
    apt-get remove --assume-yes openjdk-11-jre-headless

ADD firmware/provide_gcc.sh .
RUN ./provide_gcc.sh

ADD java_tools/gradlew ./
ADD java_tools/gradle ./gradle
RUN ./gradlew --version

WORKDIR ./rusefi/firmware
ENV PATH="$PATH:$RUSEFI_DIR/gcc-arm-none-eabi/bin"
ENTRYPOINT ["sh", "-c", "uname --all && lsb_release --all && gcc -v && arm-none-eabi-gcc -v"]
