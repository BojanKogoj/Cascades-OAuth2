/*
 * OAuth.cpp
 *
 *  Created on: 22. feb. 2015
 *      Author: Bojan
 */

#include "OAuth.h"
#include <bb/data/JsonDataAccess>
#include <QHttpMultiPart>
#include <QSettings>
#include <QDateTime>
#include <QPair>

OAuth::OAuth() :
        QObject()
{
    // TODO Auto-generated constructor stub
    loadFromSettings();
    qDebug() << "[OAuth] << access_token: " << accessToken();
    qDebug() << "[OAuth] << refresh_token: " << refreshToken();
    qDebug() << "[OAuth] << expire_datetime: " << expireDateTime();
    m_running = false;
}

void OAuth::link()
{
    QList<QPair<QString, QString> > parameters;
    parameters.append(qMakePair(QString(OAUTH_CLIENT_ID), m_client_id));
    parameters.append(qMakePair(QString(OAUTH_RESPONSE_TYPE), responseType()));

    if (!m_state.isEmpty())
        parameters.append(qMakePair(QString(OAUTH_STATE), QString(m_state)));

    if (!redirectURI().isEmpty())
        parameters.append(qMakePair(QString(OAUTH_REDIRECT_URI), QString(m_redirect_uri)));

    QUrl url(m_authorize_url);
    url.setQueryItems(parameters);
    qDebug() << "[OAuth.link] Url: " << url;
    emit openBrowser(url);
    m_running = true;
}

void OAuth::authorizationReceived(QString url)
{
    if (m_response_type == CODE)
        authorizationReceivedCode(url);
    else
        authorizationReceivedToken(url);
}

bool OAuth::isValidToken()
{
    qDebug() << "[Oauth.isValidToken] valid: " << (expireDateTime() > QDateTime::currentMSecsSinceEpoch() / 1000);
    return expireDateTime() > QDateTime::currentMSecsSinceEpoch() / 1000;
}

void OAuth::getNewAccessToken()
{
    // if two request refresh key at the same time only one request to be sent
    if (isRunning())
        return;

    netManager = new QNetworkAccessManager();
    if (!netManager) {
        qDebug() << "Unable to create QNetworkAccessManager!";
        //emit complete("Unable to create QNetworkAccessManager!", false);
        return;
    }

    QUrl url(m_refresh_url);
    //https://api.imgur.com/oauth2/token?client_id=2137fbd8fcb4a0a&client_secret=ffc769f3b79ac245e4d8871518d9617ceaf4b2cd&refresh_token=f1ae5cc3f2f231a29db009a54c5c5bb3ebc20684&grant_type=refresh_token
    QNetworkRequest req(url);
    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

    QUrl postData;
    postData.addQueryItem(OAUTH_CLIENT_ID, clientId().toAscii());
    postData.addQueryItem(OAUTH_CLIENT_SECRET, clientSecret().toAscii());

    // If redirect is required and available
    if (!m_redirect_uri.isEmpty())
        postData.addQueryItem(OAUTH_REDIRECT_URI, redirectURI().toAscii());

    if (m_response_type == TOKEN) {
        postData.addQueryItem(OAUTH_REFRESH_TOKEN, refreshToken().toAscii());
        postData.addQueryItem(OAUTH_GRANT_TYPE, OAUTH_REFRESH_TOKEN);
    } else {
        postData.addQueryItem(OAUTH_CODE, code().toAscii());
        postData.addQueryItem(OAUTH_GRANT_TYPE, OAUTH_AUTHORIZATION_CODE);
    }

    QNetworkReply* ipReply = netManager->post(req, postData.encodedQuery());
    bool connectResult;
    Q_UNUSED(connectResult);
    connectResult = connect(netManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(onNewAccessTokenReply(QNetworkReply*)));

    Q_ASSERT(connectResult);
    m_running = true;

}

void OAuth::onNewAccessTokenReply(QNetworkReply* reply)
{
    QByteArray response = reply->readAll();
    // if valid!
    qDebug() << "[OAuth.onNetworkReply] reply" << response;

    if (reply->error() > 0) {
        qDebug() << "[OAuth.onNetworkReply] Network error! " << reply->errorString();
        return;
    }

    QVariantMap result;
    bb::data::JsonDataAccess jda;
    result = jda.loadFromBuffer(response).toMap();
    if (result["access_token"].toString().size() == 0) {
        qDebug() << "[OAuth.onNetworkReply] Didn't get get access_token ";
        //emit error?
        return;
    }
    //[OAuth.onNetworkReply] reply "{"access_token":"09e6f53ee377f6xxxx","expires_in":3600,"token_type":"bearer","scope":null,"refresh_token":"c9517d8e0e489c422b745xxxxx","account_id":11140216,"account_username":"xxxx"}"

    // To make sure
    if (result[OAUTH_ACCESS_TOKEN].toString().isEmpty())
        return;

    setAccessToken(result[OAUTH_ACCESS_TOKEN].toString());
    setAccountUsername(result[OAUTH_ACCOUNT_USERNAME].toString());
    setExpiresIn(result[OAUTH_EXPIRES_IN].toInt());
    setIsLinked(true);

    settingsSaveAccessToken(accessToken());
    settingsSaveRefreshToken(refreshToken());
    settingsSaveUsername(accountUsername());
    settingsSaveExpiresDateTime(expiresIn());

    emit accessTokenRefreshed();
    reply->deleteLater();

}

void OAuth::authorizationReceivedToken(QString url)
{
    QUrl tmp(url);

    QUrl mUrl("?" + tmp.fragment()); // all info is in fragment, we have to transform it to be able to read

            // if you sent State, expect it!
    if (!m_state.isEmpty())
        if (QString(mUrl.queryItemValue(OAUTH_STATE)) == QString(m_state)) {
            qDebug() << "[OAuth.onAuthorizationReceived] State did not match! Received: " << mUrl.queryItemValue(OAUTH_STATE);
            // TODO emit signal with error!
            return;
        }

    if (mUrl.queryItemValue(OAUTH_ACCESS_TOKEN).size() == 0) {
        return;
    }
    if (mUrl.queryItemValue(OAUTH_EXPIRES_IN).size() == 0) {
        return;
    }
    if (mUrl.queryItemValue(OAUTH_TOKEN_TYPE).size() == 0) {
        return;
    }

    // in some cases refresh token will not be received
    /*if (mUrl.queryItemValue(OAUTH_REFRESH_TOKEN).size() == 0) {
     return;
     }*/

    setAccessToken(mUrl.queryItemValue(OAUTH_ACCESS_TOKEN));
    setExpiresIn(mUrl.queryItemValue(OAUTH_EXPIRES_IN).toInt());
    setTokenType(mUrl.queryItemValue(OAUTH_TOKEN_TYPE));
    setRefreshToken(mUrl.queryItemValue(OAUTH_REFRESH_TOKEN));
    setCode(mUrl.queryItemValue(OAUTH_CODE));
    settingsSaveCode(code());
    settingsSaveAccessToken(accessToken());
    settingsSaveRefreshToken(refreshToken());

    // Not considered important?
    if (mUrl.queryItemValue(OAUTH_ACCOUNT_USERNAME).size() > 0) {
        setAccountUsername(mUrl.queryItemValue(OAUTH_ACCOUNT_USERNAME));
        settingsSaveUsername(accountUsername());
    }

    setIsLinked(true);

    emit closeBrowser();
    m_running = false;
}

void OAuth::setCode(const QString& value)
{
    m_code = value;
}

QString OAuth::code()
{
    return m_code;
}

void OAuth::authorizationReceivedCode(QString url)
{
    QUrl mUrl(url);

    // if you sent State, expect it!
    if (!m_state.isEmpty())
        if (QString(mUrl.queryItemValue(OAUTH_STATE)) != QString(m_state)) {
            qDebug() << "[OAuth.onAuthorizationReceived] State did not match! Received: " << mUrl.queryItemValue(OAUTH_STATE);
            // TODO emit signal with error!
            return;
        }

    if (mUrl.queryItemValue(OAUTH_CODE).size() == 0) {
        return;
    }

    setCode(mUrl.queryItemValue(OAUTH_CODE));
    settingsSaveCode(code());

    m_running = false;
    getNewAccessToken();
    setIsLinked(true);
    emit closeBrowser();

}

/***
 * If you do not immediately require connection on application startup you can call this to refresh
 * token if necessary, to reduce load times in the future
 */
bool OAuth::verifyLogin()
{
    if (isLinked())
        if (!isValidToken())
            getNewAccessToken();
    return isLinked();
}

void OAuth::setExpiresIn(const int &value)
{
    m_expires_in = value;
    setExpireDateTime(QDateTime::currentMSecsSinceEpoch() / 1000 + value);
}

void OAuth::unlink()
{
    settingsSaveAccessToken("");
    settingsSaveRefreshToken("");
    settingsSaveUsername("");
    settingsSaveExpiresDateTime(0);
    loadFromSettings(); // to reload all

    setIsLinked(false);
    setAccountUsername("");
}

void OAuth::loadFromSettings()
{
    setExpireDateTime(settingsLoadExpiresDateTime());
    setRefreshToken(settingsLoadRefreshToken());
    setAccessToken(settingsLoadAccessToken());
    setAccountUsername(settingsLoadUsername());
    setIsLinked(refreshToken().length() > 0);
    setCode(settingsLoadCode());
}

void OAuth::settingsSaveAccessToken(QString value)
{
    saveValueFor(QString(OAUTH_SETTINGS_PREFIX) + QString(OAUTH_ACCESS_TOKEN), value);
}

void OAuth::settingsSaveRefreshToken(QString value)
{
    saveValueFor(QString(OAUTH_SETTINGS_PREFIX) + QString(OAUTH_REFRESH_TOKEN), value);
}

void OAuth::setState(const QString& value)
{
    // Generate with https://strongpasswordgenerator.com/
    m_state = value.toAscii();
}

void OAuth::settingsSaveUsername(QString value)
{
    saveValueFor(QString(OAUTH_SETTINGS_PREFIX) + QString(OAUTH_ACCOUNT_USERNAME), value);
}

uint OAuth::settingsLoadExpiresDateTime()
{
    return getValueFor(QString(OAUTH_SETTINGS_PREFIX) + QString(OAUTH_EXPIRES_DATETIME), 0).toUInt();
}

void OAuth::setClientId(const QString &value)
{
    m_client_id = value;
}

QString OAuth::clientId()
{
    return m_client_id;
}

void OAuth::setRedirectURI(const QString &value)
{
    m_redirect_uri = value;
}

QString OAuth::redirectURI()
{
    return m_redirect_uri;
}

void OAuth::setClientSecret(const QString &value)
{
    m_client_secret = value;
}

QString OAuth::clientSecret()
{
    return m_client_secret;
}
void OAuth::setAuthorizeUrl(const QString &value)
{
    m_authorize_url = value;
}

QString OAuth::authorizeUrl()
{
    return m_authorize_url;
}
void OAuth::setRefreshUrl(const QString &value)
{
    m_refresh_url = value;
}

QString OAuth::refreshUrl()
{
    return m_refresh_url;
}

void OAuth::setAccessToken(const QString &value)
{
    m_access_token = value;
}
QString OAuth::accessToken()
{
    return m_access_token;
}

void OAuth::setTokenType(const QString &value)
{
    m_token_type = value;
}

QString OAuth::tokenType()
{
    return m_token_type;
}

void OAuth::setRefreshToken(const QString &value)
{
    m_refresh_token = value;
}

QString OAuth::refreshToken()
{
    return m_refresh_token;
}

void OAuth::setAccountUsername(const QString &value)
{
    m_account_username = value;
    emit accountUsernameChanged(value);
}

QString OAuth::accountUsername()
{
    return m_account_username;
}

int OAuth::expiresIn()
{
    return m_expires_in;
}

void OAuth::setExpireDateTime(const uint &value)
{
    m_expire_datetime = value;
}

uint OAuth::expireDateTime()
{
    return m_expire_datetime;
}

QString OAuth::settingsLoadRefreshToken()
{
    return getValueFor(QString(OAUTH_SETTINGS_PREFIX) + QString(OAUTH_REFRESH_TOKEN), "");
}

QString OAuth::settingsLoadAccessToken()
{
    return getValueFor(QString(OAUTH_SETTINGS_PREFIX) + QString(OAUTH_ACCESS_TOKEN), "");
}
QString OAuth::settingsLoadUsername()
{
    return getValueFor(QString(OAUTH_SETTINGS_PREFIX) + QString(OAUTH_ACCOUNT_USERNAME), "");
}

bool OAuth::isRunning()
{
    return m_running;
}

bool OAuth::isLinked()
{
    return m_isLinked;
}

void OAuth::settingsSaveExpiresDateTime(uint value)
{
    saveValueFor(QString(OAUTH_SETTINGS_PREFIX) + QString(OAUTH_EXPIRES_DATETIME), QString::number(value));

}

void OAuth::settingsSaveCode(QString value)
{
    saveValueFor(QString(OAUTH_SETTINGS_PREFIX) + QString(OAUTH_CODE), value);
}

QString OAuth::settingsLoadCode()
{
    return getValueFor(QString(OAUTH_SETTINGS_PREFIX) + QString(OAUTH_CODE), "");
}

// standard settings access
QString OAuth::getValueFor(const QString &objectName, const QString &defaultValue)
{
    QSettings settings;

    // If no value has been saved, return the default value.
    if (settings.value(objectName).isNull()) {
        return defaultValue;
    }

    // Otherwise, return the value stored in the settings object.
    return settings.value(objectName).toString();
}

void OAuth::saveValueFor(const QString &objectName, const QString &inputValue)
{
    // A new value is saved to the application settings object.
    QSettings settings;
    settings.setValue(objectName, QVariant(inputValue));
}

void OAuth::setResponseType(const ResponseType& value)
{
    m_response_type = value;
}

QString OAuth::responseType()
{
    if (m_response_type == CODE)
        return "code";
    else
        return "token";
}

void OAuth::setIsLinked(const bool& linked)
{
    m_isLinked = linked;
    emit isLinkedChanged(linked);
}

