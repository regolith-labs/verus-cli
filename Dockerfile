# builder stage
FROM debian:bullseye-slim AS builder

RUN apt-get update && \
    apt-get install -y --no-install-recommends \
      build-essential ca-certificates && \
    rm -rf /var/lib/apt/lists/*

WORKDIR /src
COPY . .

# build verushash
RUN make

# runtime stage
FROM debian:bullseye-slim
WORKDIR /app

COPY --from=builder /src/verushash ./verushash

ENTRYPOINT ["./verushash"]
