*THIS IS WORK IN PROGRESS!*

# About
This is an example of OAuth2 implementation. Not all features are implemented

# Supporeted APIs
Imgur.com and WordPress.com

# Saved information

Once you link, the following information will be saved to QSettings
- access token (used to access API)
- refresh token (used to get new access token)
- code (if you use response_type CODE)
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

In `applicationui.cpp` add 
```
#include "OAuth/WordPress.h" // or #include "OAuth/Imgur.h"
```
Add the following to expose it to QML:
```
qml->setContextProperty("oauth", OAuthx::Instance());
```

Go to Imgur.cpp or WordPress.cpp and add information required.

At this point OAuth should be ready to use. 

*You do not need to initialize it. Only one OAuth instance should exist at any point.*

# Work flow
The same instance of OAuth is being used in multiple classes. Access it using `OAuthx::Instance();`
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
