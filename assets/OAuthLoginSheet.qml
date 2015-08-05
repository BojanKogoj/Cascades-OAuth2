import bb.cascades 1.3

Page {
    titleBar: TitleBar {
        dismissAction: ActionItem {
            title: "Close"
            onTriggered: {
                loginSheet.close()
            }
        }
    }
    Container {
        ScrollView {
            horizontalAlignment: HorizontalAlignment.Fill
            verticalAlignment: VerticalAlignment.Fill
            WebView {
                id: oAuthWebView
                horizontalAlignment: HorizontalAlignment.Fill
                verticalAlignment: VerticalAlignment.Fill

                
                onUrlChanged: {
                    console.log("[OAuthSheet.onUrlChanged] Url changed to: " + url)
                    oauth.authorizationReceived(url);
                }

            }
        }

    }
    function onOpenBrowser(url) {
        oAuthWebView.url = url;
        console.log("[OAuthLoginSheet.onOpenBrowser] Website URL received!")
    }
    function onCloseBrowser(){
        loginSheet.close();
    }
    onCreationCompleted: {
        oauth.closeBrowser.connect(onCloseBrowser);
        oauth.openBrowser.connect(onOpenBrowser);
        oauth.link();

    }

}
