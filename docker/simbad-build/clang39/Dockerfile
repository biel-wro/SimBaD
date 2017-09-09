FROM debian
RUN apt-get update &&\
    apt-get install -y \
        cmake \
        libboost-all-dev

RUN apt-get update &&\
    apt-get install -y \
        clang-3.9
        
        
RUN update-alternatives --install /usr/bin/cc  cc  /usr/bin/clang-3.9   1000 &&\
    update-alternatives --install /usr/bin/c++ c++ /usr/bin/clang++-3.9 1000