docker build -f docker/simbad-dev-clang39.Dockerfile . -t tozanski/simbad-dev:clang39
docker build -f docker/simbad-dev-gcc6.Dockerfile .  -t tozanski/simbad-dev:gcc6

docker push tozanski/simbad-dev:clang39
docker push tozanski/simbad-dev:gcc6

docker build . -f docker/simbad.Dockerfile -t tozanski/simbad
docker push tozanski/simbad
