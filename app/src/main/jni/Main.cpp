#include <list>
#include <vector>
#include <string.h>
#include <pthread.h>
#include <cstring>
#include <jni.h>
#include <unistd.h>
#include <fstream>
#include <iostream>
#include <dlfcn.h>
#include <SOCKET/client.h>
#include <Unity/ESP.h>
#include "Includes/Logger.h"
#include "Includes/obfuscate.h"
#include "Includes/Utils.h"
#include "Unity/Quaternion.hpp"
#include "Unity/Vector2.hpp"
#include "Unity/Vector3.hpp"
#include "Unity/Unity.h"
//#include <Substrate/SubstrateHook.h>
//#include "KittyMemory/MemoryPatch.h"
#include "Menu.h"

//#include "Includes/Macros.h"

ESP espOverlay;
SocketClient client;

void startDaemon();
int startClient();
bool isConnected();
void stopClient();
bool initServer();
bool stopServer();


enum Mode {
    InitMode = 1,
    HackMode = 2,
    StopMode = 3,
    EspMode = 99,
};
enum m_Features {
    g_send_enable = 99,
    g_wall_hack_car = 16,
    g_telecar = 15,
    g_fly_spreed = 14,
    g_alture = 13,
    g_enable_mod = 12,
    g_aimFov = 10,
    g_ignorInVisible = 11,
    g_aimFire = 4,
    g_aimScope = 5,
    g_aimCrouch = 6,
    g_aimEspFire = 7,
    g_enable_hook = 8,
    g_spreed_hack = 17,
    g_TriggerBot = 18,
};

struct Request {
    int Mode;
    bool m_IsOn;
    int value;
    int screenWidth;
    int screenHeight;
};

#define maxplayerCount 54


struct PlayerData {
    char PlayerName[64];
    float Health;
    float Distance;
    bool get_IsDieing;
    bool isBot;
    Vector2 HeadV2;
    Vector3 HeadLocation;
    Vector3 ToeLocation;
    Vector3 HipLocation;
    Vector3 RShoulder;
    Vector3 LShoulder;
    Vector3 DedoSLocation;
    Vector3 HandLocation;
    Vector3 PeSLocation;
    Vector3 PeDLocation;
    Vector3 LocalPlayerHead;
    Vector3 CloseEnemyHeadLocation;
    int x;
    int y;
    int z;
    int id;
    int h;
    char debug[60];
};

struct Response {
    bool Success;
    int PlayerCount;
    PlayerData Players[maxplayerCount];
};


int startClient(){
    client = SocketClient();
    if(!client.Create()){ return -1; }
    if(!client.Connect()){ return -1; }
    if(!initServer()){ return -1; }
    return 0;
}

bool isConnected(){
    return client.connected;
}

void stopClient() {
    if(client.created && isConnected()){
        stopServer();
        client.Close();
    }
}

bool initServer() {
    Request request{Mode::InitMode, true, 0};
    int code = client.sendX((void*) &request, sizeof(request));
    if(code > 0) {
        Response response{};
        size_t length = client.receive((void*) &response);
        if(length > 0) {
            return response.Success;
        }
    }
    return false;
}

bool stopServer() {
    Request request{Mode::StopMode};
    int code = client.sendX((void*) &request, sizeof(request));
    if(code > 0) {
        Response response{};
        size_t length = client.receive((void*) &response);
        if(length > 0) {
            return response.Success;
        }
    }
    return false;
}

void SendFeatuere(int number, bool ftr, int value) {
    Request request{number, ftr, value};
    int code = client.sendX((void*) &request, sizeof(request));
    if (code > 0) {
        Response response{};
        size_t length = client.receive((void*) &response);
        if (length > 0) {}
    }
}


Response getData(int screenWidth, int screenHeight){
    Request request{Mode::EspMode, screenWidth, screenHeight};
    int code = client.sendX((void*) &request, sizeof(request));
    if(code > 0){
        Response response{};
        size_t length = client.receive((void*) &response);
        if(length > 0){
            return response;
        }
    }
    Response response{false, 0};
    return response;
}




bool feature1, feature2, featureHookToggle, Health;
int sliderValue = 1, level = 0;
void *instanceBtn;
bool XAimKill = false;
float Fov_Aim = 0.0f;
bool aimFire = false;
bool aimScope= false;
bool aimCrouch = false;
bool EspFire = false;
bool EspLine = false;
bool EspBox = false;
bool enable_mod = false;


//JNI calls
extern "C" {

JNIEXPORT jobjectArray
JNICALL
Java_uk_lgl_modmenu_FloatingModMenuService_getFeatureList(JNIEnv *env, jobject context) {
    jobjectArray ret;
    //Toasts added here so it's harder to remove it
    MakeToast(env, context, OBFUSCATE("Modded by LGL"), Toast::LENGTH_LONG);

    const char *status =  isConnected() ? "Category_STATUS: CONNECTED" : "Category_STATUS: OFFLINE";
    const char *features[] = {
            status,
            OBFUSCATE("Category_The MOD MENU"), //Not counted
            OBFUSCATE("InjectBTN"), //Not counted
            OBFUSCATE("15_Toggle_ESP LINE"),
    };
    int Total_Feature = (sizeof features / sizeof features[0]);
    ret = (jobjectArray)
            env->NewObjectArray(Total_Feature, env->FindClass(OBFUSCATE("java/lang/String")),
                                env->NewStringUTF(""));
    for (int i = 0; i < Total_Feature; i++)
        env->SetObjectArrayElement(ret, i, env->NewStringUTF(features[i]));
    pthread_t ptid;
    pthread_create(&ptid, NULL, antiLeech, NULL);

    return (ret);
}

JNIEXPORT void JNICALL
        Java_uk_lgl_modmenu_Preferences_Changes(JNIEnv *env, jclass clazz, jobject obj,
                                        jint featNum, jstring featName, jint value,
                                        jboolean boolean, jstring str) {

    LOGD(OBFUSCATE("Feature name: %d - %s | Value: = %d | Bool: = %d | Text: = %s"), featNum,
         env->GetStringUTFChars(featName, 0), value,
         boolean, str != NULL ? env->GetStringUTFChars(str, 0) : "");

    // BE CAREFUL NOT TO ACCIDENTLY REMOVE break;

    switch (featNum) {
        case 10:
            Fov_Aim = (float)value;
            if(value == 0) {
                Fov_Aim = 0.0f;
            }
            break;
        case 5: XAimKill = !XAimKill; break;
        case 11: aimFire = !aimFire;  break;
        case 12: aimScope= !aimScope;  break;
        case 13: aimCrouch = !aimCrouch;  break;
        case 14: EspFire = !EspFire;  break;
        case 15: EspLine = !EspLine;  break;
        case 16: EspBox = !EspBox;  break;

        }
    }
}

bool isValidPlayer(PlayerData data) {
    return (data.HeadLocation != Vector3::Zero()); // || data.PlayerName.size() > 0
}
bool isValidCloseEnemy(PlayerData data) {
    return (data.CloseEnemyHeadLocation != Vector3::Zero());
}

void DrawESP(ESP esp, int screenWidth, int screenHeight) {
    float mScale = (float) screenHeight / (float) 1080;
    esp.DrawText(Color::Green(), 0.6f, OBFUSCATE("L3DEVIL") ,
                 Vector3((float)screenWidth / 2, (float)screenHeight / 1.03f + 10.0f),  30.0f);

    if (enable_mod) {
        if (isConnected()) {
            Response response = getData(screenWidth, screenHeight);
            if(response.Success) {
                int count = response.PlayerCount;
                if(count > 0) {
                    PlayerData localPlayer = response.Players[0];
                    for(int i = 0; i < count; i++) {
                        PlayerData player = response.Players[i];

                        if(!isValidPlayer(player)){ continue; }
                        Vector3 Head = player.HeadLocation;

                        char bx[20];
                        Vector3 End = Head;
                        Vector3 End2 = player.ToeLocation;
                        auto boxWidth = static_cast<float>(((player.RShoulder.X * 0.995) - (player.LShoulder.X * 1.005)) *1.5);
                        float Tamanho = 0.0f;
                        float Distance2 = player.Distance;
                        if (Distance2 > 10.0f) {
                            Tamanho = 10.0f;
                        } else if (Distance2 > 20.0f) {
                            Tamanho = 0.0f;
                        }

                        if (EspLine) {
                            esp.DrawLine(player.get_IsDieing ? Color::Red() : Color::Green(),
                                         0.9f, Vector3(((float)screenWidth / 2), 0),
                                         Vector3(((float)screenWidth - ((float)screenWidth - Head.X)),
                                                 ((float)screenHeight - Head.Y - 8.0f)));
                        }
                    }
                }
            }
        }
    }
}


extern "C"
JNIEXPORT void JNICALL
Java_uk_lgl_modmenu_FloatingModMenuService_DrawOn(JNIEnv *env, jclass type, jobject espView, jobject canvas) {
    espOverlay = ESP(env, espView, canvas);
    if (espOverlay.isValid()){
        DrawESP(espOverlay, espOverlay.getWidth(), espOverlay.getHeight());
    }
}

extern "C"
JNIEXPORT void JNICALL
Java_uk_lgl_modmenu_FloatingModMenuService_Init(JNIEnv *env, jobject thiz) {
    MakeToast(env, thiz, startClient() ? "CONNECTED" : "FAILED", Toast::LENGTH_LONG);
}

extern "C"
JNIEXPORT void JNICALL
    Java_uk_lgl_modmenu_FloatingModMenuService_Stop(JNIEnv *env, jobject type) {
    stopClient();
}