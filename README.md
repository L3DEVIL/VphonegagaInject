**This template is not for newbies/noobs, it is for experience modder only. You will be expected to read, learn and even google. If you don't have the knowledge, this tutorial will be too hard for you**

**If you don't like this project, do not use. Please do not spread hate and insult against me, especially the smaller kids community. Instead, tell me why you don't like and what I can improve**

# Quick links
- [Pre-requisites](#pre-requisites)
- [What will you need?](#what-will-you-need)
- [download/clone](#downloadclone)
- [Install NDK](#install-ndk)
- [Open the project](#open-the-project)
- [Making changes](#making-changes)
- [Implementing the menu to the target game](#implementing-the-menu-to-the-target-game)
- [Loading lib without mod menu](#loading-lib-without-mod-menu)
- [FAQ](#faq)
- [Reporting issues](#reporting-issues)
- [Contact](#contact)
- [Useful links](#useful-links)
- [Credits/Acknowledgements](#creditsacknowledgements)

# Introduction
Simple floating mod menu with sounds for il2cpp and other native android games, KittyMemory, MSHook, and And64InlineHook included. This template is optimized for modders who want the faster way to implement the menu in the game without hassle. Assets are stored as base64 in java/smali and does not need to be stored under assets folder.

It comes with string and offset obfuscation without using any external tool and without modifying the compiler. We use AY Obfuscator

Support Android 4.2.x way up to Android R preview. Support ARMv7, x86 and ARM64 architecture. However x86 is deprecated for Unity games so x86 is not our priority

Mod menu is based on Octowolve/Escanor and Van's template. 

Preview:

![](https://i.imgur.com/lSNYzOA.gif)

# Pre-requisites:
- Basic knowledge of C++, Java, dalvik opcodes (smali), ARM and ARM64 assembly. x86 is optional
- Be able to patch hex and hook
- Understanding how Android development works

# What will you need?
- Android Studio 4 and up: https://developer.android.com/studio
- Git if you want to clone a project though Android Studio (Optional): https://git-scm.com/download
- [Apktool.jar](https://ibotpeaches.github.io/Apktool/) or any 3rd party tools: [APK Easy Tool](https://forum.xda-developers.com/android/software-hacking/tool-apk-easy-tool-v1-02-windows-gui-t3333960), [Jasi Toolkit](https://jaspreetsingh.store/jasi-toolkit/), or [INZ APKTool](https://forum.gsmhosting.com/vbb/f831/inz-apktool-2-0-windows-gui-apk-tool-2722815/)
- Any text editor: [Notepad++](https://notepad-plus-plus.org/downloads/), [Subline](https://www.sublimetext.com/) or [Visual Studio Code](https://code.visualstudio.com/)
- Any png compression to compress your png file: https://compresspng.com/
- Any base64 encoding to encode your file: https://www.base64encode.org/
- Any audio converters to convert your sound files to .ogg (Optional): [XMedia Recode](https://www.xmedia-recode.de/en/download.php)
- This template of course

# Download/Clone
Click on the green button that says Code, the click Download ZIP

![](https://i.imgur.com/Cy1SQgI.png)

Or clone through Android Studio itself (Requires Git to be installed on your machine and be configured in Android Studio)

Click on "Get from Version Control"

![](https://i.imgur.com/y8haBYN.png)

Input the git url and Clone

![](https://i.imgur.com/z8KkW8S.png)

# Install NDK
At the bottom-right corner, click on Configure and SDK Manager

![](https://i.imgur.com/xBP1bCE.png)

Select **Android SDK**, check **NDK (Side by side)** and click OK. It will download and install

![](https://i.imgur.com/FcAd2Px.png)

# Open the project 
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

# Making changes

**LoadLib.java:**

To call toast if you load lib without mod menu

**MainActivity.java:**

Starts the main activity. No need to use if you implement the menu in the APK file

**FloatingModMenuService.java:**

The codes of floating mod menu. You don't need to change much unless you want to redesign it. The codes are explained in the comments (//...)

- GradientDrawable

A code for setting corner and stroke/inner border. Works for any View Components
```
GradientDrawable gradientdrawable = new GradientDrawable();
gradientdrawable.setCornerRadius(20); //Set corner
gradientdrawable.setColor(Color.parseColor("#1C2A35")); //Set background color
gradientdrawable.setStroke(1, Color.parseColor("#32cb00")); //Set border
```

Set the gradient drawable to the component

```
[name of your view component].setBackground(gradientdrawable);
```

**Sounds.java:**

Basically the menu sounds, that have been converted to .ogg using XMedia Recode and encoded to base64. They are automatically decoded and stored into /data/data/(package name)/cache upon startup (See StaticActivity). Remember, we want to avoid storing files under assets as possible

**StaticActivity.java:**

To initialize by game activity's OnCreate
Checks if device running Android 6.0 or above and if have overlay permission checked. Sounds being written to the cache directory.
Start() will be called when implementing the menu to the game. We will explain later

- writeToFile:
Decode base64 and write to file to a target directory

**Menu.cpp**

This is menu related

- EnableSounds: Change to false if you don't want it to play sounds

- Title: Big text

- Heading: Little text

- Icon: Compressed image that is encoded to base64

- IconWebViewData: Use icon in Web view with GIF animation support. URL requires internet permission `android.permission.INTERNET`

```From internet: (Requires android.permission.INTERNET)
return env->NewStringUTF("https://i.imgur.com/SujJ85j.gif"); 

From assets folder: (Requires android.permission.INTERNET)
return env->NewStringUTF("file:///android_asset/example.gif"); 

Base64 html:
return env->NewStringUTF("data:image/png;base64, <encoded base64 here>");

Nothing:
return NULL
```

- IconSize: Mod menu icon size 

- Toast: To get text from c++ in order to show toast in java

- getFeatureList: Here you add the mod features

**Main.cpp**

In this file, you will mostly do implementation with your codes for modding

- Changes: Get changes of toggles, seekbars, spinner and buttons to do modding. Features MUST be count from 0

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

**String obfuscation**

We use AY Obfuscator but the usage has changed to `OBFUSCATE("string here")` and `OBFUSCATE_KEY("string here", 'single letter here')`

**Encoding **

# Testing

If you have your device with adb enabled, connected your PC or your emulator with adb enabled. Android Studio will detect and you can click Play to run your app onto your device/emulator

![](https://i.imgur.com/ZegjeM8.png)

To use adb, you must enable USB debugging in the device system settings, under Developer options.

On Android 4.2 and higher, the Developer options screen is hidden by default. To make it visible, go to **Settings** > **About phone** and tap Build number seven times. Return to the previous screen to find Developer options at the bottom.

On some devices, the Developer options screen might be located or named differently.

# Implementing the menu to the target game

After you finished the menu, you can build the project to APK file.
**Build** -> **Build Bundle(s)/APK(s)** -> **Build APK(s)**

If no errors occured, you did everything right and build will succeded. You will be notified that it build successfully

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

Add the service below the start of application tag (change the package name of your menu if you had changed it)
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
If compile fail, read the log and look up on Google

If the mod menu appears and the hack are working, congratz!

If you face any problem, be sure to check the logcat, and if it was native related, write the log such as `LOGD("whatever");` in your cpp codes, recompile and capture the logcat. See what part of your code faced the problem. Logcat may also tell you if hooking fails (lib crash)

Thanks for reading the tutorial

Do not forget to check my template again. I may change it anytime =D

# Loading lib without mod menu

Just call the LoadLib in the OnCreate method
```
    invoke-static {p0}, Luk/lgl/loadlib/LoadLib;->Start(Landroid/content/Context;)V
```

And uncomment the isToastCalled check in hack_thread function

Make sure to delete `modmenu` folder from the smali to avoid reaching the method limit of the smali classes (limit is 65535)

# FAQ
### ERROR: executing external native build for ndkBuild

Install NDK first

### I have a problem decompiling and compiling APK file

Check if apk is not protected. If not, search for the related issues on Google or on Apktool Github page: https://github.com/iBotPeaches/Apktool/issues

### Getting strange issues on Android Studio or Gradle

If you can't find a solution on Google, try invalidate caches. Click **File** -> **Invalidate Caches/Restart**. Let it load. In some cases, you may need to reinstall Android Studio

### I'm getting an error "Unsigned short value out of range: 65536" if I compile

The method index can't fit into an unsigned 16-bit value, means you have too many methods in the smali due to the limit 65535. Place your code on other classes, such as smali_classes2 instead. This work for Android 5 (Lollipop) and above only. Many thanks Andnix for the tip

### How can I protect my dex and/or lib?

There are only chinese based tools I heard so far but they are super slow and I never manage to use them. I highly suggest to not use them, because we don't know if they contain malwares, spywares, you know, rumours about chinese spying. Protecting APK may end up including additional spywares and may result getting flagged by some anti-virus, so use it are your own risk. Do not be offended, i'm just warning

I will not mention their service names and please don't ask me for it.

But there is no need to protect dex since there are nothing important in java/smali codes. All the important codes such as offsets are in the lib file and they are protected enough

### How to get older version of the template?

Go to the commit page https://github.com/LGLTeam/Android-Mod-Menu/commits/master

# Reporting issues

You can report it here https://github.com/LGLTeam/Android-Mod-Menu/issues

Please give link to the APK and provide logcat from Android Studio as possible

Best way is to contact me privately. See below

# Contact

- Telegram: @Radidas
- Discord: LGL#2184

Newbies are the lowest priority, I may not be able to respond at all. You will be blocked if you beg me to teach/spoonfeed. Don't define me a teacher, i'm not a teacher :P

# Useful links
* https://piin.dev/basic-hooking-tutorial-t19.html

* https://iosgods.com/topic/65529-instance-variables-and-function-pointers/

* https://guidedhacking.com/threads/android-function-pointers-hooking-template-tutorial.14771/

* http://www.cydiasubstrate.com/api/c/MSHookFunction/

# Credits/Acknowledgements
Thanks to the following individuals whose code helped me develop this mod menu

* Octowolve/Escanor - Mod menu: https://github.com/z3r0Sec/Substrate-Template-With-Mod-Menu and Hooking: https://github.com/z3r0Sec/Substrate-Hooking-Example
 
* VanHoevenTR - Mod menu - https://github.com/LGLTeam/VanHoevenTR_Android_Mod_Menu

* MrIkso - First mod menu template https://github.com/MrIkso/FloatingModMenu

* MJx0 A.K.A Ruit - https://github.com/MJx0/KittyMemory

* Rprop - https://github.com/Rprop/And64InlineHook

* Google - Android UI sounds

* Material.io - https://material.io/design/sound/sound-resources.html#