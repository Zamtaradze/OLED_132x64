#include <stdio.h>
#include "Kmx32.h"


uint32_t  READ_REGR (uint32_t addf) {
	uint32_t 	rg_add = addf  & 0x1f, datx;
	uint32_t 	add = addf >> 5;
	 				switch (rg_add){

	 				case 0 : READ_REG_CASE(add, 0 , &datx  );	break;
	 				case 1 : READ_REG_CASE(add, 1 , &datx );		break;
	 				case 2 : READ_REG_CASE(add, 2 , &datx);		break;
	 				case 3 : READ_REG_CASE(add, 3 , &datx);		break;
	 				case 4 : READ_REG_CASE(add, 4 , &datx );		break;
	 				case 5 : READ_REG_CASE(add, 5 , &datx );
	 					 								break;
	 				case 6 : READ_REG_CASE(add, 6 , &datx );
	 					 								break;
	 				case 7 : READ_REG_CASE(add, 7 , &datx );
	 					 								break;
	 				case 8 : READ_REG_CASE(add, 8 , &datx );
	 					 								break;
	 				case 9 : READ_REG_CASE(add, 9 , &datx );
	 					 								break;
	 				case 10 : READ_REG_CASE(add, 10 , &datx );
	 					 								break;
	 				case 11 : READ_REG_CASE(add, 11 , &datx );
	 					 								break;
	 				case 12 : READ_REG_CASE(add, 12 , &datx );
	 					 								break;
	 				case 13 : READ_REG_CASE(add, 13 , &datx );
	 					 								break;
	 				case 14 : READ_REG_CASE(add, 14 , &datx );
	 					 								break;
	 				case 15 : READ_REG_CASE(add, 15 , &datx );
	 					 								break;
	 				case 16 : READ_REG_CASE(add, 16 , &datx );
	 					 								break;
	 				case 17 : READ_REG_CASE(add, 17 , &datx );
	 					 								break;
	 				case 18 : READ_REG_CASE(add, 18 , &datx );
	 					 								break;
	 				case 19 : READ_REG_CASE(add, 19 , &datx );
	 					 								break;
	 				case 20 : READ_REG_CASE(add, 20 , &datx );
	 					 								break;
	 				case 21 : READ_REG_CASE(add, 21 , &datx );
	 					 								break;
	 				case 22 : READ_REG_CASE(add, 22 , &datx );
	 					 								break;
	 				case 23 : READ_REG_CASE(add, 23 , &datx );
	 					 								break;
	 				case 24 : READ_REG_CASE(add, 24 , &datx );
	 					 								break;
	 				case 25 : READ_REG_CASE(add, 25 , &datx );
	 					 								break;
	 				case 26 : READ_REG_CASE(add, 26 , &datx );
	 					 								break;
	 				case 27 : READ_REG_CASE(add, 27 , &datx );
	 					 								break;
	 				case 28 : READ_REG_CASE(add, 28 , &datx );
	 					 								break;
	 				case 29 : READ_REG_CASE(add, 29 , &datx );
	 					 								break;
	 				case 30 : READ_REG_CASE(add, 30 , &datx );
	 					 								break;
	 				case 31 : READ_REG_CASE(add, 31 , &datx );
	 					 								break;
	 			    default:     break;


	 				}
	 				return datx;
 }



 void  WRITE_REGR (uint32_t addf, uint32_t datx) {
	uint32_t 	rg_add = addf  & 0x1f;
	uint32_t 	add = addf >> 5;
	 				switch (rg_add){

	 				case 0 : WRITE_REG_CASE(add, 0 , datx  );	break;
	 					 				case 1 : WRITE_REG_CASE(add, 1 , datx );		break;
	 					 				case 2 : WRITE_REG_CASE(add, 2 , datx);		break;
	 					 				case 3 : WRITE_REG_CASE(add, 3 , datx);		break;
	 					 				case 4 : WRITE_REG_CASE(add, 4 , datx );		break;
	 					 				case 5 : WRITE_REG_CASE(add, 5 , datx );
	 					 					 								break;
	 					 				case 6 : WRITE_REG_CASE(add, 6 , datx );
	 					 					 								break;
	 					 				case 7 : WRITE_REG_CASE(add, 7 , datx );
	 					 					 								break;
	 					 				case 8 : WRITE_REG_CASE(add, 8 , datx );
	 					 					 								break;
	 					 				case 9 : WRITE_REG_CASE(add, 9 , datx );
	 					 					 								break;
	 					 				case 10 : WRITE_REG_CASE(add, 10 , datx );
	 					 					 								break;
	 					 				case 11 : WRITE_REG_CASE(add, 11 ,datx );
	 					 					 								break;
	 					 				case 12 : WRITE_REG_CASE(add, 12 , datx );
	 					 					 								break;
	 					 				case 13 : WRITE_REG_CASE(add, 13 , datx );
	 					 					 								break;
	 					 				case 14 : WRITE_REG_CASE(add, 14 , datx );
	 					 					 								break;
	 					 				case 15 : WRITE_REG_CASE(add, 15 , datx );
	 					 					 								break;
	 					 				case 16 : WRITE_REG_CASE(add, 16 , datx );
	 					 					 								break;
	 					 				case 17 : WRITE_REG_CASE(add, 17 , datx );
	 					 					 								break;
	 					 				case 18 : WRITE_REG_CASE(add, 18 , datx );
	 					 					 								break;
	 					 				case 19 : WRITE_REG_CASE(add, 19 , datx );
	 					 					 								break;
	 					 				case 20 : WRITE_REG_CASE(add, 20 , datx );
	 					 					 								break;
	 					 				case 21 : WRITE_REG_CASE(add, 21 , datx );
	 					 					 								break;
	 					 				case 22 : WRITE_REG_CASE(add, 22 , datx );
	 					 					 								break;
	 					 				case 23 : WRITE_REG_CASE(add, 23 , datx );
	 					 					 								break;
	 					 				case 24 : WRITE_REG_CASE(add, 24 , datx );
	 					 					 								break;
	 					 				case 25 : WRITE_REG_CASE(add, 25 , datx );
	 					 					 								break;
	 					 				case 26 : WRITE_REG_CASE(add, 26 , datx );
	 					 					 								break;
	 					 				case 27 : WRITE_REG_CASE(add, 27 , datx );
	 					 					 								break;
	 					 				case 28 : WRITE_REG_CASE(add, 28 , datx );
	 					 					 								break;
	 					 				case 29 : WRITE_REG_CASE(add, 29 , datx );
	 					 					 								break;
	 					 				case 30 : WRITE_REG_CASE(add, 30 , datx );
	 					 					 								break;
	 					 				case 31 : WRITE_REG_CASE(add, 31 , datx );
	 					 					 								break;
	 					 			    default:     break;

	 				}

 }

