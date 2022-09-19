#define _CRT_SECURE_NO_WARNINGS
#define _CRT_NONSTDC_NO_WARNINGS
#define _CRT_SECURE_NO_DEPRECATE  
#define _CRT_NONSTDC_NO_DEPRECATE
#include <stdio.h>
#include "card.h"
#include <string.h>


// Reading card holder name from the user
EN_cardError_t getCardHolderName(ST_cardData_t* cardData)
{
	
	printf("\t-Enter card data- \n");
	printf("Enter card holder name: \n");
	gets(cardData->cardHolderName);

	if (strlen(cardData->cardHolderName) > 24 || strlen(cardData->cardHolderName) < 20)
	{
		return WRONG_NAME;
	}
	else
		return CARD_OK;
}

//reading the card PAN from the user
EN_cardError_t getCardPAN(ST_cardData_t* cardData)
{
	printf("Enter the Primary Account Number (PAN): \n");
	gets(cardData->primaryAccountNumber);
	if (strlen(cardData->primaryAccountNumber) > 19 || strlen(cardData->primaryAccountNumber) < 16)
	{
		return WRONG_PAN;
	}
	else
		return CARD_OK;
}

//reading the card expiration date from the user
EN_cardError_t getCardExpiryDate(ST_cardData_t* cardData)
{
	printf("Enter card expiray date: \t(MM/YY)\n");
	gets(cardData->cardExpirationDate);
	if (strlen(cardData->cardExpirationDate) != 5 || cardData->cardExpirationDate[2] != '/')
	{
		return WRONG_EXP_DATE;
	}
	else
		return CARD_OK;
}

