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

import mobi.designmyapp.arpigl.ArpiGlInstaller;
import mobi.designmyapp.arpigl.demo.custommarkers.CustomMarkersDemoActivity;
import mobi.designmyapp.arpigl.demo.custompoiproviders.PoiProviderDemoActivity;
import mobi.designmyapp.arpigl.demo.customskybox.SkyboxDemoActivity;
import mobi.designmyapp.arpigl.demo.customtileprovider.TileProviderDemoActivity;
import mobi.designmyapp.arpigl.demo.offlineproviders.OfflineDemoActivity;
import mobi.designmyapp.arpigl.demo.sensortrigger.SensorTriggerDemoActivity;
import mobi.designmyapp.arpigl.engine.impl.ArpiGlController;
import android.content.Intent;
import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.view.View;

/**
 * This class demonstrates some basic usages of the dma {@link ArpiGlController}
 * engine in an Activity. It loads a dummy GeoScene example and places some POI.
 *
 * @author Nicolas THIERION
 */
public class MenuDemoActivity extends AppCompatActivity {


    @Override
    protected final void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        // we usually use a splashscreen to do this.. But in case you don't,
        // this is your last chance to check if the resources are installed in
        // order to be able to use the ArpiGlCotroller
        final ArpiGlInstaller installer = ArpiGlInstaller.getInstance(this);
        if (!installer.isInstalled()) {
            // Start the SplashScreenActivity that will take care of the
            // resource installation.
            // Of course you can provide your own SplashScreen.
            Intent intent = new Intent(this, SplashScreenActivity.class);
            // Supply our activity class in order to get restarted once the
            // installation finishes
            intent.putExtra(SplashScreenActivity.EXTRA_CALLING_ACTIVITY_CLASS, this.getClass());
            startActivity(intent);
            finish(); // Don't forget to finish and return as we need to be
            // recreated after installation
            return;
        }

        // loads the activity's layout
        setContentView(R.layout.activity_demo_menu);

    }


    public void startTileProviderDemoActivity(View view) {
        Intent intent = new Intent(this, TileProviderDemoActivity.class);
        startActivity(intent);
    }

    public void startPoiProviderDemoActivity(View view) {
        Intent intent = new Intent(this, PoiProviderDemoActivity.class);
        startActivity(intent);
    }

    public void startArDemoActivity(View view) {
        Intent intent = new Intent(this, SensorTriggerDemoActivity.class);
        startActivity(intent);
    }

    public void startOfflineDemoActivity(View view) {
        Intent intent = new Intent(this, OfflineDemoActivity.class);
        startActivity(intent);
    }

    public void startSkyboxDemoActivity(View view) {
        Intent intent = new Intent(this, SkyboxDemoActivity.class);
        startActivity(intent);
    }

    public void startCustomMarkerDemoActivity(View view) {
        Intent intent = new Intent(this, CustomMarkersDemoActivity.class);
        startActivity(intent);
    }

}
