# GenesisOS

here we go 


Intel manual: `325384-sdm-vol-3abcd.pdf`

# Build

Prepare directories
```
make clean && make dirsetup
```

Compile:
```
make iso
```

# Run
```
make qemu
```

# Debug
```
make qemu-gdb
```

## Compile DB

For easier code navigation, use _clangd_'s vscode plugin + [compiledb](https://github.com/nickdiego/compiledb).

```
make clean && make dirsetup
compiledb make iso
```