ninja -C build-release/

docker build . -f docker/simbad.Dockerfile -t tozanski/simbad
docker push tozanski/simbad
