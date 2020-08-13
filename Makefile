.DEFAULT_GOAL := push2310

push2310: push2310.c 
	gcc -o push2310 push2310.c
clean:
	rm push2310
