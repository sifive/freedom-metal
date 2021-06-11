FROM ubuntu:16.04

RUN apt-get update && \
    apt-get upgrade -y && \
    apt-get install -y git clang-format-6.0

# Copy freedom-metal into container
RUN mkdir freedom-metal
COPY ./ freedom-metal/

WORKDIR ./freedom-metal
