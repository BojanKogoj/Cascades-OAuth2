/*
 * Copyright (c) 2011-2015 BlackBerry Limited.
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 * http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

import bb.cascades 1.3

Page {
    Container {
        Label {
            text: "Status"
            textStyle.fontSize: FontSize.XLarge
        }
        Label {
            text: oauth.isLinked ? "Linked: " + oauth.accountUsername : "not linked"
        }
        Button {
            visible: !oauth.isLinked
            text: "Log in"
            onClicked: {
                console.log(oauth.isLinked)
                loginSheet.open();
            }
        }
        Button {
            visible: oauth.isLinked
            text: "Log out"
            onClicked: {
                oauth.unlink();
                console.log(oauth.isLinked)
            }
        }
        Button {
            text: "Print access token"
            onClicked: {
                console.log("[main.onClicked] Access token: " + oauth.accessToken());
            }
        }
        Button {
            text: "Request new access token"
            onClicked: {
                /* OAuth will display network reply. In real life usage, you need to connect
                 accessTokenRefreshed() to onTokenRefreshed(), to be notified when new
                 token is received
                */
                oauth.getNewAccessToken();
            }
        }
        Button {
            text: "Is token valid?"
            onClicked: {
                console.log("Token valid: " + oauth.isValidToken());
            }
        }
    }
    attachedObjects: [
        Delegate {
            id: delegatedLogin
            OAuthLoginSheet {
                
            }
        },
        Sheet {
            id: loginSheet
            // Using delegate to reduce load at startup
            content: delegatedLogin.object
            onOpenedChanged: {
                if (opened)
                    delegatedLogin.active = true;
            }
            onClosed: {
                delegatedLogin.active = false;
            }
        }
    ]
}
