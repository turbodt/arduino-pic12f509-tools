Build:

```
mkdir build
cmake -Bbuild
```

Build and generate compilation DB:

```
cmake -Bbuild -DCMAKE_EXPORT_COMPILE_COMMANDS=YES
ln -s build/compile_commands .
```
