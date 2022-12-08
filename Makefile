all : TwosComplement

TwosComplement : TwosComplement.c
	gcc -Wall -Werror -fsanitize=address -std=c11 TwosComplement.c -o TwosComplement -lm

CanonicalBinaryFraction : CanonicalBinaryFraction.c
	gcc -Wall -Werror -fsanitize=address -std=c11 CanonicalBinaryFraction.c -o CanonicalBinaryFraction -lm

IEEE754Decimal : IEEE754Decimal.c
	gcc -Wall -Werror -fsanitize=address -std=c11 IEEE754Decimal.c -o IEEE754Decimal -lm

IEEE754Hexadecimal : IEEE754Hexadecimal.c
	gcc -Wall -Werror -fsanitize=address -std=c11 IEEE754Hexadecimal.c -o IEEE754Hexadecimal -lm