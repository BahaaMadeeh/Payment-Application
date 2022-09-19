#pragma once
#include "server.h"
#include "terminal.h"
#include "card.h"
#ifndef _APP_H_
#define _APP_H_

extern void appStart(void);

extern EN_cardError_t getCardData(ST_cardData_t* cardData);
extern EN_terminalError_t runTerminal(ST_terminalData_t* termData, ST_cardData_t* cardData);
extern EN_transState_t runServer(ST_transaction* transData);
#endif // !_APP_H_


