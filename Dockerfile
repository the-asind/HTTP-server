FROM ubuntu:20.04

# Set environment variable to avoid interactive prompts during package installation
ENV DEBIAN_FRONTEND=noninteractive

# Install dependencies
RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    libssl-dev \
    curl \
    expect-dev \
    git

# Set the working directory
WORKDIR /app

# Copy the source code into the container
COPY . /app

# Initialize and update git submodules
RUN git submodule update --init --recursive

# Configure and build the project
RUN cmake -S . -B build
RUN cmake --build build --target server
RUN cmake --build build --target client

# Create the deploy directory and copy the built binaries
RUN mkdir -p /app/deploy && cp /app/build/server /app/deploy/server && cp /app/build/client /app/deploy/client

# Expose the server port
EXPOSE 7777

# Set the command to run the server
CMD ["/app/deploy/server"]