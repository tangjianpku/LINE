@echo off

set vector_file=$1
set times=10
set portion=0.01
set output_file=result.txt

set vocab_file=program\vocab.txt
set label_file=program\label.txt
set /a T=%times%-1

program\preprocess.exe -vocab %vocab_file% -vector %vector_file% -label %label_file% -output workspace\ -debug 2 -binary 1 -times %times% -portion %portion%

for /l %%i in (0,1,%T%) do D:\v-mequ\software\liblinear\windows\train.exe -s 0 -q workspace\train%%i workspace\model%%i
for /l %%i in (0,1,%T%) do D:\v-mequ\software\liblinear\windows\predict.exe -b 1 -q workspace\test%%i workspace\model%%i workspace\predict%%i
for /l %%i in (0,1,%T%) do program\score.exe -predict workspace\predict%%i -candidate workspace\can%%i>>%output_file%