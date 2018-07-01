#! /bin/bash
#
rev_new=$(git rev-parse --short=7 HEAD)
version=$(cat ./src/version.h 2>/dev/null | cut -d '"' -f2)

    cat <<EOF > ./src/version.h
#define APP_VERSION "$version-nightly-$rev_new"
EOF

echo $version-nightly-$rev_new
