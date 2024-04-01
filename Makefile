all: main thread

main:
	gcc -Wall -Wextra -Werror main.c dblLinkedList.c -o main -lpthread -g

thread:
	gcc -Wall -Wextra -Werror threads.c dblLinkedList.c -o thread -lpthread -g
	
rebuild: clean all

clean:
	rm -rf main thread