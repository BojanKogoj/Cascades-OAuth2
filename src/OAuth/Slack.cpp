/*
 * Slack.cpp
 *
 *  Created on: 23. jun. 2016
 *      Author: Bojan
 */

#include "Slack.h"

Slack::Slack()
{
       // API info
       setAuthorizeUrl("https://slack.com/oauth/authorize");

       // Client info
       setClientId("yourclientid");
       setClientSecret("yourclientsecret");
       setRedirectURI("http://adev.si/success");
       setResponseType(CODE);
       setState("12345abc");
       setScope("chat:write:bot");
       setRedirectURI("http://example.com");
}

Slack::~Slack()
{

}



