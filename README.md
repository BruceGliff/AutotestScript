# AutotestScript

Script for autotest some programs. It requires a strong hierarchy of data and program name.
Your program: **./program**
Directories: {./program, ./Test, DataSet/, DataSet/tests/, DataSet/output/, DataSet/check/, DataSet/input/}
Check files must be **without extension**.

It calls ./program DataSet/tests/\* < DataSet/input/\* > DataSet/output/\*.out and compares output with check.

For default launching run **./Test**
