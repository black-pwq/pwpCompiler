### Recommended extensions in vscode:
- Yash
- C/C++ extension pack
- WSL

### Run minimal example
Ensure you are in the top directory
```
make
build/compiler -koopa code/mex.c -o hello.koopa
```
The output will be
```
int main() { return 0; }
```
目前项目中未添加Koopa库，上述指令中含koopa串的参数暂无意义。