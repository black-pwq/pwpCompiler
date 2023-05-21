### Recommended extensions in vscode:
- Yash
- C/C++ extension pack
- WSL

### Run minimal example
Ensure you are in the top directory
```
make
build/compiler code/mex.c
```
The output will be something like
```
CompUnit {
  FunDef {
    SimpleType {
      int
    }
    main
    BlockStmt {
      RetStmt {
        IntExpr {
          0
        }
      }
    }
  }
}
```