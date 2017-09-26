FROM gcc:7
LABEL maintainer="Nathan Lowe <public@nlowe.me>"

RUN curl -sSLo cmake.sh https://cmake.org/files/v3.9/cmake-3.9.3-Linux-x86_64.sh && \
    chmod +x cmake.sh && \
    ./cmake.sh --prefix=/usr/local --exclude-subdir && \
    rm cmake.sh
