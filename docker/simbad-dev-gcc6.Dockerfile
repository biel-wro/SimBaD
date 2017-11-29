FROM debian
RUN apt-get update &&\
    apt-get install -y \
        cmake \
        libboost-all-dev

RUN apt-get update &&\
    apt-get install -y \
        gcc-6 \
        g++-6
        
RUN update-alternatives --install /usr/bin/cc  cc  /usr/bin/gcc-6 1000 &&\
    update-alternatives --install /usr/bin/c++ c++ /usr/bin/g++-6 1000