docker build -f simbad-dev-clang39.Dockerfile . -t tozanski/simbad-dev:clang39
docker build -f simbad-dev-gcc6.Dockerfile .  -t tozanski/simbad-dev:gcc6

docker push tozanski/simbad-dev:clang39
docker push tozanski/simbad-dev:gcc6
