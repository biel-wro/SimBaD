FROM alpine
RUN apk update && apk upgrade && apk add \
  boost-program_options \
  libc6-compat \
  libstdc++
  

COPY build-release /SimBaD/
WORKDIR /SimBaD/
CMD /SimBaD/cli/SimBaD-cli --stdin=simbad
