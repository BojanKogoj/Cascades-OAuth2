/*
 * WordPress.cpp
 *
 *  Created on: 12. avg. 2015
 *      Author: Bojan
 */

#include "WordPress.h"
//#include "ResponseType.h"

WordPress::WordPress()
{
    // API info
    setAuthorizeUrl("https://public-api.wordpress.com/oauth2/authorize");
    setRefreshUrl("https://public-api.wordpress.com/oauth2/token");

    // Client info
    setClientId("12345");
    setClientSecret("clientsecret");
    setRedirectURI("http://example.com");
    setResponseType(CODE);
}

WordPress::~WordPress()
{
}

