
# Nhan Le
# CS370
#	Package foo 

	.section .rodata # start of the RODATA section, strings first

percentD:  .string "%d" # ALWAYSneeded for print int
# NEXT lines are all the strngs in the program
_L5: 	.string "Please enter your number: " 
_L0:	.string "Sum of squares from 1 to "	
_L00:	.string " is " 	# global string
_L1:	.string  "\n"	# global string
#  END of print  all the strngs in the program

	.data # start of the DATA section for variables


	.comm _SCANFHOLD,8,8  # MANDITORY space for SCANF reads

sum:	.long 0	# define a global variable with initial value
	.comm x,8,8	#  define a global variable
	.comm i,8,8	#  define a global variable
# END  of the DATA section for variables


	.text      # start of CODE segment

 	.globl main
	.type main, @function
main:		# Start of Function


	.cfi_startproc	# 	 STANDARD FUNCTION HEADER FOR GAS
	pushq   %rbp	# 	 STANDARD FUNCTION HEADER FOR GAS
	.cfi_def_cfa_offset 16	# 	 STANDARD FUNCTION HEADER FOR GAS
	.cfi_offset 6, -16	# 	 STANDARD FUNCTION HEADER FOR GAS
	movq    %rsp, %rbp	# 	 STANDARD FUNCTION HEADER FOR GAS
	.cfi_def_cfa_register 6	# 	 STANDARD FUNCTION HEADER FOR GAS


	subq $56, %rsp	# carve out activation record for function 
	mov  $0,  %rax	# # expresion a number
	mov %rax, 0(%rsp)	# #store RHS value in memory
	mov $i, %rax	# #load gobal variable from data segment
	mov 0(%rsp), %rbx	# # Get RHS stored value
	mov %rbx, (%rax)	# # ASSIGN final store
	#this is all i=0;

	#get a value into x via prompt 
	mov $x, %rax 
	mov $100, %rbx
	mov %rbx, (%rax)
	
	#get the input (number) from user and store it in _L5	
	mov  $_L5, %rdi	#  RDI is the label address
	mov $0, %rax	# RAX need to be zero
	call printf	# print a string

	#scanf the input.
	mov $_SCANFHOLD, %rsi	# read in a number 
	mov $percentD , %rdi	# rdi has integer format for scanf
	mov $0 , %rax	# No other parameters for scanf
	call  __isoc99_scanf	# call read  
	mov _SCANFHOLD, %rax	# bring value on STACK into RAX register for default value 

	mov %rax, 0(%rsp)	# #store RHS value in memory
	mov $x, %rax	# #load gobal variable from data segment
	mov 0(%rsp), %rbx	# # Get RHS stored value
	mov %rbx, (%rax)	# # ASSIGN final store 
	
	
_L2:		# # WHILE TOP target
	mov $i, %rax	# #load gobal variable from data segment
	mov (%rax), %rax	# #expression is identifier
	mov %rax, 8(%rsp)	# #STORE LHS of expression to memory
	# mov  $100,  %rax	# # expresion a number
        
	# what ever in x, we put it in rax
	mov $x, %rax	# #load global variable from data segment
	mov (%rax), %rax	# #expression is indentifier

	mov %rax, %rbx	# #right hand side needs to be set
	mov 8(%rsp), %rax	# #fetch LHS of expression from memory
	cmp %rbx, %rax	# ;EXPR LessEqual
	setle %al	# ;EXPR Lessthan
	mov $1, %rbx	# ;set rbx to one to filter rax
	and %rbx, %rax	# ;filter RAX
	cmp $0, %rax	# WHILE compare
	JE  _L3	# #WHILE branch out
	mov $sum, %rax	# #load gobal variable from data segment
	mov (%rax), %rax	# #expression is identifier
	mov %rax, 24(%rsp)	# #STORE LHS of expression to memory
	mov $i, %rax	# #load gobal variable from data segment
	mov (%rax), %rax	# #expression is identifier
	mov %rax, 16(%rsp)	# #STORE LHS of expression to memory
	mov $i, %rax	# #load gobal variable from data segment
	mov (%rax), %rax	# #expression is identifier
	mov %rax, %rbx	# #right hand side needs to be set
	mov 16(%rsp), %rax	# #fetch LHS of expression from memory
	imul %rbx, %rax	# #EXPR MULT RAX has lower word
	mov %rax, %rbx	# #right hand side needs to be set
	mov 24(%rsp), %rax	# #fetch LHS of expression from memory
	add %rbx, %rax	# #EXPR ADD
	mov %rax, 32(%rsp)	# #store RHS value in memory
	mov $sum, %rax	# #load gobal variable from data segment
	mov 32(%rsp), %rbx	# # Get RHS stored value
	mov %rbx, (%rax)	# # ASSIGN final store 
	
	mov $i, %rax	# #load gobal variable from data segment
	mov (%rax), %rax	# #expression is identifier
	mov %rax, 40(%rsp)	# #STORE LHS of expression to memory
	mov  $1,  %rax	# # expresion a number
	mov %rax, %rbx	# #right hand side needs to be set
	mov 40(%rsp), %rax	# #fetch LHS of expression from memory
	add %rbx, %rax	# #EXPR ADD
	mov %rax, 48(%rsp)	# #store RHS value in memory
	mov $i, %rax	# #load gobal variable from data segment
	mov 48(%rsp), %rbx	# # Get RHS stored value
	mov %rbx, (%rax)	# # ASSIGN final store 
	
	jmp _L2	# #WHILE Jump back
_L3:		# # End of WHILE 
	
	mov  $_L0, %rdi	#  RDI is the label address
	mov $0, %rax	# RAX need to be zero
	call printf	# print a string
	
	#add the code, so we able to print the value of number that we imput
	mov $x, %rax	# #load gobal variable from data segment
	mov (%rax), %rax	# #expression is identifier
		# #print an INT
	mov %rax, %rsi 	# #RRSI needs the value
	mov $percentD, %rdi 	# #RDI needs to be the int format
	mov $0, %rax 	# #RAX needs to be 0
	call printf	# print a number from expression

	mov $_L00, %rdi	# RDI is the label address
	mov $0, %rax	# RAX need to be zero
	call printf  	# printf a string

	
	mov $sum, %rax	# #load gobal variable from data segment
	mov (%rax), %rax	# #expression is identifier
		# #print an INT
	mov %rax, %rsi	# #RSI needs the value
	mov $percentD, %rdi	# #RDI needs to be the int format
	mov $0, %rax	# #RAX needs to be 0
	call printf	#  print a number from expression

	
	mov  $_L1, %rdi	#  RDI is the label address
	mov $0, %rax	# RAX need to be zero
	call printf	# print a string

	
	mov $0, %rax    	# #  return  NULL zero (0) 
	leave       	#  leave the function
	.cfi_def_cfa 7, 8	#  # STANDRD end function for GAS
	ret

	.cfi_endproc	#  # STANDRD end function for GAS
	.size	main, .-main	#  # STANDRD end function for GAS

	 .ident  "GCC: (SUSE Linux) 7.5.0"
	.section        .note.GNU-stack,"",@progbits
