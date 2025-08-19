# Stage 1: Build
FROM debian:bookworm as build

# Install build tools
RUN apt-get update && apt-get install -y cmake g++ make && rm -rf /var/lib/apt/lists/*

WORKDIR /app

# Copy project files
COPY CMakeLists.txt .
COPY src/ src/

# Build project
RUN cmake -B build -S . && cmake --build build --config Release

# Stage 2: Runtime (slim)
FROM debian:bookworm-slim

WORKDIR /app

# Copy the compiled binary
COPY --from=build /app/build/webserver .

# Expose port
EXPOSE 8080

# Run server
CMD ["./webserver"]
