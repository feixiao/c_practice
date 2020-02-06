FROM ubuntu:16.04

RUN mkdir -p /app

WORKDIR /app

COPY ./core-dump-demo /app


RUN echo "kernel.core_pattern = /tmp/core.%e.%p.%t">/etc/sysctl.conf && sysctl -p

# This command runs your application, comment out this line to compile only
CMD ["./core-dump-demo"]

LABEL Name=core-dump-demo Version=0.0.1