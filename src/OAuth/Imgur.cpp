/*
 * Imgur.cpp
 *
 *  Created on: 12. avg. 2015
 *      Author: Bojan
 */

#include "Imgur.h"

Imgur::Imgur()
{
    // API info
    setAuthorizeUrl("https://api.imgur.com/oauth2/authorize");
    setRefreshUrl("https://api.imgur.com/oauth2/token");

    // Client info
    setClientId("bfb7676d58afd92");
    setClientSecret("92991f3a71908986985a91acd64b1d05b33969dd");
    setState("cu7l2w3Y2lxkPIU");
    setResponseType(CODE);

}

Imgur::~Imgur()
{
    // TODO Auto-generated destructor stub
}

