//
// Created by thunder on 16/03/22.
//

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
#include "Includes/Logger.h"
#include "Includes/obfuscate.h"
#include "Includes/Utils.h"
#include "Unity/Quaternion.hpp"
#include "Unity/Vector2.hpp"
#include "Unity/Vector3.hpp"
#include "Unity/Unity.h"
#include <Substrate/SubstrateHook.h>
#include "KittyMemory/MemoryPatch.h"
#include "Menu.h"
#include "TmodzH.h"
# define HOOK(offset, ptr, orig) MSHookFunction((void *)getRealOffset(offset), (void *)ptr, (void **)&orig)
//Target lib here
#define targetLibName OBFUSCATE("libil2cpp.so")

//#include "Includes/Macros.h"
#include <SOCKET/server.h>

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


bool FOV = true;
bool TelekillPlayer = false;

static Vector2 WorldLocToScreenPoint(void *WorldCam, Vector3 WorldPos) {
    Vector2 (*_WorldToScreenScene)(void* Camera, Vector3 position) = (Vector2 (*)(void*, Vector3)) getRealOffset(0x3371FB8); // TODO: Change 1.68 // 0x3371FB8
    return _WorldToScreenScene(WorldCam,WorldPos);
}
static void *get_MyPhsXData(void *player) {
    void *(*_get_MyPhsXData)(void *component) = (void *(*)(void *))getRealOffset(0xB5EE70); //get_MyPhsXData() { }
    return _get_MyPhsXData(player);
}
static bool IsCatapultFalling(void *player) {
    bool (*_IsCatapultFalling)(void *players) = (bool (*)(void *))getRealOffset(0xB584E8);
    return _IsCatapultFalling(player);
}
static void OnStopCatapultFalling(void *player) {
    void (*_OnStopCatapultFalling)(void *players) = (void (*)(void *))getRealOffset(0xBC17B4);
    return _OnStopCatapultFalling(player);
}
Vector3 GetHeadPosition(void* player) {
    return Transform_INTERNAL_GetPosition(*(void**) ((uint64_t) player + L3DEVIL.HeadTF));
}
Vector3 GetHipPosition(void* player) {
    return Transform_INTERNAL_GetPosition(*(void**) ((uint64_t) player + L3DEVIL.HipTF));
}
Vector3 GetToePosition(void* player) {
    return Transform_INTERNAL_GetPosition(*(void**) ((uint64_t) player + L3DEVIL.ToeTF));
}
Vector3 GetRShoulderPosition(void* player){
    return Transform_INTERNAL_GetPosition(*(void**) ((uint64_t) player + L3DEVIL.RShoulder));
}
Vector3 GetLShoulderPosition(void* player){
    return Transform_INTERNAL_GetPosition(*(void**) ((uint64_t) player + L3DEVIL.LShoulder));
}
Vector3 CameraMain(void* player){
    return Transform_INTERNAL_GetPosition(*(void**) ((uint64_t) player + L3DEVIL.MainCameraTransform));
}
Vector3 GetHandPosition(void* player) {
    return Transform_INTERNAL_GetPosition(*(void**) ((uint64_t) player + L3DEVIL.HandTFF));
}
Vector3 GetPeSPosition(void* player) {
    return Transform_INTERNAL_GetPosition(*(void**) ((uint64_t) player + L3DEVIL.PeS));
}
Vector3 GetPeDPosition(void* player) {
    return Transform_INTERNAL_GetPosition(*(void**) ((uint64_t) player + L3DEVIL.PeD));
}
Vector3 GetDedoSPosition(void* player) {
    return Transform_INTERNAL_GetPosition(*(void**) ((uint64_t) player + L3DEVIL.DedoTF));
}

void *get_L3DEVIL = nullptr;
void *get_L3DEVIL_E(void *L3DEVIl) {
    get_L3DEVIL = L3DEVIl;
    return get_L3DEVIL;
}

void (*Update)(void* gamestartup);
void _Update(void* gamestartup) {
    get_L3DEVIL_E(gamestartup);
    Update(gamestartup);
}

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

SocketServer server;

int InitServer() {
    if (!server.Create()) { return -1; }
    if (!server.Bind()) { return -1; }
    if (!server.Listen()) { return -1; }
    return 0;
}

void createDataList(Response& response) {
    response.PlayerCount = 0;
    float shortestDistance = 99999.0f;
    void* L3DEVIl = get_L3DEVIL;
    if (L3DEVIl != nullptr) {
        void *current_Match = Curent_Match();
        void* local_player = GetLocalPlayer(current_Match);
        if (local_player != nullptr && current_Match != nullptr) {
            monoDictionary<uint8_t *, void **> *players = *(monoDictionary<uint8_t*, void **> **)((long)current_Match + L3DEVIL.Dictionary);
            void *camera = Camera_main();
            if (players != nullptr && camera != nullptr) {
                for(int u = 1; u < players->getNumValues(); u++) {
                    PlayerData* data = &response.Players[response.PlayerCount];
                    void* closestEnemy = players->getValues()[u];
                    if (closestEnemy != local_player && closestEnemy != nullptr && get_isVisible(closestEnemy) && !get_isLocalTeam(closestEnemy)) {
                        Vector3 LocalPlayerHead = WorldToScreenPoint(camera, GetHipPosition(local_player));
                        Vector3 RShoulder = WorldToScreenPoint(camera,GetRShoulderPosition(closestEnemy));
                        Vector3 LShoulder = WorldToScreenPoint(camera,GetLShoulderPosition(closestEnemy));
                        Vector3 Toe = WorldToScreenPoint(camera, GetToePosition(closestEnemy));
                        Vector3 Hip = WorldToScreenPoint(camera, GetHipPosition(closestEnemy));
                        Vector3 Head = WorldToScreenPoint(camera,GetHeadPosition(closestEnemy));
                        Vector3 Hand = WorldToScreenPoint(camera,GetHandPosition(closestEnemy));
                        Vector3 PeS = WorldToScreenPoint(camera,GetPeSPosition(closestEnemy));
                        Vector3 PeD = WorldToScreenPoint(camera,GetPeDPosition(closestEnemy));
                        Vector3 DedoS = WorldToScreenPoint(camera,GetDedoSPosition(closestEnemy));

                        if (RShoulder.Z < -0) continue;
                        if (LShoulder.Z < -0) continue;
                        if (Toe.Z < -0) continue;
                        if (Hip.Z < -0) continue;
                        if (Head.Z < -0) continue;
                        if (Hand.Z < -0) continue;
                        if (PeS.Z < -0) continue;
                        if (PeD.Z < -0) continue;
                        if (DedoS.Z < -0) continue;

                        data->LocalPlayerHead = LocalPlayerHead;
                        data->HeadLocation = Head;
                        data->ToeLocation = Toe;

                        response.PlayerCount++;
                        if (response.PlayerCount == maxplayerCount) {
                            continue;
                        }
                    }
                }
            }
        }
    }
}

const char* name = "libil2cpp.so";
void* hack_thread (void *) {
    ProcMap il2cppMap;
    do {
        il2cppMap = KittyMemory::getLibraryMap(name);
        sleep(1);
    } while (!il2cppMap.isValid());

    LOGI("%s has been loaded", (const char *) name);
    HOOK(0xB8AFA8, _Update, Update); // public void OnGodStateCome(bool GHDIEEIHBCK, float OCKCJAFOINK) { }
}

void *CreateServer(void *) {
    if (InitServer() == 0) {
        if (server.Accept()) {
            Request request{};
            while (server.receive((void*)&request) > 0) {
                Response response{};
                if (request.Mode == Mode::InitMode) {
                    response.Success = true;
                } else if (request.Mode == Mode::EspMode) {
                    createDataList(response);
                    response.Success = true;
                }
                server.sendX((void*)& response, sizeof(response));
            }
        }
    }
    return nullptr;
}

 bool launched = false;
extern "C"
__attribute__((visibility("default")))
void OnInject(char *) {
    if (!launched) {
        launched = true;
        pthread_t ptid;
        pthread_create(&ptid, nullptr, hack_thread, nullptr);
    }
}

__attribute__((constructor))
void OnCreate() {
    if (!launched) {
        launched = true;
        pthread_t ptid;
        pthread_create(&ptid, nullptr, hack_thread, nullptr);
    }
}

__attribute__((constructor))
void initializer() {
    pthread_t ptid;
    pthread_create(&ptid, nullptr, CreateServer, nullptr);
}


