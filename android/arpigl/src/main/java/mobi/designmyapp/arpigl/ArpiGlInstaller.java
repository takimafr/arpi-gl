/*
 * Copyright (C) 2015  eBusiness Information
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

package mobi.designmyapp.arpigl;

import android.content.Context;
import android.content.SharedPreferences;
import android.content.res.AssetManager;
import android.os.AsyncTask;
import android.os.AsyncTask.Status;
import android.preference.PreferenceManager;
import android.util.Log;

import org.json.JSONException;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.Arrays;
import java.util.HashSet;
import java.util.LinkedList;
import java.util.List;
import java.util.zip.ZipEntry;
import java.util.zip.ZipInputStream;

import mobi.designmyapp.arpigl.engine.ArpiGlController;
import mobi.designmyapp.arpigl.engine.Engine;

/**
 * Installation tool for the {@link ArpiGlController} to be usable. The
 * internal engine of {@link ArpiGlController} needs some required files to be
 * copied in order to work. These files are contained in assets folder, within
 * the archive 'arpigl-assets.zip'. The {@link ArpiGlInstaller} will also
 * handle installation of additional custom resources you want to use with your
 * application. In this case, these resources have to be placed in the assets,
 * inside a folder named 'arpigl' ({@code EngineInstaller.EXTRA_INSTALL_DIR}), or
 * an archive named 'arpigl.zip' ({@code EngineInstaller.EXTRA_INSTALL_DIR}). Note
 * that installation process may take a while if your application comes bundled
 * with a lot of resources ; Thus, installation from an archive is a better
 * option.
 * <p/>
 * Your resource folder must comply to the following data structure, that will
 * be copied onto the internal storage :
 * <p/>
 * arpigl
 * ├── geoscene             ; contains JSON description files of the startup scenes.
 * ├── material             ; contains JSON description files of the materials.
 * ├── pois                 ; contains JSON description files of the pois,
 * │                            for the {@link mobi.designmyapp.arpigl.provider.impl.PoiStorageProvider}
 * ├── shader               ; contains custom shader programs.
 * │                            (/!\ This is an advanced feature.
 * │                            Shader programs must follow the same semantic
 * │                            as built-in shaders.
 * └── texture
 * ****├── icon                 ; contains texture files, in '.png' format. Dimensions must be of power of 2
 * ****├── cubemap/skybox       ; contains cubemaps for skyboxes.
 * ****│                           Image files are stored in a folder per cubemap.
 * ****│                           See build in cubemap for example.
 * ****└── tiles                ; contains tiles for {@link mobi.designmyapp.arpigl.provider.TileProvider}
 *
 * The library is actively developed and supported by
 * Clement Gayvallet    https://github.com/clementgayvallet
 * Nicolas Thierion     https://github.com/NicolasThierion
 * Loïc Ortola          https://github.com/loicortola
 */
public final class ArpiGlInstaller {

    /**
     * Directory where resource files will be installed on internal storage.
     */
    public static final String INSTALLATION_DIR = "arpigl";

    public static final String TEXTURE_ICONS_SUBDIR = "texture/icon";
    /**
     * User may provide this additional zip to install its own file.
     */
    public static final String EXTRA_INSTALL_ZIP = INSTALLATION_DIR + ".zip";
    /**
     * User may provide this additional dir to install its own file.
     */
    public static final String EXTRA_INSTALL_DIR = INSTALLATION_DIR;
    public static final String POIS_DIR = INSTALLATION_DIR + File.separator + "pois";


    /**
     * Debug tag.
     */
    private static final String TAG = ArpiGlInstaller.class.getSimpleName();

    /* ***
     * CONSTANTS
     */
    private static final String BUILT_IN_RESOURCE_VERSION = "1.3";
    private static final String EXTRA_RESOURCE_VERSION = "1.0";
    /**
     * Installer will by default search this zip & install it.
     */
    private static final String BUILT_IN_INSTALL_ZIP = "arpigl-assets.zip";
    private static final int COPY_BUFFER_SIZE = 8192;
    /* ***
     * PREFERENCE KEYS
     */
    private static final String PREF_EXTRA_RESOURCES_INSTALLED = "pref_engine_extra_installed";
    private static final String PREF_BUILTIN_RESOURCES_INSTALLED = "pref_engine_installed";
    /**
     * this installer instance.
     */
    private static ArpiGlInstaller mInstance;
    /* ***
     * ATTRIBUTES
     */
    private final Context mContext;
    private final File mInstallDir;
    private final SharedPreferences mSharedPref;
    private final HashSet<String> mInstallZips;
    private final HashSet<String> mInstallPaths;
    private String mExtraVersion = EXTRA_RESOURCE_VERSION;
    private InstallTask mInstallTask = new InstallTask();
    private byte[] mCopyBuffer;
    private PoiInstaller mPoiInstaller;

    private ArpiGlInstaller(Context context) {
        mContext = context.getApplicationContext();
        mSharedPref = PreferenceManager.getDefaultSharedPreferences(mContext);
        mInstallDir = new File(mContext.getFilesDir(), INSTALLATION_DIR);
        mInstallZips = new HashSet<>();
        mInstallZips.add(BUILT_IN_INSTALL_ZIP);
        mInstallZips.add(EXTRA_INSTALL_ZIP);

        mInstallPaths = new HashSet<>();
        mInstallPaths.add(EXTRA_INSTALL_DIR);

        mPoiInstaller = new PoiInstaller();

    }

    /* ***
     * CONSTRUCTOR
     */

    /**
     * Get the installer instance for this context.
     *
     * @param context The context of the application.
     * @return the installer instance.
     */
    public static ArpiGlInstaller getInstance(Context context) {
        if (mInstance == null) {
            synchronized (ArpiGlInstaller.class) {
                if (mInstance == null) {
                    mInstance = new ArpiGlInstaller(context);
                }
            }
        }
        return mInstance;
    }

    private static void recursiveDelete(File root) {
        File[] files = root.listFiles();
        if (files != null) {
            for (final File f : files) {
                if (f.isDirectory()) {
                    recursiveDelete(f);
                    f.delete();
                } else {
                    f.delete();
                }
            }
        }

    }

    /* ***
     * PUBLIC METHODS
     */

    /**
     * check whether MapGL engine is installed (and thus, required asset files
     * are copied onto internal storage).
     *
     * @return true if installed engine resources version matches installer
     * version. @see setVersion
     */
    public boolean isInstalled() {
        return mExtraVersion.equals(mSharedPref.getString(PREF_EXTRA_RESOURCES_INSTALLED, null))
                && BUILT_IN_RESOURCE_VERSION.equals(mSharedPref.getString(PREF_BUILTIN_RESOURCES_INSTALLED, null));
    }

    /**
     * Sets the installer version. A new version is needed whenever you update
     * the resource package shipped with your application. Thus, the
     * {@link ArpiGlInstaller} will know that installed version is different from
     * the new resource package, and install the new resources.
     *
     * @param version the new version
     */
    public void setVersion(String version) {
        if (version == null) {
            throw new IllegalArgumentException("installer version cannot be null!");
        }
        mExtraVersion = version;
    }

    /**
     * Installs the DMA Engine on the file system.
     *
     * @return true if installation succeed.
     * @throws IOException   if cannot install the engine in case of an IO problem (no
     *                       more available space, or trying to copy a non-existing file.)
     * @throws JSONException if one of the JSON resource in malformed.
     */
    public boolean install() throws IOException, JSONException {
        class NullCallbacks implements SetupListener {
            private boolean mResult;

            public NullCallbacks() {

            }

            @Override
            public void onInstallationFinish(boolean success) {
                mResult = success;
            }

            @Override
            public void onInstallationProgress(int progress) {
            }
        }
        final NullCallbacks cb = new NullCallbacks();
        install(cb);
        return cb.mResult;
    }

    private void install(SetupListener callbacks) throws IOException, JSONException {
        if (isInstalled()) {
            Log.w(TAG, "called 'install()' twice... useless");
            callbacks.onInstallationFinish(true);
            return;
        }

        recursiveDelete(mInstallDir);

        // check if engine installation zip is present in assets, but this will
        // never happen
        if (!assetExists(BUILT_IN_INSTALL_ZIP)) {
            throw new IOException("DMA Engine assets not found. Your installation package may be corrupted.");
        }

        boolean success = false;
        float progress = 0.0f;
        mCopyBuffer = new byte[COPY_BUFFER_SIZE];
        final LinkedList<String> additionalFiles = new LinkedList<>();
        final LinkedList<String> installZips = new LinkedList<>();

        // open destination dir (private context dir)
        final File destDir = mContext.getFilesDir();
        final AssetManager assets = mContext.getAssets();
        long uncompressedSize = 0;
        long compressedSize = 0;
        long totalSize;
        InputStream in = null;
        ZipInputStream zin = null;

        try {
            // search for actually existing zips, un-index those what that are
            // missing.
            for (final String zipName : mInstallZips) {
                if (assetExists(zipName)) {
                    installZips.add(zipName);
                }
            }

            // compute zips size
            for (final String zipName : installZips) {
                compressedSize += getCompressedZipSize(zipName);
            }

            // search for additional resources to install in app assets
            for (final String dir : mInstallPaths) {
                if (isDirectory(dir)) {
                    uncompressedSize += listAssetFiles(dir, additionalFiles);
                }
            }

            totalSize = compressedSize + uncompressedSize;
            // extract zips
            for (final String zipName : installZips) {
                in = assets.open(zipName);
                zin = new ZipInputStream(in);
                ZipEntry ze;
                // extract from asset file by file.
                while ((ze = zin.getNextEntry()) != null) {
                    Log.v(TAG, "Unzipping " + ze.getName());
                    final long entrySize = extract(zin, ze, destDir);
                    final float progressStep = ((float) entrySize / (float) totalSize) * 100;
                    progress += progressStep;
                    callbacks.onInstallationProgress((int) progress);
                }
            }

            // install additional files
            for (final String filename : additionalFiles) {
                Log.v(TAG, "Copying file " + filename);

                final int size = copyFromAssets(filename, destDir);
                final float progressStep = ((float) size / (float) totalSize) * 100;
                progress += progressStep;
                callbacks.onInstallationProgress((int) progress);
            }

            success = true;
        } catch (final Exception e) {
            Log.e(TAG, "cannot extract file : " + e.getMessage());
        } finally {
            try {
                // close inputs
                if (zin != null) {
                    zin.close();
                }
                if (in != null) {
                    in.close();
                }
            } catch (final IOException e) {
                e.printStackTrace();
            }
        }

        if (success) {
            success = mPoiInstaller.install(new File(mContext.getFilesDir(), POIS_DIR));
        }

        // mark installation if success.
        if (success) {
            mSharedPref.edit().putString(PREF_EXTRA_RESOURCES_INSTALLED, mExtraVersion).apply();
            mSharedPref.edit().putString(PREF_BUILTIN_RESOURCES_INSTALLED, BUILT_IN_RESOURCE_VERSION).apply();
        }
        callbacks.onInstallationFinish(success);
    }

    /**
     * Installs the MapGL Engine on the file system.
     *
     * @param callbacks Callbacks that will be called on installation progress, & on
     *                  installation finish.
     */
    public void installAsync(SetupListener callbacks) {
        if (isInstalled()) {
            return;
        }

        synchronized (mInstallTask) {

            // cancel task if already running
            final Status status = mInstallTask.getStatus();
            if (status == Status.RUNNING || status == Status.PENDING) {
                mInstallTask.cancel(true);
            }
            mInstallTask = new InstallTask();
            mInstallTask.execute(callbacks);
        }
    }

    /* ***
     * GETTERS
     */

    /* ***
     * PRIVATE METHOD
     */
    private long extract(ZipInputStream zin, ZipEntry ze, File destDir) throws IOException {
        String target = new File(destDir, ze.getName()).getAbsolutePath();
        if (target.endsWith("/")) {
            target = target.substring(0, target.lastIndexOf("/"));
        }
        final long entrySize = ze.getCompressedSize();
        if (ze.isDirectory()) {
            new File(target).mkdir();
        } else {
            // write the target file to storage.
            final FileOutputStream fout = new FileOutputStream(target);

            int read;
            while ((read = zin.read(mCopyBuffer)) != -1) {
                fout.write(mCopyBuffer, 0, read);
            }

            zin.closeEntry();
            fout.close();
            return entrySize;
        }
        return 0;
    }

    private boolean assetExists(String assetName) {

        // As there is no method to know if asset exists, and listing asset is
        // way to heavy process,
        // we have no choice but dirty-check if the asset exists.
        try {
            final InputStream is = mContext.getAssets().open(assetName);
            is.close();
        } catch (final IOException e) {
            return false;
        }
        return true;
    }

    private long getCompressedZipSize(String zipName) throws IOException {
        long compressedSize;
        final AssetManager assets = mContext.getAssets();

        if (!assetExists(zipName)) {
            return 0;
        }

        // open resource
        final InputStream in = assets.open(zipName);
        compressedSize = in.available();

        in.close();

        return compressedSize;
    }

    private boolean isDirectory(String asset) {

        // check if this is a directory.
        try {
            final List<String> assetNames = Arrays.asList(mContext.getAssets().list(asset));
            return !assetNames.isEmpty();
        } catch (final IOException e) {
            e.printStackTrace();
        }
        return false;
    }

    private int copyFromAssets(String filename, File destination) throws IOException {

        // read assets
        final AssetManager assetManager = mContext.getAssets();
        int sizeInBytes = 0;
        InputStream in = null;
        OutputStream out = null;

        try {
            // open asset
            in = assetManager.open(filename);
            sizeInBytes = in.available();
            // open private dir
            final File outFile = new File(destination, filename);

            // copy this asset if it doesn't exists
            if (!outFile.exists()) {
                // create folders.
                outFile.getParentFile().mkdirs();
                // then copy the file.
                out = new FileOutputStream(outFile);
                copyFile(in, out);
            }
        } catch (final IOException e) {
            if (in != null) {
                in.close();
            }
            if (out != null) {
                out.close();
            }
            throw e;
        }
        return sizeInBytes;
    }

    private void copyFile(InputStream in, OutputStream out) throws IOException {
        int read;
        while ((read = in.read(mCopyBuffer)) != -1) {
            out.write(mCopyBuffer, 0, read);
        }
    }

    /**
     * List the files recursively in all folders & subfolders of assets.
     *
     * @param path     The path to search.
     * @param fileList the list of files within the "path" hierarchy.
     * @return the estimated size in bytes of the folder's content.
     * @throws IOException if an error occurs during reading the assets.
     */
    private long listAssetFiles(String path, List<String> fileList) throws IOException {
        long sizeInBytes = 0;
        String[] list;
        list = mContext.getAssets().list(path);
        if (list.length > 0) {
            // This is a folder
            for (final String file : list) {
                final long currentSize = listAssetFiles(path + "/" + file, fileList);
                if (currentSize < 0) {
                    return currentSize;
                }
                sizeInBytes += currentSize;
            }
        } else {
            fileList.add(path);
            final InputStream is = mContext.getAssets().open(path);
            sizeInBytes = is.available();
            is.close();
        }

        return sizeInBytes;
    }

    /**
     * Can be notified on {@link Engine}'s installation progress.
     *
     * @author Nicolas THIREION.
     */
    public interface SetupListener {
        /**
         * Called when {@link Engine}' s installation finished.
         *
         * @param success true if the installation succeed.
         */
        void onInstallationFinish(boolean success);

        /**
         * Called when {@link Engine}' s installation goes on.
         *
         * @param progress current installation progress.
         */
        void onInstallationProgress(int progress);
    }

    /* ***
     * PRIVATE CLASS
     */

    private class InstallTask extends AsyncTask<SetupListener, Void, Void> {

        @Override
        protected Void doInBackground(SetupListener... params) {
            try {
                install(params[0]);
            } catch (final IOException | JSONException e) {
                e.printStackTrace();
                throw new RuntimeException(e);
            }
            return null;
        }
    }
}
