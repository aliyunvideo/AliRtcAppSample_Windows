#pragma once

using namespace std;

#define CURL_STATICLIB

#include <iostream>
#include <sstream>
#include <thread>
#include "AliRtcEngine.h"
#include "curl\curl.h"
#include "jsoncpp\json.h"

#pragma comment(lib, "jsoncpp.lib")
#pragma comment(lib, "libcurl.lib")
#pragma comment(lib, "libeay32.lib")
#pragma comment(lib, "ssleay32.lib")
#pragma comment(lib, "Wldap32.lib")


// Default Test data for testing authorization of fake App server
char* pstrAppServer = "http://127.0.0.1:8080/app/v1/";
char* pstrUserPW = "hello1234";   //

                                       
                                        
size_t httpResponseCallback(void *buffer, size_t size, size_t nmemb, std::string& user_p)
{
    user_p = (char*)buffer;
    return size * nmemb;
}

/*
connect App Server
fun:ConnectToAppServer
*/
void ConnectToAppServer(
    const string& rtc_server,
    const string& room,
    const string& username,
    const string& passwd,
    AliRtcAuthInfo* auth_info) {

    CURLcode return_code;
    return_code = curl_global_init(CURL_GLOBAL_ALL);
    if (CURLE_OK != return_code)
    {
        curl_global_cleanup();
        return;
    }

    // init curl
    CURL *curl = curl_easy_init();

    if (NULL == curl)
    {
        curl_easy_cleanup(curl);
        curl_global_cleanup();
        return;
    }
    // res code
    CURLcode res = CURLE_OBSOLETE32;

    if (curl)
    {
        curl_slist *plist = curl_slist_append(NULL, "Content-Type: application/json;charset=UTF-8");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, plist);

        std::ostringstream oss;
        // normal
        oss << rtc_server << "login?passwd=" << passwd << "&user=" << username << "&room=" << room;//app/v1

        std::string url = oss.str();
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str()); // url

        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
        // if want to use https
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L); // set peer and host verify false
        curl_easy_setopt(curl, CURLOPT_VERBOSE, 1);
        curl_easy_setopt(curl, CURLOPT_READFUNCTION, NULL);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, httpResponseCallback);
        std::string connectx;

        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &connectx);
        curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1);
        curl_easy_setopt(curl, CURLOPT_HEADER, 0);
        curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 3);
        curl_easy_setopt(curl, CURLOPT_TIMEOUT_MS, 1000);

        // start req
        res = curl_easy_perform(curl);

        int retcode = 0;
        return_code = curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &retcode);

        if (res == CURLE_OK && CURLE_OK == return_code && retcode == 200) {

            Json::Value jsonData;
            Json::Reader jsonReader;

            if (jsonReader.parse(connectx, jsonData)) {

                auth_info->channel = room.c_str();

                std::string appid(jsonData["data"]["appid"].asString());
                auth_info->appid = appid.c_str();

                std::string userid(jsonData["data"]["userid"].asString());
                auth_info->user_id = userid.c_str();

                std::string nonce(jsonData["data"]["nonce"].asString());
                auth_info->nonce = nonce.c_str();

                std::string ts = jsonData["data"]["timestamp"].asString();

                std::string::size_type sz = 0;

                auth_info->timestamp = std::stoull(ts, &sz, 10);

                std::string token(jsonData["data"]["token"].asString());
                auth_info->token = token.c_str();

                std::string session(jsonData["data"]["session"].asString());

                Json::Value gslb_list = jsonData["data"]["gslb"];

                for (int i = 0; i < (int)gslb_list.size(); i++)
                {
                    auth_info->gslb.AddString(gslb_list[i].asCString());
                }
            }
        }
        curl_slist_free_all(plist);
        curl_easy_cleanup(curl);
        curl_global_cleanup();
    }
}

/*
fun:  GetPassportFromAppServer
You can change the App Server address and channel from the ".\\RtcCfg.ini" file.
*/
bool GetPassportFromAppServer(AliRtcAuthInfo &authinfo)
{
    if (authinfo.channel.size() == 0) {
        return false;
    }
    char strUserName[64] = { 0 };
    
    //get random user name
    srand((unsigned)time(NULL));
    int userNum = rand();
    std::stringstream ss;
    ss << userNum;

    bool success = false;
    int i = 5;
    while (i > 0)
    {
        //std::string user = randomString((uint64_t)this, 6, "");
        ConnectToAppServer(
            pstrAppServer, 
            authinfo.channel.c_str(),
            "user",
            pstrUserPW, &authinfo);
        if (authinfo.appid == "")
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            i--;
        }
        else
        {
            success = true;
            break;
        }
    }
    return success;
}
