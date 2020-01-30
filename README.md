# AutotestScript

Script for autotest some programs. It requires a strong hierarchy of data and program name.
Your program: ./program
Directories: {./program, ./Test, DataSet/, DataSet/tests/, DataSet/output/, DataSet/check/, DataSet/input/}

It calls ./program DataSet/tests/*.dat < DataSet/input/*.inp > DataSet/output/*.out and compares output with check.
