FROM debian AS dev

RUN apt-get update \
    && apt-get install -y \
        clang-7 \
        cmake \
        libboost1.67-dev \
        libboost-program-options1.67-dev \
    && rm -rf /var/lib/apt/lists/* \    
    && update-alternatives --install /usr/bin/cc  cc  /usr/bin/clang-7   1000 \
    && update-alternatives --install /usr/bin/c++ c++ /usr/bin/clang++-7 1000 

   
FROM dev AS builder

COPY src /code/

RUN mkdir -p /code/build \
    && cd /code/build \
    && cmake .. -DCMAKE_BUILD_TYPE=Release \
    && make -j $(nproc) \
    && ctest --VV -output-on-failure

#FROM  alpine AS runner
#
#RUN apk add --no-cache  \
#        boost-program_options 


FROM debian AS runner
RUN apt-get update \
    && apt-get install -y \
        libboost-program-options1.67 \
    && rm -rf /var/lib/apt/lists/*

COPY --from=builder /code/build/cli/SimBaD-cli /SimBaD/

# check for potential linking errors
RUN /SimBaD/SimBaD-cli --version 

CMD /SimBaD/SimBaD-cli --stdin=json
