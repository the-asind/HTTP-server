
FROM ubuntu:20.04

RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    libssl-dev \
    curl \
    expect-dev \
    git

WORKDIR /app

COPY . /app

RUN git submodule update --init --recursive

RUN cmake -S server -B build && cmake --build build --target server && cmake --build build --target client

RUN mkdir -p /app/deploy && cp /app/build/server /app/deploy/server && cp /app/build/client /app/deploy/client

EXPOSE 7777

CMD ["/app/deploy/server"]
