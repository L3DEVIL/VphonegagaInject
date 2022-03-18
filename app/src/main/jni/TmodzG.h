#ifndef L3DEVLINJECTOR_TMODZG_H
#define L3DEVLINJECTOR_TMODZG_H

struct {
    uintptr_t HeadTF = 0x220; // New 0x220; TOXIC protected Transform JIMLGADEEEB;
    uintptr_t HipTF = 0x224; // New 0x224; protected Transform CNLAPGJOGPE;
    uintptr_t ToeTF = 0x238; // New  0x238; protected Transform HOHPCJDANHA;
    uintptr_t HandTFF = 0x21C;//1.69
    uintptr_t DedoTF = 0x24C;//1.69
    uintptr_t PeS = 0x244;//1.69
    uintptr_t PeD = 0x248;//1.69
    uintptr_t PeEsquerdo = 0x1D8;//NÃO ATT
    uintptr_t OmbroD = 0x248; // 1.69
    uintptr_t OmbroE = 0x24C; // 1.69
    uintptr_t PernaD = 0x23C; // 1.69
    uintptr_t PernaE = 0x240; // 1.69
    uintptr_t BracoD = 0x244; // 1.69
    uintptr_t BracoE = 0x21C; // 1.69
    uintptr_t RShoulder = 0x254; // New 0x254;  protected Transform IIALIDGNNBN;
    uintptr_t LShoulder = 0x258; // New  0x258;  protected Transform HOFOFJPMGKP;
    uintptr_t MainCameraTransform = 0x88; // New  0x88;  public Transform MainCameraTransform;
    uintptr_t Dictionary = 0x48; // New  0x44; private Dictionary<int, ResourceID> BPDBGECNEBM;
    uintptr_t IsClientBot = 0x110; // New 0x110
    uintptr_t Component_GetTransform = 0x36D38DC;  // New public Transform get_transform() { }
    uintptr_t Transform_INTERNAL_SetPosition = 0x34B3EB0; // New private void set_position_Injected(ref Vector3 value) { }
    uintptr_t Transform_INTERNAL_GetPosition = 0x34b3e10; // New private void get_position_Injected(out Vector3 ret) { }
    uintptr_t WorldToScreenPoint = 0x36D1068; // New public Vector3 WorldToScreenPoint(Vector3 position) { }
    uintptr_t GetForward = 0x34B4970; // new public Vector3 get_forward() { }
    uintptr_t GetLocalCar = 0xB520AC; // new public Vehicle VehicleIAmIn() { }
    uintptr_t GetLocalPlayer =  0x1D6D4D0; //  // 0x1D6D4D0 new private static Player GetLocalPlayer() { } 0x198d608
    uintptr_t Current_Local_Player = 0x1515958; // new public static Player CurrentLocalPlayer() {
    uintptr_t GetLocalPlayerOrObServer = 0x1517548; //new public static Player GetLocalPlayerOrObserverTarget() { }
    uintptr_t Curent_Match = 0x1515568; // new public static NFJPHMKKEBF CurrentMatch() { }
    uintptr_t Camera_main = 0x36d19dc; //new public static Camera get_main() { }
    uintptr_t get_MyFollowCamera = 0xB4FDDC; //new public FollowCamera get_MyFollowCamera() { }
    uintptr_t get_IsFiring =  0xBA93FC; //new public bool IsFiring() { }
    uintptr_t get_IsSighting = 0xBE315C; //new public bool get_IsSighting() { } 0xBE315C
    uintptr_t get_isLocalTeam = 0xB5CD48; //new public virtual bool IsLocalTeammate() { }
    uintptr_t get_IsDriver = 0xB51D84; //new public bool get_IsDriver() { }
    uintptr_t get_isVisible = 0xB5465C; // new  public override bool IsVisible() { };
    uintptr_t get_CurHp = 0x1CCA530; // new public int get_CurHP() { }
    uintptr_t get_MaxHP =  0xB93090; // new public int get_MaxHP() { }
    uintptr_t get_IsDieing = 0xB4FC2C; // new public bool get_IsDieing() { }
    uintptr_t set_aim = 0xB5108C; // new public void SetAimRotation(Quaternion JGOGIAFGCFC) { }
    uintptr_t get_NickName = 0xB4E284; // new public string get_NickName() { }
    uintptr_t get_Chars = 0x3764460; // new public char get_Chars(int index) { }
    uintptr_t GetAttackableCenterWS = 0xB4D764; // new public override Vector3 GetAttackableCenterWS() { }
    uintptr_t CurrentUIScene =  0x15144D8; // new  public static UICOWBaseScene CurrentUIScene() { }
    uintptr_t ShowPopupMessage = 0xC18C78; //1.68
    uintptr_t U3DStr = 0x3772CE4; // new private string CreateString(sbyte* value) { } 1.68
    uintptr_t get_imo =  0xB49EE4;// new  public GPBDEDFKJNA GetActiveWeapon() {
    uintptr_t set_esp = 0x1E597B8; //1.68 //private void CPBCGAKODII(Vector3 BNDPJDJAFOM, Vector3 PFAPJJODMCM) { }
    uintptr_t get_IsCrouching = 0xB5EFB4; // //public bool IsCrouching() { }
    uintptr_t set_endWidth= 0x3589CC8; //public void set_endWidth(float value) { }
    uintptr_t set_width =  0x8FC058; // public void set_width(float value) { }
    uintptr_t set_startColor = 0x3589E08; //public void set_startColor(Color value) { }
    uintptr_t set_endColor = 0x3589EAC; //public void set_endColor(Color value) { }
} L3DEVIL;


#endif
