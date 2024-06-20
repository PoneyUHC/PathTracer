
make render.exe
./build/render.exe $1
result=$?
if [ "$result" -eq "0" ]; then
    code ./output/render.ppm
else
    echo -e "\033[0;31mErrors during execution\033[0m"
fi