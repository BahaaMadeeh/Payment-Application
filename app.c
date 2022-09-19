#include <stdio.h>
#include "app.h"
#include <stdbool.h>


void appStart(void)
{
        ST_cardData_t newCard;
        EN_cardError_t cardError = getCardData(&newCard);
        
        if (cardError != CARD_OK)
            return;

        ST_terminalData_t newTerm;
        EN_terminalError_t termError = runTerminal(&newTerm, &newCard);

        if (termError != TERMINAL_OK)
            return;

        ST_transaction newTrans;
        newTrans.cardHolderData = newCard;
        newTrans.terminalData = newTerm;

        runServer(&newTrans);
        
    }


EN_cardError_t getCardData(ST_cardData_t* cardData)
{
    printf("CARD HOLDER DATA: \n");

    EN_cardError_t cardError = getCardHolderName(cardData);

    if (cardError != CARD_OK)
    {
        printf("\nWRONG_NAME");
        return cardError;
    }
    cardError = getCardExpiryDate(cardData);
    if (cardError != CARD_OK)
    {
        printf("WRONG_DATE\n");
        return cardError;
    }
    cardError = getCardPAN(cardData);

    if (cardError != CARD_OK)
    {
        printf("WRONG_NUMBER\n");
        return cardError;
    }

    return CARD_OK;
}
EN_terminalError_t runTerminal(ST_terminalData_t* termData, ST_cardData_t* cardData)
{
   
    puts("TERMINAL: ");
    EN_terminalError_t terminalError;
    getTransactionDate(termData);
    printf("%s\n", termData->transactionDate);
    setMaxAmount(termData);

    terminalError = isCardExpired(*cardData, *termData);

    if (terminalError != TERMINAL_OK)
    {
        printf("\nEXPIRED_CARD");
        return terminalError;
    }

    terminalError = isValidCardPAN(cardData);

    if (terminalError != TERMINAL_OK)
    {
        printf("\nINVALID_CARD");
        return terminalError;
    }

    terminalError = getTransactionAmount(termData);

    if (terminalError != TERMINAL_OK)
    {
        printf("\nINVALID_AMOUNT");
        return terminalError;
    }

    terminalError = isBelowMaxAmount(termData);

    if (terminalError != TERMINAL_OK)
    {
        printf("\nEXCEEDED_MAX_AMOUNT");
        return terminalError;
    }

    return TERMINAL_OK;
}
EN_transState_t runServer(ST_transaction* transData)
{

    puts("\t_SERVER RESPONSE: \n");
    EN_transState_t serverError = recieveTransactionData(transData);
    if (serverError == APPROVED)
    {
        printf("Transaction has been completed Successfully\n");
        return;
    }
    else if (serverError == DECLINED_INSUFFECIENT_FUND)
    {
        printf("LOW_BALANCE\n");
        return;
    }
    else if (serverError == DECLINED_STOLEN_CARD)
    {
        printf("STOLEN_CARD\n");
        return;
    }
    else if (serverError == INTERNAL_SERVER_ERROR)
    {
        printf("SAVING_FAILED\n");
        return;
    }
    else {
        printf("PROBLEM OCCURRED");
        return;
    }
    return SERVER_OK;
}


