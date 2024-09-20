 #!/usr/bin/env bash

 # Execute this file to install the free cli tools into your path on OS X

 CURRENT_LOC="$( cd "$(dirname "$0")" ; pwd -P )"
 LOCATION=${CURRENT_LOC%Free-Qt.app*}

 # Ensure that the directory to symlink to exists
 sudo mkdir -p /usr/local/bin

 # Create symlinks to the cli tools
 sudo ln -s ${LOCATION}/Free-Qt.app/Contents/MacOS/freed /usr/local/bin/freed
 sudo ln -s ${LOCATION}/Free-Qt.app/Contents/MacOS/free-cli /usr/local/bin/free-cli
