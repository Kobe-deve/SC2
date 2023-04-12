echo Testing build...
gcc test.c -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer -o "SC2_test"

if %errorlevel% == 0 (
	SC2_test
) else  (
	echo "Test could not compile";
)