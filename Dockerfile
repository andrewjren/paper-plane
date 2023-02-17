FROM alpine:3.17

# Build environment
RUN apk add --no-cache \
    git \
    cmake \
    build-base \
    python3 \
    newlib-arm-none-eabi \
    gcc-arm-none-eabi

# Clone pico-sdk and initialize
RUN cd /opt/ && \
    git clone https://github.com/raspberrypi/pico-sdk.git && \
    cd pico-sdk && \
    git submodule update --init

# Set environment variable 
ENV PICO_SDK_PATH=/opt/pico-sdk

# Use /home/ as working area 
WORKDIR /home/
