*THIS IS WORK IN PROGRESS!*

# About
This is an example of OAuth2 implementation, working with Imgur.com API. Some features might be missing.

# Supporeted APIs
Right now only Imgur is supported (and example). It shouldn't be too hard to modify for other APIs

# Saved information

Once you link, the following information will be saved to QSettings
- access token (used to access API)
- refresh token (used to get new access token)
- account username (for displaying inside application)
- expiration datetime (when token becomes invalid)

All settings names will have a prefix, defined in OAUTH_SETTINGS_PREFIX. Default value is "oauth_", but you can change it.

# Setting it up in your project

First copy whole `OAuth` folder to your `src/` folder

Add this to your .pro file
```
QT += network
LIBS += -lbbdata
```

In `applicationui.h` add 
```
#include "OAuth/OAuth.h"
```
And
```
public:
    static OAuth * oauth;
```
In ApplicationUI, before `ApplicationUI::ApplicationUI(): QObject()` add
```
OAuth* ApplicationUI::oauth;
```

This example is for Imgur API
```
// OAuth
oauth = new OAuth();
// API info
oauth->setAuthorizeUrl("https://api.imgur.com/oauth2/authorize");
oauth->setRefreshUrl("https://api.imgur.com/oauth2/token");
// Client info
oauth->setClientId("bfb7676d58afxxx");
oauth->setClientSecret("92991f3a71908986985a91acd64b1d05b3396xxx");
oauth->setState("cu7l2w3Y2lxkPIU"); 
```
At this point OAuth should be ready to use

# Work flow
The same instance of OAuth is being used in multiple classes, so oauth is static. Access it using `ApplicationUI::oauth;`
This project contains a partial example.

To use:
- Check is device is already linked using oauth->isLinked(); 
- If not, call `link()` which will open browser. After user logs in the browser will close, you can now use API.

- For each request, you must call `isValidToken();`. If it returns false you need to refresh access token, otherwise you can use it. Example below
```
if (oauth->isValidToken()) {
	startRequest();
} else {
	oauth->getNewAccessToken();
	connect(oauth, SIGNAL(accessTokenRefreshed()), this, SLOT(onTokenRefreshed()));
}
```
When `onTokenRefreshed()` is called, you may start the request, and get token using `accessToken()`

# More
_Public methods_
```
Q_INVOKABLE void link();
Q_INVOKABLE void unlink();
Q_INVOKABLE bool isValidToken();
Q_INVOKABLE void authorizationReceived(QString url);
Q_INVOKABLE QString accessToken();
Q_INVOKABLE void getNewAccessToken();
bool verifyLogin();
```
_Signals_
```
void openBrowser(QUrl url);
void closeBrowser();
void accessTokenRefreshed();
void isLinkedChanged(bool value);
void accountUsernameChanged(QString value);
```
