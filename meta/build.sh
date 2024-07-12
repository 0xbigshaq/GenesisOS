#!/bin/bash
set -e
# set -x

rm -rf build/
cmake -GNinja -S . -B ./build/

# ninja -C build/ -v iso
ninja -C build/ iso
echo "[+] GenesisOS built successfully"

echo "[*] Merging compile commands databases"
ALL_DBS=build/compile_commands.json
GENESIS_DB=build/compile_commands_genesis.json
mv $ALL_DBS $GENESIS_DB

OTHER_DBS=$(find . -name "compile_commands.json")
jq -s 'add' ${GENESIS_DB} ${OTHER_DBS} > build/compile_commands.json

echo "[+] Done"