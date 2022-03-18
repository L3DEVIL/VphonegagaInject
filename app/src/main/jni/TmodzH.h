#ifndef L3DEVLINJECTOR_TMODZH_H
#define L3DEVLINJECTOR_TMODZH_H
#include "TmodzG.h"
#include <map>
# define getRealOffset(offset) AgetAbsoluteAddress("libil2cpp.so",offset)

//uintptr_t g_il2cpp;
//// ================================================================================================================================ //
//std::map<std::string, uintptr_t> Fields;
//std::map<std::string, uintptr_t> Methods;

long AfindLibrary(const char *library) {
    char filename[0xFF] = {0},
            buffer[1024] = {0};
    FILE *fp = nullptr;
    long address = 0;

    sprintf(filename, "/proc/self/maps");

    fp = fopen( filename, "rt" );
    if( fp == nullptr ){
        perror("fopen");
        goto done;
    }

    while( fgets( buffer, sizeof(buffer), fp ) ) {
        if(strstr( buffer, library ) ){
            address = (long)strtoul( buffer, NULL, 16 );
            goto done;
        }
    }

    done:

    if(fp){
        fclose(fp);
    }
    return address;
}

long ClibBase;

long AgetAbsoluteAddress(const char* libraryName, long relativeAddr) {
    if (ClibBase == 0) {
        ClibBase = AfindLibrary(libraryName);
        if (ClibBase == 0) {
            ClibBase = 0;
        }
    }
    return ClibBase + relativeAddr;
}

class Vvector3 {
public:
    float X;
    float Y;
    float Z;
    Vvector3() : X(0), Y(0), Z(0) {}
    Vvector3(float x1, float y1, float z1) : X(x1), Y(y1), Z(z1) {}
    Vvector3(const Vvector3 &v);
    ~Vvector3();
};
Vvector3::Vvector3(const Vvector3 &v) : X(v.X), Y(v.Y), Z(v.Z) {}
Vvector3::~Vvector3() {}



static void *GetLocalPlayer(void* Match) {
    void *(*_GetLocalPlayer)(void *match) = (void *(*)(void *))getRealOffset(L3DEVIL.GetLocalPlayer);
    return _GetLocalPlayer(Match);
}

static void *VehicleIAmIn(void *Vehicle) {
    void *(*_VehicleIAmIn)(void *Vehicles) = (void *(*)(void *))getRealOffset(L3DEVIL.GetLocalCar);
    return _VehicleIAmIn(Vehicle);
}

static Vector3 GetAttackableCenterWS(void *player) {
    Vector3 (*_GetAttackableCenterWS)(void *players) = (Vector3 (*)(void *))getRealOffset(L3DEVIL.GetAttackableCenterWS);
    return _GetAttackableCenterWS(player);
}

static bool get_isLocalTeam(void *player) {
    bool (*_get_isLocalTeam)(void *players) = (bool (*)(void *))getRealOffset(L3DEVIL.get_isLocalTeam);
    return _get_isLocalTeam(player);
}


static bool get_IsDieing(void *player) {
    bool (*_get_die)(void *players) = (bool (*)(void *))getRealOffset(L3DEVIL.get_IsDieing);
    return _get_die(player);
}

static bool IsClientBot(void *player) {
    bool (*_IsClientBot)(void *players) = (bool (*)(void *))getRealOffset(L3DEVIL.IsClientBot);
    return _IsClientBot(player);
}

static int get_MaxHP(void* enemy) {
    int (*_get_MaxHP)(void* player) = (int(*)(void *))getRealOffset(L3DEVIL.get_MaxHP);
    return _get_MaxHP(enemy);
}

static bool get_IsFiring(void *player) {
    bool (*_get_IsFiring)(void *players) = (bool (*)(void *))getRealOffset(L3DEVIL.get_IsFiring);
    return _get_IsFiring(player);
}

static bool get_IsSighting(void *player) {
    bool (*_get_IsSighting)(void *players) = (bool (*)(void *))getRealOffset(L3DEVIL.get_IsSighting);
    return _get_IsSighting(player);
}

static bool get_isVisible(void *player) {
    bool (*_get_isVisible)(void *players) = (bool (*)(void *))getRealOffset(L3DEVIL.get_isVisible);
    return _get_isVisible(player);
}

static Vector3 GetForward(void *player) {
    Vector3 (*_GetForward)(void *players) = (Vector3 (*)(void *))getRealOffset(L3DEVIL.GetForward);
    return _GetForward(player);
}

static void set_aim(void *player, Quaternion look) {
    void (*_set_aim)(void *players, Quaternion lock) = (void (*)(void *, Quaternion))getRealOffset(L3DEVIL.set_aim);
    _set_aim(player, look);
}

static Vector3 Transform_INTERNAL_GetPosition(void *player) {
    Vector3 out = Vector3::Zero();
    void (*_Transform_INTERNAL_GetPosition)(void *transform, Vector3 * out) = (void (*)(void *, Vector3 *))getRealOffset(L3DEVIL.Transform_INTERNAL_GetPosition);
//    void (*_Transform_INTERNAL_GetPosition)(void *transform, Vector3 * out) = (void (*)(void *, Vector3 *))(Methods["Transform::get_position"]);
    _Transform_INTERNAL_GetPosition(player, &out);
    return out;
}

static void Transform_INTERNAL_SetPosition(void *player, Vvector3 inn) {
    void (*Transform_INTERNAL_SetPosition)(void *transform, Vvector3 in) = (void (*)(void *, Vvector3))getRealOffset(L3DEVIL.Transform_INTERNAL_SetPosition);
//    void (*Transform_INTERNAL_SetPosition)(void *transform, Vvector3 in) = (void (*)(void *, Vvector3))(Methods["Transform::set_position"]);;
    Transform_INTERNAL_SetPosition(player, inn);
}

static void *Component_GetTransform(void *player) {
    void *(*_Component_GetTransform)(void *component) = (void *(*)(void *))getRealOffset(L3DEVIL.Component_GetTransform);
//    void *(*_Component_GetTransform)(void *component) = (void *(*)(void *))(Methods["Component::get_transform"]);
    return _Component_GetTransform(player);
}

static void *Camera_main() {
    void *(*_Camera_main)(void *nuls) = (void *(*)(void *))getRealOffset(L3DEVIL.Camera_main);
//    void *(*_Camera_main)(void *nuls) = (void *(*)(void *))(Methods["Camera::get_main"]);
    return _Camera_main(nullptr);
}

static void *Curent_Match() {
    void *(*_Curent_Match) (void *nuls) = (void *(*)(void *))getRealOffset(L3DEVIL.Curent_Match);
    return _Curent_Match(NULL);
}

static int GetHp(void* player) {
    int (*_GetHp)(void* players) = (int(*)(void *))getRealOffset(L3DEVIL.get_CurHp);
    return _GetHp(player);
}

char get_Chars(monoString *str, int index){
    char (*_get_Chars)(monoString *str, int index) = (char (*)(monoString *, int))getRealOffset(L3DEVIL.get_Chars);
    return _get_Chars(str, index);
}

static monoString* get_NickName(void *player) {
    monoString* (*_get_NickName)(void *players) = (monoString * (*)(void *))getRealOffset(L3DEVIL.get_NickName);
    return _get_NickName(player);
}

static Vector3 WorldToScreenPoint(void *WorldCam, Vector3 WorldPos) {
    Vector3 (*_WorldToScreenScene)(void* Camera, Vector3 position) = (Vector3 (*)(void*, Vector3)) getRealOffset(L3DEVIL.WorldToScreenPoint);
//    Vector3 (*_WorldToScreenScene)(void* Camera, Vector3 position) = (Vector3 (*)(void*, Vector3)) (Methods["Camera::WorldToScreenPoint"]);
    return _WorldToScreenScene(WorldCam,WorldPos);
}



/** ADDED BY L3DEVIL **/

static void DrawLine_Cristiano(void *instance, Vector3 start, Vector3 end, Vector3 position) {
    void (*_DrawLine_Cristiano)(void *clz, Vector3 throwPos, Vector3 throwVel, Vector3 gravity) = (void (*)(void*, Vector3, Vector3,Vector3)) getRealOffset(0xFD58E8);
    return _DrawLine_Cristiano(instance, start, end, position);
}

static void *get_imo(void *player) {
    void *(*_get_imo)(void *players) = (void *(*)(void *))getRealOffset(L3DEVIL.get_imo);
    return _get_imo(player);
}
static void set_esp(void *imo, Vector3 x, Vector3 y) {
    void (*_set_esp)(void *imo, Vector3 X, Vector3 Y) = (void (*)(void *, Vector3, Vector3))getRealOffset(L3DEVIL.set_esp);
    _set_esp(imo, x, y);
}

static bool get_IsCrouching(void *player) {
    bool (*_get_IsCrouching)(void *players) = (bool (*)(void *))getRealOffset(L3DEVIL.get_IsCrouching);
    return _get_IsCrouching(player);
}

static void *CurrentUIScene() {
    void *(*_CurrentUIScene)(void *nuls) = (void *(*)(void *))getRealOffset(L3DEVIL.CurrentUIScene);
    return _CurrentUIScene(NULL);
}

static void ShowAssistantText(void *player, monoString *nick, monoString *grup) {
    void (*_ShowAssistantText)(void *players, monoString * nicks, monoString * nickss) = (void (*)(void *, monoString *, monoString *))getRealOffset(L3DEVIL.ShowPopupMessage);
    void *ui = CurrentUIScene();
    if (ui != NULL) {
        _ShowAssistantText(player, nick, grup);
    }
}

static monoString *U3DStr(const char *str) {
    monoString *(*String_CreateString)(void *_this, const char *str) = (monoString * (*)(void *, const char *))getRealOffset(L3DEVIL.U3DStr);
    return String_CreateString(NULL, str);
}

static monoString *U3DStrFormat(float distance) {
    char buffer[128] = {0};
    sprintf(buffer, "[ %.f M ]  [BOT]", distance);
    return U3DStr(buffer);
}

bool EspGranade = false;
void *Grenade2 = NULL;
void *Render2 = NULL;


static void LineRenderer_SetPosition(void *Render, int value, Vector3 Location){
    void (*_LineRenderer_SetPosition)(void *Render, int value, Vector3 Location) = (void (*)(void*, int, Vector3))getRealOffset(0x322C02C);
    return _LineRenderer_SetPosition(Render, value, Location);
}

static void LineRenderer_Set_PositionCount(void *Render, int value){
    void (*_LineRenderer_Set_PositionCount)(void *Render, int value) = (void (*)(void*, int))getRealOffset(0x322BF80);
    return _LineRenderer_Set_PositionCount(Render, value);
}

static void GrenadeLine_DrawLine(void *instance, Vector3 start, Vector3 end, Vector3 position) {
    void (*_GrenadeLine_DrawLine)(void *clz, Vector3 throwPos, Vector3 throwVel, Vector3 gravity) = (void (*)(void*, Vector3, Vector3,Vector3)) getRealOffset(0xFD56A0);
    return _GrenadeLine_DrawLine(instance, start, end, position);
}

/** ADDED BY L3DEVIL **/

#endif

