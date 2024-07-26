# IDE Configuration
 @brief Config the IDE for easier code navigation.

The build process will generate a compilation database for your IDE, if you're using vscode, 
download [LLVM's clangd](https://clangd.llvm.org/) extension and use the following configuration in `.vscode/settings.json`:

```json
{
    "clangd.arguments": [
        "--compile-commands-dir=${workspaceFolder}/build"
    ]
}
```

This will tell the clangd server where the database is and allow easier code navigation.