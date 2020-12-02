**THIS TEMPLATE IS NOT FOR NEWBIES/NOOBS, IT IS FOR EXPERIENCE MODDERS AND PROGRAMMERS ONLY. YOU WILL BE EXPECTED TO READ, LEARN AND EVEN GOOGLE. THIS IS NOT A SIMPLE ONE-CLICK INSTALL SETUP. IF YOU DON'T HAVE THE KNOWLEDGE, THIS TUTORIAL WILL BE TOO HARD FOR YOU**

**IF YOU ARE UPDATING THIS TEMPLATE, PLEASE CLONE IT SEPARATELY. DO NOT MERGE INTO OLD ONE UNLESS YOU KNOW WHAT YOU ARE DOING!**

**This won't cover how to mod games in general, hooking functions, etc that every other online tutorial already covers, so don't ask since I won't cover them. The codes in the template simply tells you how to use them**

**For mobile users who don't have a PC, please read [README-MOBILE.md](https://github.com/LGLTeam/Android-Mod-Menu/blob/master/README-MOBILE.md) how to use this project within AIDE app**

# Quick links
- [Prerequisites](#prerequisites)
- [Softwares you need](#softwares-you-need)
- [download/clone](#downloadclone)
- [Setting up Android Studio](#setting-up-android-studio)
- [Open the project](#open-the-project)
- [Files to work with and making changes](#files-to-work-with-and-making-changes)
- [Implementing the menu to the target game](#implementing-the-menu-to-the-target-game)
- [Loading lib without mod menu](#loading-lib-without-mod-menu)
- [FAQ](#faq)
- [Credits/Acknowledgements](#creditsacknowledgements)

# Introduction
Floating mod menu for il2cpp and other native android games. KittyMemory, MSHook, and And64InlineHook included. This template is optimized for modders who want the faster way to implement the menu in the game without hassle. Assets are stored as base64 in cpp and does not need to be stored under assets folder.

It comes with string and offset obfuscation without using any external tool and without modifying the compiler. We use AY Obfuscator

Support Android 4.4.x way up to Android R. Support ARMv7, x86 and ARM64 architecture. However x86 is deprecated for Unity games so x86 is not our priority

Mod menu is based on Octowolve/Escanor and Van's template. 

Preview:

![](https://i.imgur.com/42Sh72L.gif)

# Prerequisites
* **AN EXPERIENCED MODDER, NOT A NEWBIE/BEGINNER MODDER:** You should be able to mod any games in general (does not need to be a protected games), like modifying .so files, dll files, smali files, etc.
* Basic knowledge of smali dalvik opcodes and ARM and ARM64 assembly (x86 not needed), required for patching meemory
* Be able to hook function in C++ (Not really needed, but recommended if you want to do advanced modding in the future)
* Basic knowledge of C++ and java
* Basic awareness of how Android layout works in XML and Java. This project only uses Java for layout but you will learn it easly
* Time and patience: Don't start working on this if you have deadlines or important work, only on your free time. Take your time to read, learn and get used to work with this project.
* DIY (Do it yourself): Yes, you must be able to do things yourself. If we can't or won't implement some certain features, try to implement yourself. We are not the teachers, it is not our style, so don't ask us to teach or spoonfeed.
* An inquisitive mind

# Softwares you need
* Android Studio 4 and up: https://developer.android.com/studio
* Apktool: [Apktool.jar](https://ibotpeaches.github.io/Apktool/) or any 3rd party tools
* APK Easy Tool. To get main activity: https://forum.xda-developers.com/android/software-hacking/tool-apk-easy-tool-v1-02-windows-gui-t3333960
* Any text editor. I use [Notepad++](https://notepad-plus-plus.org/downloads/)
* Any png compression to compress your png file: https://compresspng.com/
* Any base64 encoding to encode your file: https://www.base64encode.org/
* Any audio converters to convert your sound files to .ogg (Optional): [XMedia Recode](https://www.xmedia-recode.de/en/download.php)

# Download/Clone
Click on the button that says Code, and click Download ZIP

![](https://i.imgur.com/EZnbd10.png)

Or clone through Android Studio if you know how to use Git

# Setting up Android Studio

### 1. Installing NDK

At the bottom-right corner, click on Configure and SDK Manager

![](https://i.imgur.com/xBP1bCE.png)

Select **Android SDK**, check **NDK (Side by side)** and click OK. It will download and install

![](https://i.imgur.com/FcAd2Px.png)

### 2. Open the project

Extract the template project to the folder without any spaces. If any folder has spaces, it will cause problem

On the welcome screen, choose **"Open an existing Android Studio project"**

Navigate to the extracted project and open it

![](https://i.imgur.com/3etm4qX.png)

It will index and Gradle will sync the project fir the first time. Please wait for a while, it will take around 5 minutes depending your computer performance

If you encounter an error

```NDK not configured. Download it with SDK manager. Preferred NDK version is 'xx.x.xxxxxx'```

Click File and Project Structure

![](https://i.imgur.com/EWhLe7M.png)

Select default NDK version

![](https://i.imgur.com/YfmIvoN.png)

After it's done, you can start working!

# Files to work with and making changes

### Java

**`MainActivity.java`**:
You pretty don't need to work with it unless you are implementing something like login layout.

Static method `Start()` and `LoadLibOnly()` can be called from game's `OnCreate`. `Start()` checks if device running Android 6.0 or above and if have overlay permission checked before starting menu service.

**`modmenu/Preferences.java`**: Saving the menu feature preferences and calling changes via JNI

**`modmenu/FloatingModMenuService.java`**: Main codes of mod menu design

The codes of floating mod menu. You don't need to change much unless you want to redesign it. The codes are explained in the comments

Note: In the `run()` handler method inside `initFloating()`, there is a code that checks if game lib is loaded or not before loading feature lists. If you are running the app as debug and want to test preferences, comment the if-else code out except feature list thing. Otherwise it would get stuck without a game

- `GradientDrawable`

A code for setting corner and stroke/inner border. Works for any View Components
```
GradientDrawable gradientdrawable = new GradientDrawable();
gradientdrawable.setCornerRadius(20); //Set corner
gradientdrawable.setColor(Color.parseColor("#1C2A35")); //Set background color
gradientdrawable.setStroke(1, Color.parseColor("#32cb00")); //Set border
```

Set the gradient drawable to the view component

```
[name of your view component].setBackground(gradientdrawable);
```

- Resizing menu box

I have added variables so you can find it easly to resize
```
private final int MENU_WIDTH = 290;
private final int MENU_HEIGHT = 200;
```

Note: You may need to implement auto sizing due to many types of phone with different DPIs and resolutions

- Color Animation: The codes can be seen in `startAnimation()`

- Adding new view

Normally the Android development documentation does not explain the code in java. If you read the Android development documentation and you see an example like TextView

```
TextView textView = (TextView) findViewById(R.id.textView);
textView.setFontVariationSettings("'wdth' 150");
```

This is for xml. Instead, create an instance for java and add view to your Layout

```
TextView textView = new TextView(this);
textView.setFontVariationSettings("'wdth' 150");
LinearLayoutExample.addView(textView);

```

There are many more. While we can't explain much here, you can use Google. Search like `create a textview programmatically android`, `create a button programmatically android` etc for more infomation

### Cpp

**`Menu/Sounds.h`**: The sounds, that have been converted to .ogg and encoded to base64. Remember, we want to avoid storing files under assets as possible

**`Menu/Menu.h`**: Menu related with JNI calls

- `LoadSounds`: Decode base64 and save files into /data/data/(package name)/cache upon startup.

- `Title`: Big text

- `Heading`: Little text. Semi HTML is supported. Text will scroll if the text is too long

- `Icon`: Compressed image that is encoded to base64

- `IconWebViewData`: Use icon in Web view with GIF animation support. URL requires internet permission `android.permission.INTERNET`

Examples

```From internet: (Requires android.permission.INTERNET)
return env->NewStringUTF("https://i.imgur.com/SujJ85j.gif"); 

From assets folder: (Requires android.permission.INTERNET)
return env->NewStringUTF("file:///android_asset/example.gif"); 

Base64 html:
return env->NewStringUTF("data:image/png;base64, <encoded base64 here>");

Nothing:
return NULL
```

**`Toast.h`**: Your toast

**`Main.cpp`**: In this file, you will work with your mods here

- `Changes`: Get values to apply mods. BE CAREFUL NOT TO ACCIDENTLY REMOVE break;

You can also use if-else statement with string comparision

```
if (strcmp(featureName, "The button") == 0) { //Compare with string

} else if (strcmp(featureName, "The On/Off button") == 0) { //Compare with string

} else if (feature == 7) {

}
```
- `getFeatureList`: Mod features

Usage:

```
Category_(text)
Toggle_(feature name)
SeekBar_(feature name)_(min value)_(max value)
Spinner_(feature name)_(Items e.g. item1,item2,item3)
Button_(feature name)
ButtonLink_(feature name)_(URL/Link here)
ButtonOnOff_(feature name)
InputValue_(feature name)
CheckBox_(feature name)
RadioButton_(feature name)_(Items e.g. radio1,radio2,radio3)
RichTextView_(Text with limited HTML support)
RichWebView_(Full HTML support)
```

Examples:

```Toggle_God mode
Spinner_Weapons_AK47,9mm,Knife
Button_OnOff_God mode
```

Learn more about HTML https://www.w3schools.com/

- `hack_thread`: Here you add your code for hacking with KittyMemory or Hooking. We will not teach, you must have learned it already

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
```A64HookFunction((void *) getAbsoluteAddress([Lib Name], [offset]), (void *)[function], (void **)&[old function]);```

ARMv7/x86:
```MSHookFunction((void *) getAbsoluteAddress([Lib Name], [offset]), (void *)[function], (void **)&[old function]);```

**`Android.mk`**

The make file for the c++ compiler. In that file, you can change the lib name on the `LOCAL_MODULE` line
When you change the lib name, change also on `System.loadLibrary("")` under OnCreate method on FloatingModMenuService.java
Both must have same name

**C++ string obfuscation**

We use AY Obfuscator but the usage has changed to `OBFUSCATE("string here")` and `OBFUSCATE_KEY("string here", 'single letter here')`

Example
```
OBFUSCATE("Hello world")
OBFUSCATE_KEY("Hello world", 'a')
OBFUSCATE_KEY("Hello world", 'u')
```

### Others

**`proguard-rules.pro`**

See proguard rules here https://www.guardsquare.com/en/products/proguard/manual/usage

Both `shrinkResources` and `minifyEnabled` MUST be `true` in `build.gradle (:app)` in order to enable proguard obfuscation

`public static void Start` has been prevented from renaming

Add `-dontobfuscate` to disable obfuscation

**Encoding your files into base64**

You can pretty much use any tools for base64 encoding.

We use a simple website https://www.base64encode.org/

Scroll down till you see `Encode files into Base64 format`. Click or tap on the box to select a file

Click on `ENCODE` button and click on `CLICK OR TAP HERE` to download your encoded file. Now you can paste it in your code

# Testing

Connect your device to computer or run your emulator. Make sure you have USB-Debugging enabled in the developer option of your device. Android Studio will detect and you can click Play to run your app.

![](https://i.imgur.com/ZegjeM8.png)

Sometimes emulators such as NOX or MEMU fail to connect to adb automatically, in order to connect them, simply reboot the emulator.

On Android 4.2 and higher, the Developer options screen is hidden by default. To make it visible, go to **Settings** > **About phone** and tap Build number seven times. Return to the previous screen to find Developer options at the bottom.

On some devices, the Developer options screen might be located or named differently.

# Implementing the menu to the target game

### 1. Know your game's main activity

Now we are looking for main activity, there are 2 ways to do

1. Decompile the game's APK file. Open `androidmanifest.xml` and search after `<action android:name="android.intent.action.MAIN"/>`.

Example the game's main activity was `com.unity3d.player.UnityPlayerActivity`

![](https://i.imgur.com/FfOtc1K.png)

Be sure to enable Word wrap so it is easier to read

![](https://i.imgur.com/7DzU8d0.png)

2. APK Easy Tool since it can read out location of main activity without decompiling APK

![](https://i.imgur.com/JQdPjyZ.png)

Note it somewhere so you can easly remember it

### 2. Making corresponding changes in the files

Decompile the game APK

Open the game's `androidmanifest.xml`

Add the `SYSTEM_ALERT_WINDOW` permission besides other permissions if it doesn't exist. We only need one permission. Doesn't matter where you place it as long as it's above the application tag
```
<uses-permission android:name="android.permission.SYSTEM_ALERT_WINDOW"/>
```

![](https://i.imgur.com/XOxLU91.png)

Add the service above the end of application tag (change the package name of your menu if you had changed it)
```
<service android:name="uk.lgl.modmenu.FloatingModMenuService" android:enabled="true" android:exported="false" android:stopWithTask="true"/>
```

![](https://i.imgur.com/rw0hawa.png)

Now we need to call your mod menu activity

There are 2 ways to call your mod menu activity. Choose one of them you like to try. Don't know? just choose METHOD 1

**METHOD 1**

This simple way, we will call to `MainActivity.java`. `MainActivity.java` will never be used

Locate to the game's path of main activity and open the **smali** file. If the game have multi dexes, it may be located in smali_classes2.. please check all

With the path of the target game’s main activity which we determined earlier `com.unity3d.player.UnityPlayerActivity`. Think of it like a path `/com/unity3d/player/UnityPlayerActivity.smali`

Open the main acitivity's smali file, search for OnCreate method and paste this code inside (change the package name if you had changed it)
```
invoke-static {p0}, Luk/lgl/MainActivity;->Start(Landroid/content/Context;)V
```
 
![](https://i.imgur.com/7CxTCl8.png)

Save the file

**METHOD 2**

You can follow this it if the first method really fails, or if you really want to use `MainActivity.java` for a reason. Since this involve changing activites, it may cause some problems.

On your `MainActivity.java`, put the game's main activity to `public String GameActivity`

![](https://i.imgur.com/jdacwvH.png)

Uncomment this code

```
Toast.makeText(MainActivity.this, "Error. Game's main activity does not exist", Toast.LENGTH_LONG).show();
```

On `androidmanifest.xml`, remove `<action android:name="android.intent.action.MAIN"/>` from the game's activity, like this:

![](https://i.imgur.com/z1RxPjc.png)

If you don't remove `<action android:name="android.intent.action.MAIN"/>` from the game's activity, your menu will not launch. `androidmanifest.xml` can ONLY contain one `<action android:name="android.intent.action.MAIN"/>`

near the end of application tag `</application>`, add your main activity above it. `uk.lgl.MainActivity` is your main activity

```xml
<activity android:configChanges="keyboardHidden|orientation|screenSize" android:name="uk.lgl.MainActivity">
     <intent-filter>
         <action android:name="android.intent.action.MAIN"/>
         <category android:name="android.intent.category.LAUNCHER"/>
     </intent-filter>
</activity>
```

![](https://i.imgur.com/33lMPhc.png)

Save the file

_Do NOT use both methods at the same time_

### 3. Building your project and copying files

Build the project to the APK file.
**Build** -> **Build Bundle(s)/APK(s)** -> **Build APK(s)**

If no errors occured, you did everything right and build will succeded. You will be notified that it build successfully

![](https://i.imgur.com/WpSKV1L.png)

Click on **locate** to show you the location of **build.apk**. It is stored at `(your-project)\app\build\outputs\apk\app-debug.apk`

![](https://i.imgur.com/wBTPSLi.png)

Decompile your **app-debug.apk**.

Copy your mod menu from decompiled app-debug.apk smali to the game's smali folder. Example ours is uk.lgl.modmenu, we copy the `uk` folder from **app-debug** `(app-debug\smali\uk)` to the game's decompiled directory `(game name)\smali`

![](https://i.imgur.com/aO6eEab.png)
 
If the game have multidexes, just add your smali to the last `smali_classes` if possible to prevent compilation errors such as `Unsigned short value out of range: xxxxx` (Smali limit error)

Copy the library file (.so) from **app-debug.apk** to the target game. Make sure to copy .so to the correct architecture
armeabi-v7a is armeabi-v7a, arm64-v8a is arm64-v8a, and so on.

PUTTING THE .SO file ON A WRONG ARCHITECTURE WILL RESULT A CRASH!
 
![](https://i.imgur.com/oZq1Wq7.png)
 
### 4. Compiling game apk
 
Now compile and sign the apk, and install it on your device

Congrats. You have successfully implemented a mod menu.

Compile failed? read the log and look up on Google

If you face any problem, please read the [FAQ](#faq)

# Loading lib without mod menu

Just call the `LoadLibOnly` in the `OnCreate` method if you want to load your hacks without mod menu
```
    invoke-static {p0}, Luk/lgl/MainActivity;->LoadLibOnly(Landroid/content/Context;)V
```

Make sure to delete `modmenu` folder from the smali to avoid reaching the method limit of the smali classes (limit is 65535)

# FAQ
## My game crashing or freezing/My mod menu does not work
There are many reasons why, it could be your code fault, wrong offsets, bad c++ code, bad layout implementations, game protections etc.

First of all, check logcat on Android Studio to see the error logs. Connect your device/reboot emulator to reconnect, open **Logcat** window from below, and select your device, process and filter to **Error** and reproduce your problem. Once you do, you can see the errors in logcat

![](https://i.imgur.com/cutKC29.png)

Then search the error on Google. Contact me or report issues if you can't find the answers

If the game crashes or freezing while playing, check if your patches and hooks are correct. For hooks, write down the logs such as `LOGD("whatever");` like this below:

```
bool get_BoolExample(void *instance) {
    LOGD("BoolExample 1");
    if (instance != NULL && featureHookToggle) {
        LOGD("BoolExample 2");
        return true;
    }
    LOGD("BoolExample 3");
    return old_get_BoolExample(instance);
}
```

Recompile and check the logcat, to see what part of your code faced the problem.

Logcat may also report `CRASH` if lib crashed, caused by hooking

See more about logcat: https://developer.android.com/studio/debug/am-logcat

If you believe the game has protection, try recompile APK without mod and install to see if it crash. We can't help you bypassing protections

### I have a problem decompiling or compiling APK file
Check if apk is not protected. If not, search for the related issues on Google or on Apktool Github page: https://github.com/iBotPeaches/Apktool/issues

### I'm getting an error `Unsigned short value out of range: 65536` if I compile
The method index can't fit into an unsigned 16-bit value, means you have too many methods in the smali due to the limit 65535. Place your code on other classes, such as smali_classes2 instead. This work for Android 5 (Lollipop) and above only.

### I'm getting an error `ERROR: executing external native build for ndkBuild Android.mk. Affected Modules: app`
See: https://github.com/LGLTeam/Android-Studio-Solutions/wiki/Executing-external-native-build-for-ndkBuild-Android.mk

### I'm getting strange issues on Android Studio or Gradle
See: https://github.com/LGLTeam/Android-Studio-Solutions/wiki

### How to add colored text on JNI toast?
It is not implemented yet, and we don't have enough knowledge in JNI porting to do this

But it is deprecated in API level 30/Android 11, means custom toast will not work, so we will not implement it

See: https://developer.android.com/reference/android/widget/Toast#getView()

### How can I protect my dex and/or lib?

We will not mention their names, but you can search for that on Github. Using chinese tools may inject malwares and spywares in APK. We highly suggest to not use them, and please don't ask us.

There is no need to protect dex since there are nothing important in java/smali codes. All the important codes such as offsets are in the lib file and they are protected enough

### How to get older version of the template? or how to see updates/commits?

Go to the commit page https://github.com/LGLTeam/Android-Mod-Menu/commits/master

### Can I compile this project on Android using AIDE?

Likely yes and no, but we don't support AIDE at this time

### How can I соntact you?
You can соntact me via Tеlеgram or Disсоrd.

<details>
<summary>Contact:</summary>
Before you contact, please make sure you have readed everything and looking on Google before contacting

Newbies who do not understand anything must NOT соntact. You will be BLOCKED if you ask/beg to teach/spoonfeed. Why? Because we have gotten so many newbie kids who wouldn't know nothing about modding but attempt to try this for nothing, and come to me and begging and spamming for help. We are getting tired of this now. DON'T be that toxic kid please

Speak english only please. Speaking in other language will be ignored

Thanks!

Tеlеgram: @ThеᒪGᒪ

Disсоrd: ᒪGᒪ#6844
</details>

### Can you help me mod (name of game)?

Noob, we are not spoonfeeding. Don't соntact if you don't know how to mod games.
                              
Instead, try to find a couple of tutorials to learn and mod the game yourself. It's a lot easier than you think. If you can't, search on the internet and you should find a couple of forums where you can ask your questions.

### Do you have project of someones mod menu including game codes for example MITO Team mod?

No, because they used this template and they created their own mod with it, we don't support nor work with them. Ask the right owner who have them, example if mod is created by MITO Team, ask MITO Team. We are the wrong persons to ask.

### Where can I learn hooking?

* https://piin.dev/basic-hooking-tutorial-t19.html
* https://iosgods.com/topic/65529-instance-variables-and-function-pointers/
* https://guidedhacking.com/threads/android-function-pointers-hooking-template-tutorial.14771/
* http://www.cydiasubstrate.com/api/c/MSHookFunction/
* https://www.cprogramming.com/tutorial/function-pointers.html

# Credits/Acknowledgements
Thanks to the following individuals whose code helped me develop this mod menu

* Octowolve/Escanor - Mod menu: https://github.com/z3r0Sec/Substrate-Template-With-Mod-Menu and Hooking: https://github.com/z3r0Sec/Substrate-Hooking-Example
* VanHoevenTR - Mod menu - https://github.com/LGLTeam/VanHoevenTR_Android_Mod_Menu
* MrIkso - First mod menu template https://github.com/MrIkso/FloatingModMenu
* MJx0 A.K.A Ruit - https://github.com/MJx0/KittyMemory
* Rprop - https://github.com/Rprop/And64InlineHook
* Google - Android UI sounds
* Material.io - https://material.io/design/sound/sound-resources.html#
* Some modders for suggestions and ideas :)