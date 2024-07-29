
make render.exe
result=$?
if [ "$result" -ne "0" ]; then
    echo -e "\033[0;31mErrors during compilation\033[0m"
    exit
fi

OMP_NUM_THREAD=12 ./build/executable/render.exe $1
result=$?

if [ "$result" -eq "0" ]; then
    code ./output/render.ppm
else
    echo -e "\033[0;31mErrors during execution\033[0m"
fi