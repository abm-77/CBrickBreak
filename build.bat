@echo off

IF NOT EXIST build mkdir build

pushd build

del *.pdb > NUL 2 > NUL

echo WAITING FOR PDB > lock.tmp

call vcvars32

cl -nologo -Zi -DDEVELOPMENT ../source/win32_platform.c -link user32.lib gdi32.lib -incremental:no -opt:ref

del lock.tmp
del *.obj

popd