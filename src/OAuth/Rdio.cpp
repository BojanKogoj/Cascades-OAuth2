/*
 * WordPress.cpp
 *
 *  Created on: 12. avg. 2015
 *      Author: Bojan
 */

#include "Rdio.h"

Rdio::Rdio()
{
    // API info
    setAuthorizeUrl("https://www.rdio.com/account/oauth2/authorize");
    setRefreshUrl("https://services.rdio.com/oauth2/token");

    // Client info
    setClientId("yourclientid");
    setClientSecret("yourclientsecret");
    setRedirectURI("http://yourwebsite.com");
    setResponseType(CODE);
}

Rdio::~Rdio()
{
}

