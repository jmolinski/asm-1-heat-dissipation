; autor Jakub Molinski 419502

        NAN equ 01111111111111111111111111111111b
        global start
        global place
        global step

        section .bss
        align 8
        temp_matrix resq 1             ; float*
        diff_matrix resq 1             ; float*
        width resd 1                   ; int
        height resd 1                  ; int
        coolant_temp resd 1            ; float
        coeff resd 1                   ; float

        section .text

; -----------------------------------------------------------------------------
; void start(int szer, int wys, float *M, float C, float waga);
start:
        mov [rel width], edi
        mov [rel height], esi
        mov [rel temp_matrix], rdx
        mov eax, edi
        mov rcx, rdx
        imul esi                       ; rax = height * width
        lea rdx, [rcx + 4 * rax]
        mov [rel diff_matrix], rdx
        movss DWORD [rel coolant_temp], xmm0
        movss DWORD [rel coeff], xmm1

        dec eax
        xor esi, esi
        mov rdi, [rel diff_matrix]
.zero_initialize_diff_matrix:
        mov DWORD [rdi + rsi * 4], 0
        inc esi
        test esi, eax
        jne .zero_initialize_diff_matrix

        ret

; -----------------------------------------------------------------------------
; void place(int ile, int x[], int y[], float temp[]);
place:
        mov r8d, [rel width]
        mov r10, [rel temp_matrix]
        mov r11, [rel diff_matrix]
.heaters_loop:
        dec edi

        mov eax, [rdx + rdi * 4]       ; row
        mov r9d, [rsi + rdi * 4]       ; column
        movss xmm1, DWORD [rcx + rdi * 4] ; temp[i]
        imul eax, r8d
        add eax, r9d                   ; eax = r * width + c
        movss DWORD [r10 + rax * 4], xmm1 ; temp_matrix[r * width + c] = temp[i];
        mov DWORD [r11 + rax * 4], NAN ; diff_matrix[r * width + c] = NaN;

        test edi, edi
        jnz .heaters_loop

        ret

; -----------------------------------------------------------------------------
; float get_cell_temp(int row, int column);
get_cell_temp:
        mov edx, [rel height]
        mov ecx, [rel width]

        mov	eax, edi
        or eax, esi
        js .return_coolant_temp        ; row < 0 || column < 0
        cmp edx, edi
        jle .return_coolant_temp       ; row >= height
        cmp ecx, esi
        jle .return_coolant_temp       ; column >= width

        mov eax, edi
        imul eax, ecx
        add eax, esi                   ; rax = row * width + column
        mov rcx, [rel temp_matrix]
        movss xmm0, DWORD [rcx+rax*4]
        ret

.return_coolant_temp:
        movss xmm0, DWORD [rel coolant_temp]
        ret

; -----------------------------------------------------------------------------
; void calculate_diff_for_field(int r, int c);
calculate_diff_for_field:
; edi = row, esi = column
        mov	eax, [rel width]           ; width
        mov	r8d, edi                   ; row
        mov	ecx, esi                   ; column
        imul eax, edi
        add	eax, esi                   ; eax = width * row + column
        mov rdx, [rel diff_matrix]     ; diff_matrix_ptr
        lea	r9, [rdx+rax*4]
        cmp	DWORD [r9], NAN
        je	.ret_from_diff

        mov r10, [rel temp_matrix]
        movss xmm1, dword [r10 + rax*4]
        movss xmm0, xmm1
        subss xmm0, xmm1
        addss xmm1, xmm1
        addss xmm1, xmm1
        subss xmm0, xmm1
        movss xmm1, xmm0

        inc edi
        call get_cell_temp
        addss xmm1, xmm0
        sub edi, 2
        call get_cell_temp
        addss xmm1, xmm0
        inc edi

        inc esi
        call get_cell_temp
        addss xmm1, xmm0
        sub esi, 2
        call get_cell_temp
        addss xmm1, xmm0

        mulss xmm1, DWORD [rel coeff]  ; diff *= coefficient
        movss DWORD [r9], xmm1         ; diff_matrix[r * width + c] = diff
.ret_from_diff:
        ret

; -----------------------------------------------------------------------------
; void calculate_diffs()
calculate_diffs:
        push r12
        push r13
        push r14

        mov r12d, [rel width]
        mov r13d, [rel height]

.loop_height:
        dec r13d
        mov r14d, r12d
.loop_width:
        dec r14d
        mov esi, r14d                  ; column argument
        mov edi, r13d                  ; row argument
        call calculate_diff_for_field
        test r14, r14
        jnz .loop_width
        test r13, r13
        jnz .loop_height

        pop r14
        pop r13
        pop r12

        ret

; -----------------------------------------------------------------------------
; void step();
step:
        call calculate_diffs

        mov eax, [rel width]
        mov ecx, [rel height]
        imul eax, ecx                  ; rax = width * height

        mov rdx, [rel diff_matrix]
        mov rsi, [rel temp_matrix]

.loop_cells:
        dec eax

        cmp dword [rdx + rax * 4], NAN
        je	.skip_adding

        movss xmm0, [rdx + rax * 4]
        movss xmm1, [rsi + rax * 4]
        addss xmm1, xmm0
        movss [rsi + rax * 4], xmm1

.skip_adding:
        test eax, eax
        jnz .loop_cells
        ret
