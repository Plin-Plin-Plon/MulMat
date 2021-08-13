SECTION .text

%macro multiplica 4

	xor	eax, eax	; Nesta macro:	ax é utilizado como índice i
	xor	ebx, ebx	;	       		bx é utilizado como índice j	
	xor	ecx, ecx	;	       		cx é utilizado como índice k	
	mov	edx, %3		;				edx é utilizado para guardar os valores das multiplicações diretamente na matriz resultante.
				    ; A utilização de %% nas labels tem como finalidade torna-las labels locais, pois a macro é  
	%%for1:			; chamada mais de uma vez.
		xor	bx, bx				; zera o contador j
		%%for2:
			xor	cx, cx			; zera o contador k
			%%for3:
				push	edx		; guarda o endereço da matriz resultante na pilha
				push	eax		; guarda o valor do índice i na pilha	
				mov	edx, %4	
				mul	dl			; faz a multiplicação i * L
				add	ax, cx		; adiciona k à multiplicação, logo: eax = (i * L) + k

				mov	esi, %1		; esi recebe o endereço da matriz a ser multiplicada (A ou C) 
				mov	esi, [esi+eax*4]	; esi recebe o valor da posição a ser multiplicada

				mov	ax, cx		; ax recebe o índice k
				mul	dl			; faz a multiplicação k*L
				add	ax, bx		; adiciona j à multiplicação, logo: eax = (k * L) + j		

				mov	edi, %2		; edi recebe o endereço da matriz a ser multiplicada por esi (B ou AB) 
				mov	edi, [edi+eax*4]	; edi recebe o valor da posição a ser multiplicada			

				mov	eax, esi
				mul	di
				mov	esi, eax	; resultado da multplicação é passado para esi
				pop	eax			; restaura o índice i
				pop	edx			; restaura o endereço da posição da matriz resultante

				add	[edx], esi	; adiciona o resultado parcial à matriz resultante

				inc	cx
				cmp	%4, ecx		; compara o índice k com o tamanho da matriz L
				jnz	%%for3
			add	edx, 4			; avança para a próxima posição da matriz resultante
			inc	bx
			cmp	%4, ebx			; compara o índice j com o tamanho da matriz L
			jnz	%%for2
		inc	ax
		cmp	%4, eax				; compara o índice i com o tamanho da matriz L
		jnz	%%for1
%endmacro

%macro menor 2

	mov	esi, %1					; recebe o endereço da matriz ABC
	mov	edx, %2					; recebe o tamanho da matriz, L
	mov	bx, dx					; ebx contém L
	mov	ax, dx					
	mul	dl					
	mov	dx, ax					; edx contém L * L

	mov	eax, [esi]				; inicializa eax com a primeira posição da matriz ABC
	xor	ecx, ecx					
loop:
	inc	cx					
	add	cx, bx					; ecx sempre vai estar na posição i = j da matriz ABC
	cmp	dx, cx
	jle	fim	
	mov	edi, [esi+ecx*4]		; edi recebe sempre o próximo valor da diagonal principal
	cmp	eax, edi
	jg	troca
	jmp	loop
troca:
	mov	eax, edi
	jmp	loop
fim:

%endmacro

global mulmat
	mulmat:
	enter	0,0	
	push	ebx		; Salva os valores de ebx,esi,edi, pois serão utilizados 
	push	edi		; nas macros multiplica e menor
	push	esi		
	multiplica 	[ebp+8], [ebp+12], [ebp+20], [ebp+28]		; A macro recebe a matriz A, B, AB (resultante) e o tamanho da matriz
	multiplica 	[ebp+20], [ebp+16], [ebp+24], [ebp+28]		; A macro recebe a matriz AB, C, ABC (resultante) e o tamanho da matriz
	menor 	[ebp+24], [ebp+28]			; A macro recebe a matriz ABC e o tamanho da matriz
	pop	esi		
	pop	edi		; Restaura os valores dos registradores
	pop	ebx	
	leave		
	ret			; Endereço de retorno é recuperado da pilha
