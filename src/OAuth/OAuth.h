/*
 * OAuth.h
 *
 *  Created on: 22. feb. 2015
 *      Author: Bojan
 */

#ifndef OAUTH_H_
#define OAUTH_H_

#include <QNetworkReply>
#include <QUrl>
#include <QDebug>

class OAuth: public QObject
{
    Q_OBJECT
    Q_PROPERTY(uint expiresIn READ expiresIn WRITE setExpiresIn)
    Q_PROPERTY(bool isLinked READ isLinked WRITE onSetIsLinked NOTIFY isLinkedChanged)
    Q_PROPERTY(QString accountUsername READ accountUsername WRITE setAccountUsername NOTIFY accountUsernameChanged)
public:
    OAuth();Q_INVOKABLE
    void link();Q_INVOKABLE
    void unlink();Q_INVOKABLE
    bool isValidToken();
    bool verifyLogin();Q_INVOKABLE
    void authorizationReceived(QString url);Q_INVOKABLE
    QString accessToken();Q_INVOKABLE
    void getNewAccessToken();signals:
    void openBrowser(QUrl url);
    void closeBrowser();
    void accessTokenRefreshed();
    void isLinkedChanged(bool value);
    void accountUsernameChanged(QString value);

private slots:
    void onNewAccessTokenReply(QNetworkReply * reply);

public:
    QString clientId();
    void setClientId(const QString &value);
    QString clientSecret();
    void setClientSecret(const QString &value);
    QString authorizeUrl();
    void setAuthorizeUrl(const QString &value);
    QString refreshUrl();
    void setRefreshUrl(const QString &value);
    void setAccessToken(const QString& value);
    QString tokenType();
    void setTokenType(const QString& value);
    QString refreshToken();
    void setRefreshToken(const QString& value);
    QString accountUsername();
    void setAccountUsername(const QString& value);
    void setState(const QString& value);
    int expiresIn();
    void setExpiresIn(const int& value);
    uint expireDateTime();
    void setExpireDateTime(const uint& value);

    bool isLinked();
    bool isRunning();
    void onSetIsLinked(bool value)
    {
        m_isLinked = value;
        qDebug() << "[OAuth.onSetIsLinked] is linked: " << m_isLinked;
        emit isLinkedChanged(m_isLinked);
    }

protected:
    void changeExpireDateTime();

protected:
    QString m_client_id;
    QString m_client_secret;
    QString m_authorize_url;
    QString m_refresh_url;
    QString m_state;
    bool m_isLinked;

private:
    QString getValueFor(const QString &objectName, const QString &defaultValue);
    void saveValueFor(const QString &objectName, const QString &inputValue);
    void settingsSaveAccessToken(QString value);
    void settingsSaveRefreshToken(QString value);
    void settingsSaveUsername(QString value);
    void settingsSaveExpiresDateTime(uint value);
    uint settingsLoadExpiresDateTime();
    QString settingsLoadRefreshToken();
    QString settingsLoadAccessToken();
    QString settingsLoadUsername();
    void loadFromSettings();

private:
    QNetworkAccessManager * netManager;
    QString m_access_token;
    QString m_token_type;
    QString m_refresh_token;
    QString m_account_username;
    int m_expires_in;
    uint m_expire_datetime;
    bool m_running;

};

const char m_response_type[] = "token";
const char OAUTH_SETTINGS_PREFIX[] = "oauth_";
const char OAUTH_CLIENT_ID[] = "client_id";
const char OAUTH_CLIENT_SECRET[] = "client_secret";
const char OAUTH_RESPONSE_TYPE[] = "response_type";
const char OAUTH_STATE[] = "state";
const char OAUTH_ACCESS_TOKEN[] = "access_token";
const char OAUTH_EXPIRES_IN[] = "expires_in";
const char OAUTH_EXPIRES_DATETIME[] = "expires_datetime";
const char OAUTH_TOKEN_TYPE[] = "token_type";
const char OAUTH_REFRESH_TOKEN[] = "refresh_token";
const char OAUTH_ACCOUNT_USERNAME[] = "account_username";
const char OAUTH_GRANT_TYPE[] = "grant_type";

#endif /* OAUTH_H_ */