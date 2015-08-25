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

package mobi.designmyapp.arpigl.demo;

import android.content.Intent;
import android.content.pm.PackageInfo;
import android.content.pm.PackageManager;
import android.content.pm.PackageManager.NameNotFoundException;
import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.text.Html;
import android.view.View;
import android.widget.ProgressBar;
import android.widget.TextView;

import java.io.Serializable;

import mobi.designmyapp.arpigl.ArpiGlInstaller;

/**
 * Shows a loading screen on application's first launch, while the
 * {@link ArpiGlInstaller} does the installation process for copying the
 * required files from the assets on the internal storage.
 *
 * @author Nicolas THIERION
 * @author Nicolas THIERION
 */
public final class SplashScreenActivity extends AppCompatActivity {

    public static final String EXTRA_CALLING_ACTIVITY_CLASS = "CallingActivityClass";

    /* ***
     * WIDGETS
     */
    private TextView mEditBy;
    private TextView mPoweredBy;
    private ProgressBar mProgressBar;

    /* ***
     * ATTRIBUTES
     */
    private ArpiGlSetupListener mInstallationCallbacks;

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        setContentView(R.layout.activity_splashscreen);
        mEditBy = (TextView) findViewById(R.id.splashscreen_edited_by);
        mPoweredBy = (TextView) findViewById(R.id.powered_by);
        mProgressBar = (ProgressBar) findViewById(R.id.splashscreen_progress_spinner);

        mEditBy.setText(Html.fromHtml(getString(R.string.splash_screen_edited_by)));
        mPoweredBy.setText(Html.fromHtml(getString(R.string.splash_screen_powered_by)));

        mInstallationCallbacks = new ArpiGlSetupListener();

        final ArpiGlInstaller installer = ArpiGlInstaller.getInstance(this);

        // set the installable resource version according to current application
        // version.
        installer.setVersion(getVersionCode());

        if (!installer.isInstalled()) {
            mProgressBar.setVisibility(View.GONE);
            mProgressBar = (ProgressBar) findViewById(R.id.splashscreen_progressbar);
            mProgressBar.setVisibility(View.VISIBLE);
            installer.installAsync(mInstallationCallbacks);
        } else {
            startCallingActivity();
        }
    }

    @Override
    protected void onResume() {
        super.onResume();
    }

    @Override
    protected void onPause() {
        super.onPause();
    }

    /* ***
     * PRIVATE METHODS
     */
    private void startCallingActivity() {
        Serializable activityClassName = getIntent().getSerializableExtra(EXTRA_CALLING_ACTIVITY_CLASS);
        if (activityClassName != null && activityClassName instanceof Class) {
            Intent intent = new Intent(this, (Class<?>) activityClassName);
            startActivity(intent);
        }
        finish();
    }

    private String getVersionCode() {
        try {
            PackageManager manager = getPackageManager();
            PackageInfo info;
            info = manager.getPackageInfo(getPackageName(), 0);
            return info.versionName;
        } catch (NameNotFoundException e) {
            e.printStackTrace();
        }
        return "-1";
    }

    /* ***
     * PRIVATE CLASS
     */

    private class ArpiGlSetupListener implements ArpiGlInstaller.SetupListener {

        @Override
        public void onInstallationFinish(boolean arg0) {
            startCallingActivity();
        }

        @Override
        public void onInstallationProgress(int progress) {
            // toggle progressbar to 'percent-bar'
            mProgressBar.setIndeterminate(false);
            mProgressBar.setProgress(progress);
        }

    }
}
