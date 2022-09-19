#include <stdio.h>
#include "server.h"
#include "terminal.h"
#include <stdlib.h>
#include <string.h>


//The server accounts database
ST_accountsDB_t accountsDB[255] = {
	{500.0,  "4916945627505481"},
	{5000.0, "1234567891111111"},
	{1600.0, "4929101130212489"},//wrong number
	{7500.0, "4485846819394224"},
	 };

//transaction database with initial value 0 for saving transactions
ST_transaction transDB[255] = { {0,0,0},{0,0,0},{0,0,0} };

uint8_t DBmember=0;

EN_transState_t recieveTransactionData(ST_transaction* transData)
{
	//check if the account and balance valid in the database or not
	
	if (isValidAccount(&(transData->cardHolderData)) != SERVER_OK)
	{
		transData->transState = DECLINED_STOLEN_CARD;
		saveTransaction(transData);
		return DECLINED_STOLEN_CARD;
	}
	
	if (isAmountAvailable(&(transData->terminalData)) == LOW_BALANCE)
	{
		transData->transState = DECLINED_INSUFFECIENT_FUND;
		saveTransaction(transData);
	
		return DECLINED_INSUFFECIENT_FUND;
	}
	
	//update balance in database
	accountsDB[DBmember].balance -= transData->terminalData.transAmount;

	//saving the transaction in database
	if (saveTransaction(transData) == SAVING_FAILED)
	{
		transData->transState = INTERNAL_SERVER_ERROR;
		return INTERNAL_SERVER_ERROR;
	}
		

	return APPROVED;
}

EN_serverError_t isValidAccount(ST_cardData_t* cardData)
{
		
	for (uint32_t i = 0; i < 5; i++)
	{
		if (strcmp(cardData->primaryAccountNumber, accountsDB[i].primaryAccountNumber)!=0)
		{
			return ACCOUNT_NOT_FOUND;
		}
		else
		{
			DBmember = i;
			return SERVER_OK;
		}
		
		
	}
}
EN_serverError_t isAmountAvailable(ST_terminalData_t* termData)
{
	if ((termData->transAmount) > (accountsDB[DBmember].balance))
	{
		return LOW_BALANCE;
	}
	return SERVER_OK;
}

EN_serverError_t saveTransaction(ST_transaction* transData)
{
	//initializing static iterator
	static uint8_t iterator = 0;
	transDB[DBmember].cardHolderData = transData->cardHolderData;
	transDB[DBmember].terminalData = transData->terminalData;
	transDB[DBmember].transState = transData->transState;
	//transData->transactionSequenceNumber = rand();  //will take random numbers
	transDB[DBmember].transactionSequenceNumber = transData->transactionSequenceNumber;

	if (transDB[DBmember].transactionSequenceNumber == 0)
	{
		transData->transState = SAVING_FAILED;
		return SAVING_FAILED;
		printf("failed");
	}
	else if (iterator > 255)
	{
		transData->transState = SAVING_FAILED;
		return SAVING_FAILED;
		printf("failed");
	}
	iterator++;
	return APPROVED;
	printf("done");
	
}

EN_serverError_t getTransaction(uint32_t transactionSequenceNumber, ST_transaction *transData)
{
	if (DBmember == 255)
		return ACCOUNT_NOT_FOUND;

	if (transDB[DBmember].transactionSequenceNumber != transactionSequenceNumber)
	{
		transData->transState = TRANSACTION_NOT_FOUND;
		return TRANSACTION_NOT_FOUND;
		printf("not found");
	}
	return APPROVED;
	printf("found");
}

