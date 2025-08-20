# Stage 1: Build
FROM debian:bookworm as build

# Install build tools and Python
RUN apt-get update && apt-get install -y \
    g++ cmake make python3 python3-venv git wget unzip \
    && rm -rf /var/lib/apt/lists/*

# Create a Python venv for Conan
RUN python3 -m venv /opt/venv \
    && /opt/venv/bin/pip install --upgrade pip \
    && /opt/venv/bin/pip install conan==2.* \
    && ln -s /opt/venv/bin/conan /usr/local/bin/conan

ENV PATH="/opt/venv/bin:$PATH"

WORKDIR /app

# Copy project files
COPY conanfile.txt .
COPY CMakeLists.txt .
COPY src/ src/

# Configure Conan

RUN conan profile detect --force  
RUN conan install . --build=missing 
RUN cmake --preset=conan-release 
RUN cmake --build --preset=conan-release

# Stage 2: Runtime
FROM debian:bookworm-slim

WORKDIR /app

COPY --from=build  /app/build/Release/webserver webserver

EXPOSE 8080

CMD ["./webserver"]
