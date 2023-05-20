### Recommended extensions in vscode:
- Yash
- C/C++ extension pack
- WSL

### Run minimal example
Ensure you are in the top directory
```
make
build/compiler -mode code/mex.c -o hello
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