serial:
	gcc hash-attack.c -o hash-attack -O3 -lcrypto

parallel:
	module load cuda/7.0.28
	nvcc hash-attack-cuda.cu -o hash-attack -O3 -lcrypto

clean:
	rm hash-attack
