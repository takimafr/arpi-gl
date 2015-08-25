# ArpiGL
A lightweight native OpenGL ES 2.0 powered library allowing to display Points of Interest in a 3D Augmented Reality environment.  

The current version 1.0 only has an android client library, Pull requests for the iOS version are welcome.

# Screenshots
![Custom Markers](/images/custom-markers.png)
![Custom POI Providers](/images/custom-poi-providers.png)
![Custom Skybox](/images/custom-skybox.png)
![Offline Maps](/images/offline-map.png)

## Size
The native library weights 450 KB, and requires an additional 1.4 MB of assets.  
The android library bundle weighs 2.1 MB.

## Android Versions support
Library supports android SDK 15 and above  

## Environment Setup
1. Clone the ArpiGL project repository  
2. In your project with AndroidStudio File -> New Module -> Import .JAR/.AAR and import the .aar  


## Manifest attributes
Add this following permissions and features to your AndroidManifest.xml:  
```
    <uses-feature android:glEsVersion="0x00020000" />

    <uses-feature android:name="android.hardware.sensor.gyroscope" />
    <uses-feature android:name="android.hardware.sensor.accelerometer" />

    <uses-permission android:name="android.permission.ACCESS_FINE_LOCATION" />
    <uses-permission android:name="android.permission.INTERNET" />
    <uses-permission android:name="android.permission.WRITE_EXTERNAL_STORAGE" />
    <uses-permission android:name="android.permission.READ_PHONE_STATE" />
    <uses-permission android:name="android.permission.READ_EXTERNAL_STORAGE" />
```

## Gradle dependencies
In your project build.gradle add the following dependency:
```
dependencies {
    compile project(':arpi-gl')
    }
```

## App Integration


### Assets install
The **ArpiGl** library needs resources: textures, OpenGL shaders, meshes...  
Those resources are located in your library assets.  
For performance purposes, those resources need to be installed on the device's storage.  
The installation process takes a few seconds, and needs to be done one-time only.  
To install these resources, we provided a convenient **ArpiGlInstaller**.  
Use ArpiGlInstaller on your splashscreen when your app is launched.  
**N.B.: You don't need to implement anything to ensure the install process is only done once, as the checks are already built-in. Calling install() multiple times will not install multiple times.**

Synchronous install:
```java
    ArpiGlInstaller.getInstance(myAppContext).install();
```

Asynchronous install (recommended):
```java
    ArpiGlInstaller.getInstance(myAppContext).installAsync(mArpiGlSetupListener);
```

### Activity integration
1. Create an ArpiGlFragment  
2. Create a controller for that fragment  
3. Add your tile provider  
4. Add your POI provider  

```java
    ArpiGlFragment arpiFragment = (ArpiGlFragment) getFragmentManager().findFragmentById(R.id.arpi_fragment);
    ArpiGlController arpiController = new ArpiGlController(arpiFragment);
    // Add the built-in OpenStreetMap tile provider
    arpiController.setTileProvider(new OSMTileProvider());
    // This one will download POIs from opendatasoft.com. You should make your own
    arpiController.addPoiProvider(new ODSPoiProvider());
```

## Additional features

### Skybox
The library supports custom skyboxes (for instance, to show a different skybox depending on the weather.)  
To add a custom skybox, create 6 images: back.png, bottom.png, front.png, left.png, right.png, top.png and put them into your **assets/arpigl/texture/cubemap/skybox/YourSkyboxName** folder.  
Then, in your code, enable your skybox with:  
```java
    arpiController.setSkyBox("YourSkyboxName");
    arpiController.setSkyBoxEnabled(true);
```
#### IMPORTANT
Note that the image dimensions need to be a power of two, and in PNG format. PNG transparency is not supported for skyboxes.  
See the Custom Skybox sample in the sample app for more details.

### Gravity Trigger
Enabling your 3D Map view can be done multiple ways. The classic way would be a button or a menu item.  
For us, we thought that raising your phone up would be a nice trigger. Putting it down, would disable it.  
Instead of leaving you the complexity of such a trigger, we made it easy for you:  
```java
GravitySensorTrigger visibilityTrigger = new GravitySensorTrigger(mapGLFragment);
```
See the Gravity Trigger sample for more details.

### Custom Tile Providers
We have implemented one provider: the OpenStreetMap tile provider. However, you may have your own way of requesting tiles (Google Maps, Bing, etc...).  
By implementing the TileProvider interface, you can define your own tile provider and use it instead of ours.

### Custom POI Providers
When you have your own Geo API, it would not be fair to force your model to look like our POI model. Therefore, you can easily implement your own PoiProvider and provide a custom POIMapper for it.  
See the Multiple PoiProviders sample for more details.

### Offline Tiles
In some use cases, you may want to have offline tiles built-into your app.  
In order to do so, simply put your tiles into the **assets/arpigl/texture/tiles/{z}/{x}/{y}.png** folder. The tiles will be loaded by default.  
We use the **{z}/{x}/{y}.png** convention, where z is the zoom level, x and y are the tile position. Currently, only zoom level 19 is supported.  
See the Offline Providers sample for more details.

### Offline POIs
In some use cases, you may want to have offline pois built-into your app.  
In order to do so, simply put a json descriptor file containing an array of POIs into the **assets/arpigl/pois/yourFile.json**. Then, simply use the AssetsStoragePoiProvider implementation in your controller. 
```java
arpiController.addPoiProvider(new AssetsStoragePoiProvider(this));
```
You can add any number of json files in that folder, those will be processed and concatenated at first launch.  
See the Offline Providers sample for more details.

### Custom Markers
When you launch the app, your 3D Markers may have different shapes, different icons and different colors.

#### Custom Icons
To add your own icon, simply put it into the **assets/arpigl/texture/icon/yourIcon.png** folder.  
Then, use **poi.setIcon("yourIcon.png");** in your java code.  
#### IMPORTANT
Note that the icon image dimensions need to be a power of two, and in PNG format. PNG transparency is supported.

#### Custom Shapes
We built custom meshes that should do the trick for most of your uses. However, you may like to have your own shapes for your markers.  
You can create your own meshes and use them anywhere into your app.
**N.B.: Only the Wavefront .obj format is supported. You must provide vertex positions (v) and texture mapping (vt). The library generates smooth normals at runtime, but if you provide vertex normals (vn) they will be interpreted as flat normals at rendering time. Long story short: your mesh is smooth -> do not provide normals, your mesh is flat -> provide normals**

#### Custom Colors
Your Marker color can be easily modified using the **poi.setColor("FF0000");** method.  
See the CustomMarkers sample for more details

## Archive support
We understand that sometimes you may have a certain number of custom assets, that take a certain size.  
When the install is made, the ArpiInstaller will check for a arpigl.zip archive in the assets folder.  
If it finds one, it will unzip it and process it normally.  
Therefore, you can provide all your custom assets, pois, textures... in an **assets/arpigl.zip** archive.  
The archive should respect the following data structure:
```sh
.
|
-arpigl                    # the archive should contain an arpigl folder containing your custom resources.
 ├── mesh                  # contains your custom meshes as .obj files
 ├── pois                  # contains JSON files for your offline Pois.
 └── texture               
     ├── cubemap
     │   └── skybox        # contains your skyboxes subfolders.
     │                     #     skybox images (back, bottom, front...) are stored in cubemap/skybox/TheSkyboxName/{}.png.
     ├── icon              # contains texture files, in '.png' format. Dimensions must be a power of 2.
     └── tiles             # contains offline tiles.
```

## Code Samples
The `app` module contains all the code samples you can dream of to get you started:

* [Custom TileProvider](android/app/src/main/java/mobi/designmyapp/arpigl/demo/customtileprovider/TileProviderDemoActivity.java)
* [Multiple PoiProviders](android/app/src/main/java/mobi/designmyapp/arpigl/demo/custompoiproviders/PoiProviderDemoActivity.java)
* [Auto GravityTrigger](android/app/src/main/java/mobi/designmyapp/arpigl/demo/sensortrigger/SensorTriggerDemoActivity.java)
* [Custom Skybox](android/app/src/main/java/mobi/designmyapp/arpigl/demo/customskybox/SkyboxDemoActivity.java)
* [Offline Providers](android/app/src/main/java/mobi/designmyapp/arpigl/demo/offlineproviders/OfflineDemoActivity.java)
* [Custom Markers: Shapes, icons, colors](android/app/src/main/java/mobi/designmyapp/arpigl/demo/custommarkers/CustomMarkersDemoActivity.java)

## Proguard
The library has native callbacks and requires its packages not to be obfuscated.
Add these lines to your proguard config:
```sh
-keep class mobi.designmyapp.arpigl.** {
    *;
}
```

## License
The Android ArpiGl Library is OpenSource, licensed under the **GNU General Public License 3.0**.  
The native ArpiGl Library is closed source, **Copyright 2015 by eBusiness Information All-rights reserved**.

## Contribute
Contributions and Pull Requests are welcome. You may have awesome suggestions and we also have ideas on improvements and new features. Let us know what you want and how you may help!

## Commercial use
If you want to use ArpiGl in proprietary software, or want to use a non-watermarked version for your OpenSource projects, please contact us at **lortola@ebusinessinformation.fr**

## Issue Tracker
All feature requests and issues can be tracked on this Github repository.  
For direct contact, you can reach us on twitter:  
@DesignMyApp  
@LoicOrtola  