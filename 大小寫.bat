@echo off
setlocal ENABLEDELAYEDEXPANSION
set str=Test
for %%a in (a b c d e f g h i j k l m n o p q r s t u v w x y z)do set str=!str:%%a=%%a!
echo %str%

set str=Test
for %%a in (A B C D E F G H I J K L M N O P Q R S T U V W X Y Z)do set "str=!str:%%a=%%a!"
echo %str%
pause