# C-Memory-Administration

Implementa un programa en C que simula la gestión dinámica de memoria utilizando las funciones malloc, calloc, realloc y free, a partir de un archivo de entrada que contiene una secuencia de operaciones. Además, demostrarán problemas como fragmentación de memoria y fugas de memoria.



Function	Purpose	Initialization	Typical use-case
malloc()	Allocate a block of memory of given byte size	Uninitialized	You know exactly how many bytes you need immediately
calloc()	Allocate an array of n elements, each of given size	Zero-initialize	You want “clean” memory (zeros) from the start
free()	Release memory previously allocated	—	After you’re done using a dynamic memory block
realloc()	Resize (expand or shrink) an existing allocated block	Preserves old data (up to min of old/new size)	When you need to grow/shrink dynamically allocated memory

