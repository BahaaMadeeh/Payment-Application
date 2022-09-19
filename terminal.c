#include "terminal.h"
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
//to convert a character into an integer
#define charToInt(x) ((x) % 48)  //changing from char to integer using ASCII
EN_terminalError_t getTransactionDate(ST_terminalData_t* termData)
{
	//get time from system using time.h functions
	time_t now = time(NULL);
	struct tm* currTime = localtime(&now);
	strftime(termData->transactionDate, 11, "%d/%m/%Y", currTime);
	return TERMINAL_OK;
}

EN_terminalError_t isCardExpired(ST_cardData_t cardData, ST_terminalData_t termData)
{
	uint8_t cardMonth = charToInt(cardData.cardExpirationDate[0])  + charToInt(cardData.cardExpirationDate[1]);
	uint8_t cardYear = charToInt(cardData.cardExpirationDate[3])  + charToInt(cardData.cardExpirationDate[4]);

	uint8_t currMonth = charToInt(termData.transactionDate[3])  + charToInt(termData.transactionDate[4]);
	uint8_t currYear = charToInt(termData.transactionDate[8])  + charToInt(termData.transactionDate[9]);

	if (currYear < cardYear)
	{
		return EXPIRED_CARD;
	}
	else if (currYear == cardYear && currMonth > cardMonth)
	{
		return EXPIRED_CARD;
	}
		return TERMINAL_OK;
}

EN_terminalError_t getTransactionAmount(ST_terminalData_t* termData)
{
	printf("Enter the transaction amount: \n");
	scanf("%f",&termData->transAmount);
	if (termData->transAmount <= 0)
	{
		return INVALID_AMOUNT;
	}
	else
		return TERMINAL_OK;
}
EN_terminalError_t isValidCardPAN(ST_cardData_t* cardData)
{
	//check Luhn algorithm
	uint8_t i = 0, mult = 2, sum = 0, temp = 0;
	while (cardData->primaryAccountNumber[i] != '\0')
	{
		//multiply the even digits
		temp = charToInt(cardData->primaryAccountNumber[i]) * mult;
		//sum doubled value check
		if (sum > 9)
		{
			temp = (temp / 10) + (temp % 10);
		}
		//toggle mult
		if (mult == 2)
			mult--;
		else
			mult++;
		//sum all values
		sum += temp;
		i++;
	}
	//check if the sum is a multiple of 10 or not
	if (sum % 10 != 0)
		return INVALID_CARD;
	else
		return TERMINAL_OK;
}
EN_terminalError_t isBelowMaxAmount(ST_terminalData_t* termData)
{

	if (termData->transAmount > termData->maxTransAmount)
	{
		return EXCEED_MAX_AMOUNT;
	}
	return TERMINAL_OK;
}
EN_terminalError_t setMaxAmount(ST_terminalData_t* termData)
{
	termData->maxTransAmount = 5000;
	return TERMINAL_OK;
}

