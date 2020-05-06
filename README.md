# Android Mod Menu
Simple floating mod menu to il2cpp and other native android games with Android/material UI sounds. The mod menu is based on Octowolve/Escanor and Van's template. This template is the most efficient and fastest way to work and to implement menu in the game

Support both KittyMemory and MSHook and support Android 4.2.x way up to Android R preview. Sound effects included. Hook and KittyMemory support both ARMv7 and ARM64

This is how it looks like:

![](https://i.imgur.com/W63wVTj.gif)

**This tutorial is not for newbies/noobs. You need basic knowledge of C++, Java, dalvik opcodes, and also ARM and ARM64 assembly, hex patching and hooking. If you don't have the knowledge, this tutorial will be hard for you, and I won't spoon feeding**

# What will you need?
- Android Studio 3 and up: https://developer.android.com/studio
- Git (Optional) - If you want to clone a project though Android Studio: https://git-scm.com/download

- Apktool:

- -- Apktool.jar: https://ibotpeaches.github.io/Apktool/

- Or

- -- APK Easy Tool: https://forum.xda-developers.com/android/software-hacking/tool-apk-easy-tool-v1-02-windows-gui-t3333960

- Notepad:

- -- Notepad++ https://notepad-plus-plus.org/downloads/

- Or

- -- Sublime Text: https://www.sublimetext.com/

- Compress png - to compress your png file: https://compresspng.com/ 
- Base64 encode - to encode your file: https://www.base64encode.org/
- XMedia Recode - to convert your sound files to .ogg https://www.xmedia-recode.de/en/download.php
- Template: https://github.com/LGLTeam/Android-Mod-Menu

# Install Android Studio
If you have Android Studio installed, you can skip this steps 

Setting up Android Studio takes just a few clicks.

See the user guide: https://developer.android.com/studio/install

# Install NDK
Open Android Studio, you will be welcomed

![](https://i.imgur.com/Cy1SQgI.png)

At the bottom-right corner, click on Configure and SDK Manager

![](https://i.imgur.com/xBP1bCE.png)

Select **Android SDK**, check **NDK (Side by side)** and click OK. It will download and install

![](https://i.imgur.com/FcAd2Px.png)

# Open the mod menu template project 
Once you've downloaded all the necessary files, extract the template project to the folder without any spaces. If any folder has spaces, it will cause problem

On Android Studio on the welcome screen, choose **"Open an existing Android Studio project"**

Navigate to the extracted project and open it

![](https://i.imgur.com/3etm4qX.png)

It will index and Gradle will sync the project fir the first time. Please wait for a while, it will take around 5 minutes depending your computer performance

If you encounter an error 

```NDK not configured. Download it with SDK manager. Preferred NDK version is '20.0.5594570'```

Click File and Project Structure

![](https://i.imgur.com/EWhLe7M.png)

Select default NDK version

![](https://i.imgur.com/YfmIvoN.png)

After it's done, you can start working!

On the left side, you see the Project view. Default view is Android

![](https://i.imgur.com/YT71Y6B.png)

If this is somewhat confusing, change the view to Project

I will explain each of the files for you

**FloatingModMenuService.java:**

The codes of floating mod menu. You don't need to change much unless you want to redesign it. The codes are explained in the comments (//...)

**MainActivity.java:**

Starts the main activity. It won't be used if you implement the menu in the game

**Sounds.java:**

Basically the 'GTA V' sounds, have been converted to .ogg using XMedia Recode and encoded to base64. They are automatically decoded and stored into /data/data/(package name)/cache upon startup. See StaticActivity

**StaticActivity.java:**

To initialize by game activity's OnCreate
Checks if device running Android 6.0 or above and if have overlay permission checked. Sounds being written to the cache directory.
Start() will be called when implementing the menu to the game. We will explain later

- writeToFile:
Decode base64 and write to file to a target directory

**main.cpp**

In this file, you will mostly use it to edit features, credits, icon, and implement your code for KittyMemory or MS Hooking.

- EnableSounds: Change to false if you don't want it to play sounds

- Title: Big text

- Heading: Little text

- Icon: Compressed image that is encoded to base64

- IconSize: Mod menu icon size 

- Toast: To get text from c++ in order to show toast in java

- Changes: Get changes of toggles, seekbars, spinner and buttons to do modding. Features MUST be count from 0

- getFeatureList: Here you add the mod features

Usage:

```
Toggle_[feature name]
SeekBar_[feature name]_[min value]_[max value]
Spinner_[feature name]_[Items e.g. item1_item2_item3]
Button_[feature name]
Button_OnOff_[feature name]
InputValue_[feature name]
```

Examples:

```Toggle_God mode
Spinner_Weapons_AK47_9mm_Knife
Button_OnOff_God mode
```

Do not forget to count your features from 0 and remember them

- hack_thread: Here you add your code for hacking with KittyMemory or Hooking. I will not teach, you must have learned it already

- JNI_OnLoad: Initialize when the library loads

**Android.mk**

The make file for the c++ compiler. In that file, you can change the lib name on the `LOCAL_MODULE` line
When you change the lib name, change also on `System.loadLibrary("")` under OnCreate method on FloatingModMenuService.java
Both must have same name

KittyMemory usage:
```MemoryPatch::createWithHex([Lib Name], [offset], "[hex. With or without spaces]");
[Struct].get_CurrBytes().Modify();
[Struct].get_CurrBytes().Restore();

[Struct].get_TargetAddress();
[Struct].get_PatchSize();
[Struct].get_CurrBytes().c_str();
```

Example: https://github.com/MJx0/KittyMemory/blob/master/Android/test/src/main.cpp

Hook usage:
ARM64:
```A64HookFunction((void *) getAbsoluteAddress([Lib Name], [offset]), (void *) [function], (void **) &[old function]);```

ARMv7/x86:
```MSHookFunction((void *) getAbsoluteAddress([Lib Name], [offset]), (void *) [function], (void **) &[old function]);```

Other than that, find out yourself. It's a lot easier if you already have the knowledge 
Most codes have the comments that will explain for you 
Enjoy working with the menu =D

# Testing the mod menu 

If you have your device with adb enabled, connected your PC or your emulator with adb enabled. Android Studio will detect and you can click Play to run your app onto your device/emulator

![](https://i.imgur.com/ZegjeM8.png)

To use adb, you must enable USB debugging in the device system settings, under Developer options.

On Android 4.2 and higher, the Developer options screen is hidden by default. To make it visible, go to **Settings** > **About phone** and tap Build number seven times. Return to the previous screen to find Developer options at the bottom.

On some devices, the Developer options screen might be located or named differently.

# Implementing the menu in the target game 

After you finished the menu, you can build the project to APK file.
**Build** -> **Build Bundle(s)/APK(s)** -> **Build APK(s)**

If no errors occured, you did everything right and build will succed. You will be notified that it build successfully

![](https://i.imgur.com/WpSKV1L.png)

Click on **locate** to show you the location of **build.apk**. It is stored at `(your-project)\app\build\outputs\apk\ app-debug.apk`

![](https://i.imgur.com/wBTPSLi.png)

Now you will need to decompile **app-debug.apk**. Decompile the target game as well

Open the game's `androidmanifest.xml`
Add the permission besides other permissions
```
<uses-permission android:name="android.permission.SYSTEM_ALERT_WINDOW"/>
```

![](https://i.imgur.com/XOxLU91.png)

And add the service below the application tag (change the package name if you had changed it)
```
<service
           android:name="uk.lgl.modmenu.FloatingModMenuService"
            android:enabled="true"
            android:exported="false"/>
```

Save the **AndroidManifest.xml** file

Now we are looking for main activity, it is ususally written under application tag. The activity name may be different. If you spotted `android:name="android.intent.action.MAIN"` you will immediately know this is main activity

Be sure to enable Word wrap so it is easier to read

![](https://i.imgur.com/7DzU8d0.png)

Or open the apk in APK Easy Tool and look for main activity

![](https://i.imgur.com/ohp0zk1.png)

In this case, the path to main activity was `com.funcube.loa.MainActivity`. I would navigate to `(decompiled game)/smali/com/funcube/loa/` and you will see **MainActivity.smali**. If the game have multi dex, find out which smali folder has the main activity, it should be in one of these folders.

Open the main acitivity's smali file, search for OnCreate method and paste this code inside (change the package name if you had changed it)
```
    invoke-static {p0}, Luk/lgl/modmenu/StaticActivity;->Start(Landroid/content/Context;)V
```
 
![](https://i.imgur.com/7CxTCl8.png)

Save the file

Copy your mod menu from decompiled app-debug.apk smali to the game's smali folder. Example mine is uk.lgl.modmenu, I copy the `uk` folder from **app-debug** `(app-debug\smali\uk)` to the game's decompiled directory `(game name)\smali`

![](https://i.imgur.com/aO6eEab.png)
 
Very important for multi dex games. Let's say if main activity is located in **smali_classes2**, I would put my mod menu in **smali_classes2**

Copy the library file (.so) from **app-debug.apk** to the target game. Make sure to copy .so to the correct architecture
armeabi-v7a is armeabi-v7a, arm64-v8a is arm64-v8a, and so on.
Putting the .so on a wrong architecture will result a crash
 
![](https://i.imgur.com/oZq1Wq7.png)
 
Now compile and sign the apk
If compile fail, read the log and look up at Google

If the mod menu appears and the hack are working, congratz!

If you face any problem, be sure to check the logcat, and if it was native related, write the log such as `LOGD("whatever");` in your cpp codes, recompile and capture the logcat. See what part of your code faced the problem. Logcat will also tell you if hooking fails (lib crash)

Thanks for reading the tutorial, if you need any help, feel free to talk to me via Telegram https://t.me/RadidasG
I may only help for experience modders only, and no begging please =D

Do not forget to check my template again. I may change it anytime =D

# Credits/Acknowledgements
Thanks to the following individuals whose code helped me develop this mod menu

* Octowolve/Escanor - Mod menu: https://github.com/z3r0Sec/Substrate-Template-With-Mod-Menu and Hooking: https://github.com/z3r0Sec/Substrate-Hooking-Example
 
* VanHoevenTR - Mod menu - https://github.com/LGLTeam/VanHoevenTR_Android_Mod_Menu

* MrIkso - First mod menu template https://github.com/MrIkso/FloatingModMenu

* MJx0 A.K.A Ruit - https://github.com/MJx0/KittyMemory

* Rprop - https://github.com/Rprop/And64InlineHook

* Google - Android UI sounds

* Material.io - https://material.io/design/sound/sound-resources.html#

The following websites were also very helpful

* Stackoverflow - https://stackoverflow.com/

* Guided hacking - https://guidedhacking.com/forums/android-game-hacking.438/
