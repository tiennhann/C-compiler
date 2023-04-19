

#	Package foo 

	.section .rodata # start of the RODATA section, strings first

percentD:  .string "%d" # ALWAYSneeded for print int
# NEXT lines are all the strngs in the program
_L0:	.string  "enter a number "	# global string
_L1:	.string  "you entered "	# global string
_L2:	.string  "\n"	# global string
#  END of print  all the strngs in the program

	.data # start of the DATA section for variables


	.comm _SCANFHOLD,8,8  # MANDITORY space for SCANF reads

	.comm x,8,8	#  define a global variable
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


	subq $16, %rsp	# carve out activation record for function 
	mov  $_L0, %rdi	#  RDI is the label address
	mov $0, %rax	# RAX need to be zero
	call printf	# print a string

	
	
	mov $_SCANFHOLD, %rsi	# read in a number 
	mov $percentD , %rdi	# rdi has integer format for scanf
	mov $0 , %rax	# No other parameters for scanf
	call  __isoc99_scanf	# call read  
	mov _SCANFHOLD, %rax	# bring value on STACK into RAX register for default value 

	mov %rax, 0(%rsp)	# #store RHS value in memory
	mov $x, %rax	# #load gobal variable from data segment
	mov 0(%rsp), %rbx	# # Get RHS stored value
	mov %rbx, (%rax)	# # ASSIGN final store 
	
	mov  $_L1, %rdi	#  RDI is the label address
	mov $0, %rax	# RAX need to be zero
	call printf	# print a string

	
	mov $x, %rax	# #load gobal variable from data segment
	mov (%rax), %rax	# #expression is identifier
		# #print an INT
	mov %rax, %rsi	# #RSI needs the value
	mov $percentD, %rdi	# #RDI needs to be the int format
	mov $0, %rax	# #RAX needs to be 0
	call printf	#  print a number from expression

	
	mov  $_L2, %rdi	#  RDI is the label address
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
