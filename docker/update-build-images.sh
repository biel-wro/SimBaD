docker build simbad-build/clang39 -t tozanski/simbad-build:clang39
docker build simbad-build/gcc6 -t tozanski/simbad-build:gcc6

docker push tozanski/simbad-build:clang39
docker push tozanski/simbad-build:gcc6
