FROM ubuntu:22.04

RUN apt-get update
RUN apt-get install patchelf

CMD ["tail", "-f", "/dev/null"]
