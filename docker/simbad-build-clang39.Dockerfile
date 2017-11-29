FROM tozanski/simbad-dev:clang39
COPY src /code/
RUN mkdir -p /code/build && \
    cd /code/build && \
    cmake .. -DCMAKE_BUILD_TYPE=Release &&\
    make -j $(nproc) && \
    ctest --VV -output-on-failure
