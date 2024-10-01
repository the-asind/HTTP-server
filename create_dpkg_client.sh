mkdir -p ./deb_pakage_client
mkdir -p ./deb_pakage_client/bin
mkdir -p ./deb_pakage_client/DEBIAN
touch  ./deb_pakage_client/DEBIAN/control

cat << EOF > ./deb_pakage_client/DEBIAN/control
Package: client-dcsa
Version: 1.0
Architecture: amd64
Maintainer: coderatny
Description: http client for DCSA labs
EOF

cp ./build/client ./deb_pakage_client/bin/

dpkg-deb --build ./deb_pakage_client/ client-dcsa.deb