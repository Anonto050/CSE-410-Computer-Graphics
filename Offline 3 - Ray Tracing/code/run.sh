# run with cpp file name as argument
if [ $# == 0 ]; then
	echo "run with cpp filename"
	exit
fi
g++ $1 -o main -lglut -lGLU -lGL

if [ $? -eq 0 ]; then
    ./main
    exit 0
fi

exit 1
